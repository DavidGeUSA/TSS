/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

time advancement estimation module, optimized for 4-th order estimation
coding not finished
********************************************************************/
#include "TimeTss4th.h"


TimeTss4th::TimeTss4th()
{
	work = NULL;
	curlNumber = 0;
}


TimeTss4th::~TimeTss4th()
{
	cleanup();
}
void TimeTss4th::cleanup()
{
	TimeTssBase::cleanup();
	if (work != NULL)
	{
		FreeMemory(work); work = NULL;
	}
}
int TimeTss4th::initFields(FieldsSetter* f0)
{
	if (pams->smax != 2 || pams->kmax != 1)
	{
		return ERR_INVALID_ESTIMATE_ORDER;
	}
	int ret = TimeTssBase::initFields(f0);
	if (ret == ERR_OK)
	{
		if (work != NULL)
		{
			FreeMemory(work); work = NULL;
		}
		work = (Point3Dstruct *)AllocateMemory(fieldMemorySize);
		if (work == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}
	if (ret == ERR_OK)
	{
		crE2.setSpaceRange(pams->nx, pams->ny, pams->nz);
		crH2.setSpaceRange(pams->nx, pams->ny, pams->nz);
		cr1a.setSpaceRange(pams->nx, pams->ny, pams->nz);
		cr1b.setSpaceRange(pams->nx, pams->ny, pams->nz);
		//
		gee1 = GeeP[1]; ghe0 = GheP[0] / 12; ghe1 = GheP[1] / 12;
		ghh1 = GhhP[1]; geh0 = GehP[0] / 12; geh1 = GehP[1] / 12;
	}
	return ret;
}

int TimeTss4th::GetFirstCurls()
{
	Point3Dstruct *wk;
	int ret = cr1a._getNextCurlUseFastMethod(E, work);
	if (ret == ERR_OK)
	{
		wk = curlE;
		curlE = work;
		work = wk;
		ret = cr1b._getNextCurlUseFastMethod(H, work);
		if (ret == ERR_OK)
		{
			wk = curlH;
			curlH = work;
			work = wk;
			curlNumber = 1;
		}
	}
	return ret;
}
int TimeTss4th::GetNextCurls()
{
	int ret = ERR_OK;
	Point3Dstruct *wk;
	if (curlNumber == 1)
	{
		ret = crE2._getNextCurlUseFastMethod(E, work);
		if (ret == ERR_OK)
		{
			wk = curlE;
			curlE = work;
			work = wk;
			ret = crH2._getNextCurlUseFastMethod(H, work);
			if (ret == ERR_OK)
			{
				wk = curlH;
				curlH = work;
				work = wk;
				curlNumber = 2;
			}
		}
	}
	else
	{
		int ret = cr1a._getNextCurlUseFastMethod(curlE, work);
		if (ret == ERR_OK)
		{
			wk = curlE;
			curlE = work;
			work = wk;
			ret = cr1b._getNextCurlUseFastMethod(curlH, work);
			if (ret == ERR_OK)
			{
				wk = curlH;
				curlH = work;
				work = wk;
				curlNumber++;
			}
		}
	}
	return ret;
}
int TimeTss4th::AdvanceOneStep()
{
	int ret = ERR_OK;
	//calculate 2nd curls
	ret = crE2._getNextCurlUseFastMethod(E, curlE);
	if (ret == ERR_OK)
	{
		ret = crH2._getNextCurlUseFastMethod(H, curlH);
	}
	if (ret == ERR_OK)
	{
		size_t w;
		unsigned int i, j, k;
		//
		w = 0;
		for (i = 0; i <= pams->nx; i++)
		{
			for (j = 0; j <= pams->ny; j++)
			{
				for (k = 0; k <= pams->nz; k++)
				{
					work[w].x = E[w].x + gee1 * curlE[w].x + ghe0 * (cr1a.DyVz(H, i, j, k) - cr1a.DzVy(H, i, j, k)) + ghe1 * (cr1b.DyVz(curlH, i, j, k) - cr1b.DzVy(curlH, i, j, k));
					work[w].y = E[w].y + gee1 * curlE[w].y + ghe0 * (cr1a.DzVx(H, i, j, k) - cr1a.DxVz(H, i, j, k)) + ghe1 * (cr1b.DzVx(curlH, i, j, k) - cr1b.DxVz(curlH, i, j, k));
					work[w].z = E[w].z + gee1 * curlE[w].z + ghe0 * (cr1a.DxVy(H, i, j, k) - cr1a.DyVx(H, i, j, k)) + ghe1 * (cr1b.DxVy(curlH, i, j, k) - cr1b.DyVx(curlH, i, j, k));
					w++;
				}
			}
		}
		Point3Dstruct *wk = E;
		E = work;
		work = wk;
		w = 0;
		for (i = 0; i <= pams->nx; i++)
		{
			for (j = 0; j <= pams->ny; j++)
			{
				for (k = 0; k <= pams->nz; k++)
				{
					work[w].x = H[w].x + ghh1 * curlH[w].x + geh0 * (cr1a.DyVz(E, i, j, k) - cr1a.DzVy(E, i, j, k)) + geh1 * (cr1b.DyVz(curlE, i, j, k) - cr1b.DzVy(curlE, i, j, k));
					work[w].y = H[w].y + ghh1 * curlH[w].y + geh0 * (cr1a.DzVx(E, i, j, k) - cr1a.DxVz(E, i, j, k)) + geh1 * (cr1b.DzVx(curlE, i, j, k) - cr1b.DxVz(curlE, i, j, k));
					work[w].z = H[w].z + ghh1 * curlH[w].z + geh0 * (cr1a.DxVy(E, i, j, k) - cr1a.DyVx(E, i, j, k)) + geh1 * (cr1b.DxVy(curlE, i, j, k) - cr1b.DyVx(curlE, i, j, k));
					w++;
				}
			}
		}
		wk = H;
		H = work;
		work = wk;
	}
	return ret;
}
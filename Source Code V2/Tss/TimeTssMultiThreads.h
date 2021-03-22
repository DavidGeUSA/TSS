#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

time advancement estimation module, use multiple threads
********************************************************************/
#include "TimeTssBase.h"
#include "CurlOne.h"
#include "LoopWorkDivider.h"

class ThreadPool;

class TimeTssMultiThreads : virtual public TimeTssBase
{
private:
	ThreadPool *pool;
	int retH;
	int retE;
	unsigned int h;
	Point3Dstruct *wk;
#ifdef TIMESTEPPROFILING
	size_t startTE = 0, endTE = 0, startTH = 0, endTH = 0;
#endif

	Point3Dstruct *workE;
	Point3Dstruct *workH;
	CurlOne **curl_OneE;
	CurlOne **curl_OneH;
	LoopWorkDivider divider;
	unsigned int workCount;
	unsigned int workCountN1; //workCount-1
public:
	TimeTssMultiThreads();
	virtual ~TimeTssMultiThreads();
	virtual void startThreads();
	virtual void removeThreads();
	virtual void cleanup();
	virtual int initFields(FieldsSetter* f0);
	virtual int GetFirstCurls();
	virtual int GetNextCurls();
	virtual int AdvanceOneStep();
	//////////////////////////////////////////////////////////////////////
	void apply1stCurlE(unsigned int th)
	{
		unsigned int i, j, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wE = divider.StartIndexOneDim(th);
		unsigned int j0 = 0, k0 = 0;
		unsigned int j1 = pams->ny, k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wE = (pams->nz + 1)*(pams->ny + 1)*(i0);
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			j0 = pams->pml.Ln;
			k0 = pams->pml.Ln;
			j1 = pams->ny - pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		//
		for (i = i0; i <= i1; i++)
		{
			for (j = 0; j <= pams->ny; j++)
			{
				for (k = 0; k <= pams->nz; k++)
				{
					if (j >= j0 && j <= j1 && k >= k0 && k <= k1)
					{
						E[wE].x = GeeP[h] * E[wE].x + GheP[h] * curlH[wE].x;
						E[wE].y = GeeP[h] * E[wE].y + GheP[h] * curlH[wE].y;
						E[wE].z = GeeP[h] * E[wE].z + GheP[h] * curlH[wE].z;
					}
					wE++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTE = GetTimeTick();
#endif
	}
	void apply1stCurlH(unsigned int th)
	{
#ifdef TIMESTEPPROFILING
		startTH = GetTimeTick();
#endif
		unsigned int i, j, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wH = divider.StartIndexOneDim(th);
		unsigned int j0 = 0, k0 = 0;
		unsigned int j1 = pams->ny, k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wH = (pams->nz + 1)*(pams->ny + 1)*(i0);
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			j0 = pams->pml.Ln;
			k0 = pams->pml.Ln;
			j1 = pams->ny - pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		for (i = i0; i <= i1; i++)
		{
			for (j = 0; j <= pams->ny; j++)
			{
				for (k = 0; k <= pams->nz; k++)
				{
					if (j >= j0 && j <= j1 && k >= k0 && k <= k1)
					{
						H[wH].x = GhhP[h] * H[wH].x + GehP[h] * curlE[wH].x;
						H[wH].y = GhhP[h] * H[wH].y + GehP[h] * curlE[wH].y;
						H[wH].z = GhhP[h] * H[wH].z + GehP[h] * curlE[wH].z;
					}
					wH++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
#endif
	}
	void apply2ndCurlE(unsigned int th)
	{
#ifdef TIMESTEPPROFILING
		startTE = GetTimeTick();
#endif
		unsigned int i, j, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wE = divider.StartIndexOneDim(th);
		unsigned int j0 = 0, k0 = 0;
		unsigned int j1 = pams->ny, k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wE = (pams->nz + 1)*(pams->ny + 1)*(i0);
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			j0 = pams->pml.Ln;
			k0 = pams->pml.Ln;
			j1 = pams->ny - pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		for (i = i0; i <= i1; i++)
		{
			for (j = 0; j <= pams->ny; j++)
			{
				for (k = 0; k <= pams->nz; k++)
				{
					if (j >= j0 && j <= j1 && k >= k0 && k <= k1)
					{
						E[wE].x += GeeP[h] * curlE[wE].x;
						E[wE].y += GeeP[h] * curlE[wE].y;
						E[wE].z += GeeP[h] * curlE[wE].z;
					}
					//
					wE++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTE = GetTimeTick();
#endif
	}
	void apply2ndCurlH(unsigned int th)
	{
#ifdef TIMESTEPPROFILING
		startTH = GetTimeTick();
#endif
		unsigned int i, j, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wH = divider.StartIndexOneDim(th);
		unsigned int j0 = 0, k0 = 0;
		unsigned int j1 = pams->ny, k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wH = (pams->nz + 1)*(pams->ny + 1)*(i0);
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			j0 = pams->pml.Ln;
			k0 = pams->pml.Ln;
			j1 = pams->ny - pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		for (i = i0; i <= i1; i++)
		{
			for (j = 0; j <= pams->ny; j++)
			{
				for (k = 0; k <= pams->nz; k++)
				{
					if (j >= j0 && j <= j1 && k >= k0 && k <= k1)
					{
						H[wH].x += GhhP[h] * curlH[wH].x;
						H[wH].y += GhhP[h] * curlH[wH].y;
						H[wH].z += GhhP[h] * curlH[wH].z;
					}
					wH++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
#endif
	}
	void apply3rdCurlE(unsigned int th)
	{
#ifdef TIMESTEPPROFILING
		startTE = GetTimeTick();
#endif
		unsigned int i, j, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wE = divider.StartIndexOneDim(th);
		unsigned int j0 = 0, k0 = 0;
		unsigned int j1 = pams->ny, k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wE = (pams->nz + 1)*(pams->ny + 1)*(i0);
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			j0 = pams->pml.Ln;
			k0 = pams->pml.Ln;
			j1 = pams->ny - pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		for (i = i0; i <= i1; i++)
		{
			for (j = 0; j <= pams->ny; j++)
			{
				for (k = 0; k <= pams->nz; k++)
				{
					if (j >= j0 && j <= j1 && k >= k0 && k <= k1)
					{
						E[wE].x += GheP[h] * curlH[wE].x;
						E[wE].y += GheP[h] * curlH[wE].y;
						E[wE].z += GheP[h] * curlH[wE].z;
					}
					//
					wE++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTE = GetTimeTick();
#endif
	}
	void apply3rdCurlH(unsigned int th)
	{
#ifdef TIMESTEPPROFILING
		startTH = GetTimeTick();
#endif
		unsigned int i, j, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wH = divider.StartIndexOneDim(th);
		unsigned int j0 = 0, k0 = 0;
		unsigned int j1 = pams->ny, k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wH = (pams->nz + 1)*(pams->ny + 1)*(i0);
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			j0 = pams->pml.Ln;
			k0 = pams->pml.Ln;
			j1 = pams->ny - pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		for (i = i0; i <= i1; i++)
		{
			for (j = 0; j <= pams->ny; j++)
			{
				for (k = 0; k <= pams->nz; k++)
				{
					//
					if (j >= j0 && j <= j1 && k >= k0 && k <= k1)
					{
						H[wH].x += GehP[h] * curlE[wH].x;
						H[wH].y += GehP[h] * curlE[wH].y;
						H[wH].z += GehP[h] * curlE[wH].z;
					}
					wH++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
#endif
	}

};


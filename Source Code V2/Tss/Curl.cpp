
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

calculate curls
********************************************************************/
#include "Space.h"
#include "Curl.h"
#include "CurlOne.h"
#include "CurlTwo.h"
#include "CurlThreeA.h"

Curl::Curl()
{
	space = NULL;
	_calculationMethod = ByFirstOrder;
	pCurrent = NULL;
	work = NULL;
	Field = NULL;
	CurlNumber = CurlK = 0;
	isEven = false;
}

Curl::~Curl()
{
	cleanup();
}

void Curl::cleanup()
{
}

int Curl::initialize(Space *owner)
{
	int ret = ERR_OK;
	space = owner;
	M = space->GetEstimationOrder();
	CurlNumber = CurlK = 0;
	isEven = true;
	pCurrent = NULL;// field;
	//
	nx = space->Nx();
	ny = space->Ny();
	nz = space->Nz();
	//
	nxN1 = nx - 1;
	nyN1 = ny - 1;
	nzN1 = nz - 1;
	//
	nx1 = nx + 1;
	ny1 = ny + 1;
	nz1 = nz + 1;
	//
	return ret;
}

void Curl::SetField(Point3Dstruct *field)
{
	Field = field;
	CurlNumber = CurlK = 0;
	isEven = true;
	pCurrent = field;
}

/*
	for sharply changing fields, using higher order derivatives to get 
	higher order curls can more accurate, because fewer space points are used.
	this implementation only use the second order derivatives. 
*/
int Curl::_getNextCurlUseHighOrder(Point3Dstruct *pNext)
{
	int ret = ERR_OK;
	double DyFz, DzFy, DzFx, DxFz, DxFy, DyFx;
	if (pNext == pCurrent)
		return ERR_CURL_FAILED;
	if (CurlNumber == 0)
	{
		size_t w = 0;
		for (unsigned int i = 0; i <= nx; i++)
		{
			for (unsigned int j = 0; j <= ny; j++)
			{
				for (unsigned int k = 0; k <= nz; k++)
				{
					DyFz = space->dy_Fz(Field, 1, i, j, k);
					DzFy = space->dz_Fy(Field, 1, i, j, k);
					DzFx = space->dz_Fx(Field, 1, i, j, k);
					DxFz = space->dx_Fz(Field, 1, i, j, k);
					DxFy = space->dx_Fy(Field, 1, i, j, k);
					DyFx = space->dy_Fx(Field, 1, i, j, k);
					pNext[w].x = DyFz - DzFy;
					pNext[w].y = DzFx - DxFz;
					pNext[w].z = DxFy - DyFx;
					//
					w++;
				}
			}
		}
		CurlNumber = 1;
		CurlK = 0;
		isEven = false;
		pCurrent = pNext;
	}
	else if (CurlNumber == 1)
	{
		size_t w = 0;
		double DxFx, DyFy, DzFz;
		for (unsigned int i = 0; i <= nx; i++)
		{
			for (unsigned int j = 0; j <= ny; j++)
			{
				for (unsigned int k = 0; k <= nz; k++)
				{
					DxFx = space->dx_Fx(Field, 2, i, j, k);
					DyFy = space->dy_Fy(Field, 2, i, j, k);
					DzFz = space->dz_Fz(Field, 2, i, j, k);

					DyFx = space->dy_Fx(Field, 2, i, j, k);
					DzFy = space->dz_Fy(Field, 2, i, j, k);
					DxFz = space->dx_Fz(Field, 2, i, j, k);
					
					DzFx = space->dz_Fx(Field, 2, i, j, k);
					DxFy = space->dx_Fy(Field, 2, i, j, k);
					DyFz = space->dy_Fz(Field, 2, i, j, k);
					//
					pNext[w].x = -DxFx - DyFx - DzFx;
					pNext[w].y = -DxFy - DyFy - DzFy;
					pNext[w].z = -DxFz - DyFz - DzFz;
					//
					w++;
				}
			}
		}
		CurlNumber = 2;
		CurlK = 1;
		isEven = true;
		isKeven = false;
		pCurrent = pNext;
	}
	else if (CurlNumber == 2)
	{
		size_t w = 0;
		double DxFx, DyFy, DzFz;
		for (unsigned int i = 0; i <= nx; i++)
		{
			for (unsigned int j = 0; j <= ny; j++)
			{
				for (unsigned int k = 0; k <= nz; k++)
				{
					DxFx = space->dx_Fx(Field, 3, i, j, k);
					DyFy = space->dy_Fy(Field, 3, i, j, k);
					DzFz = space->dz_Fz(Field, 3, i, j, k);

					DyFx = space->dy_Fx(Field, 2, i, j, k);
					DzFy = space->dz_Fy(Field, 2, i, j, k);
					DxFz = space->dx_Fz(Field, 2, i, j, k);

					DzFx = space->dz_Fx(Field, 2, i, j, k);
					DxFy = space->dx_Fy(Field, 2, i, j, k);
					DyFz = space->dy_Fz(Field, 2, i, j, k);
					//
					pNext[w].x = -DxFx - DyFx - DzFx;
					pNext[w].y = -DxFy - DyFy - DzFy;
					pNext[w].z = -DxFz - DyFz - DzFz;
					//
					w++;
				}
			}
		}
		CurlNumber = 3;
		CurlK = 1;
		isEven = false;
		isKeven = false;
		pCurrent = pNext;
	}
	else
	{
		ret = _getNextCurlUseFirstOrder(pNext);
		CurlNumber++;
		isEven = !isEven;
	}
	return ret;
}

/*
	for not sharply changing fields, using 1st derivatives to get higher order curls
	can be more accurate, because more space points are used
*/
int Curl::_getNextCurlUseFirstOrder(Point3Dstruct *pNext)
{
	double DyFz, DzFy, DzFx, DxFz, DxFy, DyFx;
	size_t w = 0;
	if (pNext == pCurrent)
		return ERR_CURL_FAILED;
	for (unsigned int i = 0; i <= nx; i++)
	{
		for (unsigned int j = 0; j <= ny; j++)
		{
			for (unsigned int k = 0; k <= nz; k++)
			{
				DyFz = space->dy_Fz(pCurrent, 1, i, j, k);
				DzFy = space->dz_Fy(pCurrent, 1, i, j, k);
				DzFx = space->dz_Fx(pCurrent, 1, i, j, k);
				DxFz = space->dx_Fz(pCurrent, 1, i, j, k);
				DxFy = space->dx_Fy(pCurrent, 1, i, j, k);
				DyFx = space->dy_Fx(pCurrent, 1, i, j, k);
				pNext[w].x = DyFz - DzFy;
				pNext[w].y = DzFx - DxFz;
				pNext[w].z = DxFy - DyFx;
				w++;
			}
		}
	}
	CurlNumber++;
	pCurrent = pNext;
	return ERR_OK;
}

/*
	only implemented for smax=2
*/
/*
calculate 1 to 3 curls directly from the field without calculating derivatives first
*/
int Curl::_getNextCurlUseFastMethod(Point3Dstruct *pNext)
{
	int ret = ERR_OK;
	if (M != 4) return _getNextCurlUseHighOrder(pNext);
	if (pNext == pCurrent)
		return ERR_CURL_FAILED;
	if (CurlNumber == 1)
	{
		CurlTwo ctwo;
		ctwo.setSpaceRange(nx, ny, nz);
		ret = ctwo._getNextCurlUseFastMethod(Field, pNext);
		if (ret == ERR_OK)
		{
			CurlNumber++;
			pCurrent = pNext;
		}
	}
	//else if (CurlNumber == 2)
	//{
	//	CurlThreeA c3;
	//	c3.setSpaceRange(nx, ny, nz);
	//	ret = c3._getNextCurlUseFastMethod(Field, work, pNext);
	//	if (ret == ERR_OK)
	//	{
	//		CurlNumber++;
	//		pCurrent = pNext;
	//	}
	//}
	else
	{
		CurlOne cone;
		cone.setSpaceRange(nx, ny, nz);
		ret = cone._getNextCurlUseFastMethod(pCurrent, pNext);
		if (ret == ERR_OK)
		{
			CurlNumber++;
			pCurrent = pNext;
		}
	}
	return ret;
}
/*
calculate next curls optimized for speed
*/
int Curl::_getNextCurlUseFastFirstOrderMethod(Point3Dstruct *pNext)
{
	int ret = ERR_OK;
	if (M != 4) return _getNextCurlUseHighOrder(pNext);
	if (pNext == pCurrent)
		return ERR_CURL_FAILED;
	CurlOne cone;
	cone.setSpaceRange(nx, ny, nz);
	ret = cone._getNextCurlUseFastMethod(pCurrent, pNext);
	if (ret == ERR_OK)
	{
		CurlNumber++;
		pCurrent = pNext;
	}
	return ret;
}
int Curl::CalculateCurl(Point3Dstruct *curl)
{
	if (_calculationMethod == ByHigherOrder)
	{
		return _getNextCurlUseHighOrder(curl);
	}
	else if (_calculationMethod == ByFastMethod)
	{
		return _getNextCurlUseFastMethod(curl);
	}
	else if (_calculationMethod == ByFastMethodFirstOrder)
	{
		return _getNextCurlUseFastFirstOrderMethod(curl);
	}
	else
	{
		return _getNextCurlUseFirstOrder(curl);
	}
}






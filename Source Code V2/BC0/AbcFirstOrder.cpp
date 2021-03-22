
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility
********************************************************************/

#include "AbcFirstOrder.h"
#include "../MemoryMan/memman.h"
#include "../MemoryMan/MemoryManager.h"
#include "../EMField/RadiusIndex.h"



//free memory for a boundary plane
#define FREEPLANE(e) if((e) != NULL) {FreeMemory(e);e=NULL;}

//allocate memory for a boundary plane
#define ALLOCATEPLANE(e) if(ret == ERR_OK){ e = (double *)AllocateMemory(sizeof(double) * size); if(e == NULL) ret = ERR_OUTOFMEMORY;}

AbcFirstOrder::AbcFirstOrder(void)
{
	exy0 = exz0 = exy1 = exz1 = eyx0 = eyz0 = eyx1 = eyz1 = ezx0 = ezy0 = ezx1 = ezy1 = NULL;
}
AbcFirstOrder::~AbcFirstOrder(void)
{
	cleanup();
}
void AbcFirstOrder::cleanup()
{
	FREEPLANE(exy0)
	FREEPLANE(exz0)
	FREEPLANE(exy1)
	FREEPLANE(exz1)

	FREEPLANE(eyx0)
	FREEPLANE(eyz0)
	FREEPLANE(eyx1)
	FREEPLANE(eyz1)

	FREEPLANE(ezy0)
	FREEPLANE(ezx0)
	FREEPLANE(ezy1)
	FREEPLANE(ezx1)

}
/*
	initialize memory for aplying boundary condition
*/
int AbcFirstOrder::initialize(double Courant, int maximumRadius, TaskFile *taskParameters)
{
	int ret = BoundaryCondition::initialize(Courant, maximumRadius, taskParameters);
	if(ret == ERR_OK)
	{
		size_t size = gridSize * gridSize; //number of points on a boundary plane
		abccoef = (Cdtds - 1.0) / (Cdtds + 1.0);
		//allocate memory for remembering boundary planes
		cleanup();
		ALLOCATEPLANE(exy0)
		ALLOCATEPLANE(exz0)
		ALLOCATEPLANE(exy1)
		ALLOCATEPLANE(exz1)

		ALLOCATEPLANE(eyx0)
		ALLOCATEPLANE(eyz0)
		ALLOCATEPLANE(eyx1)
		ALLOCATEPLANE(eyz1)

		ALLOCATEPLANE(ezy0)
		ALLOCATEPLANE(ezx0)
		ALLOCATEPLANE(ezy1)
		ALLOCATEPLANE(ezx1)

		if(ret == ERR_OK)
		{
			for(size_t i=0;i<size;i++)
			{
				exy0[i] = exz0[i] = exy1[i] = exz1[i] = eyx0[i] = eyz0[i] = eyx1[i] = eyz1[i] = ezx0[i] = ezy0[i] = ezx1[i] = ezy1[i] = 0.0;
			}
		}
	}
	return ret;
}
/*
	apply boundary condition at space point (m,n,p)
	space point location is (x,y,z) = (m*ds, n*ds, p*ds)

	m,n,p=0,+-1,+-2,...,+-maxRadius
	because thickness = 1 this function is called when
	at least one of |m|, |n|, or |p| is maxRadius

	the code/algorithm is taken from book "understanding FDTD method" by John B. Schneider
	see https://github.com/john-b-schneider/uFDTD/blob/master/Code/Fdtd-3d/abc3dfirst.c
*/
void AbcFirstOrder::handleData(int m, int n, int p)
{
	size_t idx = SINDEX(m, n, p);
	size_t idxPlane;
	size_t idx2;
	bool atx=true,aty=true,atz=true;
	//
	if(m == -maxRadius)
	{
		//at back-plane (-maxRadius, y, z)
		//it is equivalent to plane "x0"
		/* corresponding to following code in the book "understanding FDTD method":
		Ey(mm, nn, pp) = Eyx0(nn, pp) + abccoef * (Ey(mm + 1, nn, pp) - Ey(mm, nn, pp));
		Eyx0(nn, pp) = Ey(mm + 1, nn, pp);
		*/
		idxPlane = PLANEINDEX(n, p);
		idx2 = SINDEX(m + 1, n, p);
		//
		_fields[idx].E.y = eyx0[idxPlane] + abccoef * (_fields[idx2].E.y - _fields[idx].E.y);
		eyx0[idxPlane] = _fields[idx2].E.y;
		//
		/* corresponding to following code in the book "understanding FDTD method":
		Ez(mm, nn, pp) = Ezx0(nn, pp) + abccoef * (Ez(mm + 1, nn, pp) - Ez(mm, nn, pp));
		Ezx0(nn, pp) = Ez(mm + 1, nn, pp);
		*/
		_fields[idx].E.z = ezx0[idxPlane] + abccoef * (_fields[idx2].E.z - _fields[idx].E.z);
		ezx0[idxPlane] = _fields[idx2].E.z;
	}
	else if(m == maxRadius)
	{
		//at front-plane (maRadius, y, z)
		//it is equivalent to plane "x1"
		idxPlane = PLANEINDEX(n, p);
		idx2 = SINDEX(m - 1, n, p);
		/* corresponding to following code in the book "understanding FDTD method":
		Ey(mm, nn, pp) = Eyx1(nn, pp) + abccoef * (Ey(mm - 1, nn, pp) - Ey(mm, nn, pp));
		Eyx1(nn, pp) = Ey(mm - 1, nn, pp);
		*/
		_fields[idx].E.y = eyx1[idxPlane] + abccoef * (_fields[idx2].E.y - _fields[idx].E.y);
		eyx1[idxPlane] = _fields[idx2].E.y;
		/* corresponding to following code in the book "understanding FDTD method":
		Ez(mm, nn, pp) = Ezx1(nn, pp) + abccoef * (Ez(mm - 1, nn, pp) - Ez(mm, nn, pp));
		Ezx1(nn, pp) = Ez(mm - 1, nn, pp);
		*/
		_fields[idx].E.z = ezx1[idxPlane] + abccoef * (_fields[idx2].E.z - _fields[idx].E.z);
		ezx1[idxPlane] = _fields[idx2].E.z;
	}
	else
	{
		atx = false;
	}
	//
	if(n == -maxRadius)
	{
		//at left plane (x, -maxRadius, z)
		//it is equivalent to plane "y0"
		idxPlane = PLANEINDEX(m, p);
		idx2 = SINDEX(m, n + 1, p);
		/* corresponding to following code in the book "understanding FDTD method":
		Ex(mm, nn, pp) = Exy0(mm, pp) + abccoef * (Ex(mm, nn + 1, pp) - Ex(mm, nn, pp));
		Exy0(mm, pp) = Ex(mm, nn + 1, pp);
		*/
		_fields[idx].E.x = exy0[idxPlane] + abccoef * (_fields[idx2].E.x - _fields[idx].E.x);
		exy0[idxPlane] = _fields[idx2].E.x;
		/* corresponding to following code in the book "understanding FDTD method":
		Ez(mm, nn, pp) = Ezy0(mm, pp) + abccoef * (Ez(mm, nn + 1, pp) - Ez(mm, nn, pp));
		Ezy0(mm, pp) = Ez(mm, nn + 1, pp);
		*/
		_fields[idx].E.z = ezy0[idxPlane] + abccoef * (_fields[idx2].E.z - _fields[idx].E.z);
		ezy0[idxPlane] = _fields[idx2].E.z;
	}
	else if(n == maxRadius)
	{
		//at right plane (x, maxRadius, z)
		//it is equivalent to plane "y1"
		idxPlane = PLANEINDEX(m, p);
		idx2 = SINDEX(m, n - 1, p);
		/* corresponding to following code in the book "understanding FDTD method":
		Ex(mm, nn, pp) = Exy1(mm, pp) + abccoef * (Ex(mm, nn - 1, pp) - Ex(mm, nn, pp));
		Exy1(mm, pp) = Ex(mm, nn - 1, pp);
		*/
		_fields[idx].E.x = exy1[idxPlane] + abccoef * (_fields[idx2].E.x - _fields[idx].E.x);
		exy1[idxPlane] = _fields[idx2].E.x;
		/* corresponding to following code in the book "understanding FDTD method":
		Ez(mm, nn, pp) = Ezy1(mm, pp) + abccoef * (Ez(mm, nn - 1, pp) - Ez(mm, nn, pp));
		Ezy1(mm, pp) = Ez(mm, nn - 1, pp);
		*/
		_fields[idx].E.z = ezy1[idxPlane] + abccoef * (_fields[idx2].E.z - _fields[idx].E.z);
		ezy1[idxPlane] = _fields[idx2].E.z;
	}
	else
	{
		aty = false;
	}
	//
	if(p == -maxRadius)
	{
		//at bottom plane (x, y, -maxRadius)
		//it is equivalent to plane "z0"
		idxPlane = PLANEINDEX(m, n);
		idx2 = SINDEX(m, n, p + 1);
		/* corresponding to following code in the book "understanding FDTD method":
		Ex(mm, nn, pp) = Exz0(mm, nn) + abccoef * (Ex(mm, nn, pp + 1) - Ex(mm, nn, pp));
		Exz0(mm, nn) = Ex(mm, nn, pp + 1);
		*/
		_fields[idx].E.x = exz0[idxPlane] + abccoef * (_fields[idx2].E.x - _fields[idx].E.x);
		exz0[idxPlane] = _fields[idx2].E.x;
		/* corresponding to following code in the book "understanding FDTD method":
		Ey(mm, nn, pp) = Eyz0(mm, nn) + abccoef * (Ey(mm, nn, pp + 1) - Ey(mm, nn, pp));
		Eyz0(mm, nn) = Ey(mm, nn, pp + 1);
		*/
		_fields[idx].E.y = eyz0[idxPlane] + abccoef * (_fields[idx2].E.y - _fields[idx].E.y);
		eyz0[idxPlane] = _fields[idx2].E.y;
	}
	else if(p == maxRadius)
	{
		//at top plane (x, y, maxRadius)
		//it is equivalent to plane "z1"
		idxPlane = PLANEINDEX(m, n);
		idx2 = SINDEX(m, n, p - 1);
		/* corresponding to following code in the book "understanding FDTD method":
		Ex(mm, nn, pp) = Exz1(mm, nn) + abccoef * (Ex(mm, nn, pp - 1) - Ex(mm, nn, pp));
		Exz1(mm, nn) = Ex(mm, nn, pp - 1);
		*/
		_fields[idx].E.x = exz1[idxPlane] + abccoef * (_fields[idx2].E.x - _fields[idx].E.x);
		exz1[idxPlane] = _fields[idx2].E.x;
		/* corresponding to following code in the book "understanding FDTD method":
		Ey(mm, nn, pp) = Eyz1(mm, nn) + abccoef * (Ey(mm, nn, pp - 1) - Ey(mm, nn, pp));
		Eyz1(mm, nn) = Ey(mm, nn, pp - 1);
		*/
		_fields[idx].E.y = eyz1[idxPlane] + abccoef * (_fields[idx2].E.y - _fields[idx].E.y);
		eyz1[idxPlane] = _fields[idx2].E.y;
	}
	else
	{
		atz = false;
	}
	//
	if(!atx && !aty && !atz)
	{
		//this function is callled for a space point not at a boundary plane; it is a programming error
		ret = ERR_RADIUS_HANDLE_DATA;
	}
}

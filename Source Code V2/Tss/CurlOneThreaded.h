#pragma once

/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

********************************************************************/

#include "Tss.h"
#include "CurlOne.h"
#include "LoopWorkDivider.h"

class CurlOneThreaded:virtual public CurlOne
{
private:
	Point3Dstruct *Field;
	unsigned int nx, ny, nz;     //computation domain. 0...nx; 0...ny; 0...nz
	unsigned int nxN1, nyN1, nzN1; //nx-1, ny-1, nz-1
	unsigned int nx1, ny1, nz1;    //nx+1, ny+1, nz+1
	//
	//
	Point3Dstruct *pNext;
	LoopWorkDivider *divider;
public:
	CurlOneThreaded();
	~CurlOneThreaded();
	void setSpaceRange(unsigned int nx0, unsigned int ny0, unsigned int nz0);
	size_t I(unsigned int i, unsigned int j, unsigned int k){ return k + nz1*(j + ny1*i); }
	void setDivider(LoopWorkDivider *d){ divider = d; }
	//
	//these memories are shared by threads
	void setFieldsMemory(Point3Dstruct *pField, Point3Dstruct *pCurl){ Field = pField; pNext = pCurl; }
	//work to be carried out in threads
	void calculateCurl(unsigned int th);
	//number of works divided into
	//unsigned int workCount(){ return divider.Count(); }
};


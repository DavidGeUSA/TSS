#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

calculate the third order curl
********************************************************************/
#include "Tss.h"

class CurlThreeB
{
private:
	Point3Dstruct *Field;
	unsigned int nx, ny, nz;       //computation domain. 0...nx; 0...ny; 0...nz
	unsigned int nxN1, nyN1, nzN1; //nx-1, ny-1, nz-1
	unsigned int nx1, ny1, nz1;    //nx+1, ny+1, nz+1
	//
	//work variables
	unsigned int i, j, k;
	size_t w;

public:
	CurlThreeB();
	~CurlThreeB();
	void setSpaceRange(unsigned int nx0, unsigned int ny0, unsigned int nz0);
	size_t I(unsigned int i, unsigned int j, unsigned int k){ return k + nz1*(j + ny1*i); }
	//third order
	int _getNextCurlUseFastMethod(Point3Dstruct *pField, Point3Dstruct *work, Point3Dstruct *work2, Point3Dstruct *pNext);

};


/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

calculate the third order curl
********************************************************************/
#include "CurlThreeB.h"
#include "CurlOne.h"
#include "CurlTwo.h"

CurlThreeB::CurlThreeB()
{
}


CurlThreeB::~CurlThreeB()
{
}

void CurlThreeB::setSpaceRange(unsigned int nx0, unsigned int ny0, unsigned int nz0)
{
	nx = nx0; ny = ny0; nz = nz0;
	nxN1 = nx - 1;
	nyN1 = ny - 1;
	nzN1 = nz - 1;
	//
	nx1 = nx + 1;
	ny1 = ny + 1;
	nz1 = nz + 1;
}

int CurlThreeB::_getNextCurlUseFastMethod(Point3Dstruct *pField, Point3Dstruct *work, Point3Dstruct *work2, Point3Dstruct *pNext)
{
	int ret = ERR_OK;
	Field = pField;
	return ret;
}

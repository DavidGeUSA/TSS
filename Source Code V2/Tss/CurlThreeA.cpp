/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

calculate the third order curl
********************************************************************/
#include "CurlThreeA.h"
#include "CurlOne.h"
#include "CurlTwo.h"

CurlThreeA::CurlThreeA()
{
}


CurlThreeA::~CurlThreeA()
{
}

void CurlThreeA::setSpaceRange(unsigned int nx0, unsigned int ny0, unsigned int nz0)
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
int CurlThreeA::_getNextCurlUseFastMethod(Point3Dstruct *pField, Point3Dstruct *work, Point3Dstruct *pNext)
{
	int ret = ERR_OK;
	CurlTwo c2;
	if (pNext == pField || pField == work || work == pNext)
		return ERR_CURL_FAILED;
	Field = pField;
	c2.setSpaceRange(nx, ny, nz);
	ret = c2._getNextCurlUseFastMethod(pField, work);
	if (ret == ERR_OK)
	{
		CurlOne c1;
		c1.setSpaceRange(nx, ny, nz);
		ret = c1._getNextCurlUseFastMethod(work, pNext);
	}
	return ret;
}

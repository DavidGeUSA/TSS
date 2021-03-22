
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

calculate first order curl
********************************************************************/
#include "CurlOne.h"

CurlOne::CurlOne()
{
	i0 = 0;
	i1 = 0;
	w0 = 0;
	nx = ny = nz = 0;
	Field = 0;
	pNext = 0;
}


CurlOne::~CurlOne()
{
}

void CurlOne::setSpaceRange(unsigned int nx0, unsigned int ny0, unsigned int nz0)
{
	nx = nx0; ny = ny0; nz = nz0;
	nxN1 = nx - 1;
	nyN1 = ny - 1;
	nzN1 = nz - 1;
	//
	nx1 = nx + 1;
	ny1 = ny + 1;
	nz1 = nz + 1;
	//
	i0 = 0;
	i1 = nx;
	w0 = 0;
}

//
inline double CurlOne::dxVx()   //insize boundary
{
	return 8 * (Field[I(i + 1, j, k)].x - Field[I(i - 1, j, k)].x) + Field[I(i - 2, j, k)].x - Field[I(i + 2, j, k)].x;
}
inline double CurlOne::dxVx0()  //at lower boundary
{
	return -25 * Field[w].x + 48 * Field[I(i + 1, j, k)].x - 36 * Field[I(i + 2, j, k)].x + 16 * Field[I(i + 3, j, k)].x - 3 * Field[I(i + 4, j, k)].x;
}
inline double CurlOne::dxVx1()  //near lower boundary
{
	return  -10 * Field[w].x + 18 * Field[I(i + 1, j, k)].x - 6 * Field[I(i + 2, j, k)].x + Field[I(i + 3, j, k)].x - 3 * Field[I(i - 1, j, k)].x;
}
inline double CurlOne::dxVxN1() //near upper boundary
{
	return 10 * Field[w].x - 18 * Field[I(i - 1, j, k)].x + 6 * Field[I(i - 2, j, k)].x - Field[I(i - 3, j, k)].x + 3 * Field[I(i + 1, j, k)].x;
}
inline double CurlOne::dxVxN()  //at upper boundary
{
	return 25 * Field[w].x - 48 * Field[I(i - 1, j, k)].x + 36 * Field[I(i - 2, j, k)].x - 16 * Field[I(i - 3, j, k)].x + 3 * Field[I(i - 4, j, k)].x;
}
double CurlOne::DxVx(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (i == 0)
	{
		return dxVx0();
	}
	if (i == 1)
	{
		return dxVx1();
	}
	if (i == nxN1)
	{
		return dxVxN1();
	}
	if (i == nx)
	{
		return dxVxN();
	}
	return dxVx();
}

//
inline double CurlOne::dyVy()   //insize boundary
{
	return  8 * (Field[I(i, j + 1, k)].y - Field[I(i, j - 1, k)].y) + Field[I(i, j - 2, k)].y - Field[I(i, j + 2, k)].y;
}
inline double CurlOne::dyVy0()  //at lower boundary
{
	return -25 * Field[w].y + 48 * Field[I(i, j + 1, k)].y - 36 * Field[I(i, j + 2, k)].y + 16 * Field[I(i, j + 3, k)].y - 3 * Field[I(i, j + 4, k)].y;
}
inline double CurlOne::dyVy1()  //near lower boundary
{
	return  -10 * Field[w].y + 18 * Field[I(i, j + 1, k)].y - 6 * Field[I(i, j + 2, k)].y + Field[I(i, j + 3, k)].y - 3 * Field[I(i, j - 1, k)].y;
}
inline double CurlOne::dyVyN1() //near upper boundary
{
	return 10 * Field[w].y - 18 * Field[I(i, j - 1, k)].y + 6 * Field[I(i, j - 2, k)].y - Field[I(i, j - 3, k)].y + 3 * Field[I(i, j + 1, k)].y;
}
inline double CurlOne::dyVyN()  //at upper boundary
{
	return 25 * Field[w].y - 48 * Field[I(i, j - 1, k)].y + 36 * Field[I(i, j - 2, k)].y - 16 * Field[I(i, j - 3, k)].y + 3 * Field[I(i, j - 4, k)].y;
}
double CurlOne::DyVy(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (j == 0)
	{
		return dyVy0();
	}
	if (j == 1)
	{
		return dyVy1();
	}
	if (j == nyN1)
	{
		return dyVyN1();
	}
	if (j == ny)
	{
		return dyVyN();
	}
	return dyVy();
}
//
inline double CurlOne::dzVz()   //insize boundary
{
	return 8 * (Field[w + 1].z - Field[w - 1].z) + Field[w - 2].z - Field[w + 2].z;
}
inline double CurlOne::dzVz0()  //at lower boundary
{
	return -25 * Field[w].z + 48 * Field[w + 1].z - 36 * Field[w + 2].z + 16 * Field[w + 3].z - 3 * Field[w + 4].z;
}
inline double CurlOne::dzVz1()  //near lower boundary
{
	return  -10 * Field[w].z + 18 * Field[w + 1].z - 6 * Field[w + 2].z + Field[w + 3].z - 3 * Field[w - 1].z;
}
inline double CurlOne::dzVzN1() //near upper boundary
{
	return 10 * Field[w].z - 18 * Field[w - 1].z + 6 * Field[w - 2].z - Field[w - 3].z + 3 * Field[w + 1].z;
}
inline double CurlOne::dzVzN()  //at upper boundary
{
	return 25 * Field[w].z - 48 * Field[w - 1].z + 36 * Field[w - 2].z - 16 * Field[w - 3].z + 3 * Field[w - 4].z;
}
double CurlOne::DzVz(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (k == 0)
	{
		return dzVz0();
	}
	if (k == 1)
	{
		return dzVz1();
	}
	if (k == nzN1)
	{
		return dzVzN1();
	}
	if (k == nz)
	{
		return dzVzN();
	}
	return dzVz();
}
inline double CurlOne::dyVz()   //insize boundary
{
	return  8 * (Field[I(i, j + 1, k)].z - Field[I(i, j - 1, k)].z) + Field[I(i, j - 2, k)].z - Field[I(i, j + 2, k)].z;
}
inline double CurlOne::dyVz0()  //at lower boundary
{
	return -25 * Field[w].z + 48 * Field[I(i, j + 1, k)].z - 36 * Field[I(i, j + 2, k)].z + 16 * Field[I(i, j + 3, k)].z - 3 * Field[I(i, j + 4, k)].z;
}
inline double CurlOne::dyVz1()  //near lower boundary
{
	return  -10 * Field[w].z + 18 * Field[I(i, j + 1, k)].z - 6 * Field[I(i, j + 2, k)].z + Field[I(i, j + 3, k)].z - 3 * Field[I(i, j - 1, k)].z;
}
inline double CurlOne::dyVzN1() //near upper boundary
{
	return  10 * Field[w].z - 18 * Field[I(i, j - 1, k)].z + 6 * Field[I(i, j - 2, k)].z - Field[I(i, j - 3, k)].z + 3 * Field[I(i, j + 1, k)].z;
}
inline double CurlOne::dyVzN()  //at upper boundary
{
	return 25 * Field[w].z - 48 * Field[I(i, j - 1, k)].z + 36 * Field[I(i, j - 2, k)].z - 16 * Field[I(i, j - 3, k)].z + 3 * Field[I(i, j - 4, k)].z;
}
//
inline double CurlOne::dzVy()   //insize boundary
{
	return 8 * (Field[w + 1].y - Field[w - 1].y) + Field[w - 2].y - Field[w + 2].y;
}
inline double CurlOne::dzVy0()  //at lower boundary
{
	return -25 * Field[w].y + 48 * Field[w + 1].y - 36 * Field[w + 2].y + 16 * Field[w + 3].y - 3 * Field[w + 4].y;
}
inline double CurlOne::dzVy1()  //near lower boundary
{
	return  -10 * Field[w].y + 18 * Field[w + 1].y - 6 * Field[w + 2].y + Field[w + 3].y - 3 * Field[w - 1].y;
}
inline double CurlOne::dzVyN1() //near upper boundary
{
	return 10 * Field[w].y - 18 * Field[w - 1].y + 6 * Field[w - 2].y - Field[w - 3].y + 3 * Field[w + 1].y;
}
inline double CurlOne::dzVyN()  //at upper boundary
{
	return 25 * Field[w].y - 48 * Field[w - 1].y + 36 * Field[w - 2].y - 16 * Field[w - 3].y + 3 * Field[w - 4].y;
}
//
inline double CurlOne::dzVx()   //insize boundary
{
	return 8 * (Field[w + 1].x - Field[w - 1].x) + Field[w - 2].x - Field[w + 2].x;
}
inline double CurlOne::dzVx0()  //at lower boundary
{
	return -25 * Field[w].x + 48 * Field[w + 1].x - 36 * Field[w + 2].x + 16 * Field[w + 3].x - 3 * Field[w + 4].x;
}
inline double CurlOne::dzVx1()  //near lower boundary
{
	return  -10 * Field[w].x + 18 * Field[w + 1].x - 6 * Field[w + 2].x + Field[w + 3].x - 3 * Field[w - 1].x;
}
inline double CurlOne::dzVxN1() //near upper boundary
{
	return 10 * Field[w].x - 18 * Field[w - 1].x + 6 * Field[w - 2].x - Field[w - 3].x + 3 * Field[w + 1].x;
}
inline double CurlOne::dzVxN()  //at upper boundary
{
	return 25 * Field[w].x - 48 * Field[w - 1].x + 36 * Field[w - 2].x - 16 * Field[w - 3].x + 3 * Field[w - 4].x;
}
//
inline double CurlOne::dxVz()   //insize boundary
{
	return 8 * (Field[I(i + 1, j, k)].z - Field[I(i - 1, j, k)].z) + Field[I(i - 2, j, k)].z - Field[I(i + 2, j, k)].z;
}
inline double CurlOne::dxVz0()  //at lower boundary
{
	return -25 * Field[w].z + 48 * Field[I(i + 1, j, k)].z - 36 * Field[I(i + 2, j, k)].z + 16 * Field[I(i + 3, j, k)].z - 3 * Field[I(i + 4, j, k)].z;
}
inline double CurlOne::dxVz1()  //near lower boundary
{
	return -10 * Field[w].z + 18 * Field[I(i + 1, j, k)].z - 6 * Field[I(i + 2, j, k)].z + Field[I(i + 3, j, k)].z - 3 * Field[I(i - 1, j, k)].z;
}
inline double CurlOne::dxVzN1() //near upper boundary
{
	return  10 * Field[w].z - 18 * Field[I(i - 1, j, k)].z + 6 * Field[I(i - 2, j, k)].z - Field[I(i - 3, j, k)].z + 3 * Field[I(i + 1, j, k)].z;
}
inline double CurlOne::dxVzN()  //at upper boundary
{
	return 25 * Field[w].z - 48 * Field[I(i - 1, j, k)].z + 36 * Field[I(i - 2, j, k)].z - 16 * Field[I(i - 3, j, k)].z + 3 * Field[I(i - 4, j, k)].z;
}
//
inline double CurlOne::dxVy()   //insize boundary
{
	return 8 * (Field[I(i + 1, j, k)].y - Field[I(i - 1, j, k)].y) + Field[I(i - 2, j, k)].y - Field[I(i + 2, j, k)].y;
}
inline double CurlOne::dxVy0()  //at lower boundary
{
	return -25 * Field[w].y + 48 * Field[I(i + 1, j, k)].y - 36 * Field[I(i + 2, j, k)].y + 16 * Field[I(i + 3, j, k)].y - 3 * Field[I(i + 4, j, k)].y;
}
inline double CurlOne::dxVy1()  //near lower boundary
{
	return -10 * Field[w].y + 18 * Field[I(i + 1, j, k)].y - 6 * Field[I(i + 2, j, k)].y + Field[I(i + 3, j, k)].y - 3 * Field[I(i - 1, j, k)].y;
}
inline double CurlOne::dxVyN1() //near upper boundary
{
	return 10 * Field[w].y - 18 * Field[I(i - 1, j, k)].y + 6 * Field[I(i - 2, j, k)].y - Field[I(i - 3, j, k)].y + 3 * Field[I(i + 1, j, k)].y;
}
inline double CurlOne::dxVyN()  //at upper boundary
{
	return 25 * Field[w].y - 48 * Field[I(i - 1, j, k)].y + 36 * Field[I(i - 2, j, k)].y - 16 * Field[I(i - 3, j, k)].y + 3 * Field[I(i - 4, j, k)].y;
}
//
inline double CurlOne::dyVx()   //insize boundary
{
	return 8 * (Field[I(i, j + 1, k)].x - Field[I(i, j - 1, k)].x) + Field[I(i, j - 2, k)].x - Field[I(i, j + 2, k)].x;
}
inline double CurlOne::dyVx0()  //at lower boundary
{
	return -25 * Field[w].x + 48 * Field[I(i, j + 1, k)].x - 36 * Field[I(i, j + 2, k)].x + 16 * Field[I(i, j + 3, k)].x - 3 * Field[I(i, j + 4, k)].x;
}
inline double CurlOne::dyVx1()  //near lower boundary
{
	return -10 * Field[w].x + 18 * Field[I(i, j + 1, k)].x - 6 * Field[I(i, j + 2, k)].x + Field[I(i, j + 3, k)].x - 3 * Field[I(i, j - 1, k)].x;
}
inline double CurlOne::dyVxN1() //near upper boundary
{
	return 10 * Field[w].x - 18 * Field[I(i, j - 1, k)].x + 6 * Field[I(i, j - 2, k)].x - Field[I(i, j - 3, k)].x + 3 * Field[I(i, j + 1, k)].x;
}
inline double CurlOne::dyVxN()  //at upper boundary
{
	return 25 * Field[w].x - 48 * Field[I(i, j - 1, k)].x + 36 * Field[I(i, j - 2, k)].x - 16 * Field[I(i, j - 3, k)].x + 3 * Field[I(i, j - 4, k)].x;
}

inline double CurlOne::dzVx1_dxVz1() //Cy at (1,*,1)
{
	return -10 * (Field[w].x - Field[w].z) + 18 * (Field[w + 1].x - Field[I(i + 1, j, k)].z) - 6 * (Field[w + 2].x - Field[I(i + 2, j, k)].z) + Field[w + 3].x - Field[I(i + 3, j, k)].z - 3 * (Field[w - 1].x - Field[I(i - 1, j, k)].z);
}

inline double CurlOne::dyVz0_dzVy0() //Cx at (*,0,0)
{
	return -25 * (Field[w].z - Field[w].y) + 48 * (Field[I(i, j + 1, k)].z - Field[w + 1].y) - 36 * (Field[I(i, j + 2, k)].z - Field[w + 2].y) + 16 * (Field[I(i, j + 3, k)].z - Field[w + 3].y) - 3 * (Field[I(i, j + 4, k)].z - Field[w + 4].y);
}

inline double CurlOne::dzVxN1_dxVz1() //Cy at (1,*,nz-1)
{
	return 10 * (Field[w].x + Field[w].z) - 18 * (Field[w - 1].x + Field[I(i + 1, j, k)].z) + 6 * (Field[w - 2].x + Field[I(i + 2, j, k)].z) - Field[w - 3].x - Field[I(i + 3, j, k)].z + 3 * (Field[w + 1].x + Field[I(i - 1, j, k)].z);
}

inline double CurlOne::dyVz0_dzVyN()  //Cx at (*,0,nz)
{
	return -25 * (Field[w].z + Field[w].y) + 48 * (Field[I(i, j + 1, k)].z + Field[w - 1].y) - 36 * (Field[I(i, j + 2, k)].z + Field[w - 2].y) + 16 * (Field[I(i, j + 3, k)].z + Field[w - 3].y) - 3 * (Field[I(i, j + 4, k)].z + Field[w - 4].y);
}

inline double CurlOne::dyVzN1_dzVy1() //Cx at (*,ny-1,1)
{
	return 10 * (Field[w].z + Field[w].y) - 18 * (Field[I(i, j - 1, k)].z + Field[w + 1].y) + 6 * (Field[I(i, j - 2, k)].z + Field[w + 2].y) - Field[I(i, j - 3, k)].z - Field[w + 3].y + 3 * (Field[I(i, j + 1, k)].z + Field[w - 1].y);
}

inline double CurlOne::dxVy1_dyVxN1() //Cz at (1,ny-1,*)
{
	return  -10 * (Field[w].y + Field[w].x) + 18 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].x) - 6 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].x) + Field[I(i + 3, j, k)].y + Field[I(i, j - 3, k)].x - 3 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].x);
}

inline double CurlOne::dyVzN1_dzVyN1() //Cx at (*,ny-1,nz-1)
{
	return 10 * (Field[w].z - Field[w].y) - 18 * (Field[I(i, j - 1, k)].z - Field[w - 1].y) + 6 * (Field[I(i, j - 2, k)].z - Field[w - 2].y) - Field[I(i, j - 3, k)].z + Field[w - 3].y + 3 * (Field[I(i, j + 1, k)].z - Field[w + 1].y);
}

inline double CurlOne::dyVzN_dzVy0()   //Cx at (*,ny,0)
{
	return 25 * (Field[w].z + Field[w].y) - 48 * (Field[I(i, j - 1, k)].z + Field[w + 1].y) + 36 * (Field[I(i, j - 2, k)].z + Field[w + 2].y) - 16 * (Field[I(i, j - 3, k)].z + Field[w + 3].y) + 3 * (Field[I(i, j - 4, k)].z + Field[w + 4].y);
}

inline double CurlOne::dyVzN_dzVyN()   //Cx at (*,ny,nz)
{
	return 25 * (Field[w].z - Field[w].y) - 48 * (Field[I(i, j - 1, k)].z - Field[w - 1].y) + 36 * (Field[I(i, j - 2, k)].z - Field[w - 2].y) - 16 * (Field[I(i, j - 3, k)].z - Field[w - 3].y) + 3 * (Field[I(i, j - 4, k)].z - Field[w - 4].y);
}

inline double CurlOne::dyVz_dzVy()     //Cx at (*,1<j<ny-1,1<k<nz-1)
{
	return  8 * (Field[I(i, j + 1, k)].z - Field[I(i, j - 1, k)].z + Field[w - 1].y - Field[w + 1].y) + Field[I(i, j - 2, k)].z - Field[w - 2].y - Field[I(i, j + 2, k)].z + Field[w + 2].y;
}

inline double CurlOne::dyVz1_dzVy1()   //Cx at (*,1,1)
{
	return -10 * (Field[w].z - Field[w].y) + 18 * (Field[I(i, j + 1, k)].z - Field[w + 1].y) - 6 * (Field[I(i, j + 2, k)].z - Field[w + 2].y) + Field[I(i, j + 3, k)].z - Field[w + 3].y - 3 * (Field[I(i, j - 1, k)].z - Field[w - 1].y);
}

inline double CurlOne::dzVx0_dxVz0()   //Cy at (0,*,0)
{
	return 25 * (Field[w].x - Field[w].z) + 48 * (Field[w + 1].x - Field[I(i + 1, j, k)].z) - 36 * (Field[w + 2].x - Field[I(i + 2, j, k)].z) + 16 * (Field[w + 3].x - Field[I(i + 3, j, k)].z) - 3 * (Field[w + 4].x - Field[I(i + 4, j, k)].z);
}

inline double CurlOne::dyVz1_dzVyN1()  //Cx at (*,1,nz-1)
{
	return -10 * (Field[w].z + Field[w].y) + 18 * (Field[I(i, j + 1, k)].z + Field[w - 1].y) - 6 * (Field[I(i, j + 2, k)].z + Field[w - 2].y) + Field[I(i, j + 3, k)].z + Field[w - 3].y - 3 * (Field[I(i, j - 1, k)].z + Field[w + 1].y);
}

inline double CurlOne::dzVxN_dxVz0()   //Cy at (0,*,nz)
{
	return 25 * (Field[w].x + Field[w].z) - 48 * (Field[w - 1].x + Field[I(i + 1, j, k)].z) + 36 * (Field[w - 2].x + Field[I(i + 2, j, k)].z) - 16 * (Field[w - 3].x + Field[I(i + 3, j, k)].z) + 3 * (Field[w - 4].x + Field[I(i + 4, j, k)].z);
}

inline double CurlOne::dxVy0_dyVx0()   //Cz at (0,0,*)
{
	return -25 * (Field[w].y - Field[w].x) + 48 * (Field[I(i + 1, j, k)].y - Field[I(i, j + 1, k)].x) - 36 * (Field[I(i + 2, j, k)].y - Field[I(i, j + 2, k)].x) + 16 * (Field[I(i + 3, j, k)].y - Field[I(i, j + 3, k)].x) - 3 * (Field[I(i + 4, j, k)].y - Field[I(i, j + 4, k)].x);
}

inline double CurlOne::dxVy0_dyVxN()   //Cz at (0,ny,*)
{
	return -25 * (Field[w].y + Field[w].x) + 48 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].x) - 36 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].x) + 16 * (Field[I(i + 3, j, k)].y + Field[I(i, j - 3, k)].x) - 3 * (Field[I(i + 4, j, k)].y + Field[I(i, j - 4, k)].x);
}

inline double CurlOne::dzVx1_dxVzN1()  //Cy at (nx-1,*,1)
{
	return -10 * (Field[w].x + Field[w].z) + 18 * (Field[w + 1].x + Field[I(i - 1, j, k)].z) - 6 * (Field[w + 2].x + Field[I(i - 2, j, k)].z) + Field[w + 3].x + Field[I(i - 3, j, k)].z - 3 * (Field[w - 1].x + Field[I(i + 1, j, k)].z);
}

inline double CurlOne::dxVyN1_dyVx1()  //Cz at (nx-1,1,*)
{
	return  10 * (Field[w].y + Field[w].x) - 18 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].x) + 6 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].x) - Field[I(i - 3, j, k)].y - Field[I(i, j + 3, k)].x + 3 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].x);
}

inline double CurlOne::dzVxN1_dxVzN1() //Cy at (nx-1,*,nz-1)
{
	return 10 * (Field[w].x - Field[w].z) - 18 * (Field[w - 1].x - Field[I(i - 1, j, k)].z) + 6 * (Field[w - 2].x - Field[I(i - 2, j, k)].z) - Field[w - 3].x + Field[I(i - 3, j, k)].z + 3 * (Field[w + 1].x - Field[I(i + 1, j, k)].z);
}

inline double CurlOne::dxVyN1_dyVxN1() //Cz at (nx-1,ny-1,*)
{
	return 10 * (Field[w].y - Field[w].x) - 18 * (Field[I(i - 1, j, k)].y - Field[I(i, j - 1, k)].x) + 6 * (Field[I(i - 2, j, k)].y - Field[I(i, j - 2, k)].x) - Field[I(i - 3, j, k)].y + Field[I(i, j - 3, k)].x + 3 * (Field[I(i + 1, j, k)].y - Field[I(i, j + 1, k)].x);
}

inline double CurlOne::dzVx0_dxVzN()   //Cy at (nx,*,0)
{
	return -25 * (Field[w].x + Field[w].z) + 48 * (Field[w + 1].x + Field[I(i - 1, j, k)].z) - 36 * (Field[w + 2].x + Field[I(i - 2, j, k)].z) + 16 * (Field[w + 3].x + Field[I(i - 3, j, k)].z) - 3 * (Field[w + 4].x + Field[I(i - 4, j, k)].z);
}

inline double CurlOne::dzVxN_dxVzN()   //Cy at (nx, *, nz)
{
	return 25 * (Field[w].x - Field[w].z) - 48 * (Field[w - 1].x - Field[I(i - 1, j, k)].z) + 36 * (Field[w - 2].x - Field[I(i - 2, j, k)].z) - 16 * (Field[w - 3].x - Field[I(i - 3, j, k)].z) + 3 * (Field[w - 4].x - Field[I(i - 4, j, k)].z);
}

inline double CurlOne::dxVyN_dyVx0()   //Cz at (nx, 0, *)
{
	return 25 * (Field[w].y + Field[w].x) - 48 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].x) + 36 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].x) - 16 * (Field[I(i - 3, j, k)].y + Field[I(i, j + 3, k)].x) + 3 * (Field[I(i - 4, j, k)].y + Field[I(i, j + 4, k)].x);
}

inline double CurlOne::dxVyN_dyVxN()   //Cz at (nx, ny, *)
{
	return 25 * (Field[w].y - Field[w].x) - 48 * (Field[I(i - 1, j, k)].y - Field[I(i, j - 1, k)].x) + 36 * (Field[I(i - 2, j, k)].y - Field[I(i, j - 2, k)].x) - 16 * (Field[I(i - 3, j, k)].y - Field[I(i, j - 3, k)].x) + 3 * (Field[I(i - 4, j, k)].y - Field[I(i, j - 4, k)].x);
}

inline double CurlOne::dzVx_dxVz()     //Cy at (1<i<nx-1,*,1<k<nz-1)
{
	return 8 * (Field[w + 1].x - Field[w - 1].x - Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z) + Field[w - 2].x - Field[w + 2].x - Field[I(i - 2, j, k)].z + Field[I(i + 2, j, k)].z;
}

inline double CurlOne::dxVy_dyVx()     //Cz at (1<i<nx-1,1<j<ny-1,*)
{
	return 8 * (Field[I(i + 1, j, k)].y - Field[I(i - 1, j, k)].y - Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x) + Field[I(i - 2, j, k)].y - Field[I(i + 2, j, k)].y - Field[I(i, j - 2, k)].x + Field[I(i, j + 2, k)].x;
}

inline double CurlOne::dxVy1_dyVx1()   //Cz at (1,1,*)
{
	return -10 * (Field[w].y - Field[w].x) + 18 * (Field[I(i + 1, j, k)].y - Field[I(i, j + 1, k)].x) - 6 * (Field[I(i + 2, j, k)].y - Field[I(i, j + 2, k)].x) + Field[I(i + 3, j, k)].y - Field[I(i, j + 3, k)].x - 3 * (Field[I(i - 1, j, k)].y - Field[I(i, j - 1, k)].x);
}

//test
double CurlOne::DyVx(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (j == 0)
	{
		return dyVx0();
	}
	if (j == 1)
	{
		return dyVx1();
	}
	if (j == nyN1)
	{
		return dyVxN1();
	}
	if (j == ny)
	{
		return dyVxN();
	}
	return dyVx();
}
double CurlOne::DzVy(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (k == 0)
	{
		return dzVy0();
	}
	if (k == 1)
	{
		return dzVy1();
	}
	if (k == nzN1)
	{
		return dzVyN1();
	}
	if (k == nz)
	{
		return dzVyN();
	}
	return dzVy();
}
double CurlOne::DxVz(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (i == 0)
	{
		return dxVz0();
	}
	if (i == 1)
	{
		return dxVz1();
	}
	if (i == nxN1)
	{
		return dxVzN1();
	}
	if (i == nx)
	{
		return dxVzN();
	}
	return dxVz();
}

double CurlOne::DzVx(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (k == 0)
	{
		return dzVx0();
	}
	if (k == 1)
	{
		return dzVx1();
	}
	if (k == nzN1)
	{
		return dzVxN1();
	}
	if (k == nz)
	{
		return dzVxN();
	}
	return dzVx();
}

double CurlOne::DxVy(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (i == 0)
	{
		return dxVy0();
	}
	if (i == 1)
	{
		return dxVy1();
	}
	if (i == nxN1)
	{
		return dxVyN1();
	}
	if (i == nx)
	{
		return dxVyN();
	}
	return dxVy();
}

double CurlOne::DyVz(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (j == 0)
	{
		return dyVz0();
	}
	if (j == 1)
	{
		return dyVz1();
	}
	if (j == nyN1)
	{
		return dyVzN1();
	}
	if (j == ny)
	{
		return dyVzN();
	}
	return dyVz();
}

double CurlOne::Divergence(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0, size_t w0)
{
	double d = 0;
	Field = f;
	i = i0; j = j0; k = k0;
	w = w0;
	if (i == 0)
	{
		d += dxVx0();
	}
	else if (i == 1)
	{
		d += dxVx1();
	}
	else if (i == nxN1)
	{
		d += dxVxN1();
	}
	else if (i == nx)
	{
		d += dxVxN();
	}
	else
		d += dxVx();
	if (j == 0)
	{
		d += dyVy0();
	}
	else if (j == 1)
	{
		d += dyVy1();
	}
	else if (j == nyN1)
	{
		d += dyVyN1();
	}
	else if (j == ny)
	{
		d += dyVyN();
	}
	else
		d += dyVy();
	if (k == 0)
	{
		d += dzVz0();
	}
	else if (k == 1)
	{
		d += dzVz1();
	}
	else if (k == nzN1)
	{
		d += dzVzN1();
	}
	else if (k == nz)
	{
		d += dzVzN();
	}
	else
		d += dzVz();
	return d;
}

/*
	the results need to be divided by 12
*/
int CurlOne::_getNextCurlUseFastMethod(Point3Dstruct *pCurrent, Point3Dstruct *pNextCurl)
{
	int ret = ERR_OK;
	pNext = pNextCurl;
	if (pNext == pCurrent)
		return ERR_CURL_FAILED;
	//
	Field = pCurrent;
	//
	calculateCurl();	
	return ret;
}
inline void CurlOne::calculateCurl()
{
		w = w0;
		for (i = i0; i <= i1; i++)
		{
			for (j = 0; j <= ny; j++)
			{
				for (k = 0; k <= nz; k++)
				{
					if (i == 1)
					{
						//x near the lower boundary
						if (j == 1)
						{
							//y near the lower boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(1,1,1)
								pNext[w].x = dyVz1_dzVy1();
								pNext[w].y = dzVx1_dxVz1();
								pNext[w].z = dxVy1_dyVx1();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(1,1,0)
								pNext[w].x = dyVz1() - dzVy0();
								pNext[w].y = dzVx0() - dxVz1();
								pNext[w].z = dxVy1_dyVx1();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(1,1,nz-1)
								pNext[w].x = dyVz1_dzVyN1();
								pNext[w].y = dzVxN1_dxVz1();
								pNext[w].z = dxVy1_dyVx1();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(1,1,nz)
								pNext[w].x = dyVz1() - dzVyN();
								pNext[w].y = dzVxN() - dxVz1();
								pNext[w].z = dxVy1_dyVx1();
							}
							else
							{
								//z inside the boundary
								//(1,1,1<k<nz-1)
								pNext[w].x = dyVz1() - dzVy();
								pNext[w].y = dzVx() - dxVz1();
								pNext[w].z = dxVy1_dyVx1();
							}
						}
						else if (j == 0)
						{
							//y at the lower boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(1,0,1)
								pNext[w].x = dyVz0() - dzVy1();
								pNext[w].y = dzVx1_dxVz1();
								pNext[w].z = dxVy1() - dyVx0();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(1,0,0)
								pNext[w].x = dyVz0_dzVy0();
								pNext[w].y = dzVx0() - dxVz1();
								pNext[w].z = dxVy1() - dyVx0();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(1,0,nz-1)
								pNext[w].x = dyVz0() - dzVyN1();
								pNext[w].y = dzVxN1_dxVz1();
								pNext[w].z = dxVy1() - dyVx0();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(1,0,nz)
								pNext[w].x = dyVz0_dzVyN();
								pNext[w].y = dzVxN() - dxVz1();
								pNext[w].z = dxVy1() - dyVx0();
							}
							else
							{
								//z inside the boundary
								//(1,0,1<k<nz-1)
								pNext[w].x = dyVz0() - dzVy();
								pNext[w].y = dzVx() - dxVz1();
								pNext[w].z = dxVy1() - dyVx0();
							}
						}
						else if (j == nyN1)
						{
							//y near the upper boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(1,ny-1,1)
								pNext[w].x = dyVzN1_dzVy1();
								pNext[w].y = dzVx1_dxVz1();
								pNext[w].z = dxVy1_dyVxN1();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(1,ny-1,0)
								pNext[w].x = dyVzN1() - dzVy0();
								pNext[w].y = dzVx0() - dxVz1();
								pNext[w].z = dxVy1_dyVxN1();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(1,ny-1,nz-1)
								pNext[w].x = dyVzN1_dzVyN1();
								pNext[w].y = dzVxN1_dxVz1();
								pNext[w].z = dxVy1_dyVxN1();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(1,ny-1,nz)
								pNext[w].x = dyVzN1() - dzVyN();
								pNext[w].y = dzVxN() - dxVz1();
								pNext[w].z = dxVy1_dyVxN1();
							}
							else
							{
								//z inside the boundary
								//(1,ny-1,1<k<nz-1)
								pNext[w].x = dyVzN1() - dzVy();
								pNext[w].y = dzVx() - dxVz1();
								pNext[w].z = dxVy1_dyVxN1();
							}
						}
						else if (j == ny)
						{
							//y at the upper boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(1,ny,1)
								pNext[w].x = dyVzN() - dzVy1();
								pNext[w].y = dzVx1_dxVz1();
								pNext[w].z = dxVy1() - dyVxN();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(1,ny,0)
								pNext[w].x = dyVzN_dzVy0();
								pNext[w].y = dzVx0() - dxVz1();
								pNext[w].z = dxVy1() - dyVxN();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(1,ny,nz-1)
								pNext[w].x = dyVzN() - dzVyN1();
								pNext[w].y = dzVxN1_dxVz1();
								pNext[w].z = dxVy1() - dyVxN();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(1,ny,nz)
								pNext[w].x = dyVzN_dzVyN();
								pNext[w].y = dzVxN() - dxVz1();
								pNext[w].z = dxVy1() - dyVxN();
							}
							else
							{
								//z inside the boundary
								//(1,ny,1<k<nz-1)
								pNext[w].x = dyVzN() - dzVy();
								pNext[w].y = dzVx() - dxVz1();
								pNext[w].z = dxVy1() - dyVxN();
							}
						}
						else
						{
							//y inside boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(1,1<j<ny-1,1)
								pNext[w].x = dyVz() - dzVy1();
								pNext[w].y = dzVx1_dxVz1();
								pNext[w].z = dxVy1() - dyVx();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(1,1<j<ny-1,0)
								pNext[w].x = dyVz() - dzVy0();
								pNext[w].y = dzVx0() - dxVz1();
								pNext[w].z = dxVy1() - dyVx();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(1,1<j<ny-1,nz-1)
								pNext[w].x = dyVz() - dzVyN1();
								pNext[w].y = dzVxN1_dxVz1();
								pNext[w].z = dxVy1() - dyVx();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(1,1<j<ny-1,nz)
								pNext[w].x = dyVz() - dzVyN();
								pNext[w].y = dzVxN() - dxVz1();
								pNext[w].z = dxVy1() - dyVx();
							}
							else
							{
								//z inside the boundary
								//(1,1<j<ny-1,1<k<nz-1)
								pNext[w].x = dyVz_dzVy();
								pNext[w].y = dzVx() - dxVz1();
								pNext[w].z = dxVy1() - dyVx();
							}
						}
					}
					else if (i == 0)
					{
						//x at the lower boundary
						if (j == 1)
						{
							//y near the lower boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(0,1,1)
								pNext[w].x = dyVz1_dzVy1();
								pNext[w].y = dzVx1() - dxVz0();
								pNext[w].z = dxVy0() - dyVx1();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(0,1,0)
								pNext[w].x = dyVz1() - dzVy0();
								pNext[w].y = dzVx0_dxVz0();
								pNext[w].z = dxVy0() - dyVx1();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(0,1,nz-1)
								pNext[w].x = dyVz1_dzVyN1();
								pNext[w].y = dzVxN1() - dxVz0();
								pNext[w].z = dxVy0() - dyVx1();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(0,1,nz)
								pNext[w].x = dyVz1() - dzVyN();
								pNext[w].y = dzVxN_dxVz0();
								pNext[w].z = dxVy0() - dyVx1();
							}
							else
							{
								//z inside the boundary
								//(0,1,1<k<nz-1)
								pNext[w].x = dyVz1() - dzVy();
								pNext[w].y = dzVx() - dxVz0();
								pNext[w].z = dxVy0() - dyVx1();
							}
						}
						else if (j == 0)
						{
							//y at the lower boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(0,0,1)
								pNext[w].x = dyVz0() - dzVy1();
								pNext[w].y = dzVx1() - dxVz0();
								pNext[w].z = dxVy0_dyVx0();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(0,0,0)
								pNext[w].x = dyVz0_dzVy0();
								pNext[w].y = dzVx0_dxVz0();
								pNext[w].z = dxVy0_dyVx0();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(0,0,nz-1)
								pNext[w].x = dyVz0() - dzVyN1();
								pNext[w].y = dzVxN1() - dxVz0();
								pNext[w].z = dxVy0_dyVx0();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(0,0,nz)
								pNext[w].x = dyVz0_dzVyN();
								pNext[w].y = dzVxN_dxVz0();
								pNext[w].z = dxVy0_dyVx0();
							}
							else
							{
								//z inside the boundary
								//(0,0,1<k<nz-1)
								pNext[w].x = dyVz0() - dzVy();
								pNext[w].y = dzVx() - dxVz0();
								pNext[w].z = dxVy0_dyVx0();
							}
						}
						else if (j == nyN1)
						{
							//y near the upper boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(0,ny-1,1)
								pNext[w].x = dyVzN1_dzVy1();
								pNext[w].y = dzVx1() - dxVz0();
								pNext[w].z = dxVy0() - dyVxN1();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(0,ny-1,0)
								pNext[w].x = dyVzN1() - dzVy0();
								pNext[w].y = dzVx0_dxVz0();
								pNext[w].z = dxVy0() - dyVxN1();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(0,ny-1,nz-1)
								pNext[w].x = dyVzN1_dzVyN1();
								pNext[w].y = dzVxN1() - dxVz0();
								pNext[w].z = dxVy0() - dyVxN1();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(0,ny-1,nz)
								pNext[w].x = dyVzN1() - dzVyN();
								pNext[w].y = dzVxN_dxVz0();
								pNext[w].z = dxVy0() - dyVxN1();
							}
							else
							{
								//z inside the boundary
								//(0,ny-1,1<k<nz-1)
								pNext[w].x = dyVzN1() - dzVy();
								pNext[w].y = dzVx() - dxVz0();
								pNext[w].z = dxVy0() - dyVxN1();
							}
						}
						else if (j == ny)
						{
							//y at the upper boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(0,ny,1)
								pNext[w].x = dyVzN() - dzVy1();
								pNext[w].y = dzVx1() - dxVz0();
								pNext[w].z = dxVy0_dyVxN();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(0,ny,0)
								pNext[w].x = dyVzN_dzVy0();
								pNext[w].y = dzVx0_dxVz0();
								pNext[w].z = dxVy0_dyVxN();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(0,ny,nz-1)
								pNext[w].x = dyVzN() - dzVyN1();
								pNext[w].y = dzVxN1() - dxVz0();
								pNext[w].z = dxVy0_dyVxN();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(0,ny,nz)
								pNext[w].x = dyVzN_dzVyN();
								pNext[w].y = dzVxN_dxVz0();
								pNext[w].z = dxVy0_dyVxN();
							}
							else
							{
								//z inside the boundary
								//(0,ny,1<k<nz-1)
								pNext[w].x = dyVzN() - dzVy();
								pNext[w].y = dzVx() - dxVz0();
								pNext[w].z = dxVy0_dyVxN();
							}
						}
						else
						{
							//y inside boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(0,1<j<ny-1,1)
								pNext[w].x = dyVz() - dzVy1();
								pNext[w].y = dzVx1() - dxVz0();
								pNext[w].z = dxVy0() - dyVx();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(0,1<j<ny-1,0)
								pNext[w].x = dyVz() - dzVy0();
								pNext[w].y = dzVx0_dxVz0();
								pNext[w].z = dxVy0() - dyVx();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(0, 1<j<ny - 1, nz-1)
								pNext[w].x = dyVz() - dzVyN1();
								pNext[w].y = dzVxN1() - dxVz0();
								pNext[w].z = dxVy0() - dyVx();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(0,1<j<ny-1,nz)
								pNext[w].x = dyVz() - dzVyN();
								pNext[w].y = dzVxN_dxVz0();
								pNext[w].z = dxVy0() - dyVx();
							}
							else
							{
								//z inside the boundary
								//(0,1<j<ny-1,1<k<nz-1)
								pNext[w].x = dyVz_dzVy();
								pNext[w].y = dzVx() - dxVz0();
								pNext[w].z = dxVy0() - dyVx();
							}
						}
					}
					else if (i == nxN1)
					{
						//x near the upper boundary
						if (j == 1)
						{
							//y near the lower boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(nx-1,1,1)
								pNext[w].x = dyVz1_dzVy1();
								pNext[w].y = dzVx1_dxVzN1();
								pNext[w].z = dxVyN1_dyVx1();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(nx-1,1,0)
								pNext[w].x = dyVz1() - dzVy0();
								pNext[w].y = dzVx0() - dxVzN1();
								pNext[w].z = dxVyN1_dyVx1();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(nx-1,1,nz-1)
								pNext[w].x = dyVz1_dzVyN1();
								pNext[w].y = dzVxN1_dxVzN1();
								pNext[w].z = dxVyN1_dyVx1();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(nx-1,1,nz)
								pNext[w].x = dyVz1() - dzVyN();
								pNext[w].y = dzVxN() - dxVzN1();
								pNext[w].z = dxVyN1_dyVx1();
							}
							else
							{
								//z inside the boundary
								//(nx-1,1,1<k<nz-1)
								pNext[w].x = dyVz1() - dzVy();
								pNext[w].y = dzVx() - dxVzN1();
								pNext[w].z = dxVyN1_dyVx1();
							}
						}
						else if (j == 0)
						{
							//y at the lower boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(nx-1,0,1)
								pNext[w].x = dyVz0() - dzVy1();
								pNext[w].y = dzVx1_dxVzN1();
								pNext[w].z = dxVyN1() - dyVx0();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(nx-1,0,0)
								pNext[w].x = dyVz0_dzVy0();
								pNext[w].y = dzVx0() - dxVzN1();
								pNext[w].z = dxVyN1() - dyVx0();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(nx-1,0,nz-1)
								pNext[w].x = dyVz0() - dzVyN1();
								pNext[w].y = dzVxN1_dxVzN1();
								pNext[w].z = dxVyN1() - dyVx0();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(nx-1,0,nz)
								pNext[w].x = dyVz0_dzVyN();
								pNext[w].y = dzVxN() - dxVzN1();
								pNext[w].z = dxVyN1() - dyVx0();
							}
							else
							{
								//z inside the boundary
								//(nx-1,0,1<k<nz-1)
								pNext[w].x = dyVz0() - dzVy();
								pNext[w].y = dzVx() - dxVzN1();
								pNext[w].z = dxVyN1() - dyVx0();
							}
						}
						else if (j == nyN1)
						{
							//y near the upper boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(nx-1,ny-1,1)
								pNext[w].x = dyVzN1_dzVy1();
								pNext[w].y = dzVx1_dxVzN1();
								pNext[w].z = dxVyN1_dyVxN1();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(nx-1,ny-1,0)
								pNext[w].x = dyVzN1() - dzVy0();
								pNext[w].y = dzVx0() - dxVzN1();
								pNext[w].z = dxVyN1_dyVxN1();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(nx-1,ny-1,nz-1)
								pNext[w].x = dyVzN1_dzVyN1();
								pNext[w].y = dzVxN1_dxVzN1();
								pNext[w].z = dxVyN1_dyVxN1();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(nx-1,ny-1,nz)
								pNext[w].x = dyVzN1() - dzVyN();
								pNext[w].y = dzVxN() - dxVzN1();
								pNext[w].z = dxVyN1_dyVxN1();
							}
							else
							{
								//z inside the boundary
								//(nx-1,ny-1,1<k<nz-1)
								pNext[w].x = dyVzN1() - dzVy();
								pNext[w].y = dzVx() - dxVzN1();
								pNext[w].z = dxVyN1_dyVxN1();
							}
						}
						else if (j == ny)
						{
							//y at the upper boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(nx-1,ny,1)
								pNext[w].x = dyVzN() - dzVy1();
								pNext[w].y = dzVx1_dxVzN1();
								pNext[w].z = dxVyN1() - dyVxN();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(nx-1,ny,0)
								pNext[w].x = dyVzN_dzVy0();
								pNext[w].y = dzVx0() - dxVzN1();
								pNext[w].z = dxVyN1() - dyVxN();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(nx-1,ny,nz-1)
								pNext[w].x = dyVzN() - dzVyN1();
								pNext[w].y = dzVxN1_dxVzN1();
								pNext[w].z = dxVyN1() - dyVxN();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(nx-1,ny,nz)
								pNext[w].x = dyVzN_dzVyN();
								pNext[w].y = dzVxN() - dxVzN1();
								pNext[w].z = dxVyN1() - dyVxN();
							}
							else
							{
								//z inside the boundary
								//(nx-1,ny,1<k<nz-1)
								pNext[w].x = dyVzN() - dzVy();
								pNext[w].y = dzVx() - dxVzN1();
								pNext[w].z = dxVyN1() - dyVxN();
							}
						}
						else
						{
							//y inside boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(nx-1,1<j<ny-1,1)
								pNext[w].x = dyVz() - dzVy1();
								pNext[w].y = dzVx1_dxVzN1();
								pNext[w].z = dxVyN1() - dyVx();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(nx-1,1<j<ny-1,0)
								pNext[w].x = dyVz() - dzVy0();
								pNext[w].y = dzVx0() - dxVzN1();
								pNext[w].z = dxVyN1() - dyVx();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(nx-1,1<j<ny-1,nz-1)
								pNext[w].x = dyVz() - dzVyN1();
								pNext[w].y = dzVxN1_dxVzN1();
								pNext[w].z = dxVyN1() - dyVx();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(nx-1,1<j<ny-1,nz)
								pNext[w].x = dyVz() - dzVyN();
								pNext[w].y = dzVxN() - dxVzN1();
								pNext[w].z = dxVyN1() - dyVx();
							}
							else
							{
								//z inside the boundary
								//(nx-1,1<j<ny-1,1<k<nz-1)
								pNext[w].x = dyVz_dzVy();
								pNext[w].y = dzVx() - dxVzN1();
								pNext[w].z = dxVyN1() - dyVx();
							}
						}
					}
					else if (i == nx)
					{
						//x at the upper boundary
						if (j == 1)
						{
							//y near the lower boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(nx,1,1)
								pNext[w].x = dyVz1_dzVy1();
								pNext[w].y = dzVx1() - dxVzN();
								pNext[w].z = dxVyN() - dyVx1();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(nx,1,0)
								pNext[w].x = dyVz1() - dzVy0();
								pNext[w].y = dzVx0_dxVzN();
								pNext[w].z = dxVyN() - dyVx1();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(nx,1,nz-1)
								pNext[w].x = dyVz1_dzVyN1();
								pNext[w].y = dzVxN1() - dxVzN();
								pNext[w].z = dxVyN() - dyVx1();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(nx,1,nz)
								pNext[w].x = dyVz1() - dzVyN();
								pNext[w].y = dzVxN_dxVzN();
								pNext[w].z = dxVyN() - dyVx1();
							}
							else
							{
								//z inside the boundary
								//(nx,1,1<k<nz-1)
								pNext[w].x = dyVz1() - dzVy();
								pNext[w].y = dzVx() - dxVzN();
								pNext[w].z = dxVyN() - dyVx1();
							}
						}
						else if (j == 0)
						{
							//y at the lower boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(nx,0,1)
								pNext[w].x = dyVz0() - dzVy1();
								pNext[w].y = dzVx1() - dxVzN();
								pNext[w].z = dxVyN_dyVx0();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(nx,0,0)
								pNext[w].x = dyVz0_dzVy0();
								pNext[w].y = dzVx0_dxVzN();
								pNext[w].z = dxVyN_dyVx0();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(nx,0,nz-1)
								pNext[w].x = dyVz0() - dzVyN1();
								pNext[w].y = dzVxN1() - dxVzN();
								pNext[w].z = dxVyN_dyVx0();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(nx,0,nz)
								pNext[w].x = dyVz0_dzVyN();
								pNext[w].y = dzVxN_dxVzN();
								pNext[w].z = dxVyN_dyVx0();
							}
							else
							{
								//z inside the boundary
								//(nx,0,1<k<nz-1)
								pNext[w].x = dyVz0() - dzVy();
								pNext[w].y = dzVx() - dxVzN();
								pNext[w].z = dxVyN_dyVx0();
							}
						}
						else if (j == nyN1)
						{
							//y near the upper boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(nx,ny-1,1)
								pNext[w].x = dyVzN1_dzVy1();
								pNext[w].y = dzVx1() - dxVzN();
								pNext[w].z = dxVyN() - dyVxN1();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(nx,ny-1,0)
								pNext[w].x = dyVzN1() - dzVy0();
								pNext[w].y = dzVx0_dxVzN();
								pNext[w].z = dxVyN() - dyVxN1();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(nx,ny-1,nz-1)
								pNext[w].x = dyVzN1_dzVyN1();
								pNext[w].y = dzVxN1() - dxVzN();
								pNext[w].z = dxVyN() - dyVxN1();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(nx,ny-1,nz)
								pNext[w].x = dyVzN1() - dzVyN();
								pNext[w].y = dzVxN_dxVzN();
								pNext[w].z = dxVyN() - dyVxN1();
							}
							else
							{
								//z inside the boundary
								//(nx,ny-1,1<k<nz-1)
								pNext[w].x = dyVzN1() - dzVy();
								pNext[w].y = dzVx() - dxVzN();
								pNext[w].z = dxVyN() - dyVxN1();
							}
						}
						else if (j == ny)
						{
							//y at the upper boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(nx,ny,1)
								pNext[w].x = dyVzN() - dzVy1();
								pNext[w].y = dzVx1() - dxVzN();
								pNext[w].z = dxVyN_dyVxN();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(nx,ny,0)
								pNext[w].x = dyVzN_dzVy0();
								pNext[w].y = dzVx0_dxVzN();
								pNext[w].z = dxVyN_dyVxN();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(nx,ny,nz-1)
								pNext[w].x = dyVzN() - dzVyN1();
								pNext[w].y = dzVxN1() - dxVzN();
								pNext[w].z = dxVyN_dyVxN();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(nx,ny,nz)
								pNext[w].x = dyVzN_dzVyN();
								pNext[w].y = dzVxN_dxVzN();
								pNext[w].z = dxVyN_dyVxN();
							}
							else
							{
								//z inside the boundary
								//(nx,ny,1<k<nz-1)
								pNext[w].x = dyVzN() - dzVy();
								pNext[w].y = dzVx() - dxVzN();
								pNext[w].z = dxVyN_dyVxN();
							}
						}
						else
						{
							//y inside boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(nx,1<j<ny-1,1)
								pNext[w].x = dyVz() - dzVy1();
								pNext[w].y = dzVx1() - dxVzN();
								pNext[w].z = dxVyN() - dyVx();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(nx,1<j<ny-1,0)
								pNext[w].x = dyVz() - dzVy0();
								pNext[w].y = dzVx0_dxVzN();
								pNext[w].z = dxVyN() - dyVx();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(nx,1<j<ny-1,nz-1)
								pNext[w].x = dyVz() - dzVyN1();
								pNext[w].y = dzVxN1() - dxVzN();
								pNext[w].z = dxVyN() - dyVx();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(nx,1<j<ny-1,nz)
								pNext[w].x = dyVz() - dzVyN();
								pNext[w].y = dzVxN_dxVzN();
								pNext[w].z = dxVyN() - dyVx();
							}
							else
							{
								//z inside the boundary
								//(nx,1<j<ny-1,1<k<nz-1)
								pNext[w].x = dyVz_dzVy();
								pNext[w].y = dzVx() - dxVzN();
								pNext[w].z = dxVyN() - dyVx();
							}
						}
					}
					else
					{
						//x inside boundary
						if (j == 1)
						{
							//y near the lower boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(1<i<nx-1,1,1)
								pNext[w].x = dyVz1_dzVy1();
								pNext[w].y = dzVx1() - dxVz();
								pNext[w].z = dxVy() - dyVx1();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(1<i<nx-1,1,0)
								pNext[w].x = dyVz1() - dzVy0();
								pNext[w].y = dzVx0() - dxVz();
								pNext[w].z = dxVy() - dyVx1();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(1<i<nx-1,1,nz-1)
								pNext[w].x = dyVz1_dzVyN1();
								pNext[w].y = dzVxN1() - dxVz();
								pNext[w].z = dxVy() - dyVx1();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(1<i<nx-1,1,nz)
								pNext[w].x = dyVz1() - dzVyN();
								pNext[w].y = dzVxN() - dxVz();
								pNext[w].z = dxVy() - dyVx1();
							}
							else
							{
								//z inside the boundary
								//(1<i<nx-1,1,1<k<nz-1)
								pNext[w].x = dyVz1() - dzVy();
								pNext[w].y = dzVx_dxVz();
								pNext[w].z = dxVy() - dyVx1();
							}
						}
						else if (j == 0)
						{
							//y at the lower boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(1<i<nx-1,0,1)
								pNext[w].x = dyVz0() - dzVy1();
								pNext[w].y = dzVx1() - dxVz();
								pNext[w].z = dxVy() - dyVx0();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(1<i<nx-1,0,0)
								pNext[w].x = dyVz0_dzVy0();
								pNext[w].y = dzVx0() - dxVz();
								pNext[w].z = dxVy() - dyVx0();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(1<i<nx-1,0,nz-1)
								pNext[w].x = dyVz0() - dzVyN1();
								pNext[w].y = dzVxN1() - dxVz();
								pNext[w].z = dxVy() - dyVx0();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(1<i<nx-1,0,nz)
								pNext[w].x = dyVz0_dzVyN();
								pNext[w].y = dzVxN() - dxVz();
								pNext[w].z = dxVy() - dyVx0();
							}
							else
							{
								//z inside the boundary
								//(1<i<nx-1,0,1<k<nz-1)
								pNext[w].x = dyVz0() - dzVy();
								pNext[w].y = dzVx_dxVz();
								pNext[w].z = dxVy() - dyVx0();
							}
						}
						else if (j == nyN1)
						{
							//y near the upper boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(1<i<nx-1,ny-1,1)
								pNext[w].x = dyVzN1_dzVy1();
								pNext[w].y = dzVx1() - dxVz();
								pNext[w].z = dxVy() - dyVxN1();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(1<i<nx-1,ny-1,0)
								pNext[w].x = dyVzN1() - dzVy0();
								pNext[w].y = dzVx0() - dxVz();
								pNext[w].z = dxVy() - dyVxN1();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(1<i<nx-1,ny-1,nz-1)
								pNext[w].x = dyVzN1_dzVyN1();
								pNext[w].y = dzVxN1() - dxVz();
								pNext[w].z = dxVy() - dyVxN1();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(1<i<nx-1,ny-1,nz)
								pNext[w].x = dyVzN1() - dzVyN();
								pNext[w].y = dzVxN() - dxVz();
								pNext[w].z = dxVy() - dyVxN1();
							}
							else
							{
								//z inside the boundary
								//(1<i<nx-1,ny-1,1<k<nz-1)
								pNext[w].x = dyVzN1() - dzVy();
								pNext[w].y = dzVx_dxVz();
								pNext[w].z = dxVy() - dyVxN1();
							}
						}
						else if (j == ny)
						{
							//y at the upper boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(1<i<nx-1,ny,1)
								pNext[w].x = dyVzN() - dzVy1();
								pNext[w].y = dzVx1() - dxVz();
								pNext[w].z = dxVy() - dyVxN();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(1<i<nx-1,ny,0)
								pNext[w].x = dyVzN_dzVy0();
								pNext[w].y = dzVx0() - dxVz();
								pNext[w].z = dxVy() - dyVxN();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(1<i<nx-1,ny,nz-1)
								pNext[w].x = dyVzN() - dzVyN1();
								pNext[w].y = dzVxN1() - dxVz();
								pNext[w].z = dxVy() - dyVxN();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(1<i<nx-1,ny,nz)
								pNext[w].x = dyVzN_dzVyN();
								pNext[w].y = dzVxN() - dxVz();
								pNext[w].z = dxVy() - dyVxN();
							}
							else
							{
								//z inside the boundary
								//(1<i<nx-1,ny,1<k<nz-1)
								pNext[w].x = dyVzN() - dzVy();
								pNext[w].y = dzVx_dxVz();
								pNext[w].z = dxVy() - dyVxN();
							}
						}
						else
						{
							//y inside boundary
							if (k == 1)
							{
								//z near the lower boundary
								//(1<i<nx-1,1<j<ny-1,1)
								pNext[w].x = dyVz() - dzVy1();
								pNext[w].y = dzVx1() - dxVz();
								pNext[w].z = dxVy_dyVx();
							}
							else if (k == 0)
							{
								//z at the lower boundary
								//(1<i<nx-1,1<j<ny-1,0)
								pNext[w].x = dyVz() - dzVy0();
								pNext[w].y = dzVx0() - dxVz();
								pNext[w].z = dxVy_dyVx();
							}
							else if (k == nzN1)
							{
								//z near the upper boundary
								//(1<i<nx-1,1<j<ny-1,nz-1)
								pNext[w].x = dyVz() - dzVyN1();
								pNext[w].y = dzVxN1() - dxVz();
								pNext[w].z = dxVy_dyVx();
							}
							else if (k == nz)
							{
								//z at the upper boundary
								//(1<i<nx-1,1<j<ny-1,nz)
								pNext[w].x = dyVz() - dzVyN();
								pNext[w].y = dzVxN() - dxVz();
								pNext[w].z = dxVy_dyVx();
							}
							else
							{
								//(x,y,z) inside boundary
								pNext[w].x = dyVz_dzVy();
								pNext[w].y = dzVx_dxVz();
								pNext[w].z = dxVy_dyVx();
							}
						}
					}
					//
					pNext[w].x = pNext[w].x / 12;
					pNext[w].y = pNext[w].y / 12;
					pNext[w].z = pNext[w].z / 12;
					//
					w++;
				}
			}
		}
}


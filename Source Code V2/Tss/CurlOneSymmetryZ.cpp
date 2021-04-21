
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 04/02/2021
Allrights reserved by David Ge

calculate first order curl using rotate symmetry
********************************************************************/
#include "CurlOneSymmetryZ.h"

CurlOneSymmetryZ::CurlOneSymmetryZ()
{
	i0 = 0;
	i1 = 0;
	//w0 = 0;
	nx = ny = nz = 0;
	Field = 0;
	pNext = 0;
	v0.x = 0;
	v0.y = 0;
	v0.z = 0;
}


CurlOneSymmetryZ::~CurlOneSymmetryZ()
{
}

void CurlOneSymmetryZ::setSpaceRange(unsigned int nx0, unsigned int ny0, unsigned int nz0)
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
	//w0 = 0;
	ic = nx / 2;
	j = ic;
}

//
inline double CurlOneSymmetryZ::dxVx()   //insize boundary
{
	return 8 * (Field->getFieldx(i + 1, j, k) - Field->getFieldx(i - 1, j, k)) + Field->getFieldx(i - 2, j, k) - Field->getFieldx(i + 2, j, k);
}
inline double CurlOneSymmetryZ::dxVx0()  //at lower boundary
{
	return -25 * Field->getFieldx(i, j, k) + 48 * Field->getFieldx(i+1, j, k) - 36 * Field->getFieldx(i+2, j, k) + 16 * Field->getFieldx(i+3, j, k) - 3 * Field->getFieldx(i+4, j, k);
}
inline double CurlOneSymmetryZ::dxVx1()  //near lower boundary
{
	return  -10 * Field->getFieldx(i, j, k) + 18 * Field->getFieldx(i+1, j, k) - 6 * Field->getFieldx(i+2, j, k) + Field->getFieldx(i+3, j, k) - 3 * Field->getFieldx(i-1, j, k);
}
inline double CurlOneSymmetryZ::dxVxN1() //near upper boundary
{
	return 10 * Field->getFieldx(i, j, k) - 18 * Field->getFieldx(i-1, j, k) + 6 * Field->getFieldx(i-2, j, k) - Field->getFieldx(i-3, j, k) + 3 * Field->getFieldx(i+1, j, k);
}
inline double CurlOneSymmetryZ::dxVxN()  //at upper boundary
{
	return 25 * Field->getFieldx(i, j, k) - 48 * Field->getFieldx(i-1, j, k) + 36 * Field->getFieldx(i-2, j, k) - 16 * Field->getFieldx(i-3, j, k) + 3 * Field->getFieldx(i-4, j, k);
}
double CurlOneSymmetryZ::DxVx(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	//w = I(i, j, k);
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
inline double CurlOneSymmetryZ::dyVy()   //insize boundary
{
	return  8 * (Field->getFieldy(i, j+1, k) - Field->getFieldy(i, j-1, k)) + Field->getFieldy(i, j-2, k) - Field->getFieldy(i, j+2, k);
}
inline double CurlOneSymmetryZ::dyVy0()  //at lower boundary
{
	return -25 * Field->getFieldy(i, j, k) + 48 * Field->getFieldy(i, j+1, k) - 36 * Field->getFieldy(i, j+2, k) + 16 * Field->getFieldy(i, j+3, k) - 3 * Field->getFieldy(i, j+4, k);
}
inline double CurlOneSymmetryZ::dyVy1()  //near lower boundary
{
	return  -10 * Field->getFieldy(i, j, k) + 18 * Field->getFieldy(i, j+1, k) - 6 * Field->getFieldy(i, j+2, k) + Field->getFieldy(i, j+3, k) - 3 * Field->getFieldy(i, j-1, k);
}
inline double CurlOneSymmetryZ::dyVyN1() //near upper boundary
{
	return 10 * Field->getFieldy(i, j, k) - 18 * Field->getFieldy(i, j-1, k) + 6 * Field->getFieldy(i, j-2, k) - Field->getFieldy(i, j-3, k) + 3 * Field->getFieldy(i, j+1, k);
}
inline double CurlOneSymmetryZ::dyVyN()  //at upper boundary
{
	return 25 * Field->getFieldy(i, j, k) - 48 * Field->getFieldy(i, j-1, k) + 36 * Field->getFieldy(i, j-2, k) - 16 * Field->getFieldy(i, j-3, k) + 3 * Field->getFieldy(i, j-4, k);
}
double CurlOneSymmetryZ::DyVy(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	//w = I(i, j, k);
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
inline double CurlOneSymmetryZ::dzVz()   //insize boundary
{
	return 8 * (Field->getFieldz(i, j, k+1) - Field->getFieldz(i, j, k-1)) + Field->getFieldz(i, j, k-2) - Field->getFieldz(i, j, k+2);
}
inline double CurlOneSymmetryZ::dzVz0()  //at lower boundary
{
	return -25 * Field->getFieldz(i, j, k) + 48 * Field->getFieldz(i, j, k+1) - 36 * Field->getFieldz(i, j, k+2) + 16 * Field->getFieldz(i, j, k+3) - 3 * Field->getFieldz(i, j, k+4);
}
inline double CurlOneSymmetryZ::dzVz1()  //near lower boundary
{
	return  -10 * Field->getFieldz(i, j, k) + 18 * Field->getFieldz(i, j, k+1) - 6 * Field->getFieldz(i, j, k+2) + Field->getFieldz(i, j, k+3) - 3 * Field->getFieldz(i, j, k-1);
}
inline double CurlOneSymmetryZ::dzVzN1() //near upper boundary
{
	return 10 * Field->getFieldz(i, j, k) - 18 * Field->getFieldz(i, j, k-1) + 6 * Field->getFieldz(i, j, k-2) - Field->getFieldz(i, j, k-3) + 3 * Field->getFieldz(i, j, k+1);
}
inline double CurlOneSymmetryZ::dzVzN()  //at upper boundary
{
	return 25 * Field->getFieldz(i, j, k) - 48 * Field->getFieldz(i, j, k-1) + 36 * Field->getFieldz(i, j, k-2) - 16 * Field->getFieldz(i, j, k-3) + 3 * Field->getFieldz(i, j, k-4);
}
double CurlOneSymmetryZ::DzVz(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	//w = I(i, j, k);
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
inline double CurlOneSymmetryZ::dyVz()   //insize boundary
{
	return  8 * (Field->getFieldz(i, j+1, k) - Field->getFieldz(i, j-1, k)) + Field->getFieldz(i, j-2, k) - Field->getFieldz(i, j+2, k);
}
inline double CurlOneSymmetryZ::dyVz0()  //at lower boundary
{
	return -25 * Field->getFieldz(i, j, k) + 48 * Field->getFieldz(i, j+1, k) - 36 * Field->getFieldz(i, j+2, k) + 16 * Field->getFieldz(i, j+3, k) - 3 * Field->getFieldz(i, j+4, k);
}
inline double CurlOneSymmetryZ::dyVz1()  //near lower boundary
{
	return  -10 * Field->getFieldz(i, j, k) + 18 * Field->getFieldz(i, j+1, k) - 6 * Field->getFieldz(i, j+2, k) + Field->getFieldz(i, j+3, k) - 3 * Field->getFieldz(i, j-1, k);
}
inline double CurlOneSymmetryZ::dyVzN1() //near upper boundary
{
	return  10 * Field->getFieldz(i, j, k) - 18 * Field->getFieldz(i, j-1, k) + 6 * Field->getFieldz(i, j-2, k) - Field->getFieldz(i, j-3, k) + 3 * Field->getFieldz(i, j+1, k);
}
inline double CurlOneSymmetryZ::dyVzN()  //at upper boundary
{
	return 25 * Field->getFieldz(i, j, k) - 48 * Field->getFieldz(i, j-1, k) + 36 * Field->getFieldz(i, j-2, k) - 16 * Field->getFieldz(i, j-3, k) + 3 * Field->getFieldz(i, j-4, k);
}
//
inline double CurlOneSymmetryZ::dzVy()   //insize boundary
{
	return 8 * (Field->getFieldy(i, j, k+1) - Field->getFieldy(i, j, k-1)) + Field->getFieldy(i, j, k-2) - Field->getFieldy(i, j, k+2);
}
inline double CurlOneSymmetryZ::dzVy0()  //at lower boundary
{
	return -25 * Field->getFieldy(i, j, k) + 48 * Field->getFieldy(i, j, k+1) - 36 * Field->getFieldy(i, j, k+2) + 16 * Field->getFieldy(i, j, k+3) - 3 * Field->getFieldy(i, j, k+4);
}
inline double CurlOneSymmetryZ::dzVy1()  //near lower boundary
{
	return  -10 * Field->getFieldy(i, j, k) + 18 * Field->getFieldy(i, j, k+1) - 6 * Field->getFieldy(i, j, k+2) + Field->getFieldy(i, j, k+3) - 3 * Field->getFieldy(i, j, k-1);
}
inline double CurlOneSymmetryZ::dzVyN1() //near upper boundary
{
	return 10 * Field->getFieldy(i, j, k) - 18 * Field->getFieldy(i, j, k-1) + 6 * Field->getFieldy(i, j, k-2) - Field->getFieldy(i, j, k-3) + 3 * Field->getFieldy(i, j, k+1);
}
inline double CurlOneSymmetryZ::dzVyN()  //at upper boundary
{
	return 25 * Field->getFieldy(i, j, k) - 48 * Field->getFieldy(i, j, k-1) + 36 * Field->getFieldy(i, j, k-2) - 16 * Field->getFieldy(i, j, k-3) + 3 * Field->getFieldy(i, j, k-4);
}
//
inline double CurlOneSymmetryZ::dzVx()   //insize boundary
{
	return 8 * (Field->getFieldx(i, j, k+1) - Field->getFieldx(i, j, k-1)) + Field->getFieldx(i, j, k-2) - Field->getFieldx(i, j, k+2);
}
inline double CurlOneSymmetryZ::dzVx0()  //at lower boundary
{
	return -25 * Field->getFieldx(i, j, k) + 48 * Field->getFieldx(i, j, k+1) - 36 * Field->getFieldx(i, j, k+2) + 16 * Field->getFieldx(i, j, k+3) - 3 * Field->getFieldx(i, j, k+4);
}
inline double CurlOneSymmetryZ::dzVx1()  //near lower boundary
{
	return  -10 * Field->getFieldx(i, j, k) + 18 * Field->getFieldx(i, j, k+1) - 6 * Field->getFieldx(i, j, k+2) + Field->getFieldx(i, j, k+3) - 3 * Field->getFieldx(i, j, k-1);
}
inline double CurlOneSymmetryZ::dzVxN1() //near upper boundary
{
	return 10 * Field->getFieldx(i, j, k) - 18 * Field->getFieldx(i, j, k-1) + 6 * Field->getFieldx(i, j, k-2) - Field->getFieldx(i, j, k-3) + 3 * Field->getFieldx(i, j, k+1);
}
inline double CurlOneSymmetryZ::dzVxN()  //at upper boundary
{
	return 25 * Field->getFieldx(i, j, k) - 48 * Field->getFieldx(i, j, k-1) + 36 * Field->getFieldx(i, j, k-2) - 16 * Field->getFieldx(i, j, k-3) + 3 * Field->getFieldx(i, j, k-4);
}
//
inline double CurlOneSymmetryZ::dxVz()   //insize boundary
{
	return 8 * (Field->getFieldz(i+1, j, k) - Field->getFieldz(i-1, j, k)) + Field->getFieldz(i-2, j, k) - Field->getFieldz(i+2, j, k);
}
inline double CurlOneSymmetryZ::dxVz0()  //at lower boundary
{
	return -25 * Field->getFieldz(i, j, k) + 48 * Field->getFieldz(i+1, j, k) - 36 * Field->getFieldz(i+2, j, k) + 16 * Field->getFieldz(i+3, j, k) - 3 * Field->getFieldz(i+4, j, k);
}
inline double CurlOneSymmetryZ::dxVz1()  //near lower boundary
{
	return -10 * Field->getFieldz(i, j, k) + 18 * Field->getFieldz(i+1, j, k) - 6 * Field->getFieldz(i+2, j, k) + Field->getFieldz(i+3, j, k) - 3 * Field->getFieldz(i-1, j, k);
}
inline double CurlOneSymmetryZ::dxVzN1() //near upper boundary
{
	return  10 * Field->getFieldz(i, j, k) - 18 * Field->getFieldz(i-1, j, k) + 6 * Field->getFieldz(i-2, j, k) - Field->getFieldz(i-3, j, k) + 3 * Field->getFieldz(i+1, j, k);
}
inline double CurlOneSymmetryZ::dxVzN()  //at upper boundary
{
	return 25 * Field->getFieldz(i, j, k) - 48 * Field->getFieldz(i-1, j, k) + 36 * Field->getFieldz(i-2, j, k) - 16 * Field->getFieldz(i-3, j, k) + 3 * Field->getFieldz(i-4, j, k);
}
//
inline double CurlOneSymmetryZ::dxVy()   //insize boundary
{
	return 8 * (Field->getFieldy(i+1, j, k) - Field->getFieldy(i-1, j, k)) + Field->getFieldy(i-2, j, k) - Field->getFieldy(i+2, j, k);
}
inline double CurlOneSymmetryZ::dxVy0()  //at lower boundary
{
	return -25 * Field->getFieldy(i, j, k) + 48 * Field->getFieldy(i+1, j, k) - 36 * Field->getFieldy(i+2, j, k) + 16 * Field->getFieldy(i+3, j, k) - 3 * Field->getFieldy(i+4, j, k);
}
inline double CurlOneSymmetryZ::dxVy1()  //near lower boundary
{
	return -10 * Field->getFieldy(i, j, k) + 18 * Field->getFieldy(i+1, j, k) - 6 * Field->getFieldy(i+2, j, k) + Field->getFieldy(i+3, j, k) - 3 * Field->getFieldy(i-1, j, k);
}
inline double CurlOneSymmetryZ::dxVyN1() //near upper boundary
{
	return 10 * Field->getFieldy(i, j, k) - 18 * Field->getFieldy(i-1, j, k) + 6 * Field->getFieldy(i-2, j, k) - Field->getFieldy(i-3,j,k) + 3 * Field->getFieldy(i+1, j, k);
}
inline double CurlOneSymmetryZ::dxVyN()  //at upper boundary
{
	return 25 * Field->getFieldy(i, j, k) - 48 * Field->getFieldy(i-1, j, k) + 36 * Field->getFieldy(i-2, j, k) - 16 * Field->getFieldy(i-3,j,k) + 3 * Field->getFieldy(i-4, j, k);
}
//
inline double CurlOneSymmetryZ::dyVx()   //insize boundary
{
	return 8 * (Field->getFieldx(i, j+1, k) - Field->getFieldx(i, j-1, k)) + Field->getFieldx(i, j-2, k) - Field->getFieldx(i, j+2, k);
}
inline double CurlOneSymmetryZ::dyVx0()  //at lower boundary
{
	return -25 * Field->getFieldx(i, j, k) + 48 * Field->getFieldx(i, j+1, k) - 36 * Field->getFieldx(i, j+2, k) + 16 * Field->getFieldx(i, j+3, k) - 3 * Field->getFieldx(i, j+4, k);
}
inline double CurlOneSymmetryZ::dyVx1()  //near lower boundary
{
	return -10 * Field->getFieldx(i, j, k) + 18 * Field->getFieldx(i, j+1, k) - 6 * Field->getFieldx(i, j+2, k) + Field->getFieldx(i, j+3, k) - 3 * Field->getFieldx(i, j-1, k);
}
inline double CurlOneSymmetryZ::dyVxN1() //near upper boundary
{
	return 10 * Field->getFieldx(i, j, k) - 18 * Field->getFieldx(i, j-1, k) + 6 * Field->getFieldx(i, j-2, k) - Field->getFieldx(i, j-3, k) + 3 * Field->getFieldx(i, j+1, k);
}
inline double CurlOneSymmetryZ::dyVxN()  //at upper boundary
{
	return 25 * Field->getFieldx(i, j, k) - 48 * Field->getFieldx(i, j-1, k) + 36 * Field->getFieldx(i, j-2, k) - 16 * Field->getFieldx(i, j-3, k) + 3 * Field->getFieldx(i, j-4, k);
}

inline double CurlOneSymmetryZ::dzVx1_dxVz1() //Cy at (1,*,1)
{
	return -10 * (Field->getFieldx(i, j, k) - Field->getFieldz(i, j, k)) + 18 * (Field->getFieldx(i, j, k+1) - Field->getFieldz(i+1, j, k)) - 6 * (Field->getFieldx(i, j, k+2) - Field->getFieldz(i+2, j, k)) + Field->getFieldx(i, j, k+3) - Field->getFieldz(i+3, j, k) - 3 * (Field->getFieldx(i, j, k-1) - Field->getFieldz(i-1, j, k));
}

inline double CurlOneSymmetryZ::dyVz0_dzVy0() //Cx at (*,0,0)
{
	return -25 * (Field->getFieldz(i, j, k) - Field->getFieldy(i, j, k)) + 48 * (Field->getFieldz(i, j+1, k) - Field->getFieldy(i, j, k+1)) - 36 * (Field->getFieldz(i, j+2, k) - Field->getFieldy(i, j, k+2)) + 16 * (Field->getFieldz(i, j+3, k) - Field->getFieldy(i, j, k+3)) - 3 * (Field->getFieldz(i, j+4, k) - Field->getFieldy(i, j, k+4));
}

inline double CurlOneSymmetryZ::dzVxN1_dxVz1() //Cy at (1,*,nz-1)
{
	return 10 * (Field->getFieldx(i, j, k) + Field->getFieldz(i, j, k)) - 18 * (Field->getFieldx(i, j, k-1) + Field->getFieldz(i+1, j, k)) + 6 * (Field->getFieldx(i, j, k-2) + Field->getFieldz(i+2, j, k)) - Field->getFieldx(i, j, k-3) - Field->getFieldz(i+3, j, k) + 3 * (Field->getFieldx(i, j, k+1) + Field->getFieldz(i-1, j, k));
}

inline double CurlOneSymmetryZ::dyVz0_dzVyN()  //Cx at (*,0,nz)
{
	return -25 * (Field->getFieldz(i, j, k) + Field->getFieldy(i, j, k)) + 48 * (Field->getFieldz(i, j+1, k) + Field->getFieldy(i, j, k-1)) - 36 * (Field->getFieldz(i, j+2, k) + Field->getFieldy(i, j, k-2)) + 16 * (Field->getFieldz(i, j+3, k) + Field->getFieldy(i, j, k-3)) - 3 * (Field->getFieldz(i, j+4, k) + Field->getFieldy(i, j, k-4));
}

inline double CurlOneSymmetryZ::dyVzN1_dzVy1() //Cx at (*,ny-1,1)
{
	return 10 * (Field->getFieldz(i, j, k) + Field->getFieldy(i, j, k)) - 18 * (Field->getFieldz(i, j-1, k) + Field->getFieldy(i, j, k+1)) + 6 * (Field->getFieldz(i, j-2, k) + Field->getFieldy(i, j, k+2)) - Field->getFieldz(i, j-3, k) - Field->getFieldy(i, j, k+3) + 3 * (Field->getFieldz(i, j+1, k) + Field->getFieldy(i, j, k-1));
}

inline double CurlOneSymmetryZ::dxVy1_dyVxN1() //Cz at (1,ny-1,*)
{
	return  -10 * (Field->getFieldy(i, j, k) + Field->getFieldx(i, j, k)) + 18 * (Field->getFieldy(i+1, j, k) + Field->getFieldx(i, j-1, k)) - 6 * (Field->getFieldy(i+2, j, k) + Field->getFieldx(i, j-2, k)) + Field->getFieldy(i+3, j, k) + Field->getFieldx(i, j-3, k) - 3 * (Field->getFieldy(i-1, j, k) + Field->getFieldx(i, j+1, k));
}

inline double CurlOneSymmetryZ::dyVzN1_dzVyN1() //Cx at (*,ny-1,nz-1)
{
	return 10 * (Field->getFieldz(i, j, k) - Field->getFieldy(i, j, k)) - 18 * (Field->getFieldz(i, j-1, k) - Field->getFieldy(i, j, k-1)) + 6 * (Field->getFieldz(i, j-2, k) - Field->getFieldy(i, j, k-2)) - Field->getFieldz(i, j-3, k) + Field->getFieldy(i, j, k-3) + 3 * (Field->getFieldz(i, j+1, k) - Field->getFieldy(i, j, k+1));
}

inline double CurlOneSymmetryZ::dyVzN_dzVy0()   //Cx at (*,ny,0)
{
	return 25 * (Field->getFieldz(i, j, k) + Field->getFieldy(i, j, k)) - 48 * (Field->getFieldz(i, j-1, k) + Field->getFieldy(i, j, k+1)) + 36 * (Field->getFieldz(i, j-2, k) + Field->getFieldy(i, j, k+2)) - 16 * (Field->getFieldz(i, j-3, k) + Field->getFieldy(i, j, k+3)) + 3 * (Field->getFieldz(i, j-4, k) + Field->getFieldy(i, j, k+4));
}

inline double CurlOneSymmetryZ::dyVzN_dzVyN()   //Cx at (*,ny,nz)
{
	return 25 * (Field->getFieldz(i, j, k) - Field->getFieldy(i, j, k)) - 48 * (Field->getFieldz(i, j-1, k) - Field->getFieldy(i, j, k-1)) + 36 * (Field->getFieldz(i, j-2, k) - Field->getFieldy(i, j, k-2)) - 16 * (Field->getFieldz(i, j-3, k) - Field->getFieldy(i, j, k-3)) + 3 * (Field->getFieldz(i, j-4, k) - Field->getFieldy(i, j, k-4));
}

inline double CurlOneSymmetryZ::dyVz_dzVy()     //Cx at (*,1<j<ny-1,1<k<nz-1)
{
	return  8 * (Field->getFieldz(i, j+1, k) - Field->getFieldz(i, j-1, k) + Field->getFieldy(i, j, k-1) - Field->getFieldy(i, j, k+1)) + Field->getFieldz(i, j-2, k) - Field->getFieldy(i, j, k-2) - Field->getFieldz(i, j+2, k) + Field->getFieldy(i, j, k+2);
}

inline double CurlOneSymmetryZ::dyVz1_dzVy1()   //Cx at (*,1,1)
{
	return -10 * (Field->getFieldz(i, j, k) - Field->getFieldy(i, j, k)) + 18 * (Field->getFieldz(i, j+1, k) - Field->getFieldy(i, j, k+1)) - 6 * (Field->getFieldz(i, j+2, k) - Field->getFieldy(i, j, k+2)) + Field->getFieldz(i, j+3, k) - Field->getFieldy(i, j, k+3) - 3 * (Field->getFieldz(i, j-1, k) - Field->getFieldy(i, j, k-1));
}

inline double CurlOneSymmetryZ::dzVx0_dxVz0()   //Cy at (0,*,0)
{
	return 25 * (Field->getFieldx(i, j, k) - Field->getFieldz(i, j, k)) + 48 * (Field->getFieldx(i, j, k+1) - Field->getFieldz(i+1, j, k)) - 36 * (Field->getFieldx(i, j, k+2) - Field->getFieldz(i+2, j, k)) + 16 * (Field->getFieldx(i, j, k+3) - Field->getFieldz(i+3, j, k)) - 3 * (Field->getFieldx(i, j, k+4) - Field->getFieldz(i+4, j, k));
}

inline double CurlOneSymmetryZ::dyVz1_dzVyN1()  //Cx at (*,1,nz-1)
{
	return -10 * (Field->getFieldz(i, j, k) + Field->getFieldy(i, j, k)) + 18 * (Field->getFieldz(i, j+1, k) + Field->getFieldy(i, j, k-1)) - 6 * (Field->getFieldz(i, j+2, k) + Field->getFieldy(i, j, k-2)) + Field->getFieldz(i, j+3, k) + Field->getFieldy(i, j, k-3) - 3 * (Field->getFieldz(i, j-1, k) + Field->getFieldy(i, j, k+1));
}

inline double CurlOneSymmetryZ::dzVxN_dxVz0()   //Cy at (0,*,nz)
{
	return 25 * (Field->getFieldx(i, j, k) + Field->getFieldz(i, j, k)) - 48 * (Field->getFieldx(i, j, k-1) + Field->getFieldz(i+1, j, k)) + 36 * (Field->getFieldx(i, j, k-2) + Field->getFieldz(i+2, j, k)) - 16 * (Field->getFieldx(i, j, k-3) + Field->getFieldz(i+3, j, k)) + 3 * (Field->getFieldx(i, j, k-4) + Field->getFieldz(i+4, j, k));
}

inline double CurlOneSymmetryZ::dxVy0_dyVx0()   //Cz at (0,0,*)
{
	return -25 * (Field->getFieldy(i, j, k) - Field->getFieldx(i, j, k)) + 48 * (Field->getFieldy(i+1, j, k) - Field->getFieldx(i, j+1, k)) - 36 * (Field->getFieldy(i+2, j, k) - Field->getFieldx(i, j+2, k)) + 16 * (Field->getFieldy(i+3, j, k) - Field->getFieldx(i, j+3, k)) - 3 * (Field->getFieldy(i+4, j, k) - Field->getFieldx(i, j+4, k));
}

inline double CurlOneSymmetryZ::dxVy0_dyVxN()   //Cz at (0,ny,*)
{
	return -25 * (Field->getFieldy(i, j, k) + Field->getFieldx(i, j, k)) + 48 * (Field->getFieldy(i+1, j, k) + Field->getFieldx(i, j-1, k)) - 36 * (Field->getFieldy(i+2, j, k) + Field->getFieldx(i, j-2, k)) + 16 * (Field->getFieldy(i+3, j, k) + Field->getFieldx(i, j-3, k)) - 3 * (Field->getFieldy(i+4, j, k) + Field->getFieldx(i, j-4, k));
}

inline double CurlOneSymmetryZ::dzVx1_dxVzN1()  //Cy at (nx-1,*,1)
{
	return -10 * (Field->getFieldx(i, j, k) + Field->getFieldz(i, j, k)) + 18 * (Field->getFieldx(i, j, k+1) + Field->getFieldz(i-1, j, k)) - 6 * (Field->getFieldx(i, j, k+2) + Field->getFieldz(i-2, j, k)) + Field->getFieldx(i, j, k+3) + Field->getFieldz(i-3, j, k) - 3 * (Field->getFieldx(i, j, k-1) + Field->getFieldz(i+1, j, k));
}

inline double CurlOneSymmetryZ::dxVyN1_dyVx1()  //Cz at (nx-1,1,*)
{
	return  10 * (Field->getFieldy(i, j, k) + Field->getFieldx(i, j, k)) - 18 * (Field->getFieldy(i-1, j, k) + Field->getFieldx(i, j+1, k)) + 6 * (Field->getFieldy(i-2, j, k) + Field->getFieldx(i, j+2, k)) - Field->getFieldy(i-3,j,k) - Field->getFieldx(i, j+3, k) + 3 * (Field->getFieldy(i+1, j, k) + Field->getFieldx(i, j-1, k));
}

inline double CurlOneSymmetryZ::dzVxN1_dxVzN1() //Cy at (nx-1,*,nz-1)
{
	return 10 * (Field->getFieldx(i, j, k) - Field->getFieldz(i, j, k)) - 18 * (Field->getFieldx(i, j, k-1) - Field->getFieldz(i-1, j, k)) + 6 * (Field->getFieldx(i, j, k-2) - Field->getFieldz(i-2, j, k)) - Field->getFieldx(i, j, k-3) + Field->getFieldz(i-3, j, k) + 3 * (Field->getFieldx(i, j, k+1) - Field->getFieldz(i+1, j, k));
}

inline double CurlOneSymmetryZ::dxVyN1_dyVxN1() //Cz at (nx-1,ny-1,*)
{
	return 10 * (Field->getFieldy(i, j, k) - Field->getFieldx(i, j, k)) - 18 * (Field->getFieldy(i-1, j, k) - Field->getFieldx(i, j-1, k)) + 6 * (Field->getFieldy(i-2, j, k) - Field->getFieldx(i, j-2, k)) - Field->getFieldy(i-3,j,k) + Field->getFieldx(i, j-3, k) + 3 * (Field->getFieldy(i+1, j, k) - Field->getFieldx(i, j+1, k));
}

inline double CurlOneSymmetryZ::dzVx0_dxVzN()   //Cy at (nx,*,0)
{
	return -25 * (Field->getFieldx(i, j, k) + Field->getFieldz(i, j, k)) + 48 * (Field->getFieldx(i, j, k+1) + Field->getFieldz(i-1, j, k)) - 36 * (Field->getFieldx(i, j, k+2) + Field->getFieldz(i-2, j, k)) + 16 * (Field->getFieldx(i, j, k+3) + Field->getFieldz(i-3, j, k)) - 3 * (Field->getFieldx(i, j, k+4) + Field->getFieldz(i-4, j, k));
}

inline double CurlOneSymmetryZ::dzVxN_dxVzN()   //Cy at (nx, *, nz)
{
	return 25 * (Field->getFieldx(i, j, k) - Field->getFieldz(i, j, k)) - 48 * (Field->getFieldx(i, j, k-1) - Field->getFieldz(i-1, j, k)) + 36 * (Field->getFieldx(i, j, k-2) - Field->getFieldz(i-2, j, k)) - 16 * (Field->getFieldx(i, j, k-3) - Field->getFieldz(i-3, j, k)) + 3 * (Field->getFieldx(i, j, k-4) - Field->getFieldz(i-4, j, k));
}

inline double CurlOneSymmetryZ::dxVyN_dyVx0()   //Cz at (nx, 0, *)
{
	return 25 * (Field->getFieldy(i, j, k) + Field->getFieldx(i, j, k)) - 48 * (Field->getFieldy(i-1, j, k) + Field->getFieldx(i, j+1, k)) + 36 * (Field->getFieldy(i-2, j, k) + Field->getFieldx(i, j+2, k)) - 16 * (Field->getFieldy(i-3,j,k) + Field->getFieldx(i, j+3, k)) + 3 * (Field->getFieldy(i-4, j, k) + Field->getFieldx(i, j+4, k));
}

inline double CurlOneSymmetryZ::dxVyN_dyVxN()   //Cz at (nx, ny, *)
{
	return 25 * (Field->getFieldy(i, j, k) - Field->getFieldx(i, j, k)) - 48 * (Field->getFieldy(i-1, j, k) - Field->getFieldx(i, j-1, k)) + 36 * (Field->getFieldy(i-2, j, k) - Field->getFieldx(i, j-2, k)) - 16 * (Field->getFieldy(i-3,j,k) - Field->getFieldx(i, j-3, k)) + 3 * (Field->getFieldy(i-4, j, k) - Field->getFieldx(i, j-4, k));
}

inline double CurlOneSymmetryZ::dzVx_dxVz()     //Cy at (1<i<nx-1,*,1<k<nz-1)
{
	return 8 * (Field->getFieldx(i, j, k+1) - Field->getFieldx(i, j, k-1) - Field->getFieldz(i+1, j, k) + Field->getFieldz(i-1, j, k)) + Field->getFieldx(i, j, k-2) - Field->getFieldx(i, j, k+2) - Field->getFieldz(i-2, j, k) + Field->getFieldz(i+2, j, k);
}

inline double CurlOneSymmetryZ::dxVy_dyVx()     //Cz at (1<i<nx-1,1<j<ny-1,*)
{
	return 8 * (Field->getFieldy(i+1, j, k) - Field->getFieldy(i-1, j, k) - Field->getFieldx(i, j+1, k) + Field->getFieldx(i, j-1, k)) + Field->getFieldy(i-2, j, k) - Field->getFieldy(i+2, j, k) - Field->getFieldx(i, j-2, k) + Field->getFieldx(i, j+2, k);
}

inline double CurlOneSymmetryZ::dxVy1_dyVx1()   //Cz at (1,1,*)
{
	return -10 * (Field->getFieldy(i, j, k) - Field->getFieldx(i, j, k)) + 18 * (Field->getFieldy(i+1, j, k) - Field->getFieldx(i, j+1, k)) - 6 * (Field->getFieldy(i+2, j, k) - Field->getFieldx(i, j+2, k)) + Field->getFieldy(i+3, j, k) - Field->getFieldx(i, j+3, k) - 3 * (Field->getFieldy(i-1, j, k) - Field->getFieldx(i, j-1, k));
}

//test
double CurlOneSymmetryZ::DyVx(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	//w = I(i, j, k);
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
double CurlOneSymmetryZ::DzVy(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	//w = I(i, j, k);
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
double CurlOneSymmetryZ::DxVz(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	//w = I(i, j, k);
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

double CurlOneSymmetryZ::DzVx(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	//w = I(i, j, k);
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

double CurlOneSymmetryZ::DxVy(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	//w = I(i, j, k);
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

double CurlOneSymmetryZ::DyVz(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	//w = I(i, j, k);
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

double CurlOneSymmetryZ::Divergence(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0, size_t w0)
{
	double d = 0;
	Field = f;
	i = i0; j = j0; k = k0;
	//w = w0;
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
	d = d / 12;
	return d;
}

/*
	the results need to be divided by 12
*/
int CurlOneSymmetryZ::_getNextCurlUseFastMethod(RotateSymmetryField *pCurrent, RotateSymmetryField *pNextCurl)
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
inline void CurlOneSymmetryZ::calculateCurl()
{
	//w = w0;
	j = ic;
	for (i = i0; i <= i1; i++)
	{
		//for (j = 0; j <= ny; j++)
		//{
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
							v0.x = dyVz1_dzVy1();
							v0.y = dzVx1_dxVz1();
							v0.z = dxVy1_dyVx1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1,1,0)
							v0.x = dyVz1() - dzVy0();
							v0.y = dzVx0() - dxVz1();
							v0.z = dxVy1_dyVx1();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1,1,nz-1)
							v0.x = dyVz1_dzVyN1();
							v0.y = dzVxN1_dxVz1();
							v0.z = dxVy1_dyVx1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1,1,nz)
							v0.x = dyVz1() - dzVyN();
							v0.y = dzVxN() - dxVz1();
							v0.z = dxVy1_dyVx1();
						}
						else
						{
							//z inside the boundary
							//(1,1,1<k<nz-1)
							v0.x = dyVz1() - dzVy();
							v0.y = dzVx() - dxVz1();
							v0.z = dxVy1_dyVx1();
						}
					}
					else if (j == 0)
					{
						//y at the lower boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1,0,1)
							v0.x = dyVz0() - dzVy1();
							v0.y = dzVx1_dxVz1();
							v0.z = dxVy1() - dyVx0();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1,0,0)
							v0.x = dyVz0_dzVy0();
							v0.y = dzVx0() - dxVz1();
							v0.z = dxVy1() - dyVx0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1,0,nz-1)
							v0.x = dyVz0() - dzVyN1();
							v0.y = dzVxN1_dxVz1();
							v0.z = dxVy1() - dyVx0();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1,0,nz)
							v0.x = dyVz0_dzVyN();
							v0.y = dzVxN() - dxVz1();
							v0.z = dxVy1() - dyVx0();
						}
						else
						{
							//z inside the boundary
							//(1,0,1<k<nz-1)
							v0.x = dyVz0() - dzVy();
							v0.y = dzVx() - dxVz1();
							v0.z = dxVy1() - dyVx0();
						}
					}
					else if (j == nyN1)
					{
						//y near the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1,ny-1,1)
							v0.x = dyVzN1_dzVy1();
							v0.y = dzVx1_dxVz1();
							v0.z = dxVy1_dyVxN1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1,ny-1,0)
							v0.x = dyVzN1() - dzVy0();
							v0.y = dzVx0() - dxVz1();
							v0.z = dxVy1_dyVxN1();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1,ny-1,nz-1)
							v0.x = dyVzN1_dzVyN1();
							v0.y = dzVxN1_dxVz1();
							v0.z = dxVy1_dyVxN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1,ny-1,nz)
							v0.x = dyVzN1() - dzVyN();
							v0.y = dzVxN() - dxVz1();
							v0.z = dxVy1_dyVxN1();
						}
						else
						{
							//z inside the boundary
							//(1,ny-1,1<k<nz-1)
							v0.x = dyVzN1() - dzVy();
							v0.y = dzVx() - dxVz1();
							v0.z = dxVy1_dyVxN1();
						}
					}
					else if (j == ny)
					{
						//y at the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1,ny,1)
							v0.x = dyVzN() - dzVy1();
							v0.y = dzVx1_dxVz1();
							v0.z = dxVy1() - dyVxN();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1,ny,0)
							v0.x = dyVzN_dzVy0();
							v0.y = dzVx0() - dxVz1();
							v0.z = dxVy1() - dyVxN();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1,ny,nz-1)
							v0.x = dyVzN() - dzVyN1();
							v0.y = dzVxN1_dxVz1();
							v0.z = dxVy1() - dyVxN();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1,ny,nz)
							v0.x = dyVzN_dzVyN();
							v0.y = dzVxN() - dxVz1();
							v0.z = dxVy1() - dyVxN();
						}
						else
						{
							//z inside the boundary
							//(1,ny,1<k<nz-1)
							v0.x = dyVzN() - dzVy();
							v0.y = dzVx() - dxVz1();
							v0.z = dxVy1() - dyVxN();
						}
					}
					else
					{
						//y inside boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1,1<j<ny-1,1)
							v0.x = dyVz() - dzVy1();
							v0.y = dzVx1_dxVz1();
							v0.z = dxVy1() - dyVx();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1,1<j<ny-1,0)
							v0.x = dyVz() - dzVy0();
							v0.y = dzVx0() - dxVz1();
							v0.z = dxVy1() - dyVx();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1,1<j<ny-1,nz-1)
							v0.x = dyVz() - dzVyN1();
							v0.y = dzVxN1_dxVz1();
							v0.z = dxVy1() - dyVx();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1,1<j<ny-1,nz)
							v0.x = dyVz() - dzVyN();
							v0.y = dzVxN() - dxVz1();
							v0.z = dxVy1() - dyVx();
						}
						else
						{
							//z inside the boundary
							//(1,1<j<ny-1,1<k<nz-1)
							v0.x = dyVz_dzVy();
							v0.y = dzVx() - dxVz1();
							v0.z = dxVy1() - dyVx();
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
							v0.x = dyVz1_dzVy1();
							v0.y = dzVx1() - dxVz0();
							v0.z = dxVy0() - dyVx1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(0,1,0)
							v0.x = dyVz1() - dzVy0();
							v0.y = dzVx0_dxVz0();
							v0.z = dxVy0() - dyVx1();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(0,1,nz-1)
							v0.x = dyVz1_dzVyN1();
							v0.y = dzVxN1() - dxVz0();
							v0.z = dxVy0() - dyVx1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(0,1,nz)
							v0.x = dyVz1() - dzVyN();
							v0.y = dzVxN_dxVz0();
							v0.z = dxVy0() - dyVx1();
						}
						else
						{
							//z inside the boundary
							//(0,1,1<k<nz-1)
							v0.x = dyVz1() - dzVy();
							v0.y = dzVx() - dxVz0();
							v0.z = dxVy0() - dyVx1();
						}
					}
					else if (j == 0)
					{
						//y at the lower boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(0,0,1)
							v0.x = dyVz0() - dzVy1();
							v0.y = dzVx1() - dxVz0();
							v0.z = dxVy0_dyVx0();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(0,0,0)
							v0.x = dyVz0_dzVy0();
							v0.y = dzVx0_dxVz0();
							v0.z = dxVy0_dyVx0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(0,0,nz-1)
							v0.x = dyVz0() - dzVyN1();
							v0.y = dzVxN1() - dxVz0();
							v0.z = dxVy0_dyVx0();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(0,0,nz)
							v0.x = dyVz0_dzVyN();
							v0.y = dzVxN_dxVz0();
							v0.z = dxVy0_dyVx0();
						}
						else
						{
							//z inside the boundary
							//(0,0,1<k<nz-1)
							v0.x = dyVz0() - dzVy();
							v0.y = dzVx() - dxVz0();
							v0.z = dxVy0_dyVx0();
						}
					}
					else if (j == nyN1)
					{
						//y near the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(0,ny-1,1)
							v0.x = dyVzN1_dzVy1();
							v0.y = dzVx1() - dxVz0();
							v0.z = dxVy0() - dyVxN1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(0,ny-1,0)
							v0.x = dyVzN1() - dzVy0();
							v0.y = dzVx0_dxVz0();
							v0.z = dxVy0() - dyVxN1();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(0,ny-1,nz-1)
							v0.x = dyVzN1_dzVyN1();
							v0.y = dzVxN1() - dxVz0();
							v0.z = dxVy0() - dyVxN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(0,ny-1,nz)
							v0.x = dyVzN1() - dzVyN();
							v0.y = dzVxN_dxVz0();
							v0.z = dxVy0() - dyVxN1();
						}
						else
						{
							//z inside the boundary
							//(0,ny-1,1<k<nz-1)
							v0.x = dyVzN1() - dzVy();
							v0.y = dzVx() - dxVz0();
							v0.z = dxVy0() - dyVxN1();
						}
					}
					else if (j == ny)
					{
						//y at the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(0,ny,1)
							v0.x = dyVzN() - dzVy1();
							v0.y = dzVx1() - dxVz0();
							v0.z = dxVy0_dyVxN();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(0,ny,0)
							v0.x = dyVzN_dzVy0();
							v0.y = dzVx0_dxVz0();
							v0.z = dxVy0_dyVxN();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(0,ny,nz-1)
							v0.x = dyVzN() - dzVyN1();
							v0.y = dzVxN1() - dxVz0();
							v0.z = dxVy0_dyVxN();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(0,ny,nz)
							v0.x = dyVzN_dzVyN();
							v0.y = dzVxN_dxVz0();
							v0.z = dxVy0_dyVxN();
						}
						else
						{
							//z inside the boundary
							//(0,ny,1<k<nz-1)
							v0.x = dyVzN() - dzVy();
							v0.y = dzVx() - dxVz0();
							v0.z = dxVy0_dyVxN();
						}
					}
					else
					{
						//y inside boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(0,1<j<ny-1,1)
							v0.x = dyVz() - dzVy1();
							v0.y = dzVx1() - dxVz0();
							v0.z = dxVy0() - dyVx();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(0,1<j<ny-1,0)
							v0.x = dyVz() - dzVy0();
							v0.y = dzVx0_dxVz0();
							v0.z = dxVy0() - dyVx();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(0, 1<j<ny - 1, nz-1)
							v0.x = dyVz() - dzVyN1();
							v0.y = dzVxN1() - dxVz0();
							v0.z = dxVy0() - dyVx();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(0,1<j<ny-1,nz)
							v0.x = dyVz() - dzVyN();
							v0.y = dzVxN_dxVz0();
							v0.z = dxVy0() - dyVx();
						}
						else
						{
							//z inside the boundary
							//(0,1<j<ny-1,1<k<nz-1)
							v0.x = dyVz_dzVy();
							v0.y = dzVx() - dxVz0();
							v0.z = dxVy0() - dyVx();
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
							v0.x = dyVz1_dzVy1();
							v0.y = dzVx1_dxVzN1();
							v0.z = dxVyN1_dyVx1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx-1,1,0)
							v0.x = dyVz1() - dzVy0();
							v0.y = dzVx0() - dxVzN1();
							v0.z = dxVyN1_dyVx1();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx-1,1,nz-1)
							v0.x = dyVz1_dzVyN1();
							v0.y = dzVxN1_dxVzN1();
							v0.z = dxVyN1_dyVx1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx-1,1,nz)
							v0.x = dyVz1() - dzVyN();
							v0.y = dzVxN() - dxVzN1();
							v0.z = dxVyN1_dyVx1();
						}
						else
						{
							//z inside the boundary
							//(nx-1,1,1<k<nz-1)
							v0.x = dyVz1() - dzVy();
							v0.y = dzVx() - dxVzN1();
							v0.z = dxVyN1_dyVx1();
						}
					}
					else if (j == 0)
					{
						//y at the lower boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx-1,0,1)
							v0.x = dyVz0() - dzVy1();
							v0.y = dzVx1_dxVzN1();
							v0.z = dxVyN1() - dyVx0();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx-1,0,0)
							v0.x = dyVz0_dzVy0();
							v0.y = dzVx0() - dxVzN1();
							v0.z = dxVyN1() - dyVx0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx-1,0,nz-1)
							v0.x = dyVz0() - dzVyN1();
							v0.y = dzVxN1_dxVzN1();
							v0.z = dxVyN1() - dyVx0();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx-1,0,nz)
							v0.x = dyVz0_dzVyN();
							v0.y = dzVxN() - dxVzN1();
							v0.z = dxVyN1() - dyVx0();
						}
						else
						{
							//z inside the boundary
							//(nx-1,0,1<k<nz-1)
							v0.x = dyVz0() - dzVy();
							v0.y = dzVx() - dxVzN1();
							v0.z = dxVyN1() - dyVx0();
						}
					}
					else if (j == nyN1)
					{
						//y near the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx-1,ny-1,1)
							v0.x = dyVzN1_dzVy1();
							v0.y = dzVx1_dxVzN1();
							v0.z = dxVyN1_dyVxN1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx-1,ny-1,0)
							v0.x = dyVzN1() - dzVy0();
							v0.y = dzVx0() - dxVzN1();
							v0.z = dxVyN1_dyVxN1();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx-1,ny-1,nz-1)
							v0.x = dyVzN1_dzVyN1();
							v0.y = dzVxN1_dxVzN1();
							v0.z = dxVyN1_dyVxN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx-1,ny-1,nz)
							v0.x = dyVzN1() - dzVyN();
							v0.y = dzVxN() - dxVzN1();
							v0.z = dxVyN1_dyVxN1();
						}
						else
						{
							//z inside the boundary
							//(nx-1,ny-1,1<k<nz-1)
							v0.x = dyVzN1() - dzVy();
							v0.y = dzVx() - dxVzN1();
							v0.z = dxVyN1_dyVxN1();
						}
					}
					else if (j == ny)
					{
						//y at the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx-1,ny,1)
							v0.x = dyVzN() - dzVy1();
							v0.y = dzVx1_dxVzN1();
							v0.z = dxVyN1() - dyVxN();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx-1,ny,0)
							v0.x = dyVzN_dzVy0();
							v0.y = dzVx0() - dxVzN1();
							v0.z = dxVyN1() - dyVxN();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx-1,ny,nz-1)
							v0.x = dyVzN() - dzVyN1();
							v0.y = dzVxN1_dxVzN1();
							v0.z = dxVyN1() - dyVxN();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx-1,ny,nz)
							v0.x = dyVzN_dzVyN();
							v0.y = dzVxN() - dxVzN1();
							v0.z = dxVyN1() - dyVxN();
						}
						else
						{
							//z inside the boundary
							//(nx-1,ny,1<k<nz-1)
							v0.x = dyVzN() - dzVy();
							v0.y = dzVx() - dxVzN1();
							v0.z = dxVyN1() - dyVxN();
						}
					}
					else
					{
						//y inside boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx-1,1<j<ny-1,1)
							v0.x = dyVz() - dzVy1();
							v0.y = dzVx1_dxVzN1();
							v0.z = dxVyN1() - dyVx();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx-1,1<j<ny-1,0)
							v0.x = dyVz() - dzVy0();
							v0.y = dzVx0() - dxVzN1();
							v0.z = dxVyN1() - dyVx();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx-1,1<j<ny-1,nz-1)
							v0.x = dyVz() - dzVyN1();
							v0.y = dzVxN1_dxVzN1();
							v0.z = dxVyN1() - dyVx();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx-1,1<j<ny-1,nz)
							v0.x = dyVz() - dzVyN();
							v0.y = dzVxN() - dxVzN1();
							v0.z = dxVyN1() - dyVx();
						}
						else
						{
							//z inside the boundary
							//(nx-1,1<j<ny-1,1<k<nz-1)
							v0.x = dyVz_dzVy();
							v0.y = dzVx() - dxVzN1();
							v0.z = dxVyN1() - dyVx();
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
							v0.x = dyVz1_dzVy1();
							v0.y = dzVx1() - dxVzN();
							v0.z = dxVyN() - dyVx1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx,1,0)
							v0.x = dyVz1() - dzVy0();
							v0.y = dzVx0_dxVzN();
							v0.z = dxVyN() - dyVx1();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx,1,nz-1)
							v0.x = dyVz1_dzVyN1();
							v0.y = dzVxN1() - dxVzN();
							v0.z = dxVyN() - dyVx1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx,1,nz)
							v0.x = dyVz1() - dzVyN();
							v0.y = dzVxN_dxVzN();
							v0.z = dxVyN() - dyVx1();
						}
						else
						{
							//z inside the boundary
							//(nx,1,1<k<nz-1)
							v0.x = dyVz1() - dzVy();
							v0.y = dzVx() - dxVzN();
							v0.z = dxVyN() - dyVx1();
						}
					}
					else if (j == 0)
					{
						//y at the lower boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx,0,1)
							v0.x = dyVz0() - dzVy1();
							v0.y = dzVx1() - dxVzN();
							v0.z = dxVyN_dyVx0();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx,0,0)
							v0.x = dyVz0_dzVy0();
							v0.y = dzVx0_dxVzN();
							v0.z = dxVyN_dyVx0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx,0,nz-1)
							v0.x = dyVz0() - dzVyN1();
							v0.y = dzVxN1() - dxVzN();
							v0.z = dxVyN_dyVx0();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx,0,nz)
							v0.x = dyVz0_dzVyN();
							v0.y = dzVxN_dxVzN();
							v0.z = dxVyN_dyVx0();
						}
						else
						{
							//z inside the boundary
							//(nx,0,1<k<nz-1)
							v0.x = dyVz0() - dzVy();
							v0.y = dzVx() - dxVzN();
							v0.z = dxVyN_dyVx0();
						}
					}
					else if (j == nyN1)
					{
						//y near the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx,ny-1,1)
							v0.x = dyVzN1_dzVy1();
							v0.y = dzVx1() - dxVzN();
							v0.z = dxVyN() - dyVxN1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx,ny-1,0)
							v0.x = dyVzN1() - dzVy0();
							v0.y = dzVx0_dxVzN();
							v0.z = dxVyN() - dyVxN1();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx,ny-1,nz-1)
							v0.x = dyVzN1_dzVyN1();
							v0.y = dzVxN1() - dxVzN();
							v0.z = dxVyN() - dyVxN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx,ny-1,nz)
							v0.x = dyVzN1() - dzVyN();
							v0.y = dzVxN_dxVzN();
							v0.z = dxVyN() - dyVxN1();
						}
						else
						{
							//z inside the boundary
							//(nx,ny-1,1<k<nz-1)
							v0.x = dyVzN1() - dzVy();
							v0.y = dzVx() - dxVzN();
							v0.z = dxVyN() - dyVxN1();
						}
					}
					else if (j == ny)
					{
						//y at the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx,ny,1)
							v0.x = dyVzN() - dzVy1();
							v0.y = dzVx1() - dxVzN();
							v0.z = dxVyN_dyVxN();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx,ny,0)
							v0.x = dyVzN_dzVy0();
							v0.y = dzVx0_dxVzN();
							v0.z = dxVyN_dyVxN();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx,ny,nz-1)
							v0.x = dyVzN() - dzVyN1();
							v0.y = dzVxN1() - dxVzN();
							v0.z = dxVyN_dyVxN();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx,ny,nz)
							v0.x = dyVzN_dzVyN();
							v0.y = dzVxN_dxVzN();
							v0.z = dxVyN_dyVxN();
						}
						else
						{
							//z inside the boundary
							//(nx,ny,1<k<nz-1)
							v0.x = dyVzN() - dzVy();
							v0.y = dzVx() - dxVzN();
							v0.z = dxVyN_dyVxN();
						}
					}
					else
					{
						//y inside boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx,1<j<ny-1,1)
							v0.x = dyVz() - dzVy1();
							v0.y = dzVx1() - dxVzN();
							v0.z = dxVyN() - dyVx();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx,1<j<ny-1,0)
							v0.x = dyVz() - dzVy0();
							v0.y = dzVx0_dxVzN();
							v0.z = dxVyN() - dyVx();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx,1<j<ny-1,nz-1)
							v0.x = dyVz() - dzVyN1();
							v0.y = dzVxN1() - dxVzN();
							v0.z = dxVyN() - dyVx();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx,1<j<ny-1,nz)
							v0.x = dyVz() - dzVyN();
							v0.y = dzVxN_dxVzN();
							v0.z = dxVyN() - dyVx();
						}
						else
						{
							//z inside the boundary
							//(nx,1<j<ny-1,1<k<nz-1)
							v0.x = dyVz_dzVy();
							v0.y = dzVx() - dxVzN();
							v0.z = dxVyN() - dyVx();
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
							v0.x = dyVz1_dzVy1();
							v0.y = dzVx1() - dxVz();
							v0.z = dxVy() - dyVx1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1<i<nx-1,1,0)
							v0.x = dyVz1() - dzVy0();
							v0.y = dzVx0() - dxVz();
							v0.z = dxVy() - dyVx1();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1<i<nx-1,1,nz-1)
							v0.x = dyVz1_dzVyN1();
							v0.y = dzVxN1() - dxVz();
							v0.z = dxVy() - dyVx1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1<i<nx-1,1,nz)
							v0.x = dyVz1() - dzVyN();
							v0.y = dzVxN() - dxVz();
							v0.z = dxVy() - dyVx1();
						}
						else
						{
							//z inside the boundary
							//(1<i<nx-1,1,1<k<nz-1)
							v0.x = dyVz1() - dzVy();
							v0.y = dzVx_dxVz();
							v0.z = dxVy() - dyVx1();
						}
					}
					else if (j == 0)
					{
						//y at the lower boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1<i<nx-1,0,1)
							v0.x = dyVz0() - dzVy1();
							v0.y = dzVx1() - dxVz();
							v0.z = dxVy() - dyVx0();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1<i<nx-1,0,0)
							v0.x = dyVz0_dzVy0();
							v0.y = dzVx0() - dxVz();
							v0.z = dxVy() - dyVx0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1<i<nx-1,0,nz-1)
							v0.x = dyVz0() - dzVyN1();
							v0.y = dzVxN1() - dxVz();
							v0.z = dxVy() - dyVx0();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1<i<nx-1,0,nz)
							v0.x = dyVz0_dzVyN();
							v0.y = dzVxN() - dxVz();
							v0.z = dxVy() - dyVx0();
						}
						else
						{
							//z inside the boundary
							//(1<i<nx-1,0,1<k<nz-1)
							v0.x = dyVz0() - dzVy();
							v0.y = dzVx_dxVz();
							v0.z = dxVy() - dyVx0();
						}
					}
					else if (j == nyN1)
					{
						//y near the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1<i<nx-1,ny-1,1)
							v0.x = dyVzN1_dzVy1();
							v0.y = dzVx1() - dxVz();
							v0.z = dxVy() - dyVxN1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1<i<nx-1,ny-1,0)
							v0.x = dyVzN1() - dzVy0();
							v0.y = dzVx0() - dxVz();
							v0.z = dxVy() - dyVxN1();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1<i<nx-1,ny-1,nz-1)
							v0.x = dyVzN1_dzVyN1();
							v0.y = dzVxN1() - dxVz();
							v0.z = dxVy() - dyVxN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1<i<nx-1,ny-1,nz)
							v0.x = dyVzN1() - dzVyN();
							v0.y = dzVxN() - dxVz();
							v0.z = dxVy() - dyVxN1();
						}
						else
						{
							//z inside the boundary
							//(1<i<nx-1,ny-1,1<k<nz-1)
							v0.x = dyVzN1() - dzVy();
							v0.y = dzVx_dxVz();
							v0.z = dxVy() - dyVxN1();
						}
					}
					else if (j == ny)
					{
						//y at the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1<i<nx-1,ny,1)
							v0.x = dyVzN() - dzVy1();
							v0.y = dzVx1() - dxVz();
							v0.z = dxVy() - dyVxN();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1<i<nx-1,ny,0)
							v0.x = dyVzN_dzVy0();
							v0.y = dzVx0() - dxVz();
							v0.z = dxVy() - dyVxN();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1<i<nx-1,ny,nz-1)
							v0.x = dyVzN() - dzVyN1();
							v0.y = dzVxN1() - dxVz();
							v0.z = dxVy() - dyVxN();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1<i<nx-1,ny,nz)
							v0.x = dyVzN_dzVyN();
							v0.y = dzVxN() - dxVz();
							v0.z = dxVy() - dyVxN();
						}
						else
						{
							//z inside the boundary
							//(1<i<nx-1,ny,1<k<nz-1)
							v0.x = dyVzN() - dzVy();
							v0.y = dzVx_dxVz();
							v0.z = dxVy() - dyVxN();
						}
					}
					else
					{
						//y inside boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1<i<nx-1,1<j<ny-1,1)
							v0.x = dyVz() - dzVy1();
							v0.y = dzVx1() - dxVz();
							v0.z = dxVy_dyVx();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1<i<nx-1,1<j<ny-1,0)
							v0.x = dyVz() - dzVy0();
							v0.y = dzVx0() - dxVz();
							v0.z = dxVy_dyVx();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1<i<nx-1,1<j<ny-1,nz-1)
							v0.x = dyVz() - dzVyN1();
							v0.y = dzVxN1() - dxVz();
							v0.z = dxVy_dyVx();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1<i<nx-1,1<j<ny-1,nz)
							v0.x = dyVz() - dzVyN();
							v0.y = dzVxN() - dxVz();
							v0.z = dxVy_dyVx();
						}
						else
						{
							//(x,y,z) inside boundary
							v0.x = dyVz_dzVy();
							v0.y = dzVx_dxVz();
							v0.z = dxVy_dyVx();
						}
					}
				}
				//
				v0.x = v0.x / 12;
				v0.y = v0.y / 12;
				v0.z = v0.z / 12;
				//
				pNext->setField(i, j, k, &v0);
				//
				//w++;
			}
		//}
	}
}


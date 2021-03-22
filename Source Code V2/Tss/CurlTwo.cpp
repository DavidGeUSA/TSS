/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

calculate the second order curl
********************************************************************/
#include "CurlTwo.h"


CurlTwo::CurlTwo()
{
}


CurlTwo::~CurlTwo()
{
}

void CurlTwo::setSpaceRange(unsigned int nx0, unsigned int ny0, unsigned int nz0)
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
////////////////second order///////////////////////
//inside boundary
  double CurlTwo::dx2Vx()
{
	return -30 * Field[I(i, j, k)].x + 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x) - Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x;
}

  double CurlTwo::dy2Vx()
{
	return -30 * Field[I(i, j, k)].x + 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x) - Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x;
}

  double CurlTwo::dz2Vx()
{
	return -30 * Field[I(i, j, k)].x + 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x) - Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}

  double CurlTwo::dx2Vy()
{
	return -30 * Field[I(i, j, k)].y + 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y) - Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y;
}

  double CurlTwo::dy2Vy()
{
	return -30 * Field[I(i, j, k)].y + 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y) - Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y;
}

  double CurlTwo::dz2Vy()
{
	return -30 * Field[I(i, j, k)].y + 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y) - Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}

  double CurlTwo::dx2Vz()
{
	return -30 * Field[I(i, j, k)].z + 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z) - Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z;
}

  double CurlTwo::dy2Vz()
{
	return -30 * Field[I(i, j, k)].z + 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z) - Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z;
}

  double CurlTwo::dz2Vz()
{
	return -30 * Field[I(i, j, k)].z + 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z) - Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}

//////near the lower boundary///////////////////
  double CurlTwo::dx2Vx1()
{
	return -20 * Field[I(i, j, k)].x + 6 * Field[I(i + 1, j, k)].x + 4 * Field[I(i + 2, j, k)].x - Field[I(i + 3, j, k)].x + 11 * Field[I(i - 1, j, k)].x;
}
  double CurlTwo::dy2Vx1()
{
	return -20 * Field[I(i, j, k)].x + 6 * Field[I(i, j + 1, k)].x + 4 * Field[I(i, j + 2, k)].x - Field[I(i, j + 3, k)].x + 11 * Field[I(i, j - 1, k)].x;
}
  double CurlTwo::dz2Vx1()
{
	return -20 * Field[I(i, j, k)].x + 6 * Field[I(i, j, k + 1)].x + 4 * Field[I(i, j, k + 2)].x - Field[I(i, j, k + 3)].x + 11 * Field[I(i, j, k - 1)].x;
}

  double CurlTwo::dx2Vy1()
{
	return -20 * Field[I(i, j, k)].y + 6 * Field[I(i + 1, j, k)].y + 4 * Field[I(i + 2, j, k)].y - Field[I(i + 3, j, k)].y + 11 * Field[I(i - 1, j, k)].y;
}
  double CurlTwo::dy2Vy1()
{
	return -20 * Field[I(i, j, k)].y + 6 * Field[I(i, j + 1, k)].y + 4 * Field[I(i, j + 2, k)].y - Field[I(i, j + 3, k)].y + 11 * Field[I(i, j - 1, k)].y;
}
  double CurlTwo::dz2Vy1()
{
	return -20 * Field[I(i, j, k)].y + 6 * Field[I(i, j, k + 1)].y + 4 * Field[I(i, j, k + 2)].y - Field[I(i, j, k + 3)].y + 11 * Field[I(i, j, k - 1)].y;
}

  double CurlTwo::dx2Vz1()
{
	return -20 * Field[I(i, j, k)].z + 6 * Field[I(i + 1, j, k)].z + 4 * Field[I(i + 2, j, k)].z - Field[I(i + 3, j, k)].z + 11 * Field[I(i - 1, j, k)].z;
}
  double CurlTwo::dy2Vz1()
{
	return -20 * Field[I(i, j, k)].z + 6 * Field[I(i, j + 1, k)].z + 4 * Field[I(i, j + 2, k)].z - Field[I(i, j + 3, k)].z + 11 * Field[I(i, j - 1, k)].z;
}
  double CurlTwo::dz2Vz1()
{
	return -20 * Field[I(i, j, k)].z + 6 * Field[I(i, j, k + 1)].z + 4 * Field[I(i, j, k + 2)].z - Field[I(i, j, k + 3)].z + 11 * Field[I(i, j, k - 1)].z;
}

//at the lower boundary
  double CurlTwo::dx2Vx0()
{
	return 35 * Field[I(i, j, k)].x - 104 * Field[I(i + 1, j, k)].x + 114 * Field[I(i + 2, j, k)].x - 56 * Field[I(i + 3, j, k)].x + 11 * Field[I(i + 4, j, k)].x;
}
  double CurlTwo::dy2Vx0()
{
	return 35 * Field[I(i, j, k)].x - 104 * Field[I(i, j + 1, k)].x + 114 * Field[I(i, j + 2, k)].x - 56 * Field[I(i, j + 3, k)].x + 11 * Field[I(i, j + 4, k)].x;
}
  double CurlTwo::dz2Vx0()
{
	return 35 * Field[I(i, j, k)].x - 104 * Field[I(i, j, k + 1)].x + 114 * Field[I(i, j, k + 2)].x - 56 * Field[I(i, j, k + 3)].x + 11 * Field[I(i, j, k + 4)].x;
}

  double CurlTwo::dx2Vy0()
{
	return 35 * Field[I(i, j, k)].y - 104 * Field[I(i + 1, j, k)].y + 114 * Field[I(i + 2, j, k)].y - 56 * Field[I(i + 3, j, k)].y + 11 * Field[I(i + 4, j, k)].y;
}
  double CurlTwo::dy2Vy0()
{
	return 35 * Field[I(i, j, k)].y - 104 * Field[I(i, j + 1, k)].y + 114 * Field[I(i, j + 2, k)].y - 56 * Field[I(i, j + 3, k)].y + 11 * Field[I(i, j + 4, k)].y;
}
  double CurlTwo::dz2Vy0()
{
	return 35 * Field[I(i, j, k)].y - 104 * Field[I(i, j, k + 1)].y + 114 * Field[I(i, j, k + 2)].y - 56 * Field[I(i, j, k + 3)].y + 11 * Field[I(i, j, k + 4)].y;
}

  double CurlTwo::dx2Vz0()
{
	return 35 * Field[I(i, j, k)].z - 104 * Field[I(i + 1, j, k)].z + 114 * Field[I(i + 2, j, k)].z - 56 * Field[I(i + 3, j, k)].z + 11 * Field[I(i + 4, j, k)].z;
}
  double CurlTwo::dy2Vz0()
{
	return 35 * Field[I(i, j, k)].z - 104 * Field[I(i, j + 1, k)].z + 114 * Field[I(i, j + 2, k)].z - 56 * Field[I(i, j + 3, k)].z + 11 * Field[I(i, j + 4, k)].z;
}
  double CurlTwo::dz2Vz0()
{
	return 35 * Field[I(i, j, k)].z - 104 * Field[I(i, j, k + 1)].z + 114 * Field[I(i, j, k + 2)].z - 56 * Field[I(i, j, k + 3)].z + 11 * Field[I(i, j, k + 4)].z;
}

//near the upper boundary
  double CurlTwo::dx2VxN1()
{
	return -20 * Field[I(i, j, k)].x + 6 * Field[I(i - 1, j, k)].x + 4 * Field[I(i - 2, j, k)].x - Field[I(i - 3, j, k)].x + 11 * Field[I(i + 1, j, k)].x;
}
  double CurlTwo::dy2VxN1()
{
	return -20 * Field[I(i, j, k)].x + 6 * Field[I(i, j - 1, k)].x + 4 * Field[I(i, j - 2, k)].x - Field[I(i, j - 3, k)].x + 11 * Field[I(i, j + 1, k)].x;
}
  double CurlTwo::dz2VxN1()
{
	return -20 * Field[I(i, j, k)].x + 6 * Field[I(i, j, k - 1)].x + 4 * Field[I(i, j, k - 2)].x - Field[I(i, j, k - 3)].x + 11 * Field[I(i, j, k + 1)].x;
}

  double CurlTwo::dx2VyN1()
{
	return -20 * Field[I(i, j, k)].y + 6 * Field[I(i - 1, j, k)].y + 4 * Field[I(i - 2, j, k)].y - Field[I(i - 3, j, k)].y + 11 * Field[I(i + 1, j, k)].y;
}
  double CurlTwo::dy2VyN1()
{
	return -20 * Field[I(i, j, k)].y + 6 * Field[I(i, j - 1, k)].y + 4 * Field[I(i, j - 2, k)].y - Field[I(i, j - 3, k)].y + 11 * Field[I(i, j + 1, k)].y;
}
  double CurlTwo::dz2VyN1()
{
	return -20 * Field[I(i, j, k)].y + 6 * Field[I(i, j, k - 1)].y + 4 * Field[I(i, j, k - 2)].y - Field[I(i, j, k - 3)].y + 11 * Field[I(i, j, k + 1)].y;
}

  double CurlTwo::dx2VzN1()
{
	return -20 * Field[I(i, j, k)].z + 6 * Field[I(i - 1, j, k)].z + 4 * Field[I(i - 2, j, k)].z - Field[I(i - 3, j, k)].z + 11 * Field[I(i + 1, j, k)].z;
}
  double CurlTwo::dy2VzN1()
{
	return -20 * Field[I(i, j, k)].z + 6 * Field[I(i, j - 1, k)].z + 4 * Field[I(i, j - 2, k)].z - Field[I(i, j - 3, k)].z + 11 * Field[I(i, j + 1, k)].z;
}
  double CurlTwo::dz2VzN1()
{
	return -20 * Field[I(i, j, k)].z + 6 * Field[I(i, j, k - 1)].z + 4 * Field[I(i, j, k - 2)].z - Field[I(i, j, k - 3)].z + 11 * Field[I(i, j, k + 1)].z;
}

//at the upper boundary
  double CurlTwo::dx2VxN()
{
	return 35 * Field[I(i, j, k)].x - 104 * Field[I(i - 1, j, k)].x + 114 * Field[I(i - 2, j, k)].x - 56 * Field[I(i - 3, j, k)].x + 11 * Field[I(i - 4, j, k)].x;
}
  double CurlTwo::dy2VxN()
{
	return 35 * Field[I(i, j, k)].x - 104 * Field[I(i, j - 1, k)].x + 114 * Field[I(i, j - 2, k)].x - 56 * Field[I(i, j - 3, k)].x + 11 * Field[I(i, j - 4, k)].x;
}
  double CurlTwo::dz2VxN()
{
	return 35 * Field[I(i, j, k)].x - 104 * Field[I(i, j, k - 1)].x + 114 * Field[I(i, j, k - 2)].x - 56 * Field[I(i, j, k - 3)].x + 11 * Field[I(i, j, k - 4)].x;
}

  double CurlTwo::dx2VyN()
{
	return 35 * Field[I(i, j, k)].y - 104 * Field[I(i - 1, j, k)].y + 114 * Field[I(i - 2, j, k)].y - 56 * Field[I(i - 3, j, k)].y + 11 * Field[I(i - 4, j, k)].y;
}
  double CurlTwo::dy2VyN()
{
	return 35 * Field[I(i, j, k)].y - 104 * Field[I(i, j - 1, k)].y + 114 * Field[I(i, j - 2, k)].y - 56 * Field[I(i, j - 3, k)].y + 11 * Field[I(i, j - 4, k)].y;
}
  double CurlTwo::dz2VyN()
{
	return 35 * Field[I(i, j, k)].y - 104 * Field[I(i, j, k - 1)].y + 114 * Field[I(i, j, k - 2)].y - 56 * Field[I(i, j, k - 3)].y + 11 * Field[I(i, j, k - 4)].y;
}

  double CurlTwo::dx2VzN()
{
	return 35 * Field[I(i, j, k)].z - 104 * Field[I(i - 1, j, k)].z + 114 * Field[I(i - 2, j, k)].z - 56 * Field[I(i - 3, j, k)].z + 11 * Field[I(i - 4, j, k)].z;
}
  double CurlTwo::dy2VzN()
{
	return 35 * Field[I(i, j, k)].z - 104 * Field[I(i, j - 1, k)].z + 114 * Field[I(i, j - 2, k)].z - 56 * Field[I(i, j - 3, k)].z + 11 * Field[I(i, j - 4, k)].z;
}
  double CurlTwo::dz2VzN()
{
	return 35 * Field[I(i, j, k)].z - 104 * Field[I(i, j, k - 1)].z + 114 * Field[I(i, j, k - 2)].z - 56 * Field[I(i, j, k - 3)].z + 11 * Field[I(i, j, k - 4)].z;
}


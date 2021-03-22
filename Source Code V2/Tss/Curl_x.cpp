
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

calculate the second order curl - x-axis
********************************************************************/

#include "CurlTwo.h"

//(0,0,0)
  double CurlTwo::dx2Vx0_dy2Vx0_dz2Vx0()
{
	return 105 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j + 2, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j + 3, k)].x + Field[I(i, j, k + 3)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k + 4)].x);
}
//(0,0,1)
  double CurlTwo::dx2Vx0_dy2Vx0_dz2Vx1()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j + 2, k)].x)
		-  56 * (Field[I(i + 3, j, k)].x + Field[I(i, j + 3, k)].x)
		+  11 * (Field[I(i + 4, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k - 1)].x)
		+ 6 * Field[I(i, j, k + 1)].x + 4 * Field[I(i, j, k + 2)].x - Field[I(i, j, k + 3)].x;
}
//(0,0,N1)
  double CurlTwo::dx2Vx0_dy2Vx0_dz2VxN1()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j + 2, k)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j + 3, k)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k + 1)].x)
		+ 6 * Field[I(i, j, k - 1)].x + 4 * Field[I(i, j, k - 2)].x - Field[I(i, j, k - 3)].x;
}
//(0,0,N)
  double CurlTwo::dx2Vx0_dy2Vx0_dz2VxN()
{
	return 105 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j + 2, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j + 3, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k - 4)].x);
}
//(0,0,1<k<nz-1)
  double CurlTwo::dx2Vx0_dy2Vx0_dz2Vx()
{
	return 40 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j + 2, k)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j + 3, k)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j + 4, k)].x)
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x) - Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//(0,1,0)
  double CurlTwo::dx2Vx0_dy2Vx1_dz2Vx0()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j, k + 3)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j, k + 4)].x + Field[I(i, j - 1, k)].x)
		+ 6 * Field[I(i, j + 1, k)].x + 4 * Field[I(i, j + 2, k)].x - Field[I(i, j + 3, k)].x;
}
//(0,1,1)
  double CurlTwo::dx2Vx0_dy2Vx1_dz2Vx1()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i, j + 3, k)].x - Field[I(i, j, k + 3)].x
		+ 11 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x + Field[I(i + 4, j, k)].x)
		- 104 * Field[I(i + 1, j, k)].x + 114 * Field[I(i + 2, j, k)].x - 56 * Field[I(i + 3, j, k)].x;

}
//(0,1,N1)
  double CurlTwo::dx2Vx0_dy2Vx1_dz2VxN1()
{
	return -5 * Field[I(i, j, k)].x
		- 104 * Field[I(i + 1, j, k)].x + 114 * Field[I(i + 2, j, k)].x
		- 56 * Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 6 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i, j + 3, k)].x
		- Field[I(i, j, k - 3)].x;
}
//(0,1,N)
  double CurlTwo::dx2Vx0_dy2Vx1_dz2VxN()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k - 4)].x)
		+ 6 * Field[I(i, j + 1, k)].x + 4 * Field[I(i, j + 2, k)].x - Field[I(i, j + 3, k)].x;
}
//(0,1,1<k<nz-1)
  double CurlTwo::dx2Vx0_dy2Vx1_dz2Vx()
{
	return -15 * Field[I(i, j, k)].x
		- 104 * Field[I(i + 1, j, k)].x
		+ 114 * Field[I(i + 2, j, k)].x
		- 56 * Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 6 * Field[I(i, j + 1, k)].x + 4 * Field[I(i, j + 2, k)].x - Field[I(i, j + 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x) - Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//(0,N1,0)
  double CurlTwo::dx2Vx0_dy2VxN1_dz2Vx0()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k + 2)].x)
		-  56 * (Field[I(i + 3, j, k)].x + Field[I(i, j, k + 3)].x)
		+  11 * (Field[I(i + 4, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 4)].x)
		+ 6 * Field[I(i, j - 1, k)].x + 4 * Field[I(i, j - 2, k)].x - Field[I(i, j - 3, k)].x;
}
//(0,N1,1)
  double CurlTwo::dx2Vx0_dy2VxN1_dz2Vx1()
{
	return -5 * Field[I(i, j, k)].x - 104 * Field[I(i + 1, j, k)].x + 114 * Field[I(i + 2, j, k)].x - 56 * Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 6 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k + 2)].x) - Field[I(i, j - 3, k)].x
		- Field[I(i, j, k + 3)].x;
}

//(0,N1,N1)
  double CurlTwo::dx2Vx0_dy2VxN1_dz2VxN1()
{
	return -5 * Field[I(i, j, k)].x - 104 * Field[I(i + 1, j, k)].x + 114 * Field[I(i + 2, j, k)].x
		- 56 * Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 6 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i, j - 3, k)].x
		- Field[I(i, j, k - 3)].x;

}
//(0,N1,N)
  double CurlTwo::dx2Vx0_dy2VxN1_dz2VxN()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 4)].x)
		+ 6 * Field[I(i, j - 1, k)].x + 4 * Field[I(i, j - 2, k)].x - Field[I(i, j - 3, k)].x;
}
//(0,N1,1<k<nz-1)
  double CurlTwo::dx2Vx0_dy2VxN1_dz2Vx()
{
	return -15 * Field[I(i, j, k)].x - 104 * Field[I(i + 1, j, k)].x
		+ 114 * Field[I(i + 2, j, k)].x - 56 * Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 6 * Field[I(i, j - 1, k)].x + 4 * Field[I(i, j - 2, k)].x - Field[I(i, j - 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x) - Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}

//(0,N,0)
  double CurlTwo::dx2Vx0_dy2VxN_dz2Vx0()
{
	return 105 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j - 2, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j - 3, k)].x + Field[I(i, j, k + 3)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k + 4)].x);
}
//(0,N,1)
  double CurlTwo::dx2Vx0_dy2VxN_dz2Vx1()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j - 2, k)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j - 3, k)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k - 1)].x)
		+ 6 * Field[I(i, j, k + 1)].x + 4 * Field[I(i, j, k + 2)].x - Field[I(i, j, k + 3)].x;

}
//(0,N,N1)
  double CurlTwo::dx2Vx0_dy2VxN_dz2VxN1()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j - 2, k)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j - 3, k)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k + 1)].x)
		+ 6 * Field[I(i, j, k - 1)].x + 4 * Field[I(i, j, k - 2)].x - Field[I(i, j, k - 3)].x;

}
//(0,N,N)
  double CurlTwo::dx2Vx0_dy2VxN_dz2VxN()
{
	return 105 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j - 2, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j - 3, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k - 4)].x);
}
//(0,N,1<k<nz-1)
  double CurlTwo::dx2Vx0_dy2VxN_dz2Vx()
{
	return 40 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j - 2, k)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j - 3, k)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j - 4, k)].x)
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x) - Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}

//(0,1<j<ny-1,0)
  double CurlTwo::dx2Vx0_dy2Vx_dz2Vx0()
{
	return 40 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j, k + 3)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j, k + 4)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x;
}
//(0,1<j<ny-1,1)
  double CurlTwo::dx2Vx0_dy2Vx_dz2Vx1()
{
	return -15 * Field[I(i, j, k)].x
		- 104 * Field[I(i + 1, j, k)].x
		+ 114 * Field[I(i + 2, j, k)].x
		- 56 * Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		+ 6 * Field[I(i, j, k + 1)].x + 4 * Field[I(i, j, k + 2)].x - Field[I(i, j, k + 3)].x;
}
//(0,1<j<ny-1,N1)
  double CurlTwo::dx2Vx0_dy2Vx_dz2VxN1()
{
	return -15 * Field[I(i, j, k)].x
		- 104 * Field[I(i + 1, j, k)].x
		+ 114 * Field[I(i + 2, j, k)].x
		- 56 * Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		+ 6 * Field[I(i, j, k - 1)].x + 4 * Field[I(i, j, k - 2)].x - Field[I(i, j, k - 3)].x;

}
//(0,1<j<ny-1,N)
  double CurlTwo::dx2Vx0_dy2Vx_dz2VxN()
{
	return 40 * Field[I(i, j, k)].x
		- 104 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i + 3, j, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i + 4, j, k)].x + Field[I(i, j, k - 4)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x;
}
//(0,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2Vx0_dy2Vx_dz2Vx()
{
	return -25 * Field[I(i, j, k)].x
		- 104 * Field[I(i + 1, j, k)].x
		+ 114 * Field[I(i + 2, j, k)].x
		- 56 * Field[I(i + 3, j, k)].x
		+ 11 * Field[I(i + 4, j, k)].x
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;

}

//(1,0,0)
  double CurlTwo::dx2Vx1_dy2Vx0_dz2Vx0()
{
	return 50 * Field[I(i, j, k)].x
		+ 6 * Field[I(i + 1, j, k)].x
		+ 4 * Field[I(i + 2, j, k)].x
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k + 4)].x)
		- 104 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i, j + 3, k)].x + Field[I(i, j, k + 3)].x);
}
//(1,0,1)
  double CurlTwo::dx2Vx1_dy2Vx0_dz2Vx1()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k - 1)].x)
		- 104 * Field[I(i, j + 1, k)].x
		+ 114 * Field[I(i, j + 2, k)].x
		- 56 * Field[I(i, j + 3, k)].x
		- Field[I(i, j, k + 3)].x;

}
//(1,0,N1)
  double CurlTwo::dx2Vx1_dy2Vx0_dz2VxN1()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k + 1)].x)
		- 104 * Field[I(i, j + 1, k)].x
		+ 114 * Field[I(i, j + 2, k)].x
		- 56 * Field[I(i, j + 3, k)].x
		- Field[I(i, j, k - 3)].x;
}
//(1,0,N)
  double CurlTwo::dx2Vx1_dy2Vx0_dz2VxN()
{
	return 50 * Field[I(i, j, k)].x
		+ 6 * Field[I(i + 1, j, k)].x
		+ 4 * Field[I(i + 2, j, k)].x
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k - 4)].x)
		- 104 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i, j + 3, k)].x + Field[I(i, j, k - 3)].x);
}
//(1,0,1<k<nz-1)
  double CurlTwo::dx2Vx1_dy2Vx0_dz2Vx()
{
	return -15 * Field[I(i, j, k)].x
		+ 6 * Field[I(i + 1, j, k)].x
		+ 4 * Field[I(i + 2, j, k)].x
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 4, k)].x)
		- 104 * Field[I(i, j + 1, k)].x
		+ 114 * Field[I(i, j + 2, k)].x
		- 56 * Field[I(i, j + 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}

//
//(1,1,0)
  double CurlTwo::dx2Vx1_dy2Vx1_dz2Vx0()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j + 2, k)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 4)].x)
		- Field[I(i, j + 3, k)].x
		- 104 * Field[I(i, j, k + 1)].x + 114 * Field[I(i, j, k + 2)].x - 56 * Field[I(i, j, k + 3)].x;

}
//(1,1,1)
  double CurlTwo::dx2Vx1_dy2Vx1_dz2Vx1()
{
	return -60 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j + 2, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j + 3, k)].x
		- Field[I(i, j, k + 3)].x;

}
//(1,1,N1)
  double CurlTwo::dx2Vx1_dy2Vx1_dz2VxN1()
{
	return -60 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j + 2, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		- Field[I(i, j + 3, k)].x
		- Field[I(i, j, k - 3)].x;

}
//(1,1,N)
  double CurlTwo::dx2Vx1_dy2Vx1_dz2VxN()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j + 2, k)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k - 4)].x)
		- Field[I(i, j + 3, k)].x
		- 104 * Field[I(i, j, k - 1)].x + 114 * Field[I(i, j, k - 2)].x
		- 56 * Field[I(i, j, k - 3)].x;

}
//(1,1,1<k<nz-1)
  double CurlTwo::dx2Vx1_dy2Vx1_dz2Vx()
{
	return -70 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j + 2, k)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(1,N1,0)
  double CurlTwo::dx2Vx1_dy2VxN1_dz2Vx0()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j - 2, k)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 4)].x)
		- Field[I(i, j - 3, k)].x
		- 104 * Field[I(i, j, k + 1)].x + 114 * Field[I(i, j, k + 2)].x
		- 56 * Field[I(i, j, k + 3)].x;

}
//(1,N1,1)
  double CurlTwo::dx2Vx1_dy2VxN1_dz2Vx1()
{
	return -60 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j - 2, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j - 3, k)].x
		- Field[I(i, j, k + 3)].x;
}
//(1,N1,N1)
  double CurlTwo::dx2Vx1_dy2VxN1_dz2VxN1()
{
	return -60 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j - 2, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		- Field[I(i, j - 3, k)].x
		- Field[I(i, j, k - 3)].x;

}
//(1,N1,N)
  double CurlTwo::dx2Vx1_dy2VxN1_dz2VxN()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j - 2, k)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 4)].x)
		- Field[I(i, j - 3, k)].x
		- 104 * Field[I(i, j, k - 1)].x + 114 * Field[I(i, j, k - 2)].x - 56 * Field[I(i, j, k - 3)].x;

}
//(1,N1,1<k<nz-1)
  double CurlTwo::dx2Vx1_dy2VxN1_dz2Vx()
{
	return -70 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j - 2, k)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x)
		- Field[I(i, j - 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;

}
//
//(1,N,0)
  double CurlTwo::dx2Vx1_dy2VxN_dz2Vx0()
{
	return 50 * Field[I(i, j, k)].x + 6 * Field[I(i + 1, j, k)].x + 4 * Field[I(i + 2, j, k)].x
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k + 4)].x)
		- 104 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i, j - 3, k)].x + Field[I(i, j, k + 3)].x);
}
//(1,N,1)
  double CurlTwo::dx2Vx1_dy2VxN_dz2Vx1()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k - 1)].x)
		- 104 * Field[I(i, j - 1, k)].x + 114 * Field[I(i, j - 2, k)].x
		- 56 * Field[I(i, j - 3, k)].x
		- Field[I(i, j, k + 3)].x;

}
//(1,N,N1)
  double CurlTwo::dx2Vx1_dy2VxN_dz2VxN1()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k + 1)].x)
		- 104 * Field[I(i, j - 1, k)].x + 114 * Field[I(i, j - 2, k)].x
		- 56 * Field[I(i, j - 3, k)].x
		- Field[I(i, j, k - 3)].x;

}
//(1,N,N)
  double CurlTwo::dx2Vx1_dy2VxN_dz2VxN()
{
	return 50 * Field[I(i, j, k)].x + 6 * Field[I(i + 1, j, k)].x + 4 * Field[I(i + 2, j, k)].x
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k - 4)].x)
		- 104 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i, j - 3, k)].x + Field[I(i, j, k - 3)].x);
}
//(1,N,1<k<nz-1)
  double CurlTwo::dx2Vx1_dy2VxN_dz2Vx()
{
	return -15 * Field[I(i, j, k)].x + 6 * Field[I(i + 1, j, k)].x + 4 * Field[I(i + 2, j, k)].x
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 4, k)].x)
		- 104 * Field[I(i, j - 1, k)].x + 114 * Field[I(i, j - 2, k)].x
		- 56 * Field[I(i, j - 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(1,1<j<ny-1,0)
  double CurlTwo::dx2Vx1_dy2Vx_dz2Vx0()
{
	return -15 * Field[I(i, j, k)].x
		+ 6 * Field[I(i + 1, j, k)].x + 4 * Field[I(i + 2, j, k)].x
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k + 4)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- 104 * Field[I(i, j, k + 1)].x + 114 * Field[I(i, j, k + 2)].x - 56 * Field[I(i, j, k + 3)].x;

}
//(1,1<j<ny-1,1)
  double CurlTwo::dx2Vx1_dy2Vx_dz2Vx1()
{
	return -70 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- Field[I(i, j, k + 3)].x;

}
//(1,1<j<ny-1,N1)
  double CurlTwo::dx2Vx1_dy2Vx_dz2VxN1()
{
	return -70 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i + 2, j, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i + 3, j, k)].x
		+ 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- Field[I(i, j, k - 3)].x;
}
//(1,1<j<ny-1,N)
  double CurlTwo::dx2Vx1_dy2Vx_dz2VxN()
{
	return -15 * Field[I(i, j, k)].x
		+ 6 * Field[I(i + 1, j, k)].x + 4 * Field[I(i + 2, j, k)].x
		- Field[I(i + 3, j, k)].x + 11 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k - 4)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- 104 * Field[I(i, j, k - 1)].x + 114 * Field[I(i, j, k - 2)].x - 56 * Field[I(i, j, k - 3)].x;
}
//(1,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2Vx1_dy2Vx_dz2Vx()
{
	return -80 * Field[I(i, j, k)].x
		+ 6 * Field[I(i + 1, j, k)].x + 4 * Field[I(i + 2, j, k)].x
		- Field[I(i + 3, j, k)].x + 11 * Field[I(i - 1, j, k)].x
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//-------------------------------------------
//---start of i=nx-1-----------------------------------
//(nx-1,0,0)
  double CurlTwo::dx2VxN1_dy2Vx0_dz2Vx0()
{
	return 50 * Field[I(i, j, k)].x + 6 * Field[I(i - 1, j, k)].x + 4 * Field[I(i - 2, j, k)].x
		- Field[I(i - 3, j, k)].x + 11 * Field[I(i + 1, j, k)].x
		- 104 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x) + 114 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i, j + 3, k)].x + Field[I(i, j, k + 3)].x) + 11 * (Field[I(i, j + 4, k)].x + Field[I(i, j, k + 4)].x);
}
//(nx-1,0,1)
  double CurlTwo::dx2VxN1_dy2Vx0_dz2Vx1()
{
	return -5 * Field[I(i, j, k)].x + 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i - 3, j, k)].x + 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k - 1)].x)
		- 104 * Field[I(i, j + 1, k)].x + 114 * Field[I(i, j + 2, k)].x
		- 56 * Field[I(i, j + 3, k)].x + 11 * Field[I(i, j + 4, k)].x
		- Field[I(i, j, k + 3)].x;

}
//(nx-1,0,N1)
  double CurlTwo::dx2VxN1_dy2Vx0_dz2VxN1()
{
	return -5 * Field[I(i, j, k)].x + 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k + 1)].x)
		- 104 * Field[I(i, j + 1, k)].x + 114 * Field[I(i, j + 2, k)].x
		- 56 * Field[I(i, j + 3, k)].x - Field[I(i, j, k - 3)].x;
}
//(nx-1,0,N)
  double CurlTwo::dx2VxN1_dy2Vx0_dz2VxN()
{
	return 50 * Field[I(i, j, k)].x + 6 * Field[I(i - 1, j, k)].x + 4 * Field[I(i - 2, j, k)].x
		- Field[I(i - 3, j, k)].x + 11 * Field[I(i + 1, j, k)].x
		- 104 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i, j + 3, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i, j + 4, k)].x + Field[I(i, j, k - 4)].x);
}
//(nx-1,0,1<k<nz-1)
  double CurlTwo::dx2VxN1_dy2Vx0_dz2Vx()
{
	return -15 * Field[I(i, j, k)].x + 6 * Field[I(i - 1, j, k)].x + 4 * Field[I(i - 2, j, k)].x
		- Field[I(i - 3, j, k)].x + 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 4, k)].x)
		- 104 * Field[I(i, j + 1, k)].x + 114 * Field[I(i, j + 2, k)].x
		- 56 * Field[I(i, j + 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(nx-1,1,0)
  double CurlTwo::dx2VxN1_dy2Vx1_dz2Vx0()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j + 2, k)].x) - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 4)].x)
		- Field[I(i, j + 3, k)].x
		- 104 * Field[I(i, j, k + 1)].x + 114 * Field[I(i, j, k + 2)].x - 56 * Field[I(i, j, k + 3)].x;
}
//(nx-1,1,1)
  double CurlTwo::dx2VxN1_dy2Vx1_dz2Vx1()
{
	return -60 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j + 2, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j + 3, k)].x - Field[I(i, j, k + 3)].x;
}
//(nx-1,1,N1)
  double CurlTwo::dx2VxN1_dy2Vx1_dz2VxN1()
{
	return -60 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j + 2, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		- Field[I(i, j + 3, k)].x - Field[I(i, j, k - 3)].x;
}
//(nx-1,1,N)
  double CurlTwo::dx2VxN1_dy2Vx1_dz2VxN()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j + 2, k)].x) - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k - 4)].x)
		- Field[I(i, j + 3, k)].x
		- 104 * Field[I(i, j, k - 1)].x + 114 * Field[I(i, j, k - 2)].x - 56 * Field[I(i, j, k - 3)].x;
}
//(nx-1,1,1<k<nz-1)
  double CurlTwo::dx2VxN1_dy2Vx1_dz2Vx()
{
	return -70 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j + 2, k)].x) - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x) - Field[I(i, j, k + 2)].x
		- Field[I(i, j, k - 2)].x;
}
//
//(nx-1,ny-1,0)
  double CurlTwo::dx2VxN1_dy2VxN1_dz2Vx0()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j - 2, k)].x) - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 4)].x)
		- Field[I(i, j - 3, k)].x
		- 104 * Field[I(i, j, k + 1)].x + 114 * Field[I(i, j, k + 2)].x - 56 * Field[I(i, j, k + 3)].x;
}
//(nx-1,N1,1)
  double CurlTwo::dx2VxN1_dy2VxN1_dz2Vx1()
{
	return -60 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j - 2, k)].x + Field[I(i, j, k + 2)].x) - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j - 3, k)].x - Field[I(i, j, k + 3)].x;
}
//(nx-1,N1,N1)
  double CurlTwo::dx2VxN1_dy2VxN1_dz2VxN1()
{
	return -60 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j - 2, k)].x + Field[I(i, j, k - 2)].x) - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		- Field[I(i, j - 3, k)].x - Field[I(i, j, k - 3)].x;
}
//(nx-1,N1,N)
  double CurlTwo::dx2VxN1_dy2VxN1_dz2VxN()
{
	return -5 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j - 2, k)].x) - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 4)].x)
		- Field[I(i, j - 3, k)].x
		- 104 * Field[I(i, j, k - 1)].x + 114 * Field[I(i, j, k - 2)].x - 56 * Field[I(i, j, k - 3)].x;

}
//(nx-1,N1,1<k<nz-1)
  double CurlTwo::dx2VxN1_dy2VxN1_dz2Vx()
{
	return -70 * Field[I(i, j, k)].x
		+ 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j - 2, k)].x) - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j + 1, k)].x)
		- Field[I(i, j - 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(nx-1,N,0)
  double CurlTwo::dx2VxN1_dy2VxN_dz2Vx0()
{
	return 50 * Field[I(i, j, k)].x + 6 * Field[I(i - 1, j, k)].x
		+ 4 * Field[I(i - 2, j, k)].x - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k + 4)].x)
		- 104 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i, j - 3, k)].x + Field[I(i, j, k + 3)].x);
}
//(nx-1,N,1)
  double CurlTwo::dx2VxN1_dy2VxN_dz2Vx1()
{
	return -5 * Field[I(i, j, k)].x + 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k + 2)].x) - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k - 1)].x)
		- 104 * Field[I(i, j - 1, k)].x + 114 * Field[I(i, j - 2, k)].x - 56 * Field[I(i, j - 3, k)].x
		- Field[I(i, j, k + 3)].x;
}
//(nx-1,N,N1)
  double CurlTwo::dx2VxN1_dy2VxN_dz2VxN1()
{
	return -5 * Field[I(i, j, k)].x + 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k - 2)].x) - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k + 1)].x)
		- 104 * Field[I(i, j - 1, k)].x + 114 * Field[I(i, j - 2, k)].x - 56 * Field[I(i, j - 3, k)].x
		- Field[I(i, j, k - 3)].x;
}
//(nx-1,N,N)
  double CurlTwo::dx2VxN1_dy2VxN_dz2VxN()
{
	return 50 * Field[I(i, j, k)].x + 6 * Field[I(i - 1, j, k)].x
		+ 4 * Field[I(i - 2, j, k)].x - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k - 4)].x)
		- 104 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i, j - 3, k)].x + Field[I(i, j, k - 3)].x);
}
//(nx-1,N,1<k<nz-1)
  double CurlTwo::dx2VxN1_dy2VxN_dz2Vx()
{
	return -15 * Field[I(i, j, k)].x + 6 * Field[I(i - 1, j, k)].x
		+ 4 * Field[I(i - 2, j, k)].x - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j - 4, k)].x)
		- 104 * Field[I(i, j - 1, k)].x + 114 * Field[I(i, j - 2, k)].x - 56 * Field[I(i, j - 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(nx-1,1<j<ny-1,0)
  double CurlTwo::dx2VxN1_dy2Vx_dz2Vx0()
{
	return -15 * Field[I(i, j, k)].x + 6 * Field[I(i - 1, j, k)].x
		+ 4 * Field[I(i - 2, j, k)].x - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k + 4)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- 104 * Field[I(i, j, k + 1)].x + 114 * Field[I(i, j, k + 2)].x - 56 * Field[I(i, j, k + 3)].x;
}
//(nx-1,1<j<ny-1,1)
  double CurlTwo::dx2VxN1_dy2Vx_dz2Vx1()
{
	return -70 * Field[I(i, j, k)].x + 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x - Field[I(i, j, k + 3)].x;
}
//(nx-1,1<j<ny-1,N1)
  double CurlTwo::dx2VxN1_dy2Vx_dz2VxN1()
{
	return -70 * Field[I(i, j, k)].x + 6 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i - 3, j, k)].x + 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x - Field[I(i, j, k - 3)].x;
}
//(nx-1,1<j<ny-1,N)
  double CurlTwo::dx2VxN1_dy2Vx_dz2VxN()
{
	return -15 * Field[I(i, j, k)].x + 6 * Field[I(i - 1, j, k)].x
		+ 4 * Field[I(i - 2, j, k)].x - Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i + 1, j, k)].x + Field[I(i, j, k - 4)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- 104 * Field[I(i, j, k - 1)].x + 114 * Field[I(i, j, k - 2)].x - 56 * Field[I(i, j, k - 3)].x;

}
//(nx-1,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2VxN1_dy2Vx_dz2Vx()
{
	return -80 * Field[I(i, j, k)].x + 6 * Field[I(i - 1, j, k)].x
		+ 4 * Field[I(i - 2, j, k)].x - Field[I(i - 3, j, k)].x + 11 * Field[I(i + 1, j, k)].x
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//-end of i=N1------------------------------------------
//
//-start of i=N-----------------------------------------
//(nx,0,0)
  double CurlTwo::dx2VxN_dy2Vx0_dz2Vx0()
{
	return 105 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j + 2, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j + 3, k)].x + Field[I(i, j, k + 3)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k + 4)].x);
}
//(nx,0,1)
  double CurlTwo::dx2VxN_dy2Vx0_dz2Vx1()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j + 2, k)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j + 3, k)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k - 1)].x)
		+ 6 * Field[I(i, j, k + 1)].x + 4 * Field[I(i, j, k + 2)].x - Field[I(i, j, k + 3)].x;
}
//(nx,0,N1)
  double CurlTwo::dx2VxN_dy2Vx0_dz2VxN1()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j + 2, k)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j + 3, k)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k + 1)].x)
		+ 6 * Field[I(i, j, k - 1)].x + 4 * Field[I(i, j, k - 2)].x - Field[I(i, j, k - 3)].x;

}
//(nx,0,N)
  double CurlTwo::dx2VxN_dy2Vx0_dz2VxN()
{
	return 105 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j + 2, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j + 3, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j + 4, k)].x + Field[I(i, j, k - 4)].x);
}
//(nx,0,1<k<nz-1)
  double CurlTwo::dx2VxN_dy2Vx0_dz2Vx()
{
	return 40 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j + 2, k)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j + 3, k)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j + 4, k)].x)
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(nx,1,0)
  double CurlTwo::dx2VxN_dy2Vx1_dz2Vx0()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j, k + 3)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 4)].x)
		+ 6 * Field[I(i, j + 1, k)].x + 4 * Field[I(i, j + 2, k)].x - Field[I(i, j + 3, k)].x;
}
//(nx,1,1)
  double CurlTwo::dx2VxN_dy2Vx1_dz2Vx1()
{
	return -5 * Field[I(i, j, k)].x
		- 104 * Field[I(i - 1, j, k)].x
		+ 114 * Field[I(i - 2, j, k)].x
		- 56 * Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 6 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i, j + 3, k)].x - Field[I(i, j, k + 3)].x;
}
//(nx,1,N1)
  double CurlTwo::dx2VxN_dy2Vx1_dz2VxN1()
{
	return -5 * Field[I(i, j, k)].x
		- 104 * Field[I(i - 1, j, k)].x
		+ 114 * Field[I(i - 2, j, k)].x
		- 56 * Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 6 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i, j + 3, k)].x - Field[I(i, j, k - 3)].x;
}
//(nx,1,N)
  double CurlTwo::dx2VxN_dy2Vx1_dz2VxN()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k - 4)].x)
		+ 6 * Field[I(i, j + 1, k)].x + 4 * Field[I(i, j + 2, k)].x - Field[I(i, j + 3, k)].x;
}
//(nx,1,1<k<nz-1)
  double CurlTwo::dx2VxN_dy2Vx1_dz2Vx()
{
	return -15 * Field[I(i, j, k)].x
		- 104 * Field[I(i - 1, j, k)].x
		+ 114 * Field[I(i - 2, j, k)].x
		- 56 * Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 6 * Field[I(i, j + 1, k)].x + 4 * Field[I(i, j + 2, k)].x - Field[I(i, j + 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(nx,ny-1,0)
  double CurlTwo::dx2VxN_dy2VxN1_dz2Vx0()
{
	return 50 * Field[I(i, j, k)].x - 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j, k + 3)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 4)].x)
		+ 6 * Field[I(i, j - 1, k)].x + 4 * Field[I(i, j - 2, k)].x - Field[I(i, j - 3, k)].x;
}
//(nx,N1,1)
  double CurlTwo::dx2VxN_dy2VxN1_dz2Vx1()
{
	return -5 * Field[I(i, j, k)].x - 104 * Field[I(i - 1, j, k)].x
		+ 114 * Field[I(i - 2, j, k)].x - 56 * Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 6 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i, j - 3, k)].x - Field[I(i, j, k + 3)].x;
}
//(nx,N1,N1)
  double CurlTwo::dx2VxN_dy2VxN1_dz2VxN1()
{
	return -5 * Field[I(i, j, k)].x - 104 * Field[I(i - 1, j, k)].x
		+ 114 * Field[I(i - 2, j, k)].x - 56 * Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 6 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i, j - 3, k)].x - Field[I(i, j, k - 3)].x;
}
//(nx,N1,N)
  double CurlTwo::dx2VxN_dy2VxN1_dz2VxN()
{
	return 50 * Field[I(i, j, k)].x - 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j, k - 4)].x)
		+ 6 * Field[I(i, j - 1, k)].x + 4 * Field[I(i, j - 2, k)].x - Field[I(i, j - 3, k)].x;
}
//(nx,N1,1<k<nz-1)
  double CurlTwo::dx2VxN_dy2VxN1_dz2Vx()
{
	return -15 * Field[I(i, j, k)].x - 104 * Field[I(i - 1, j, k)].x
		+ 114 * Field[I(i - 2, j, k)].x - 56 * Field[I(i - 3, j, k)].x
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j + 1, k)].x)
		+ 6 * Field[I(i, j - 1, k)].x + 4 * Field[I(i, j - 2, k)].x - Field[I(i, j - 3, k)].x
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(nx,N,0)
  double CurlTwo::dx2VxN_dy2VxN_dz2Vx0()
{
	return 105 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j - 2, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j - 3, k)].x + Field[I(i, j, k + 3)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k + 4)].x);
}
//(nx,N,1)
  double CurlTwo::dx2VxN_dy2VxN_dz2Vx1()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j - 2, k)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j - 3, k)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k - 1)].x)
		+ 6 * Field[I(i, j, k + 1)].x + 4 * Field[I(i, j, k + 2)].x - Field[I(i, j, k + 3)].x;

}
//(nx,N,N1)
  double CurlTwo::dx2VxN_dy2VxN_dz2VxN1()
{
	return 50 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j - 2, k)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j - 3, k)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k + 1)].x)
		+ 6 * Field[I(i, j, k - 1)].x + 4 * Field[I(i, j, k - 2)].x - Field[I(i, j, k - 3)].x;

}
//(nx,N,N)
  double CurlTwo::dx2VxN_dy2VxN_dz2VxN()
{
	return 105 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j - 2, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j - 3, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j - 4, k)].x + Field[I(i, j, k - 4)].x);
}
//(nx,N,1<k<nz-1)
  double CurlTwo::dx2VxN_dy2VxN_dz2Vx()
{
	return 40 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j - 1, k)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j - 2, k)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j - 3, k)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j - 4, k)].x)
		+ 16 * (Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;

}
//
//(nx,1<j<ny-1,0)
  double CurlTwo::dx2VxN_dy2Vx_dz2Vx0()
{
	return 40 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j, k + 3)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j, k + 4)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x;
}
//(nx,1<j<ny-1,1)
  double CurlTwo::dx2VxN_dy2Vx_dz2Vx1()
{
	return -15 * Field[I(i, j, k)].x
		- 104 * Field[I(i - 1, j, k)].x
		+ 114 * Field[I(i - 2, j, k)].x
		-  56 * Field[I(i - 3, j, k)].x
		+  11 * (Field[I(i - 4, j, k)].x + Field[I(i, j, k - 1)].x)
		+  16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		+ 6 * Field[I(i, j, k + 1)].x + 4 * Field[I(i, j, k + 2)].x - Field[I(i, j, k + 3)].x;
}
//(nx,1<j<ny-1,N1)
  double CurlTwo::dx2VxN_dy2Vx_dz2VxN1()
{
	return -15 * Field[I(i, j, k)].x
		- 104 * Field[I(i - 1, j, k)].x
		+ 114 * Field[I(i - 2, j, k)].x
		-  56 * Field[I(i - 3, j, k)].x
		+  11 * (Field[I(i - 4, j, k)].x + Field[I(i, j, k + 1)].x)
		+  16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		+ 6 * Field[I(i, j, k - 1)].x + 4 * Field[I(i, j, k - 2)].x - Field[I(i, j, k - 3)].x;
}
//(nx,1<j<ny-1,N)
  double CurlTwo::dx2VxN_dy2Vx_dz2VxN()
{
	return 40 * Field[I(i, j, k)].x
		- 104 * (Field[I(i - 1, j, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i - 2, j, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i - 3, j, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i - 4, j, k)].x + Field[I(i, j, k - 4)].x)
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x;
}
//(nx,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2VxN_dy2Vx_dz2Vx()
{
	return -25 * Field[I(i, j, k)].x
		- 104 * Field[I(i - 1, j, k)].x
		+ 114 * Field[I(i - 2, j, k)].x
		- 56 * Field[I(i - 3, j, k)].x
		+ 11 * Field[I(i - 4, j, k)].x
		+ 16 * (Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//-end of i=nx==============================
//==start of 1<i<nx-1========================
//(1<i<nx-1,0,0)
  double CurlTwo::dx2Vx_dy2Vx0_dz2Vx0()
{
	return 40 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- 104 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i, j + 3, k)].x + Field[I(i, j, k + 3)].x)
		+ 11 * (Field[I(i, j + 4, k)].x + Field[I(i, j, k + 4)].x);
}
//(1<i<nx-1,0,1)
  double CurlTwo::dx2Vx_dy2Vx0_dz2Vx1()
{
	return -15 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- 104 * Field[I(i, j + 1, k)].x
		+ 114 * Field[I(i, j + 2, k)].x
		- 56 * Field[I(i, j + 3, k)].x
		+ 11 * (Field[I(i, j + 4, k)].x + Field[I(i, j, k - 1)].x)
		+ 6 * Field[I(i, j, k + 1)].x + 4 * Field[I(i, j, k + 2)].x - Field[I(i, j, k + 3)].x;
}
//(1<i<nx-1,0,N1)
  double CurlTwo::dx2Vx_dy2Vx0_dz2VxN1()
{
	return -15 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- 104 * Field[I(i, j + 1, k)].x
		+ 114 * Field[I(i, j + 2, k)].x
		- 56 * Field[I(i, j + 3, k)].x
		+ 11 * (Field[I(i, j + 4, k)].x + Field[I(i, j, k + 1)].x)
		+ 6 * Field[I(i, j, k - 1)].x + 4 * Field[I(i, j, k - 2)].x - Field[I(i, j, k - 3)].x;

}
//(1<i<nx-1,0,N)
  double CurlTwo::dx2Vx_dy2Vx0_dz2VxN()
{
	return 40 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- 104 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i, j + 3, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i, j + 4, k)].x + Field[I(i, j, k - 4)].x);
}
//(1<i<nx-1,0,1<k<nz-1)
  double CurlTwo::dx2Vx_dy2Vx0_dz2Vx()
{
	return -25 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x + Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- 104 * Field[I(i, j + 1, k)].x
		+ 114 * Field[I(i, j + 2, k)].x
		- 56 * Field[I(i, j + 3, k)].x
		+ 11 * Field[I(i, j + 4, k)].x
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(1<i<nx-1,1,0)
  double CurlTwo::dx2Vx_dy2Vx1_dz2Vx0()
{
	return -15 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		+ 6 * Field[I(i, j + 1, k)].x + 4 * Field[I(i, j + 2, k)].x
		- Field[I(i, j + 3, k)].x
		+ 11 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k + 4)].x)
		- 104 * Field[I(i, j, k + 1)].x + 114 * Field[I(i, j, k + 2)].x - 56 * Field[I(i, j, k + 3)].x;
}
//(1<i<nx-1,1,1)
  double CurlTwo::dx2Vx_dy2Vx1_dz2Vx1()
{
	return -70 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		+ 6 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i, j + 3, k)].x
		+ 11 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 3)].x;
}
//(1<i<nx-1,1,N1)
  double CurlTwo::dx2Vx_dy2Vx1_dz2VxN1()
{
	return -70 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		+ 6 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i, j + 2, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i, j + 3, k)].x
		+ 11 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		- Field[I(i, j, k - 3)].x;
}
//(1<i<nx-1,1,N)
  double CurlTwo::dx2Vx_dy2Vx1_dz2VxN()
{
	return -15 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		+ 6 * Field[I(i, j + 1, k)].x + 4 * Field[I(i, j + 2, k)].x
		- Field[I(i, j + 3, k)].x
		+ 11 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k - 4)].x)
		- 104 * Field[I(i, j, k - 1)].x + 114 * Field[I(i, j, k - 2)].x
		- 56 * Field[I(i, j, k - 3)].x;
}
//(1<i<nx-1,1,1<k<nz-1)
  double CurlTwo::dx2Vx_dy2Vx1_dz2Vx()
{
	return -80 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x + Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		+ 6 * Field[I(i, j + 1, k)].x + 4 * Field[I(i, j + 2, k)].x
		- Field[I(i, j + 3, k)].x + 11 * Field[I(i, j - 1, k)].x
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(1<i<nx-1,ny-1,0)
  double CurlTwo::dx2Vx_dy2VxN1_dz2Vx0()
{
	return -15 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		+ 6 * Field[I(i, j - 1, k)].x + 4 * Field[I(i, j - 2, k)].x
		- Field[I(i, j - 3, k)].x + 11 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k + 4)].x)
		- 104 * Field[I(i, j, k + 1)].x + 114 * Field[I(i, j, k + 2)].x - 56 * Field[I(i, j, k + 3)].x;

}
//(1<i<nx-1,N1,1)
  double CurlTwo::dx2Vx_dy2VxN1_dz2Vx1()
{
	return -70 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		+ 6 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 4 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k + 2)].x)
		- Field[I(i, j - 3, k)].x
		+ 11 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i, j, k + 3)].x;
}
//(1<i<nx-1,N1,N1)
  double CurlTwo::dx2Vx_dy2VxN1_dz2VxN1()
{
	return -70 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		+ 6 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 4 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k - 2)].x)
		- Field[I(i, j - 3, k)].x
		+ 11 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k + 1)].x)
		- Field[I(i, j, k - 3)].x;

}
//(1<i<nx-1,N1,N)
  double CurlTwo::dx2Vx_dy2VxN1_dz2VxN()
{
	return -15 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		+ 6 * Field[I(i, j - 1, k)].x + 4 * Field[I(i, j - 2, k)].x
		- Field[I(i, j - 3, k)].x + 11 * (Field[I(i, j + 1, k)].x + Field[I(i, j, k - 4)].x)
		- 104 * Field[I(i, j, k - 1)].x + 114 * Field[I(i, j, k - 2)].x - 56 * Field[I(i, j, k - 3)].x;
}
//(1<i<nx-1,N1,1<k<nz-1)
  double CurlTwo::dx2Vx_dy2VxN1_dz2Vx()
{
	return -80 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x + Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		+ 6 * Field[I(i, j - 1, k)].x + 4 * Field[I(i, j - 2, k)].x
		- Field[I(i, j - 3, k)].x + 11 * Field[I(i, j + 1, k)].x
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(1<i<nx-1,N,0)
  double CurlTwo::dx2Vx_dy2VxN_dz2Vx0()
{
	return 40 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- 104 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k + 1)].x)
		+ 114 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k + 2)].x)
		- 56 * (Field[I(i, j - 3, k)].x + Field[I(i, j, k + 3)].x)
		+ 11 * (Field[I(i, j - 4, k)].x + Field[I(i, j, k + 4)].x);
}
//(1<i<nx-1,N,1)
  double CurlTwo::dx2Vx_dy2VxN_dz2Vx1()
{
	return -15 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- 104 * Field[I(i, j - 1, k)].x
		+ 114 * Field[I(i, j - 2, k)].x
		- 56 * Field[I(i, j - 3, k)].x
		+ 11 * (Field[I(i, j - 4, k)].x + Field[I(i, j, k - 1)].x)
		+ 6 * Field[I(i, j, k + 1)].x + 4 * Field[I(i, j, k + 2)].x - Field[I(i, j, k + 3)].x;

}
//(1<i<nx-1,N,N1)
  double CurlTwo::dx2Vx_dy2VxN_dz2VxN1()
{
	return -15 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- 104 * Field[I(i, j - 1, k)].x
		+ 114 * Field[I(i, j - 2, k)].x
		- 56 * Field[I(i, j - 3, k)].x
		+ 11 * (Field[I(i, j - 4, k)].x + Field[I(i, j, k + 1)].x)
		+ 6 * Field[I(i, j, k - 1)].x + 4 * Field[I(i, j, k - 2)].x - Field[I(i, j, k - 3)].x;
}
//(1<i<nx-1,N,N)
  double CurlTwo::dx2Vx_dy2VxN_dz2VxN()
{
	return 40 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- 104 * (Field[I(i, j - 1, k)].x + Field[I(i, j, k - 1)].x)
		+ 114 * (Field[I(i, j - 2, k)].x + Field[I(i, j, k - 2)].x)
		- 56 * (Field[I(i, j - 3, k)].x + Field[I(i, j, k - 3)].x)
		+ 11 * (Field[I(i, j - 4, k)].x + Field[I(i, j, k - 4)].x);
}
//(1<i<nx-1,N,1<k<nz-1)
  double CurlTwo::dx2Vx_dy2VxN_dz2Vx()
{
	return -25 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x + Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- 104 * Field[I(i, j - 1, k)].x
		+ 114 * Field[I(i, j - 2, k)].x
		- 56 * Field[I(i, j - 3, k)].x
		+ 11 * Field[I(i, j - 4, k)].x
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//
//(1<i<nx-1,1<j<ny-1,0)
  double CurlTwo::dx2Vx_dy2Vx_dz2Vx0()
{
	return -25 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- 104 * Field[I(i, j, k + 1)].x + 114 * Field[I(i, j, k + 2)].x
		- 56 * Field[I(i, j, k + 3)].x + 11 * Field[I(i, j, k + 4)].x;
}
//(1<i<nx-1,1<j<ny-1,1)
  double CurlTwo::dx2Vx_dy2Vx_dz2Vx1()
{
	return -80 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		+ 6 * Field[I(i, j, k + 1)].x + 4 * Field[I(i, j, k + 2)].x
		- Field[I(i, j, k + 3)].x + 11 * Field[I(i, j, k - 1)].x;
}
//(1<i<nx-1,1<j<ny-1,N1)
  double CurlTwo::dx2Vx_dy2Vx_dz2VxN1()
{
	return -80 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		+ 6 * Field[I(i, j, k - 1)].x + 4 * Field[I(i, j, k - 2)].x
		- Field[I(i, j, k - 3)].x + 11 * Field[I(i, j, k + 1)].x;
}
//(1<i<nx-1,1<j<ny-1,N)
  double CurlTwo::dx2Vx_dy2Vx_dz2VxN()
{
	return -25 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x + Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- 104 * Field[I(i, j, k - 1)].x + 114 * Field[I(i, j, k - 2)].x
		- 56 * Field[I(i, j, k - 3)].x + 11 * Field[I(i, j, k - 4)].x;
}
//(1<i<nx-1,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2Vx_dy2Vx_dz2Vx()
{
	return -90 * Field[I(i, j, k)].x
		+ 16 * (Field[I(i + 1, j, k)].x + Field[I(i - 1, j, k)].x
		+ Field[I(i, j + 1, k)].x + Field[I(i, j - 1, k)].x
		+ Field[I(i, j, k + 1)].x + Field[I(i, j, k - 1)].x)
		- Field[I(i + 2, j, k)].x - Field[I(i - 2, j, k)].x
		- Field[I(i, j + 2, k)].x - Field[I(i, j - 2, k)].x
		- Field[I(i, j, k + 2)].x - Field[I(i, j, k - 2)].x;
}
//==end of 1<i<nx-1========================

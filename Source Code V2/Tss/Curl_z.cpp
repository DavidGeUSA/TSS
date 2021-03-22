
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

calculate the second order curl - z-axis
********************************************************************/

#include "CurlTwo.h"
//(0,0,0)
  double CurlTwo::dx2Vz0_dy2Vz0_dz2Vz0()
{
	return 105 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j + 2, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j + 3, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k + 4)].z);
}
//(0,0,1)
  double CurlTwo::dx2Vz0_dy2Vz0_dz2Vz1()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j + 2, k)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j + 3, k)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k - 1)].z)
		+ 6 * Field[I(i, j, k + 1)].z + 4 * Field[I(i, j, k + 2)].z - Field[I(i, j, k + 3)].z;
}
//(0,0,N1)
  double CurlTwo::dx2Vz0_dy2Vz0_dz2VzN1()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j + 2, k)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j + 3, k)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k + 1)].z)
		+ 6 * Field[I(i, j, k - 1)].z + 4 * Field[I(i, j, k - 2)].z - Field[I(i, j, k - 3)].z;
}
//(0,0,N)
  double CurlTwo::dx2Vz0_dy2Vz0_dz2VzN()
{
	return 105 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j + 2, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j + 3, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k - 4)].z);
}
//(0,0,1<k<nz-1)
  double CurlTwo::dx2Vz0_dy2Vz0_dz2Vz()
{
	return 40 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j + 2, k)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j + 3, k)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j + 4, k)].z)
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z) - Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//(0,1,0)
  double CurlTwo::dx2Vz0_dy2Vz1_dz2Vz0()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j, k + 4)].z + Field[I(i, j - 1, k)].z)
		+ 6 * Field[I(i, j + 1, k)].z + 4 * Field[I(i, j + 2, k)].z - Field[I(i, j + 3, k)].z;
}
//(0,1,1)
  double CurlTwo::dx2Vz0_dy2Vz1_dz2Vz1()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i, j + 3, k)].z - Field[I(i, j, k + 3)].z
		+ 11 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z + Field[I(i + 4, j, k)].z)
		- 104 * Field[I(i + 1, j, k)].z + 114 * Field[I(i + 2, j, k)].z - 56 * Field[I(i + 3, j, k)].z;

}
//(0,1,N1)
  double CurlTwo::dx2Vz0_dy2Vz1_dz2VzN1()
{
	return -5 * Field[I(i, j, k)].z
		- 104 * Field[I(i + 1, j, k)].z + 114 * Field[I(i + 2, j, k)].z
		- 56 * Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 6 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i, j + 3, k)].z
		- Field[I(i, j, k - 3)].z;
}
//(0,1,N)
  double CurlTwo::dx2Vz0_dy2Vz1_dz2VzN()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k - 4)].z)
		+ 6 * Field[I(i, j + 1, k)].z + 4 * Field[I(i, j + 2, k)].z - Field[I(i, j + 3, k)].z;
}
//(0,1,1<k<nz-1)
  double CurlTwo::dx2Vz0_dy2Vz1_dz2Vz()
{
	return -15 * Field[I(i, j, k)].z
		- 104 * Field[I(i + 1, j, k)].z
		+ 114 * Field[I(i + 2, j, k)].z
		- 56 * Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 6 * Field[I(i, j + 1, k)].z + 4 * Field[I(i, j + 2, k)].z - Field[I(i, j + 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z) - Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//(0,N1,0)
  double CurlTwo::dx2Vz0_dy2VzN1_dz2Vz0()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 4)].z)
		+ 6 * Field[I(i, j - 1, k)].z + 4 * Field[I(i, j - 2, k)].z - Field[I(i, j - 3, k)].z;
}
//(0,N1,1)
  double CurlTwo::dx2Vz0_dy2VzN1_dz2Vz1()
{
	return -5 * Field[I(i, j, k)].z - 104 * Field[I(i + 1, j, k)].z + 114 * Field[I(i + 2, j, k)].z - 56 * Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 6 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k + 2)].z) - Field[I(i, j - 3, k)].z
		- Field[I(i, j, k + 3)].z;
}

//(0,N1,N1)
  double CurlTwo::dx2Vz0_dy2VzN1_dz2VzN1()
{
	return -5 * Field[I(i, j, k)].z - 104 * Field[I(i + 1, j, k)].z + 114 * Field[I(i + 2, j, k)].z
		- 56 * Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 6 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i, j - 3, k)].z
		- Field[I(i, j, k - 3)].z;

}
//(0,N1,N)
  double CurlTwo::dx2Vz0_dy2VzN1_dz2VzN()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 4)].z)
		+ 6 * Field[I(i, j - 1, k)].z + 4 * Field[I(i, j - 2, k)].z - Field[I(i, j - 3, k)].z;
}
//(0,N1,1<k<nz-1)
  double CurlTwo::dx2Vz0_dy2VzN1_dz2Vz()
{
	return -15 * Field[I(i, j, k)].z - 104 * Field[I(i + 1, j, k)].z
		+ 114 * Field[I(i + 2, j, k)].z - 56 * Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 6 * Field[I(i, j - 1, k)].z + 4 * Field[I(i, j - 2, k)].z - Field[I(i, j - 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z) - Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}

//(0,N,0)
  double CurlTwo::dx2Vz0_dy2VzN_dz2Vz0()
{
	return 105 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j - 2, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j - 3, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k + 4)].z);
}
//(0,N,1)
  double CurlTwo::dx2Vz0_dy2VzN_dz2Vz1()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j - 2, k)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j - 3, k)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k - 1)].z)
		+ 6 * Field[I(i, j, k + 1)].z + 4 * Field[I(i, j, k + 2)].z - Field[I(i, j, k + 3)].z;

}
//(0,N,N1)
  double CurlTwo::dx2Vz0_dy2VzN_dz2VzN1()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j - 2, k)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j - 3, k)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k + 1)].z)
		+ 6 * Field[I(i, j, k - 1)].z + 4 * Field[I(i, j, k - 2)].z - Field[I(i, j, k - 3)].z;

}
//(0,N,N)
  double CurlTwo::dx2Vz0_dy2VzN_dz2VzN()
{
	return 105 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j - 2, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j - 3, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k - 4)].z);
}
//(0,N,1<k<nz-1)
  double CurlTwo::dx2Vz0_dy2VzN_dz2Vz()
{
	return 40 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j - 2, k)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j - 3, k)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j - 4, k)].z)
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z) - Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}

//(0,1<j<ny-1,0)
  double CurlTwo::dx2Vz0_dy2Vz_dz2Vz0()
{
	return 40 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j, k + 4)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z;
}
//(0,1<j<ny-1,1)
  double CurlTwo::dx2Vz0_dy2Vz_dz2Vz1()
{
	return -15 * Field[I(i, j, k)].z
		- 104 * Field[I(i + 1, j, k)].z
		+ 114 * Field[I(i + 2, j, k)].z
		- 56 * Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		+ 6 * Field[I(i, j, k + 1)].z + 4 * Field[I(i, j, k + 2)].z - Field[I(i, j, k + 3)].z;
}
//(0,1<j<ny-1,N1)
  double CurlTwo::dx2Vz0_dy2Vz_dz2VzN1()
{
	return -15 * Field[I(i, j, k)].z
		- 104 * Field[I(i + 1, j, k)].z
		+ 114 * Field[I(i + 2, j, k)].z
		- 56 * Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		+ 6 * Field[I(i, j, k - 1)].z + 4 * Field[I(i, j, k - 2)].z - Field[I(i, j, k - 3)].z;

}
//(0,1<j<ny-1,N)
  double CurlTwo::dx2Vz0_dy2Vz_dz2VzN()
{
	return 40 * Field[I(i, j, k)].z
		- 104 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i + 3, j, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i + 4, j, k)].z + Field[I(i, j, k - 4)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z;
}
//(0,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2Vz0_dy2Vz_dz2Vz()
{
	return -25 * Field[I(i, j, k)].z
		- 104 * Field[I(i + 1, j, k)].z
		+ 114 * Field[I(i + 2, j, k)].z
		- 56 * Field[I(i + 3, j, k)].z
		+ 11 * Field[I(i + 4, j, k)].z
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;

}

//(1,0,0)
  double CurlTwo::dx2Vz1_dy2Vz0_dz2Vz0()
{
	return 50 * Field[I(i, j, k)].z
		+ 6 * Field[I(i + 1, j, k)].z
		+ 4 * Field[I(i + 2, j, k)].z
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k + 4)].z)
		- 104 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i, j + 3, k)].z + Field[I(i, j, k + 3)].z);
}
//(1,0,1)
  double CurlTwo::dx2Vz1_dy2Vz0_dz2Vz1()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k - 1)].z)
		- 104 * Field[I(i, j + 1, k)].z
		+ 114 * Field[I(i, j + 2, k)].z
		- 56 * Field[I(i, j + 3, k)].z
		- Field[I(i, j, k + 3)].z;

}
//(1,0,N1)
  double CurlTwo::dx2Vz1_dy2Vz0_dz2VzN1()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k + 1)].z)
		- 104 * Field[I(i, j + 1, k)].z
		+ 114 * Field[I(i, j + 2, k)].z
		- 56 * Field[I(i, j + 3, k)].z
		- Field[I(i, j, k - 3)].z;
}
//(1,0,N)
  double CurlTwo::dx2Vz1_dy2Vz0_dz2VzN()
{
	return 50 * Field[I(i, j, k)].z
		+ 6 * Field[I(i + 1, j, k)].z
		+ 4 * Field[I(i + 2, j, k)].z
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k - 4)].z)
		- 104 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i, j + 3, k)].z + Field[I(i, j, k - 3)].z);
}
//(1,0,1<k<nz-1)
  double CurlTwo::dx2Vz1_dy2Vz0_dz2Vz()
{
	return -15 * Field[I(i, j, k)].z
		+ 6 * Field[I(i + 1, j, k)].z
		+ 4 * Field[I(i + 2, j, k)].z
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 4, k)].z)
		- 104 * Field[I(i, j + 1, k)].z
		+ 114 * Field[I(i, j + 2, k)].z
		- 56 * Field[I(i, j + 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}

//
//(1,1,0)
  double CurlTwo::dx2Vz1_dy2Vz1_dz2Vz0()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j + 2, k)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 4)].z)
		- Field[I(i, j + 3, k)].z
		- 104 * Field[I(i, j, k + 1)].z + 114 * Field[I(i, j, k + 2)].z - 56 * Field[I(i, j, k + 3)].z;

}
//(1,1,1)
  double CurlTwo::dx2Vz1_dy2Vz1_dz2Vz1()
{
	return -60 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j + 2, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j + 3, k)].z
		- Field[I(i, j, k + 3)].z;

}
//(1,1,N1)
  double CurlTwo::dx2Vz1_dy2Vz1_dz2VzN1()
{
	return -60 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j + 2, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		- Field[I(i, j + 3, k)].z
		- Field[I(i, j, k - 3)].z;

}
//(1,1,N)
  double CurlTwo::dx2Vz1_dy2Vz1_dz2VzN()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j + 2, k)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k - 4)].z)
		- Field[I(i, j + 3, k)].z
		- 104 * Field[I(i, j, k - 1)].z + 114 * Field[I(i, j, k - 2)].z
		- 56 * Field[I(i, j, k - 3)].z;

}
//(1,1,1<k<nz-1)
  double CurlTwo::dx2Vz1_dy2Vz1_dz2Vz()
{
	return -70 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j + 2, k)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(1,N1,0)
  double CurlTwo::dx2Vz1_dy2VzN1_dz2Vz0()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j - 2, k)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 4)].z)
		- Field[I(i, j - 3, k)].z
		- 104 * Field[I(i, j, k + 1)].z + 114 * Field[I(i, j, k + 2)].z
		- 56 * Field[I(i, j, k + 3)].z;

}
//(1,N1,1)
  double CurlTwo::dx2Vz1_dy2VzN1_dz2Vz1()
{
	return -60 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j - 2, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j - 3, k)].z
		- Field[I(i, j, k + 3)].z;
}
//(1,N1,N1)
  double CurlTwo::dx2Vz1_dy2VzN1_dz2VzN1()
{
	return -60 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j - 2, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		- Field[I(i, j - 3, k)].z
		- Field[I(i, j, k - 3)].z;

}
//(1,N1,N)
  double CurlTwo::dx2Vz1_dy2VzN1_dz2VzN()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j - 2, k)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 4)].z)
		- Field[I(i, j - 3, k)].z
		- 104 * Field[I(i, j, k - 1)].z + 114 * Field[I(i, j, k - 2)].z - 56 * Field[I(i, j, k - 3)].z;

}
//(1,N1,1<k<nz-1)
  double CurlTwo::dx2Vz1_dy2VzN1_dz2Vz()
{
	return -70 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j - 2, k)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z)
		- Field[I(i, j - 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;

}
//
//(1,N,0)
  double CurlTwo::dx2Vz1_dy2VzN_dz2Vz0()
{
	return 50 * Field[I(i, j, k)].z + 6 * Field[I(i + 1, j, k)].z + 4 * Field[I(i + 2, j, k)].z
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k + 4)].z)
		- 104 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i, j - 3, k)].z + Field[I(i, j, k + 3)].z);
}
//(1,N,1)
  double CurlTwo::dx2Vz1_dy2VzN_dz2Vz1()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k - 1)].z)
		- 104 * Field[I(i, j - 1, k)].z + 114 * Field[I(i, j - 2, k)].z
		- 56 * Field[I(i, j - 3, k)].z
		- Field[I(i, j, k + 3)].z;

}
//(1,N,N1)
  double CurlTwo::dx2Vz1_dy2VzN_dz2VzN1()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k + 1)].z)
		- 104 * Field[I(i, j - 1, k)].z + 114 * Field[I(i, j - 2, k)].z
		- 56 * Field[I(i, j - 3, k)].z
		- Field[I(i, j, k - 3)].z;

}
//(1,N,N)
  double CurlTwo::dx2Vz1_dy2VzN_dz2VzN()
{
	return 50 * Field[I(i, j, k)].z + 6 * Field[I(i + 1, j, k)].z + 4 * Field[I(i + 2, j, k)].z
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k - 4)].z)
		- 104 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i, j - 3, k)].z + Field[I(i, j, k - 3)].z);
}
//(1,N,1<k<nz-1)
  double CurlTwo::dx2Vz1_dy2VzN_dz2Vz()
{
	return -15 * Field[I(i, j, k)].z + 6 * Field[I(i + 1, j, k)].z + 4 * Field[I(i + 2, j, k)].z
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 4, k)].z)
		- 104 * Field[I(i, j - 1, k)].z + 114 * Field[I(i, j - 2, k)].z
		- 56 * Field[I(i, j - 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(1,1<j<ny-1,0)
  double CurlTwo::dx2Vz1_dy2Vz_dz2Vz0()
{
	return -15 * Field[I(i, j, k)].z
		+ 6 * Field[I(i + 1, j, k)].z + 4 * Field[I(i + 2, j, k)].z
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k + 4)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- 104 * Field[I(i, j, k + 1)].z + 114 * Field[I(i, j, k + 2)].z - 56 * Field[I(i, j, k + 3)].z;

}
//(1,1<j<ny-1,1)
  double CurlTwo::dx2Vz1_dy2Vz_dz2Vz1()
{
	return -70 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- Field[I(i, j, k + 3)].z;

}
//(1,1<j<ny-1,N1)
  double CurlTwo::dx2Vz1_dy2Vz_dz2VzN1()
{
	return -70 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i + 2, j, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i + 3, j, k)].z
		+ 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- Field[I(i, j, k - 3)].z;
}
//(1,1<j<ny-1,N)
  double CurlTwo::dx2Vz1_dy2Vz_dz2VzN()
{
	return -15 * Field[I(i, j, k)].z
		+ 6 * Field[I(i + 1, j, k)].z + 4 * Field[I(i + 2, j, k)].z
		- Field[I(i + 3, j, k)].z + 11 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k - 4)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- 104 * Field[I(i, j, k - 1)].z + 114 * Field[I(i, j, k - 2)].z - 56 * Field[I(i, j, k - 3)].z;
}
//(1,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2Vz1_dy2Vz_dz2Vz()
{
	return -80 * Field[I(i, j, k)].z
		+ 6 * Field[I(i + 1, j, k)].z + 4 * Field[I(i + 2, j, k)].z
		- Field[I(i + 3, j, k)].z + 11 * Field[I(i - 1, j, k)].z
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//-------------------------------------------
//---start of i=nx-1-----------------------------------
//(nx-1,0,0)
  double CurlTwo::dx2VzN1_dy2Vz0_dz2Vz0()
{
	return 50 * Field[I(i, j, k)].z + 6 * Field[I(i - 1, j, k)].z + 4 * Field[I(i - 2, j, k)].z
		- Field[I(i - 3, j, k)].z + 11 * Field[I(i + 1, j, k)].z
		- 104 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z) + 114 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i, j + 3, k)].z + Field[I(i, j, k + 3)].z) + 11 * (Field[I(i, j + 4, k)].z + Field[I(i, j, k + 4)].z);
}
//(nx-1,0,1)
  double CurlTwo::dx2VzN1_dy2Vz0_dz2Vz1()
{
	return -5 * Field[I(i, j, k)].z + 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i - 3, j, k)].z + 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k - 1)].z)
		- 104 * Field[I(i, j + 1, k)].z + 114 * Field[I(i, j + 2, k)].z
		- 56 * Field[I(i, j + 3, k)].z + 11 * Field[I(i, j + 4, k)].z
		- Field[I(i, j, k + 3)].z;

}
//(nx-1,0,N1)
  double CurlTwo::dx2VzN1_dy2Vz0_dz2VzN1()
{
	return -5 * Field[I(i, j, k)].z + 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k + 1)].z)
		- 104 * Field[I(i, j + 1, k)].z + 114 * Field[I(i, j + 2, k)].z
		- 56 * Field[I(i, j + 3, k)].z - Field[I(i, j, k - 3)].z;
}
//(nx-1,0,N)
  double CurlTwo::dx2VzN1_dy2Vz0_dz2VzN()
{
	return 50 * Field[I(i, j, k)].z + 6 * Field[I(i - 1, j, k)].z + 4 * Field[I(i - 2, j, k)].z
		- Field[I(i - 3, j, k)].z + 11 * Field[I(i + 1, j, k)].z
		- 104 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i, j + 3, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i, j + 4, k)].z + Field[I(i, j, k - 4)].z);
}
//(nx-1,0,1<k<nz-1)
  double CurlTwo::dx2VzN1_dy2Vz0_dz2Vz()
{
	return -15 * Field[I(i, j, k)].z + 6 * Field[I(i - 1, j, k)].z + 4 * Field[I(i - 2, j, k)].z
		- Field[I(i - 3, j, k)].z + 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 4, k)].z)
		- 104 * Field[I(i, j + 1, k)].z + 114 * Field[I(i, j + 2, k)].z
		- 56 * Field[I(i, j + 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(nx-1,1,0)
  double CurlTwo::dx2VzN1_dy2Vz1_dz2Vz0()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j + 2, k)].z) - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 4)].z)
		- Field[I(i, j + 3, k)].z
		- 104 * Field[I(i, j, k + 1)].z + 114 * Field[I(i, j, k + 2)].z - 56 * Field[I(i, j, k + 3)].z;
}
//(nx-1,1,1)
  double CurlTwo::dx2VzN1_dy2Vz1_dz2Vz1()
{
	return -60 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j + 2, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j + 3, k)].z - Field[I(i, j, k + 3)].z;
}
//(nx-1,1,N1)
  double CurlTwo::dx2VzN1_dy2Vz1_dz2VzN1()
{
	return -60 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j + 2, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		- Field[I(i, j + 3, k)].z - Field[I(i, j, k - 3)].z;
}
//(nx-1,1,N)
  double CurlTwo::dx2VzN1_dy2Vz1_dz2VzN()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j + 2, k)].z) - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k - 4)].z)
		- Field[I(i, j + 3, k)].z
		- 104 * Field[I(i, j, k - 1)].z + 114 * Field[I(i, j, k - 2)].z - 56 * Field[I(i, j, k - 3)].z;
}
//(nx-1,1,1<k<nz-1)
  double CurlTwo::dx2VzN1_dy2Vz1_dz2Vz()
{
	return -70 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j + 2, k)].z) - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z) - Field[I(i, j, k + 2)].z
		- Field[I(i, j, k - 2)].z;
}
//
//(nx-1,ny-1,0)
  double CurlTwo::dx2VzN1_dy2VzN1_dz2Vz0()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j - 2, k)].z) - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 4)].z)
		- Field[I(i, j - 3, k)].z
		- 104 * Field[I(i, j, k + 1)].z + 114 * Field[I(i, j, k + 2)].z - 56 * Field[I(i, j, k + 3)].z;
}
//(nx-1,N1,1)
  double CurlTwo::dx2VzN1_dy2VzN1_dz2Vz1()
{
	return -60 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j - 2, k)].z + Field[I(i, j, k + 2)].z) - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j - 3, k)].z - Field[I(i, j, k + 3)].z;
}
//(nx-1,N1,N1)
  double CurlTwo::dx2VzN1_dy2VzN1_dz2VzN1()
{
	return -60 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j - 2, k)].z + Field[I(i, j, k - 2)].z) - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		- Field[I(i, j - 3, k)].z - Field[I(i, j, k - 3)].z;
}
//(nx-1,N1,N)
  double CurlTwo::dx2VzN1_dy2VzN1_dz2VzN()
{
	return -5 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j - 2, k)].z) - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 4)].z)
		- Field[I(i, j - 3, k)].z
		- 104 * Field[I(i, j, k - 1)].z + 114 * Field[I(i, j, k - 2)].z - 56 * Field[I(i, j, k - 3)].z;

}
//(nx-1,N1,1<k<nz-1)
  double CurlTwo::dx2VzN1_dy2VzN1_dz2Vz()
{
	return -70 * Field[I(i, j, k)].z
		+ 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j - 2, k)].z) - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j + 1, k)].z)
		- Field[I(i, j - 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(nx-1,N,0)
  double CurlTwo::dx2VzN1_dy2VzN_dz2Vz0()
{
	return 50 * Field[I(i, j, k)].z + 6 * Field[I(i - 1, j, k)].z
		+ 4 * Field[I(i - 2, j, k)].z - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k + 4)].z)
		- 104 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i, j - 3, k)].z + Field[I(i, j, k + 3)].z);
}
//(nx-1,N,1)
  double CurlTwo::dx2VzN1_dy2VzN_dz2Vz1()
{
	return -5 * Field[I(i, j, k)].z + 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k + 2)].z) - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k - 1)].z)
		- 104 * Field[I(i, j - 1, k)].z + 114 * Field[I(i, j - 2, k)].z - 56 * Field[I(i, j - 3, k)].z
		- Field[I(i, j, k + 3)].z;
}
//(nx-1,N,N1)
  double CurlTwo::dx2VzN1_dy2VzN_dz2VzN1()
{
	return -5 * Field[I(i, j, k)].z + 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k - 2)].z) - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k + 1)].z)
		- 104 * Field[I(i, j - 1, k)].z + 114 * Field[I(i, j - 2, k)].z - 56 * Field[I(i, j - 3, k)].z
		- Field[I(i, j, k - 3)].z;
}
//(nx-1,N,N)
  double CurlTwo::dx2VzN1_dy2VzN_dz2VzN()
{
	return 50 * Field[I(i, j, k)].z + 6 * Field[I(i - 1, j, k)].z
		+ 4 * Field[I(i - 2, j, k)].z - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k - 4)].z)
		- 104 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i, j - 3, k)].z + Field[I(i, j, k - 3)].z);
}
//(nx-1,N,1<k<nz-1)
  double CurlTwo::dx2VzN1_dy2VzN_dz2Vz()
{
	return -15 * Field[I(i, j, k)].z + 6 * Field[I(i - 1, j, k)].z
		+ 4 * Field[I(i - 2, j, k)].z - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j - 4, k)].z)
		- 104 * Field[I(i, j - 1, k)].z + 114 * Field[I(i, j - 2, k)].z - 56 * Field[I(i, j - 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(nx-1,1<j<ny-1,0)
  double CurlTwo::dx2VzN1_dy2Vz_dz2Vz0()
{
	return -15 * Field[I(i, j, k)].z + 6 * Field[I(i - 1, j, k)].z
		+ 4 * Field[I(i - 2, j, k)].z - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k + 4)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- 104 * Field[I(i, j, k + 1)].z + 114 * Field[I(i, j, k + 2)].z - 56 * Field[I(i, j, k + 3)].z;
}
//(nx-1,1<j<ny-1,1)
  double CurlTwo::dx2VzN1_dy2Vz_dz2Vz1()
{
	return -70 * Field[I(i, j, k)].z + 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z - Field[I(i, j, k + 3)].z;
}
//(nx-1,1<j<ny-1,N1)
  double CurlTwo::dx2VzN1_dy2Vz_dz2VzN1()
{
	return -70 * Field[I(i, j, k)].z + 6 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i - 3, j, k)].z + 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z - Field[I(i, j, k - 3)].z;
}
//(nx-1,1<j<ny-1,N)
  double CurlTwo::dx2VzN1_dy2Vz_dz2VzN()
{
	return -15 * Field[I(i, j, k)].z + 6 * Field[I(i - 1, j, k)].z
		+ 4 * Field[I(i - 2, j, k)].z - Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i + 1, j, k)].z + Field[I(i, j, k - 4)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- 104 * Field[I(i, j, k - 1)].z + 114 * Field[I(i, j, k - 2)].z - 56 * Field[I(i, j, k - 3)].z;

}
//(nx-1,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2VzN1_dy2Vz_dz2Vz()
{
	return -80 * Field[I(i, j, k)].z + 6 * Field[I(i - 1, j, k)].z
		+ 4 * Field[I(i - 2, j, k)].z - Field[I(i - 3, j, k)].z + 11 * Field[I(i + 1, j, k)].z
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//-end of i=N1------------------------------------------
//
//-start of i=N-----------------------------------------
//(nx,0,0)
  double CurlTwo::dx2VzN_dy2Vz0_dz2Vz0()
{
	return 105 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j + 2, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j + 3, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k + 4)].z);
}
//(nx,0,1)
  double CurlTwo::dx2VzN_dy2Vz0_dz2Vz1()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j + 2, k)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j + 3, k)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k - 1)].z)
		+ 6 * Field[I(i, j, k + 1)].z + 4 * Field[I(i, j, k + 2)].z - Field[I(i, j, k + 3)].z;
}
//(nx,0,N1)
  double CurlTwo::dx2VzN_dy2Vz0_dz2VzN1()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j + 2, k)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j + 3, k)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k + 1)].z)
		+ 6 * Field[I(i, j, k - 1)].z + 4 * Field[I(i, j, k - 2)].z - Field[I(i, j, k - 3)].z;

}
//(nx,0,N)
  double CurlTwo::dx2VzN_dy2Vz0_dz2VzN()
{
	return 105 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j + 2, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j + 3, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j + 4, k)].z + Field[I(i, j, k - 4)].z);
}
//(nx,0,1<k<nz-1)
  double CurlTwo::dx2VzN_dy2Vz0_dz2Vz()
{
	return 40 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j + 2, k)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j + 3, k)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j + 4, k)].z)
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(nx,1,0)
  double CurlTwo::dx2VzN_dy2Vz1_dz2Vz0()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 4)].z)
		+ 6 * Field[I(i, j + 1, k)].z + 4 * Field[I(i, j + 2, k)].z - Field[I(i, j + 3, k)].z;
}
//(nx,1,1)
  double CurlTwo::dx2VzN_dy2Vz1_dz2Vz1()
{
	return -5 * Field[I(i, j, k)].z
		- 104 * Field[I(i - 1, j, k)].z
		+ 114 * Field[I(i - 2, j, k)].z
		- 56 * Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 6 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i, j + 3, k)].z - Field[I(i, j, k + 3)].z;
}
//(nx,1,N1)
  double CurlTwo::dx2VzN_dy2Vz1_dz2VzN1()
{
	return -5 * Field[I(i, j, k)].z
		- 104 * Field[I(i - 1, j, k)].z
		+ 114 * Field[I(i - 2, j, k)].z
		- 56 * Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 6 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i, j + 3, k)].z - Field[I(i, j, k - 3)].z;
}
//(nx,1,N)
  double CurlTwo::dx2VzN_dy2Vz1_dz2VzN()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k - 4)].z)
		+ 6 * Field[I(i, j + 1, k)].z + 4 * Field[I(i, j + 2, k)].z - Field[I(i, j + 3, k)].z;
}
//(nx,1,1<k<nz-1)
  double CurlTwo::dx2VzN_dy2Vz1_dz2Vz()
{
	return -15 * Field[I(i, j, k)].z
		- 104 * Field[I(i - 1, j, k)].z
		+ 114 * Field[I(i - 2, j, k)].z
		- 56 * Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 6 * Field[I(i, j + 1, k)].z + 4 * Field[I(i, j + 2, k)].z - Field[I(i, j + 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(nx,ny-1,0)
  double CurlTwo::dx2VzN_dy2VzN1_dz2Vz0()
{
	return 50 * Field[I(i, j, k)].z - 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 4)].z)
		+ 6 * Field[I(i, j - 1, k)].z + 4 * Field[I(i, j - 2, k)].z - Field[I(i, j - 3, k)].z;
}
//(nx,N1,1)
  double CurlTwo::dx2VzN_dy2VzN1_dz2Vz1()
{
	return -5 * Field[I(i, j, k)].z - 104 * Field[I(i - 1, j, k)].z
		+ 114 * Field[I(i - 2, j, k)].z - 56 * Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 6 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i, j - 3, k)].z - Field[I(i, j, k + 3)].z;
}
//(nx,N1,N1)
  double CurlTwo::dx2VzN_dy2VzN1_dz2VzN1()
{
	return -5 * Field[I(i, j, k)].z - 104 * Field[I(i - 1, j, k)].z
		+ 114 * Field[I(i - 2, j, k)].z - 56 * Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 6 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i, j - 3, k)].z - Field[I(i, j, k - 3)].z;
}
//(nx,N1,N)
  double CurlTwo::dx2VzN_dy2VzN1_dz2VzN()
{
	return 50 * Field[I(i, j, k)].z - 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j, k - 4)].z)
		+ 6 * Field[I(i, j - 1, k)].z + 4 * Field[I(i, j - 2, k)].z - Field[I(i, j - 3, k)].z;
}
//(nx,N1,1<k<nz-1)
  double CurlTwo::dx2VzN_dy2VzN1_dz2Vz()
{
	return -15 * Field[I(i, j, k)].z - 104 * Field[I(i - 1, j, k)].z
		+ 114 * Field[I(i - 2, j, k)].z - 56 * Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j + 1, k)].z)
		+ 6 * Field[I(i, j - 1, k)].z + 4 * Field[I(i, j - 2, k)].z - Field[I(i, j - 3, k)].z
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(nx,N,0)
  double CurlTwo::dx2VzN_dy2VzN_dz2Vz0()
{
	return 105 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j - 2, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j - 3, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k + 4)].z);
}
//(nx,N,1)
  double CurlTwo::dx2VzN_dy2VzN_dz2Vz1()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j - 2, k)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j - 3, k)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k - 1)].z)
		+ 6 * Field[I(i, j, k + 1)].z + 4 * Field[I(i, j, k + 2)].z - Field[I(i, j, k + 3)].z;

}
//(nx,N,N1)
  double CurlTwo::dx2VzN_dy2VzN_dz2VzN1()
{
	return 50 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j - 2, k)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j - 3, k)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k + 1)].z)
		+ 6 * Field[I(i, j, k - 1)].z + 4 * Field[I(i, j, k - 2)].z - Field[I(i, j, k - 3)].z;

}
//(nx,N,N)
  double CurlTwo::dx2VzN_dy2VzN_dz2VzN()
{
	return 105 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j - 2, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j - 3, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j - 4, k)].z + Field[I(i, j, k - 4)].z);
}
//(nx,N,1<k<nz-1)
  double CurlTwo::dx2VzN_dy2VzN_dz2Vz()
{
	return 40 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j - 1, k)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j - 2, k)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j - 3, k)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j - 4, k)].z)
		+ 16 * (Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;

}
//
//(nx,1<j<ny-1,0)
  double CurlTwo::dx2VzN_dy2Vz_dz2Vz0()
{
	return 40 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j, k + 4)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z;
}
//(nx,1<j<ny-1,1)
  double CurlTwo::dx2VzN_dy2Vz_dz2Vz1()
{
	return -15 * Field[I(i, j, k)].z
		- 104 * Field[I(i - 1, j, k)].z
		+ 114 * Field[I(i - 2, j, k)].z
		- 56 * Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		+ 6 * Field[I(i, j, k + 1)].z + 4 * Field[I(i, j, k + 2)].z
		- Field[I(i, j, k + 3)].z;
}
//(nx,1<j<ny-1,N1)
  double CurlTwo::dx2VzN_dy2Vz_dz2VzN1()
{
	return -15 * Field[I(i, j, k)].z
		- 104 * Field[I(i - 1, j, k)].z
		+ 114 * Field[I(i - 2, j, k)].z
		- 56 * Field[I(i - 3, j, k)].z
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j, k + 1)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		+ 6 * Field[I(i, j, k - 1)].z + 4 * Field[I(i, j, k - 2)].z - Field[I(i, j, k - 3)].z;
}
//(nx,1<j<ny-1,N)
  double CurlTwo::dx2VzN_dy2Vz_dz2VzN()
{
	return 40 * Field[I(i, j, k)].z
		- 104 * (Field[I(i - 1, j, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i - 2, j, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i - 3, j, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i - 4, j, k)].z + Field[I(i, j, k - 4)].z)
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z;
}
//(nx,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2VzN_dy2Vz_dz2Vz()
{
	return -25 * Field[I(i, j, k)].z
		- 104 * Field[I(i - 1, j, k)].z
		+ 114 * Field[I(i - 2, j, k)].z
		- 56 * Field[I(i - 3, j, k)].z
		+ 11 * Field[I(i - 4, j, k)].z
		+ 16 * (Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//-end of i=nx==============================
//==start of 1<i<nx-1========================
//(1<i<nx-1,0,0)
  double CurlTwo::dx2Vz_dy2Vz0_dz2Vz0()
{
	return 40 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- 104 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i, j + 3, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i, j + 4, k)].z + Field[I(i, j, k + 4)].z);
}
//(1<i<nx-1,0,1)
  double CurlTwo::dx2Vz_dy2Vz0_dz2Vz1()
{
	return -15 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- 104 * Field[I(i, j + 1, k)].z
		+ 114 * Field[I(i, j + 2, k)].z
		- 56 * Field[I(i, j + 3, k)].z
		+ 11 * (Field[I(i, j + 4, k)].z + Field[I(i, j, k - 1)].z)
		+ 6 * Field[I(i, j, k + 1)].z + 4 * Field[I(i, j, k + 2)].z - Field[I(i, j, k + 3)].z;
}
//(1<i<nx-1,0,N1)
  double CurlTwo::dx2Vz_dy2Vz0_dz2VzN1()
{
	return -15 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- 104 * Field[I(i, j + 1, k)].z
		+ 114 * Field[I(i, j + 2, k)].z
		- 56 * Field[I(i, j + 3, k)].z
		+ 11 * (Field[I(i, j + 4, k)].z + Field[I(i, j, k + 1)].z)
		+ 6 * Field[I(i, j, k - 1)].z + 4 * Field[I(i, j, k - 2)].z - Field[I(i, j, k - 3)].z;

}
//(1<i<nx-1,0,N)
  double CurlTwo::dx2Vz_dy2Vz0_dz2VzN()
{
	return 40 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- 104 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i, j + 3, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i, j + 4, k)].z + Field[I(i, j, k - 4)].z);
}
//(1<i<nx-1,0,1<k<nz-1)
  double CurlTwo::dx2Vz_dy2Vz0_dz2Vz()
{
	return -25 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z + Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- 104 * Field[I(i, j + 1, k)].z
		+ 114 * Field[I(i, j + 2, k)].z
		- 56 * Field[I(i, j + 3, k)].z
		+ 11 * Field[I(i, j + 4, k)].z
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(1<i<nx-1,1,0)
  double CurlTwo::dx2Vz_dy2Vz1_dz2Vz0()
{
	return -15 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		+ 6 * Field[I(i, j + 1, k)].z + 4 * Field[I(i, j + 2, k)].z
		- Field[I(i, j + 3, k)].z
		+ 11 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k + 4)].z)
		- 104 * Field[I(i, j, k + 1)].z + 114 * Field[I(i, j, k + 2)].z - 56 * Field[I(i, j, k + 3)].z;
}
//(1<i<nx-1,1,1)
  double CurlTwo::dx2Vz_dy2Vz1_dz2Vz1()
{
	return -70 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		+ 6 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i, j + 3, k)].z
		+ 11 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 3)].z;
}
//(1<i<nx-1,1,N1)
  double CurlTwo::dx2Vz_dy2Vz1_dz2VzN1()
{
	return -70 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		+ 6 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i, j + 2, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i, j + 3, k)].z
		+ 11 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		- Field[I(i, j, k - 3)].z;
}
//(1<i<nx-1,1,N)
  double CurlTwo::dx2Vz_dy2Vz1_dz2VzN()
{
	return -15 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		+ 6 * Field[I(i, j + 1, k)].z + 4 * Field[I(i, j + 2, k)].z
		- Field[I(i, j + 3, k)].z
		+ 11 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k - 4)].z)
		- 104 * Field[I(i, j, k - 1)].z + 114 * Field[I(i, j, k - 2)].z
		- 56 * Field[I(i, j, k - 3)].z;
}
//(1<i<nx-1,1,1<k<nz-1)
  double CurlTwo::dx2Vz_dy2Vz1_dz2Vz()
{
	return -80 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z + Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		+ 6 * Field[I(i, j + 1, k)].z + 4 * Field[I(i, j + 2, k)].z
		- Field[I(i, j + 3, k)].z + 11 * Field[I(i, j - 1, k)].z
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(1<i<nx-1,ny-1,0)
  double CurlTwo::dx2Vz_dy2VzN1_dz2Vz0()
{
	return -15 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		+ 6 * Field[I(i, j - 1, k)].z + 4 * Field[I(i, j - 2, k)].z
		- Field[I(i, j - 3, k)].z + 11 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k + 4)].z)
		- 104 * Field[I(i, j, k + 1)].z + 114 * Field[I(i, j, k + 2)].z - 56 * Field[I(i, j, k + 3)].z;

}
//(1<i<nx-1,N1,1)
  double CurlTwo::dx2Vz_dy2VzN1_dz2Vz1()
{
	return -70 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		+ 6 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 4 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k + 2)].z)
		- Field[I(i, j - 3, k)].z
		+ 11 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i, j, k + 3)].z;
}
//(1<i<nx-1,N1,N1)
  double CurlTwo::dx2Vz_dy2VzN1_dz2VzN1()
{
	return -70 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		+ 6 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 4 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k - 2)].z)
		- Field[I(i, j - 3, k)].z
		+ 11 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k + 1)].z)
		- Field[I(i, j, k - 3)].z;

}
//(1<i<nx-1,N1,N)
  double CurlTwo::dx2Vz_dy2VzN1_dz2VzN()
{
	return -15 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		+ 6 * Field[I(i, j - 1, k)].z + 4 * Field[I(i, j - 2, k)].z
		- Field[I(i, j - 3, k)].z + 11 * (Field[I(i, j + 1, k)].z + Field[I(i, j, k - 4)].z)
		- 104 * Field[I(i, j, k - 1)].z + 114 * Field[I(i, j, k - 2)].z - 56 * Field[I(i, j, k - 3)].z;
}
//(1<i<nx-1,N1,1<k<nz-1)
  double CurlTwo::dx2Vz_dy2VzN1_dz2Vz()
{
	return -80 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z + Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		+ 6 * Field[I(i, j - 1, k)].z + 4 * Field[I(i, j - 2, k)].z
		- Field[I(i, j - 3, k)].z + 11 * Field[I(i, j + 1, k)].z
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(1<i<nx-1,N,0)
  double CurlTwo::dx2Vz_dy2VzN_dz2Vz0()
{
	return 40 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- 104 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k + 1)].z)
		+ 114 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k + 2)].z)
		- 56 * (Field[I(i, j - 3, k)].z + Field[I(i, j, k + 3)].z)
		+ 11 * (Field[I(i, j - 4, k)].z + Field[I(i, j, k + 4)].z);
}
//(1<i<nx-1,N,1)
  double CurlTwo::dx2Vz_dy2VzN_dz2Vz1()
{
	return -15 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- 104 * Field[I(i, j - 1, k)].z
		+ 114 * Field[I(i, j - 2, k)].z
		- 56 * Field[I(i, j - 3, k)].z
		+ 11 * (Field[I(i, j - 4, k)].z + Field[I(i, j, k - 1)].z)
		+ 6 * Field[I(i, j, k + 1)].z + 4 * Field[I(i, j, k + 2)].z - Field[I(i, j, k + 3)].z;

}
//(1<i<nx-1,N,N1)
  double CurlTwo::dx2Vz_dy2VzN_dz2VzN1()
{
	return -15 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- 104 * Field[I(i, j - 1, k)].z
		+ 114 * Field[I(i, j - 2, k)].z
		- 56 * Field[I(i, j - 3, k)].z
		+ 11 * (Field[I(i, j - 4, k)].z + Field[I(i, j, k + 1)].z)
		+ 6 * Field[I(i, j, k - 1)].z + 4 * Field[I(i, j, k - 2)].z - Field[I(i, j, k - 3)].z;
}
//(1<i<nx-1,N,N)
  double CurlTwo::dx2Vz_dy2VzN_dz2VzN()
{
	return 40 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- 104 * (Field[I(i, j - 1, k)].z + Field[I(i, j, k - 1)].z)
		+ 114 * (Field[I(i, j - 2, k)].z + Field[I(i, j, k - 2)].z)
		- 56 * (Field[I(i, j - 3, k)].z + Field[I(i, j, k - 3)].z)
		+ 11 * (Field[I(i, j - 4, k)].z + Field[I(i, j, k - 4)].z);
}
//(1<i<nx-1,N,1<k<nz-1)
  double CurlTwo::dx2Vz_dy2VzN_dz2Vz()
{
	return -25 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z + Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- 104 * Field[I(i, j - 1, k)].z
		+ 114 * Field[I(i, j - 2, k)].z
		- 56 * Field[I(i, j - 3, k)].z
		+ 11 * Field[I(i, j - 4, k)].z
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//
//(1<i<nx-1,1<j<ny-1,0)
  double CurlTwo::dx2Vz_dy2Vz_dz2Vz0()
{
	return -25 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- 104 * Field[I(i, j, k + 1)].z + 114 * Field[I(i, j, k + 2)].z
		- 56 * Field[I(i, j, k + 3)].z + 11 * Field[I(i, j, k + 4)].z;
}
//(1<i<nx-1,1<j<ny-1,1)
  double CurlTwo::dx2Vz_dy2Vz_dz2Vz1()
{
	return -80 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		+ 6 * Field[I(i, j, k + 1)].z + 4 * Field[I(i, j, k + 2)].z
		- Field[I(i, j, k + 3)].z + 11 * Field[I(i, j, k - 1)].z;
}
//(1<i<nx-1,1<j<ny-1,N1)
  double CurlTwo::dx2Vz_dy2Vz_dz2VzN1()
{
	return -80 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		+ 6 * Field[I(i, j, k - 1)].z + 4 * Field[I(i, j, k - 2)].z
		- Field[I(i, j, k - 3)].z + 11 * Field[I(i, j, k + 1)].z;
}
//(1<i<nx-1,1<j<ny-1,N)
  double CurlTwo::dx2Vz_dy2Vz_dz2VzN()
{
	return -25 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z + Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- 104 * Field[I(i, j, k - 1)].z + 114 * Field[I(i, j, k - 2)].z
		- 56 * Field[I(i, j, k - 3)].z + 11 * Field[I(i, j, k - 4)].z;
}
//(1<i<nx-1,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2Vz_dy2Vz_dz2Vz()
{
	return -90 * Field[I(i, j, k)].z
		+ 16 * (Field[I(i + 1, j, k)].z + Field[I(i - 1, j, k)].z
		+ Field[I(i, j + 1, k)].z + Field[I(i, j - 1, k)].z
		+ Field[I(i, j, k + 1)].z + Field[I(i, j, k - 1)].z)
		- Field[I(i + 2, j, k)].z - Field[I(i - 2, j, k)].z
		- Field[I(i, j + 2, k)].z - Field[I(i, j - 2, k)].z
		- Field[I(i, j, k + 2)].z - Field[I(i, j, k - 2)].z;
}
//==end of 1<i<nx-1========================

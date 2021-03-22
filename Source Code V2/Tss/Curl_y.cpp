
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

calculate the second order curl - y-axis
********************************************************************/

#include "CurlTwo.h"

//(0,0,0)
  double CurlTwo::dx2Vy0_dy2Vy0_dz2Vy0()
{
	return 105 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j + 2, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j + 3, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k + 4)].y);
}
//(0,0,1)
  double CurlTwo::dx2Vy0_dy2Vy0_dz2Vy1()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j + 2, k)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j + 3, k)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k - 1)].y)
		+ 6 * Field[I(i, j, k + 1)].y + 4 * Field[I(i, j, k + 2)].y - Field[I(i, j, k + 3)].y;
}
//(0,0,N1)
  double CurlTwo::dx2Vy0_dy2Vy0_dz2VyN1()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j + 2, k)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j + 3, k)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k + 1)].y)
		+ 6 * Field[I(i, j, k - 1)].y + 4 * Field[I(i, j, k - 2)].y - Field[I(i, j, k - 3)].y;
}
//(0,0,N)
  double CurlTwo::dx2Vy0_dy2Vy0_dz2VyN()
{
	return 105 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j + 2, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j + 3, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k - 4)].y);
}
//(0,0,1<k<nz-1)
  double CurlTwo::dx2Vy0_dy2Vy0_dz2Vy()
{
	return 40 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j + 2, k)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j + 3, k)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j + 4, k)].y)
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y) - Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//(0,1,0)
  double CurlTwo::dx2Vy0_dy2Vy1_dz2Vy0()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j, k + 4)].y + Field[I(i, j - 1, k)].y)
		+ 6 * Field[I(i, j + 1, k)].y + 4 * Field[I(i, j + 2, k)].y - Field[I(i, j + 3, k)].y;
}
//(0,1,1)
  double CurlTwo::dx2Vy0_dy2Vy1_dz2Vy1()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i, j + 3, k)].y - Field[I(i, j, k + 3)].y
		+ 11 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y + Field[I(i + 4, j, k)].y)
		- 104 * Field[I(i + 1, j, k)].y + 114 * Field[I(i + 2, j, k)].y - 56 * Field[I(i + 3, j, k)].y;

}
//(0,1,N1)
  double CurlTwo::dx2Vy0_dy2Vy1_dz2VyN1()
{
	return -5 * Field[I(i, j, k)].y
		- 104 * Field[I(i + 1, j, k)].y + 114 * Field[I(i + 2, j, k)].y
		- 56 * Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 6 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i, j + 3, k)].y
		- Field[I(i, j, k - 3)].y;
}
//(0,1,N)
  double CurlTwo::dx2Vy0_dy2Vy1_dz2VyN()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k - 4)].y)
		+ 6 * Field[I(i, j + 1, k)].y + 4 * Field[I(i, j + 2, k)].y - Field[I(i, j + 3, k)].y;
}
//(0,1,1<k<nz-1)
  double CurlTwo::dx2Vy0_dy2Vy1_dz2Vy()
{
	return -15 * Field[I(i, j, k)].y
		- 104 * Field[I(i + 1, j, k)].y
		+ 114 * Field[I(i + 2, j, k)].y
		- 56 * Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 6 * Field[I(i, j + 1, k)].y + 4 * Field[I(i, j + 2, k)].y - Field[I(i, j + 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y) - Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//(0,N1,0)
  double CurlTwo::dx2Vy0_dy2VyN1_dz2Vy0()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 4)].y)
		+ 6 * Field[I(i, j - 1, k)].y + 4 * Field[I(i, j - 2, k)].y - Field[I(i, j - 3, k)].y;
}
//(0,N1,1)
  double CurlTwo::dx2Vy0_dy2VyN1_dz2Vy1()
{
	return -5 * Field[I(i, j, k)].y - 104 * Field[I(i + 1, j, k)].y + 114 * Field[I(i + 2, j, k)].y - 56 * Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 6 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k + 2)].y) - Field[I(i, j - 3, k)].y
		- Field[I(i, j, k + 3)].y;
}

//(0,N1,N1)
  double CurlTwo::dx2Vy0_dy2VyN1_dz2VyN1()
{
	return -5 * Field[I(i, j, k)].y - 104 * Field[I(i + 1, j, k)].y + 114 * Field[I(i + 2, j, k)].y
		- 56 * Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 6 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i, j - 3, k)].y
		- Field[I(i, j, k - 3)].y;

}
//(0,N1,N)
  double CurlTwo::dx2Vy0_dy2VyN1_dz2VyN()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 4)].y)
		+ 6 * Field[I(i, j - 1, k)].y + 4 * Field[I(i, j - 2, k)].y - Field[I(i, j - 3, k)].y;
}
//(0,N1,1<k<nz-1)
  double CurlTwo::dx2Vy0_dy2VyN1_dz2Vy()
{
	return -15 * Field[I(i, j, k)].y - 104 * Field[I(i + 1, j, k)].y
		+ 114 * Field[I(i + 2, j, k)].y - 56 * Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 6 * Field[I(i, j - 1, k)].y + 4 * Field[I(i, j - 2, k)].y - Field[I(i, j - 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y) - Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}

//(0,N,0)
  double CurlTwo::dx2Vy0_dy2VyN_dz2Vy0()
{
	return 105 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j - 3, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k + 4)].y);
}
//(0,N,1)
  double CurlTwo::dx2Vy0_dy2VyN_dz2Vy1()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j - 3, k)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k - 1)].y)
		+ 6 * Field[I(i, j, k + 1)].y + 4 * Field[I(i, j, k + 2)].y - Field[I(i, j, k + 3)].y;

}
//(0,N,N1)
  double CurlTwo::dx2Vy0_dy2VyN_dz2VyN1()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j - 3, k)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k + 1)].y)
		+ 6 * Field[I(i, j, k - 1)].y + 4 * Field[I(i, j, k - 2)].y - Field[I(i, j, k - 3)].y;

}
//(0,N,N)
  double CurlTwo::dx2Vy0_dy2VyN_dz2VyN()
{
	return 105 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j - 3, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k - 4)].y);
}
//(0,N,1<k<nz-1)
  double CurlTwo::dx2Vy0_dy2VyN_dz2Vy()
{
	return 40 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j - 3, k)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j - 4, k)].y)
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y) - Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}

//(0,1<j<ny-1,0)
  double CurlTwo::dx2Vy0_dy2Vy_dz2Vy0()
{
	return 40 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j, k + 4)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y;
}
//(0,1<j<ny-1,1)
  double CurlTwo::dx2Vy0_dy2Vy_dz2Vy1()
{
	return -15 * Field[I(i, j, k)].y
		- 104 * Field[I(i + 1, j, k)].y
		+ 114 * Field[I(i + 2, j, k)].y
		- 56 * Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		+ 6 * Field[I(i, j, k + 1)].y + 4 * Field[I(i, j, k + 2)].y - Field[I(i, j, k + 3)].y;
}
//(0,1<j<ny-1,N1)
  double CurlTwo::dx2Vy0_dy2Vy_dz2VyN1()
{
	return -15 * Field[I(i, j, k)].y
		- 104 * Field[I(i + 1, j, k)].y
		+ 114 * Field[I(i + 2, j, k)].y
		- 56 * Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		+ 6 * Field[I(i, j, k - 1)].y + 4 * Field[I(i, j, k - 2)].y - Field[I(i, j, k - 3)].y;

}
//(0,1<j<ny-1,N)
  double CurlTwo::dx2Vy0_dy2Vy_dz2VyN()
{
	return 40 * Field[I(i, j, k)].y
		- 104 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i + 3, j, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i + 4, j, k)].y + Field[I(i, j, k - 4)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y;
}
//(0,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2Vy0_dy2Vy_dz2Vy()
{
	return -25 * Field[I(i, j, k)].y
		- 104 * Field[I(i + 1, j, k)].y
		+ 114 * Field[I(i + 2, j, k)].y
		- 56 * Field[I(i + 3, j, k)].y
		+ 11 * Field[I(i + 4, j, k)].y
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;

}

//(1,0,0)
  double CurlTwo::dx2Vy1_dy2Vy0_dz2Vy0()
{
	return 50 * Field[I(i, j, k)].y
		+ 6 * Field[I(i + 1, j, k)].y
		+ 4 * Field[I(i + 2, j, k)].y
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k + 4)].y)
		- 104 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i, j + 3, k)].y + Field[I(i, j, k + 3)].y);
}
//(1,0,1)
  double CurlTwo::dx2Vy1_dy2Vy0_dz2Vy1()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k - 1)].y)
		- 104 * Field[I(i, j + 1, k)].y
		+ 114 * Field[I(i, j + 2, k)].y
		- 56 * Field[I(i, j + 3, k)].y
		- Field[I(i, j, k + 3)].y;

}
//(1,0,N1)
  double CurlTwo::dx2Vy1_dy2Vy0_dz2VyN1()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k + 1)].y)
		- 104 * Field[I(i, j + 1, k)].y
		+ 114 * Field[I(i, j + 2, k)].y
		- 56 * Field[I(i, j + 3, k)].y
		- Field[I(i, j, k - 3)].y;
}
//(1,0,N)
  double CurlTwo::dx2Vy1_dy2Vy0_dz2VyN()
{
	return 50 * Field[I(i, j, k)].y
		+ 6 * Field[I(i + 1, j, k)].y
		+ 4 * Field[I(i + 2, j, k)].y
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k - 4)].y)
		- 104 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i, j + 3, k)].y + Field[I(i, j, k - 3)].y);
}
//(1,0,1<k<nz-1)
  double CurlTwo::dx2Vy1_dy2Vy0_dz2Vy()
{
	return -15 * Field[I(i, j, k)].y
		+ 6 * Field[I(i + 1, j, k)].y
		+ 4 * Field[I(i + 2, j, k)].y
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 4, k)].y)
		- 104 * Field[I(i, j + 1, k)].y
		+ 114 * Field[I(i, j + 2, k)].y
		- 56 * Field[I(i, j + 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}

//
//(1,1,0)
  double CurlTwo::dx2Vy1_dy2Vy1_dz2Vy0()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j + 2, k)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 4)].y)
		- Field[I(i, j + 3, k)].y
		- 104 * Field[I(i, j, k + 1)].y + 114 * Field[I(i, j, k + 2)].y - 56 * Field[I(i, j, k + 3)].y;

}
//(1,1,1)
  double CurlTwo::dx2Vy1_dy2Vy1_dz2Vy1()
{
	return -60 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j + 2, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j + 3, k)].y
		- Field[I(i, j, k + 3)].y;

}
//(1,1,N1)
  double CurlTwo::dx2Vy1_dy2Vy1_dz2VyN1()
{
	return -60 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j + 2, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		- Field[I(i, j + 3, k)].y
		- Field[I(i, j, k - 3)].y;

}
//(1,1,N)
  double CurlTwo::dx2Vy1_dy2Vy1_dz2VyN()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j + 2, k)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k - 4)].y)
		- Field[I(i, j + 3, k)].y
		- 104 * Field[I(i, j, k - 1)].y + 114 * Field[I(i, j, k - 2)].y
		- 56 * Field[I(i, j, k - 3)].y;

}
//(1,1,1<k<nz-1)
  double CurlTwo::dx2Vy1_dy2Vy1_dz2Vy()
{
	return -70 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j + 2, k)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(1,N1,0)
  double CurlTwo::dx2Vy1_dy2VyN1_dz2Vy0()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 4)].y)
		- Field[I(i, j - 3, k)].y
		- 104 * Field[I(i, j, k + 1)].y + 114 * Field[I(i, j, k + 2)].y
		- 56 * Field[I(i, j, k + 3)].y;

}
//(1,N1,1)
  double CurlTwo::dx2Vy1_dy2VyN1_dz2Vy1()
{
	return -60 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j - 3, k)].y
		- Field[I(i, j, k + 3)].y;
}
//(1,N1,N1)
  double CurlTwo::dx2Vy1_dy2VyN1_dz2VyN1()
{
	return -60 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		- Field[I(i, j - 3, k)].y
		- Field[I(i, j, k - 3)].y;

}
//(1,N1,N)
  double CurlTwo::dx2Vy1_dy2VyN1_dz2VyN()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 4)].y)
		- Field[I(i, j - 3, k)].y
		- 104 * Field[I(i, j, k - 1)].y + 114 * Field[I(i, j, k - 2)].y - 56 * Field[I(i, j, k - 3)].y;

}
//(1,N1,1<k<nz-1)
  double CurlTwo::dx2Vy1_dy2VyN1_dz2Vy()
{
	return -70 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j - 2, k)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y)
		- Field[I(i, j - 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;

}
//
//(1,N,0)
  double CurlTwo::dx2Vy1_dy2VyN_dz2Vy0()
{
	return 50 * Field[I(i, j, k)].y + 6 * Field[I(i + 1, j, k)].y + 4 * Field[I(i + 2, j, k)].y
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k + 4)].y)
		- 104 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i, j - 3, k)].y + Field[I(i, j, k + 3)].y);
}
//(1,N,1)
  double CurlTwo::dx2Vy1_dy2VyN_dz2Vy1()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k - 1)].y)
		- 104 * Field[I(i, j - 1, k)].y + 114 * Field[I(i, j - 2, k)].y
		- 56 * Field[I(i, j - 3, k)].y
		- Field[I(i, j, k + 3)].y;

}
//(1,N,N1)
  double CurlTwo::dx2Vy1_dy2VyN_dz2VyN1()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k + 1)].y)
		- 104 * Field[I(i, j - 1, k)].y + 114 * Field[I(i, j - 2, k)].y
		- 56 * Field[I(i, j - 3, k)].y
		- Field[I(i, j, k - 3)].y;

}
//(1,N,N)
  double CurlTwo::dx2Vy1_dy2VyN_dz2VyN()
{
	return 50 * Field[I(i, j, k)].y + 6 * Field[I(i + 1, j, k)].y + 4 * Field[I(i + 2, j, k)].y
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k - 4)].y)
		- 104 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i, j - 3, k)].y + Field[I(i, j, k - 3)].y);
}
//(1,N,1<k<nz-1)
  double CurlTwo::dx2Vy1_dy2VyN_dz2Vy()
{
	return -15 * Field[I(i, j, k)].y + 6 * Field[I(i + 1, j, k)].y + 4 * Field[I(i + 2, j, k)].y
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 4, k)].y)
		- 104 * Field[I(i, j - 1, k)].y + 114 * Field[I(i, j - 2, k)].y
		- 56 * Field[I(i, j - 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(1,1<j<ny-1,0)
  double CurlTwo::dx2Vy1_dy2Vy_dz2Vy0()
{
	return -15 * Field[I(i, j, k)].y
		+ 6 * Field[I(i + 1, j, k)].y + 4 * Field[I(i + 2, j, k)].y
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k + 4)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- 104 * Field[I(i, j, k + 1)].y + 114 * Field[I(i, j, k + 2)].y - 56 * Field[I(i, j, k + 3)].y;

}
//(1,1<j<ny-1,1)
  double CurlTwo::dx2Vy1_dy2Vy_dz2Vy1()
{
	return -70 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- Field[I(i, j, k + 3)].y;

}
//(1,1<j<ny-1,N1)
  double CurlTwo::dx2Vy1_dy2Vy_dz2VyN1()
{
	return -70 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i + 2, j, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i + 3, j, k)].y
		+ 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- Field[I(i, j, k - 3)].y;
}
//(1,1<j<ny-1,N)
  double CurlTwo::dx2Vy1_dy2Vy_dz2VyN()
{
	return -15 * Field[I(i, j, k)].y
		+ 6 * Field[I(i + 1, j, k)].y + 4 * Field[I(i + 2, j, k)].y
		- Field[I(i + 3, j, k)].y + 11 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k - 4)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- 104 * Field[I(i, j, k - 1)].y + 114 * Field[I(i, j, k - 2)].y - 56 * Field[I(i, j, k - 3)].y;
}
//(1,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2Vy1_dy2Vy_dz2Vy()
{
	return -80 * Field[I(i, j, k)].y
		+ 6 * Field[I(i + 1, j, k)].y + 4 * Field[I(i + 2, j, k)].y
		- Field[I(i + 3, j, k)].y + 11 * Field[I(i - 1, j, k)].y
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//-------------------------------------------
//---start of i=nx-1-----------------------------------
//(nx-1,0,0)
  double CurlTwo::dx2VyN1_dy2Vy0_dz2Vy0()
{
	return 50 * Field[I(i, j, k)].y + 6 * Field[I(i - 1, j, k)].y + 4 * Field[I(i - 2, j, k)].y
		- Field[I(i - 3, j, k)].y + 11 * Field[I(i + 1, j, k)].y
		- 104 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y) + 114 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i, j + 3, k)].y + Field[I(i, j, k + 3)].y) + 11 * (Field[I(i, j + 4, k)].y + Field[I(i, j, k + 4)].y);
}
//(nx-1,0,1)
  double CurlTwo::dx2VyN1_dy2Vy0_dz2Vy1()
{
	return -5 * Field[I(i, j, k)].y + 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i - 3, j, k)].y + 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k - 1)].y)
		- 104 * Field[I(i, j + 1, k)].y + 114 * Field[I(i, j + 2, k)].y
		- 56 * Field[I(i, j + 3, k)].y + 11 * Field[I(i, j + 4, k)].y
		- Field[I(i, j, k + 3)].y;

}
//(nx-1,0,N1)
  double CurlTwo::dx2VyN1_dy2Vy0_dz2VyN1()
{
	return -5 * Field[I(i, j, k)].y + 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k + 1)].y)
		- 104 * Field[I(i, j + 1, k)].y + 114 * Field[I(i, j + 2, k)].y
		- 56 * Field[I(i, j + 3, k)].y - Field[I(i, j, k - 3)].y;
}
//(nx-1,0,N)
  double CurlTwo::dx2VyN1_dy2Vy0_dz2VyN()
{
	return 50 * Field[I(i, j, k)].y + 6 * Field[I(i - 1, j, k)].y + 4 * Field[I(i - 2, j, k)].y
		- Field[I(i - 3, j, k)].y + 11 * Field[I(i + 1, j, k)].y
		- 104 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i, j + 3, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i, j + 4, k)].y + Field[I(i, j, k - 4)].y);
}
//(nx-1,0,1<k<nz-1)
  double CurlTwo::dx2VyN1_dy2Vy0_dz2Vy()
{
	return -15 * Field[I(i, j, k)].y + 6 * Field[I(i - 1, j, k)].y + 4 * Field[I(i - 2, j, k)].y
		- Field[I(i - 3, j, k)].y + 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 4, k)].y)
		- 104 * Field[I(i, j + 1, k)].y + 114 * Field[I(i, j + 2, k)].y
		- 56 * Field[I(i, j + 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(nx-1,1,0)
  double CurlTwo::dx2VyN1_dy2Vy1_dz2Vy0()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].y) - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 4)].y)
		- Field[I(i, j + 3, k)].y
		- 104 * Field[I(i, j, k + 1)].y + 114 * Field[I(i, j, k + 2)].y - 56 * Field[I(i, j, k + 3)].y;
}
//(nx-1,1,1)
  double CurlTwo::dx2VyN1_dy2Vy1_dz2Vy1()
{
	return -60 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j + 3, k)].y - Field[I(i, j, k + 3)].y;
}
//(nx-1,1,N1)
  double CurlTwo::dx2VyN1_dy2Vy1_dz2VyN1()
{
	return -60 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		- Field[I(i, j + 3, k)].y - Field[I(i, j, k - 3)].y;
}
//(nx-1,1,N)
  double CurlTwo::dx2VyN1_dy2Vy1_dz2VyN()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].y) - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k - 4)].y)
		- Field[I(i, j + 3, k)].y
		- 104 * Field[I(i, j, k - 1)].y + 114 * Field[I(i, j, k - 2)].y - 56 * Field[I(i, j, k - 3)].y;
}
//(nx-1,1,1<k<nz-1)
  double CurlTwo::dx2VyN1_dy2Vy1_dz2Vy()
{
	return -70 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].y) - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y) - Field[I(i, j, k + 2)].y
		- Field[I(i, j, k - 2)].y;
}
//
//(nx-1,ny-1,0)
  double CurlTwo::dx2VyN1_dy2VyN1_dz2Vy0()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j - 2, k)].y) - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 4)].y)
		- Field[I(i, j - 3, k)].y
		- 104 * Field[I(i, j, k + 1)].y + 114 * Field[I(i, j, k + 2)].y - 56 * Field[I(i, j, k + 3)].y;
}
//(nx-1,N1,1)
  double CurlTwo::dx2VyN1_dy2VyN1_dz2Vy1()
{
	return -60 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j - 2, k)].y + Field[I(i, j, k + 2)].y) - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j - 3, k)].y - Field[I(i, j, k + 3)].y;
}
//(nx-1,N1,N1)
  double CurlTwo::dx2VyN1_dy2VyN1_dz2VyN1()
{
	return -60 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j - 2, k)].y + Field[I(i, j, k - 2)].y) - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		- Field[I(i, j - 3, k)].y - Field[I(i, j, k - 3)].y;
}
//(nx-1,N1,N)
  double CurlTwo::dx2VyN1_dy2VyN1_dz2VyN()
{
	return -5 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j - 2, k)].y) - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 4)].y)
		- Field[I(i, j - 3, k)].y
		- 104 * Field[I(i, j, k - 1)].y + 114 * Field[I(i, j, k - 2)].y - 56 * Field[I(i, j, k - 3)].y;

}
//(nx-1,N1,1<k<nz-1)
  double CurlTwo::dx2VyN1_dy2VyN1_dz2Vy()
{
	return -70 * Field[I(i, j, k)].y
		+ 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j - 2, k)].y) - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j + 1, k)].y)
		- Field[I(i, j - 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(nx-1,N,0)
  double CurlTwo::dx2VyN1_dy2VyN_dz2Vy0()
{
	return 50 * Field[I(i, j, k)].y + 6 * Field[I(i - 1, j, k)].y
		+ 4 * Field[I(i - 2, j, k)].y - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k + 4)].y)
		- 104 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i, j - 3, k)].y + Field[I(i, j, k + 3)].y);
}
//(nx-1,N,1)
  double CurlTwo::dx2VyN1_dy2VyN_dz2Vy1()
{
	return -5 * Field[I(i, j, k)].y + 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k + 2)].y) - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k - 1)].y)
		- 104 * Field[I(i, j - 1, k)].y + 114 * Field[I(i, j - 2, k)].y - 56 * Field[I(i, j - 3, k)].y
		- Field[I(i, j, k + 3)].y;
}
//(nx-1,N,N1)
  double CurlTwo::dx2VyN1_dy2VyN_dz2VyN1()
{
	return -5 * Field[I(i, j, k)].y + 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k - 2)].y) - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k + 1)].y)
		- 104 * Field[I(i, j - 1, k)].y + 114 * Field[I(i, j - 2, k)].y - 56 * Field[I(i, j - 3, k)].y
		- Field[I(i, j, k - 3)].y;
}
//(nx-1,N,N)
  double CurlTwo::dx2VyN1_dy2VyN_dz2VyN()
{
	return 50 * Field[I(i, j, k)].y + 6 * Field[I(i - 1, j, k)].y
		+ 4 * Field[I(i - 2, j, k)].y - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k - 4)].y)
		- 104 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i, j - 3, k)].y + Field[I(i, j, k - 3)].y);
}
//(nx-1,N,1<k<nz-1)
  double CurlTwo::dx2VyN1_dy2VyN_dz2Vy()
{
	return -15 * Field[I(i, j, k)].y + 6 * Field[I(i - 1, j, k)].y
		+ 4 * Field[I(i - 2, j, k)].y - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j - 4, k)].y)
		- 104 * Field[I(i, j - 1, k)].y + 114 * Field[I(i, j - 2, k)].y - 56 * Field[I(i, j - 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(nx-1,1<j<ny-1,0)
  double CurlTwo::dx2VyN1_dy2Vy_dz2Vy0()
{
	return -15 * Field[I(i, j, k)].y + 6 * Field[I(i - 1, j, k)].y
		+ 4 * Field[I(i - 2, j, k)].y - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k + 4)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- 104 * Field[I(i, j, k + 1)].y + 114 * Field[I(i, j, k + 2)].y - 56 * Field[I(i, j, k + 3)].y;
}
//(nx-1,1<j<ny-1,1)
  double CurlTwo::dx2VyN1_dy2Vy_dz2Vy1()
{
	return -70 * Field[I(i, j, k)].y + 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y - Field[I(i, j, k + 3)].y;
}
//(nx-1,1<j<ny-1,N1)
  double CurlTwo::dx2VyN1_dy2Vy_dz2VyN1()
{
	return -70 * Field[I(i, j, k)].y + 6 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i - 3, j, k)].y + 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y - Field[I(i, j, k - 3)].y;
}
//(nx-1,1<j<ny-1,N)
  double CurlTwo::dx2VyN1_dy2Vy_dz2VyN()
{
	return -15 * Field[I(i, j, k)].y + 6 * Field[I(i - 1, j, k)].y
		+ 4 * Field[I(i - 2, j, k)].y - Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i + 1, j, k)].y + Field[I(i, j, k - 4)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- 104 * Field[I(i, j, k - 1)].y + 114 * Field[I(i, j, k - 2)].y - 56 * Field[I(i, j, k - 3)].y;

}
//(nx-1,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2VyN1_dy2Vy_dz2Vy()
{
	return -80 * Field[I(i, j, k)].y + 6 * Field[I(i - 1, j, k)].y
		+ 4 * Field[I(i - 2, j, k)].y - Field[I(i - 3, j, k)].y + 11 * Field[I(i + 1, j, k)].y
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//-end of i=N1------------------------------------------
//
//-start of i=N-----------------------------------------
//(nx,0,0)
  double CurlTwo::dx2VyN_dy2Vy0_dz2Vy0()
{
	return 105 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j + 3, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k + 4)].y);
}
//(nx,0,1)
  double CurlTwo::dx2VyN_dy2Vy0_dz2Vy1()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j + 3, k)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k - 1)].y)
		+ 6 * Field[I(i, j, k + 1)].y + 4 * Field[I(i, j, k + 2)].y - Field[I(i, j, k + 3)].y;
}
//(nx,0,N1)
  double CurlTwo::dx2VyN_dy2Vy0_dz2VyN1()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j + 3, k)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k + 1)].y)
		+ 6 * Field[I(i, j, k - 1)].y + 4 * Field[I(i, j, k - 2)].y - Field[I(i, j, k - 3)].y;

}
//(nx,0,N)
  double CurlTwo::dx2VyN_dy2Vy0_dz2VyN()
{
	return 105 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j + 3, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j + 4, k)].y + Field[I(i, j, k - 4)].y);
}
//(nx,0,1<k<nz-1)
  double CurlTwo::dx2VyN_dy2Vy0_dz2Vy()
{
	return 40 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j + 2, k)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j + 3, k)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j + 4, k)].y)
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(nx,1,0)
  double CurlTwo::dx2VyN_dy2Vy1_dz2Vy0()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 4)].y)
		+ 6 * Field[I(i, j + 1, k)].y + 4 * Field[I(i, j + 2, k)].y - Field[I(i, j + 3, k)].y;
}
//(nx,1,1)
  double CurlTwo::dx2VyN_dy2Vy1_dz2Vy1()
{
	return -5 * Field[I(i, j, k)].y
		- 104 * Field[I(i - 1, j, k)].y
		+ 114 * Field[I(i - 2, j, k)].y
		- 56 * Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 6 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i, j + 3, k)].y - Field[I(i, j, k + 3)].y;
}
//(nx,1,N1)
  double CurlTwo::dx2VyN_dy2Vy1_dz2VyN1()
{
	return -5 * Field[I(i, j, k)].y
		- 104 * Field[I(i - 1, j, k)].y
		+ 114 * Field[I(i - 2, j, k)].y
		- 56 * Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 6 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i, j + 3, k)].y - Field[I(i, j, k - 3)].y;
}
//(nx,1,N)
  double CurlTwo::dx2VyN_dy2Vy1_dz2VyN()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k - 4)].y)
		+ 6 * Field[I(i, j + 1, k)].y + 4 * Field[I(i, j + 2, k)].y - Field[I(i, j + 3, k)].y;
}
//(nx,1,1<k<nz-1)
  double CurlTwo::dx2VyN_dy2Vy1_dz2Vy()
{
	return -15 * Field[I(i, j, k)].y
		- 104 * Field[I(i - 1, j, k)].y
		+ 114 * Field[I(i - 2, j, k)].y
		- 56 * Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 6 * Field[I(i, j + 1, k)].y + 4 * Field[I(i, j + 2, k)].y - Field[I(i, j + 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(nx,ny-1,0)
  double CurlTwo::dx2VyN_dy2VyN1_dz2Vy0()
{
	return 50 * Field[I(i, j, k)].y - 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 4)].y)
		+ 6 * Field[I(i, j - 1, k)].y + 4 * Field[I(i, j - 2, k)].y - Field[I(i, j - 3, k)].y;
}
//(nx,N1,1)
  double CurlTwo::dx2VyN_dy2VyN1_dz2Vy1()
{
	return -5 * Field[I(i, j, k)].y - 104 * Field[I(i - 1, j, k)].y
		+ 114 * Field[I(i - 2, j, k)].y - 56 * Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 6 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i, j - 3, k)].y - Field[I(i, j, k + 3)].y;
}
//(nx,N1,N1)
  double CurlTwo::dx2VyN_dy2VyN1_dz2VyN1()
{
	return -5 * Field[I(i, j, k)].y - 104 * Field[I(i - 1, j, k)].y
		+ 114 * Field[I(i - 2, j, k)].y - 56 * Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 6 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i, j - 3, k)].y - Field[I(i, j, k - 3)].y;
}
//(nx,N1,N)
  double CurlTwo::dx2VyN_dy2VyN1_dz2VyN()
{
	return 50 * Field[I(i, j, k)].y - 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j, k - 4)].y)
		+ 6 * Field[I(i, j - 1, k)].y + 4 * Field[I(i, j - 2, k)].y - Field[I(i, j - 3, k)].y;
}
//(nx,N1,1<k<nz-1)
  double CurlTwo::dx2VyN_dy2VyN1_dz2Vy()
{
	return -15 * Field[I(i, j, k)].y - 104 * Field[I(i - 1, j, k)].y
		+ 114 * Field[I(i - 2, j, k)].y - 56 * Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j + 1, k)].y)
		+ 6 * Field[I(i, j - 1, k)].y + 4 * Field[I(i, j - 2, k)].y - Field[I(i, j - 3, k)].y
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(nx,N,0)
  double CurlTwo::dx2VyN_dy2VyN_dz2Vy0()
{
	return 105 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j - 2, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j - 3, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k + 4)].y);
}
//(nx,N,1)
  double CurlTwo::dx2VyN_dy2VyN_dz2Vy1()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j - 2, k)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j - 3, k)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k - 1)].y)
		+ 6 * Field[I(i, j, k + 1)].y + 4 * Field[I(i, j, k + 2)].y - Field[I(i, j, k + 3)].y;

}
//(nx,N,N1)
  double CurlTwo::dx2VyN_dy2VyN_dz2VyN1()
{
	return 50 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j - 2, k)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j - 3, k)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k + 1)].y)
		+ 6 * Field[I(i, j, k - 1)].y + 4 * Field[I(i, j, k - 2)].y - Field[I(i, j, k - 3)].y;

}
//(nx,N,N)
  double CurlTwo::dx2VyN_dy2VyN_dz2VyN()
{
	return 105 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j - 2, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j - 3, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j - 4, k)].y + Field[I(i, j, k - 4)].y);
}
//(nx,N,1<k<nz-1)
  double CurlTwo::dx2VyN_dy2VyN_dz2Vy()
{
	return 40 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j - 1, k)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j - 2, k)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j - 3, k)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j - 4, k)].y)
		+ 16 * (Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;

}
//
//(nx,1<j<ny-1,0)
  double CurlTwo::dx2VyN_dy2Vy_dz2Vy0()
{
	return 40 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j, k + 4)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y;
}
//(nx,1<j<ny-1,1)
  double CurlTwo::dx2VyN_dy2Vy_dz2Vy1()
{
	return -15 * Field[I(i, j, k)].y
		- 104 * Field[I(i - 1, j, k)].y
		+ 114 * Field[I(i - 2, j, k)].y
		-  56 * Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		+ 6 * Field[I(i, j, k + 1)].y + 4 * Field[I(i, j, k + 2)].y - Field[I(i, j, k + 3)].y;
}
//(nx,1<j<ny-1,N1)
  double CurlTwo::dx2VyN_dy2Vy_dz2VyN1()
{
	return -15 * Field[I(i, j, k)].y
		- 104 * Field[I(i - 1, j, k)].y
		+ 114 * Field[I(i - 2, j, k)].y
		- 56 * Field[I(i - 3, j, k)].y
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j, k + 1)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		+ 6 * Field[I(i, j, k - 1)].y + 4 * Field[I(i, j, k - 2)].y - Field[I(i, j, k - 3)].y;
}
//(nx,1<j<ny-1,N)
  double CurlTwo::dx2VyN_dy2Vy_dz2VyN()
{
	return 40 * Field[I(i, j, k)].y
		- 104 * (Field[I(i - 1, j, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i - 2, j, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i - 3, j, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i - 4, j, k)].y + Field[I(i, j, k - 4)].y)
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y;
}
//(nx,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2VyN_dy2Vy_dz2Vy()
{
	return -25 * Field[I(i, j, k)].y
		- 104 * Field[I(i - 1, j, k)].y
		+ 114 * Field[I(i - 2, j, k)].y
		- 56 * Field[I(i - 3, j, k)].y
		+ 11 * Field[I(i - 4, j, k)].y
		+ 16 * (Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//-end of i=nx==============================
//==start of 1<i<nx-1========================
//(1<i<nx-1,0,0)
  double CurlTwo::dx2Vy_dy2Vy0_dz2Vy0()
{
	return 40 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- 104 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i, j + 3, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i, j + 4, k)].y + Field[I(i, j, k + 4)].y);
}
//(1<i<nx-1,0,1)
  double CurlTwo::dx2Vy_dy2Vy0_dz2Vy1()
{
	return -15 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- 104 * Field[I(i, j + 1, k)].y
		+ 114 * Field[I(i, j + 2, k)].y
		- 56 * Field[I(i, j + 3, k)].y
		+ 11 * (Field[I(i, j + 4, k)].y + Field[I(i, j, k - 1)].y)
		+ 6 * Field[I(i, j, k + 1)].y + 4 * Field[I(i, j, k + 2)].y - Field[I(i, j, k + 3)].y;
}
//(1<i<nx-1,0,N1)
  double CurlTwo::dx2Vy_dy2Vy0_dz2VyN1()
{
	return -15 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- 104 * Field[I(i, j + 1, k)].y
		+ 114 * Field[I(i, j + 2, k)].y
		- 56 * Field[I(i, j + 3, k)].y
		+ 11 * (Field[I(i, j + 4, k)].y + Field[I(i, j, k + 1)].y)
		+ 6 * Field[I(i, j, k - 1)].y + 4 * Field[I(i, j, k - 2)].y - Field[I(i, j, k - 3)].y;

}
//(1<i<nx-1,0,N)
  double CurlTwo::dx2Vy_dy2Vy0_dz2VyN()
{
	return 40 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- 104 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i, j + 3, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i, j + 4, k)].y + Field[I(i, j, k - 4)].y);
}
//(1<i<nx-1,0,1<k<nz-1)
  double CurlTwo::dx2Vy_dy2Vy0_dz2Vy()
{
	return -25 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y + Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- 104 * Field[I(i, j + 1, k)].y
		+ 114 * Field[I(i, j + 2, k)].y
		- 56 * Field[I(i, j + 3, k)].y
		+ 11 * Field[I(i, j + 4, k)].y
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(1<i<nx-1,1,0)
  double CurlTwo::dx2Vy_dy2Vy1_dz2Vy0()
{
	return -15 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		+ 6 * Field[I(i, j + 1, k)].y + 4 * Field[I(i, j + 2, k)].y
		- Field[I(i, j + 3, k)].y
		+ 11 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k + 4)].y)
		- 104 * Field[I(i, j, k + 1)].y + 114 * Field[I(i, j, k + 2)].y - 56 * Field[I(i, j, k + 3)].y;
}
//(1<i<nx-1,1,1)
  double CurlTwo::dx2Vy_dy2Vy1_dz2Vy1()
{
	return -70 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		+ 6 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i, j + 3, k)].y
		+ 11 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 3)].y;
}
//(1<i<nx-1,1,N1)
  double CurlTwo::dx2Vy_dy2Vy1_dz2VyN1()
{
	return -70 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		+ 6 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i, j + 2, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i, j + 3, k)].y
		+ 11 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		- Field[I(i, j, k - 3)].y;
}
//(1<i<nx-1,1,N)
  double CurlTwo::dx2Vy_dy2Vy1_dz2VyN()
{
	return -15 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		+ 6 * Field[I(i, j + 1, k)].y + 4 * Field[I(i, j + 2, k)].y
		- Field[I(i, j + 3, k)].y
		+ 11 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k - 4)].y)
		- 104 * Field[I(i, j, k - 1)].y + 114 * Field[I(i, j, k - 2)].y
		- 56 * Field[I(i, j, k - 3)].y;
}
//(1<i<nx-1,1,1<k<nz-1)
  double CurlTwo::dx2Vy_dy2Vy1_dz2Vy()
{
	return -80 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y + Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		+ 6 * Field[I(i, j + 1, k)].y + 4 * Field[I(i, j + 2, k)].y
		- Field[I(i, j + 3, k)].y + 11 * Field[I(i, j - 1, k)].y
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(1<i<nx-1,ny-1,0)
  double CurlTwo::dx2Vy_dy2VyN1_dz2Vy0()
{
	return -15 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		+ 6 * Field[I(i, j - 1, k)].y + 4 * Field[I(i, j - 2, k)].y
		- Field[I(i, j - 3, k)].y + 11 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k + 4)].y)
		- 104 * Field[I(i, j, k + 1)].y + 114 * Field[I(i, j, k + 2)].y - 56 * Field[I(i, j, k + 3)].y;

}
//(1<i<nx-1,N1,1)
  double CurlTwo::dx2Vy_dy2VyN1_dz2Vy1()
{
	return -70 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		+ 6 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 4 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k + 2)].y)
		- Field[I(i, j - 3, k)].y
		+ 11 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i, j, k + 3)].y;
}
//(1<i<nx-1,N1,N1)
  double CurlTwo::dx2Vy_dy2VyN1_dz2VyN1()
{
	return -70 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		+ 6 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 4 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k - 2)].y)
		- Field[I(i, j - 3, k)].y
		+ 11 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k + 1)].y)
		- Field[I(i, j, k - 3)].y;

}
//(1<i<nx-1,N1,N)
  double CurlTwo::dx2Vy_dy2VyN1_dz2VyN()
{
	return -15 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		+ 6 * Field[I(i, j - 1, k)].y + 4 * Field[I(i, j - 2, k)].y
		- Field[I(i, j - 3, k)].y + 11 * (Field[I(i, j + 1, k)].y + Field[I(i, j, k - 4)].y)
		- 104 * Field[I(i, j, k - 1)].y + 114 * Field[I(i, j, k - 2)].y - 56 * Field[I(i, j, k - 3)].y;
}
//(1<i<nx-1,N1,1<k<nz-1)
  double CurlTwo::dx2Vy_dy2VyN1_dz2Vy()
{
	return -80 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y + Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		+ 6 * Field[I(i, j - 1, k)].y + 4 * Field[I(i, j - 2, k)].y
		- Field[I(i, j - 3, k)].y + 11 * Field[I(i, j + 1, k)].y
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(1<i<nx-1,N,0)
  double CurlTwo::dx2Vy_dy2VyN_dz2Vy0()
{
	return 40 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- 104 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k + 1)].y)
		+ 114 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k + 2)].y)
		- 56 * (Field[I(i, j - 3, k)].y + Field[I(i, j, k + 3)].y)
		+ 11 * (Field[I(i, j - 4, k)].y + Field[I(i, j, k + 4)].y);
}
//(1<i<nx-1,N,1)
  double CurlTwo::dx2Vy_dy2VyN_dz2Vy1()
{
	return -15 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- 104 * Field[I(i, j - 1, k)].y
		+ 114 * Field[I(i, j - 2, k)].y
		- 56 * Field[I(i, j - 3, k)].y
		+ 11 * (Field[I(i, j - 4, k)].y + Field[I(i, j, k - 1)].y)
		+ 6 * Field[I(i, j, k + 1)].y + 4 * Field[I(i, j, k + 2)].y - Field[I(i, j, k + 3)].y;

}
//(1<i<nx-1,N,N1)
  double CurlTwo::dx2Vy_dy2VyN_dz2VyN1()
{
	return -15 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- 104 * Field[I(i, j - 1, k)].y
		+ 114 * Field[I(i, j - 2, k)].y
		- 56 * Field[I(i, j - 3, k)].y
		+ 11 * (Field[I(i, j - 4, k)].y + Field[I(i, j, k + 1)].y)
		+ 6 * Field[I(i, j, k - 1)].y + 4 * Field[I(i, j, k - 2)].y - Field[I(i, j, k - 3)].y;
}
//(1<i<nx-1,N,N)
  double CurlTwo::dx2Vy_dy2VyN_dz2VyN()
{
	return 40 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- 104 * (Field[I(i, j - 1, k)].y + Field[I(i, j, k - 1)].y)
		+ 114 * (Field[I(i, j - 2, k)].y + Field[I(i, j, k - 2)].y)
		- 56 * (Field[I(i, j - 3, k)].y + Field[I(i, j, k - 3)].y)
		+ 11 * (Field[I(i, j - 4, k)].y + Field[I(i, j, k - 4)].y);
}
//(1<i<nx-1,N,1<k<nz-1)
  double CurlTwo::dx2Vy_dy2VyN_dz2Vy()
{
	return -25 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y + Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- 104 * Field[I(i, j - 1, k)].y
		+ 114 * Field[I(i, j - 2, k)].y
		- 56 * Field[I(i, j - 3, k)].y
		+ 11 * Field[I(i, j - 4, k)].y
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//
//(1<i<nx-1,1<j<ny-1,0)
  double CurlTwo::dx2Vy_dy2Vy_dz2Vy0()
{
	return -25 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- 104 * Field[I(i, j, k + 1)].y + 114 * Field[I(i, j, k + 2)].y
		- 56 * Field[I(i, j, k + 3)].y + 11 * Field[I(i, j, k + 4)].y;
}
//(1<i<nx-1,1<j<ny-1,1)
  double CurlTwo::dx2Vy_dy2Vy_dz2Vy1()
{
	return -80 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		+ 6 * Field[I(i, j, k + 1)].y + 4 * Field[I(i, j, k + 2)].y
		- Field[I(i, j, k + 3)].y + 11 * Field[I(i, j, k - 1)].y;
}
//(1<i<nx-1,1<j<ny-1,N1)
  double CurlTwo::dx2Vy_dy2Vy_dz2VyN1()
{
	return -80 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		+ 6 * Field[I(i, j, k - 1)].y + 4 * Field[I(i, j, k - 2)].y
		- Field[I(i, j, k - 3)].y + 11 * Field[I(i, j, k + 1)].y;
}
//(1<i<nx-1,1<j<ny-1,N)
  double CurlTwo::dx2Vy_dy2Vy_dz2VyN()
{
	return -25 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y + Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- 104 * Field[I(i, j, k - 1)].y + 114 * Field[I(i, j, k - 2)].y
		- 56 * Field[I(i, j, k - 3)].y + 11 * Field[I(i, j, k - 4)].y;
}
//(1<i<nx-1,1<j<ny-1,1<k<nz-1)
  double CurlTwo::dx2Vy_dy2Vy_dz2Vy()
{
	return -90 * Field[I(i, j, k)].y
		+ 16 * (Field[I(i + 1, j, k)].y + Field[I(i - 1, j, k)].y
		+ Field[I(i, j + 1, k)].y + Field[I(i, j - 1, k)].y
		+ Field[I(i, j, k + 1)].y + Field[I(i, j, k - 1)].y)
		- Field[I(i + 2, j, k)].y - Field[I(i - 2, j, k)].y
		- Field[I(i, j + 2, k)].y - Field[I(i, j - 2, k)].y
		- Field[I(i, j, k + 2)].y - Field[I(i, j, k - 2)].y;
}
//==end of 1<i<nx-1========================




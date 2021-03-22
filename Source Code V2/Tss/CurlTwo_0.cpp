
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

calculate the second order curl
********************************************************************/

#include "CurlTwo.h"
double CurlTwo::Dx2Vx(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (i == 0)
	{
		return dx2Vx0();
	}
	if (i == 1)
	{
		return dx2Vx1();
	}
	if (i == nxN1)
	{
		return dx2VxN1();
	}
	if (i == nx)
	{
		return dx2VxN();
	}
	return dx2Vx();

}
double CurlTwo::Dy2Vy(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (j == 0)
	{
		return dy2Vy0();
	}
	if (j == 1)
	{
		return dy2Vy1();
	}
	if (j == nyN1)
	{
		return dy2VyN1();
	}
	if (j == ny)
	{
		return dy2VyN();
	}
	return dy2Vy();
}
double CurlTwo::Dz2Vz(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (k == 0)
	{
		return dz2Vz0();
	}
	if (k == 1)
	{
		return dz2Vz1();
	}
	if (k == nzN1)
	{
		return dz2VzN1();
	}
	if (k == nz)
	{
		return dz2VzN();
	}
	return dz2Vz();
}
//
double CurlTwo::Dy2Vx(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (j == 0)
	{
		return dy2Vx0();
	}
	if (j == 1)
	{
		return dy2Vx1();
	}
	if (j == nyN1)
	{
		return dy2VxN1();
	}
	if (j == ny)
	{
		return dy2VxN();
	}
	return dy2Vx();
}
double CurlTwo::Dz2Vy(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (k == 0)
	{
		return dz2Vy0();
	}
	if (k == 1)
	{
		return dz2Vy1();
	}
	if (k == nzN1)
	{
		return dz2VyN1();
	}
	if (k == nz)
	{
		return dz2VyN();
	}
	return dz2Vy();
}
double CurlTwo::Dx2Vz(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (i == 0)
	{
		return dx2Vz0();
	}
	if (i == 1)
	{
		return dx2Vz1();
	}
	if (i == nxN1)
	{
		return dx2VzN1();
	}
	if (i == nx)
	{
		return dx2VzN();
	}
	return dx2Vz();
}
//
double CurlTwo::Dz2Vx(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (k == 0)
	{
		return dz2Vx0();
	}
	if (k == 1)
	{
		return dz2Vx1();
	}
	if (k == nzN1)
	{
		return dz2VxN1();
	}
	if (k == nz)
	{
		return dz2VxN();
	}
	return dz2Vx();
}
double CurlTwo::Dx2Vy(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (i == 0)
	{
		return dx2Vy0();
	}
	if (i == 1)
	{
		return dx2Vy1();
	}
	if (i == nxN1)
	{
		return dx2VyN1();
	}
	if (i == nx)
	{
		return dx2VyN();
	}
	return dx2Vy();
}
double CurlTwo::Dy2Vz(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0)
{
	Field = f;
	i = i0; j = j0; k = k0;
	w = I(i, j, k);
	if (j == 0)
	{
		return dy2Vz0();
	}
	if (j == 1)
	{
		return dy2Vz1();
	}
	if (j == nyN1)
	{
		return dy2VzN1();
	}
	if (j == ny)
	{
		return dy2VzN();
	}
	return dy2Vz();
}

int CurlTwo::_getNextCurlUseFastMethod(Point3Dstruct *pField, Point3Dstruct *pNext)
{
	int ret = ERR_OK;
	if (pNext == pField)
		return ERR_CURL_FAILED;
	//the results need to be divided by 12
	Field = pField;
	w = 0;
	for (i = 0; i <= nx; i++)
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
							pNext[w].x = dx2Vx1_dy2Vx1_dz2Vx1();
							pNext[w].y = dx2Vy1_dy2Vy1_dz2Vy1();
							pNext[w].z = dx2Vz1_dy2Vz1_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1,1,0)
							pNext[w].x = dx2Vx1_dy2Vx1_dz2Vx0();
							pNext[w].y = dx2Vy1_dy2Vy1_dz2Vy0();
							pNext[w].z = dx2Vz1_dy2Vz1_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1,1,nz-1)
							pNext[w].x = dx2Vx1_dy2Vx1_dz2VxN1();
							pNext[w].y = dx2Vy1_dy2Vy1_dz2VyN1();
							pNext[w].z = dx2Vz1_dy2Vz1_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1,1,nz)
							pNext[w].x = dx2Vx1_dy2Vx1_dz2VxN();
							pNext[w].y = dx2Vy1_dy2Vy1_dz2VyN();
							pNext[w].z = dx2Vz1_dy2Vz1_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(1,1,1<k<nz-1)
							pNext[w].x = dx2Vx1_dy2Vx1_dz2Vx();
							pNext[w].y = dx2Vy1_dy2Vy1_dz2Vy();
							pNext[w].z = dx2Vz1_dy2Vz1_dz2Vz();
						}
					}
					else if (j == 0)
					{
						//y at the lower boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1,0,1)
							pNext[w].x = dx2Vx1_dy2Vx0_dz2Vx1();
							pNext[w].y = dx2Vy1_dy2Vy0_dz2Vy1();
							pNext[w].z = dx2Vz1_dy2Vz0_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1,0,0)
							pNext[w].x = dx2Vx1_dy2Vx0_dz2Vx0();
							pNext[w].y = dx2Vy1_dy2Vy0_dz2Vy0();
							pNext[w].z = dx2Vz1_dy2Vz0_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1,0,nz-1)
							pNext[w].x = dx2Vx1_dy2Vx0_dz2VxN1();
							pNext[w].y = dx2Vy1_dy2Vy0_dz2VyN1();
							pNext[w].z = dx2Vz1_dy2Vz0_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1,0,nz)
							pNext[w].x = dx2Vx1_dy2Vx0_dz2VxN();
							pNext[w].y = dx2Vy1_dy2Vy0_dz2VyN();
							pNext[w].z = dx2Vz1_dy2Vz0_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(1,0,1<k<nz-1)
							pNext[w].x = dx2Vx1_dy2Vx0_dz2Vx();
							pNext[w].y = dx2Vy1_dy2Vy0_dz2Vy();
							pNext[w].z = dx2Vz1_dy2Vz0_dz2Vz();
						}
					}
					else if (j == nyN1)
					{
						//y near the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1,ny-1,1)
							pNext[w].x = dx2Vx1_dy2VxN1_dz2Vx1();
							pNext[w].y = dx2Vy1_dy2VyN1_dz2Vy1();
							pNext[w].z = dx2Vz1_dy2VzN1_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1,ny-1,0)
							pNext[w].x = dx2Vx1_dy2VxN1_dz2Vx0();
							pNext[w].y = dx2Vy1_dy2VyN1_dz2Vy0();
							pNext[w].z = dx2Vz1_dy2VzN1_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1,ny-1,nz-1)
							pNext[w].x = dx2Vx1_dy2VxN1_dz2VxN1();
							pNext[w].y = dx2Vy1_dy2VyN1_dz2VyN1();
							pNext[w].z = dx2Vz1_dy2VzN1_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1,ny-1,nz)
							pNext[w].x = dx2Vx1_dy2VxN1_dz2VxN();
							pNext[w].y = dx2Vy1_dy2VyN1_dz2VyN();
							pNext[w].z = dx2Vz1_dy2VzN1_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(1,ny-1,1<k<nz-1)
							pNext[w].x = dx2Vx1_dy2VxN1_dz2Vx();
							pNext[w].y = dx2Vy1_dy2VyN1_dz2Vy();
							pNext[w].z = dx2Vz1_dy2VzN1_dz2Vz();
						}
					}
					else if (j == ny)
					{
						//y at the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1,ny,1)
							pNext[w].x = dx2Vx1_dy2VxN_dz2Vx1();
							pNext[w].y = dx2Vy1_dy2VyN_dz2Vy1();
							pNext[w].z = dx2Vz1_dy2VzN_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1,ny,0)
							pNext[w].x = dx2Vx1_dy2VxN_dz2Vx0();
							pNext[w].y = dx2Vy1_dy2VyN_dz2Vy0();
							pNext[w].z = dx2Vz1_dy2VzN_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1,ny,nz-1)
							pNext[w].x = dx2Vx1_dy2VxN_dz2VxN1();
							pNext[w].y = dx2Vy1_dy2VyN_dz2VyN1();
							pNext[w].z = dx2Vz1_dy2VzN_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1,ny,nz)
							pNext[w].x = dx2Vx1_dy2VxN_dz2VxN();
							pNext[w].y = dx2Vy1_dy2VyN_dz2VyN();
							pNext[w].z = dx2Vz1_dy2VzN_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(1,ny,1<k<nz-1)
							pNext[w].x = dx2Vx1_dy2VxN_dz2Vx();
							pNext[w].y = dx2Vy1_dy2VyN_dz2Vy();
							pNext[w].z = dx2Vz1_dy2VzN_dz2Vz();
						}
					}
					else
					{
						//y inside boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1,1<j<ny-1,1)
							pNext[w].x = dx2Vx1_dy2Vx_dz2Vx1();
							pNext[w].y = dx2Vy1_dy2Vy_dz2Vy1();
							pNext[w].z = dx2Vz1_dy2Vz_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1,1<j<ny-1,0)
							pNext[w].x = dx2Vx1_dy2Vx_dz2Vx0();
							pNext[w].y = dx2Vy1_dy2Vy_dz2Vy0();
							pNext[w].z = dx2Vz1_dy2Vz_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1,1<j<ny-1,nz-1)
							pNext[w].x = dx2Vx1_dy2Vx_dz2VxN1();
							pNext[w].y = dx2Vy1_dy2Vy_dz2VyN1();
							pNext[w].z = dx2Vz1_dy2Vz_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1,1<j<ny-1,nz)
							pNext[w].x = dx2Vx1_dy2Vx_dz2VxN();
							pNext[w].y = dx2Vy1_dy2Vy_dz2VyN();
							pNext[w].z = dx2Vz1_dy2Vz_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(1,1<j<ny-1,1<k<nz-1)
							pNext[w].x = dx2Vx1_dy2Vx_dz2Vx();
							pNext[w].y = dx2Vy1_dy2Vy_dz2Vy();
							pNext[w].z = dx2Vz1_dy2Vz_dz2Vz();
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
							pNext[w].x = dx2Vx0_dy2Vx1_dz2Vx1();
							pNext[w].y = dx2Vy0_dy2Vy1_dz2Vy1();
							pNext[w].z = dx2Vz0_dy2Vz1_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(0,1,0)
							pNext[w].x = dx2Vx0_dy2Vx1_dz2Vx0();
							pNext[w].y = dx2Vy0_dy2Vy1_dz2Vy0();
							pNext[w].z = dx2Vz0_dy2Vz1_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(0,1,nz-1)
							pNext[w].x = dx2Vx0_dy2Vx1_dz2VxN1();
							pNext[w].y = dx2Vy0_dy2Vy1_dz2VyN1();
							pNext[w].z = dx2Vz0_dy2Vz1_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(0,1,nz)
							pNext[w].x = dx2Vx0_dy2Vx1_dz2VxN();
							pNext[w].y = dx2Vy0_dy2Vy1_dz2VyN();
							pNext[w].z = dx2Vz0_dy2Vz1_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(0,1,1<k<nz-1)
							pNext[w].x = dx2Vx0_dy2Vx1_dz2Vx();
							pNext[w].y = dx2Vy0_dy2Vy1_dz2Vy();
							pNext[w].z = dx2Vz0_dy2Vz1_dz2Vz();
						}
					}
					else if (j == 0)
					{
						//y at the lower boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(0,0,1)
							pNext[w].x = dx2Vx0_dy2Vx0_dz2Vx1();
							pNext[w].y = dx2Vy0_dy2Vy0_dz2Vy1();
							pNext[w].z = dx2Vz0_dy2Vz0_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(0,0,0)
							pNext[w].x = dx2Vx0_dy2Vx0_dz2Vx0();
							pNext[w].y = dx2Vy0_dy2Vy0_dz2Vy0();
							pNext[w].z = dx2Vz0_dy2Vz0_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(0,0,nz-1)
							pNext[w].x = dx2Vx0_dy2Vx0_dz2VxN1();
							pNext[w].y = dx2Vy0_dy2Vy0_dz2VyN1();
							pNext[w].z = dx2Vz0_dy2Vz0_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(0,0,nz)
							pNext[w].x = dx2Vx0_dy2Vx0_dz2VxN();
							pNext[w].y = dx2Vy0_dy2Vy0_dz2VyN();
							pNext[w].z = dx2Vz0_dy2Vz0_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(0,0,1<k<nz-1)
							pNext[w].x = dx2Vx0_dy2Vx0_dz2Vx();
							pNext[w].y = dx2Vy0_dy2Vy0_dz2Vy();
							pNext[w].z = dx2Vz0_dy2Vz0_dz2Vz();
						}
					}
					else if (j == nyN1)
					{
						//y near the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(0,ny-1,1)
							pNext[w].x = dx2Vx0_dy2VxN1_dz2Vx1();
							pNext[w].y = dx2Vy0_dy2VyN1_dz2Vy1();
							pNext[w].z = dx2Vz0_dy2VzN1_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(0,ny-1,0)
							pNext[w].x = dx2Vx0_dy2VxN1_dz2Vx0();
							pNext[w].y = dx2Vy0_dy2VyN1_dz2Vy0();
							pNext[w].z = dx2Vz0_dy2VzN1_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(0,ny-1,nz-1)
							pNext[w].x = dx2Vx0_dy2VxN1_dz2VxN1();
							pNext[w].y = dx2Vy0_dy2VyN1_dz2VyN1();
							pNext[w].z = dx2Vz0_dy2VzN1_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(0,ny-1,nz)
							pNext[w].x = dx2Vx0_dy2VxN1_dz2VxN();
							pNext[w].y = dx2Vy0_dy2VyN1_dz2VyN();
							pNext[w].z = dx2Vz0_dy2VzN1_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(0,ny-1,1<k<nz-1)
							pNext[w].x = dx2Vx0_dy2VxN1_dz2Vx();
							pNext[w].y = dx2Vy0_dy2VyN1_dz2Vy();
							pNext[w].z = dx2Vz0_dy2VzN1_dz2Vz();
						}
					}
					else if (j == ny)
					{
						//y at the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(0,ny,1)
							pNext[w].x = dx2Vx0_dy2VxN_dz2Vx1();
							pNext[w].y = dx2Vy0_dy2VyN_dz2Vy1();
							pNext[w].z = dx2Vz0_dy2VzN_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(0,ny,0)
							pNext[w].x = dx2Vx0_dy2VxN_dz2Vx0();
							pNext[w].y = dx2Vy0_dy2VyN_dz2Vy0();
							pNext[w].z = dx2Vz0_dy2VzN_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(0,ny,nz-1)
							pNext[w].x = dx2Vx0_dy2VxN_dz2VxN1();
							pNext[w].y = dx2Vy0_dy2VyN_dz2VyN1();
							pNext[w].z = dx2Vz0_dy2VzN_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(0,ny,nz)
							pNext[w].x = dx2Vx0_dy2VxN_dz2VxN();
							pNext[w].y = dx2Vy0_dy2VyN_dz2VyN();
							pNext[w].z = dx2Vz0_dy2VzN_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(0,ny,1<k<nz-1)
							pNext[w].x = dx2Vx0_dy2VxN_dz2Vx();
							pNext[w].y = dx2Vy0_dy2VyN_dz2Vy();
							pNext[w].z = dx2Vz0_dy2VzN_dz2Vz();
						}
					}
					else
					{
						//y inside boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(0,1<j<ny-1,1)
							pNext[w].x = dx2Vx0_dy2Vx_dz2Vx1();
							pNext[w].y = dx2Vy0_dy2Vy_dz2Vy1();
							pNext[w].z = dx2Vz0_dy2Vz_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(0,1<j<ny-1,0)
							pNext[w].x = dx2Vx0_dy2Vx_dz2Vx0();
							pNext[w].y = dx2Vy0_dy2Vy_dz2Vy0();
							pNext[w].z = dx2Vz0_dy2Vz_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(0, 1<j<ny - 1, nz-1)
							pNext[w].x = dx2Vx0_dy2Vx_dz2VxN1();
							pNext[w].y = dx2Vy0_dy2Vy_dz2VyN1();
							pNext[w].z = dx2Vz0_dy2Vz_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(0,1<j<ny-1,nz)
							pNext[w].x = dx2Vx0_dy2Vx_dz2VxN();
							pNext[w].y = dx2Vy0_dy2Vy_dz2VyN();
							pNext[w].z = dx2Vz0_dy2Vz_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(0,1<j<ny-1,1<k<nz-1)
							pNext[w].x = dx2Vx0_dy2Vx_dz2Vx();
							pNext[w].y = dx2Vy0_dy2Vy_dz2Vy();
							pNext[w].z = dx2Vz0_dy2Vz_dz2Vz();
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
							pNext[w].x = dx2VxN1_dy2Vx1_dz2Vx1();
							pNext[w].y = dx2VyN1_dy2Vy1_dz2Vy1();
							pNext[w].z = dx2VzN1_dy2Vz1_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx-1,1,0)
							pNext[w].x = dx2VxN1_dy2Vx1_dz2Vx0();
							pNext[w].y = dx2VyN1_dy2Vy1_dz2Vy0();
							pNext[w].z = dx2VzN1_dy2Vz1_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx-1,1,nz-1)
							pNext[w].x = dx2VxN1_dy2Vx1_dz2VxN1();
							pNext[w].y = dx2VyN1_dy2Vy1_dz2VyN1();
							pNext[w].z = dx2VzN1_dy2Vz1_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx-1,1,nz)
							pNext[w].x = dx2VxN1_dy2Vx1_dz2VxN();
							pNext[w].y = dx2VyN1_dy2Vy1_dz2VyN();
							pNext[w].z = dx2VzN1_dy2Vz1_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(nx-1,1,1<k<nz-1)
							pNext[w].x = dx2VxN1_dy2Vx1_dz2Vx();
							pNext[w].y = dx2VyN1_dy2Vy1_dz2Vy();
							pNext[w].z = dx2VzN1_dy2Vz1_dz2Vz();
						}
					}
					else if (j == 0)
					{
						//y at the lower boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx-1,0,1)
							pNext[w].x = dx2VxN1_dy2Vx0_dz2Vx1();
							pNext[w].y = dx2VyN1_dy2Vy0_dz2Vy1();
							pNext[w].z = dx2VzN1_dy2Vz0_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx-1,0,0)
							pNext[w].x = dx2VxN1_dy2Vx0_dz2Vx0();
							pNext[w].y = dx2VyN1_dy2Vy0_dz2Vy0();
							pNext[w].z = dx2VzN1_dy2Vz0_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx-1,0,nz-1)
							pNext[w].x = dx2VxN1_dy2Vx0_dz2VxN1();
							pNext[w].y = dx2VyN1_dy2Vy0_dz2VyN1();
							pNext[w].z = dx2VzN1_dy2Vz0_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx-1,0,nz)
							pNext[w].x = dx2VxN1_dy2Vx0_dz2VxN();
							pNext[w].y = dx2VyN1_dy2Vy0_dz2VyN();
							pNext[w].z = dx2VzN1_dy2Vz0_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(nx-1,0,1<k<nz-1)
							pNext[w].x = dx2VxN1_dy2Vx0_dz2Vx();
							pNext[w].y = dx2VyN1_dy2Vy0_dz2Vy();
							pNext[w].z = dx2VzN1_dy2Vz0_dz2Vz();
						}
					}
					else if (j == nyN1)
					{
						//y near the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx-1,ny-1,1)
							pNext[w].x = dx2VxN1_dy2VxN1_dz2Vx1();
							pNext[w].y = dx2VyN1_dy2VyN1_dz2Vy1();
							pNext[w].z = dx2VzN1_dy2VzN1_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx-1,ny-1,0)
							pNext[w].x = dx2VxN1_dy2VxN1_dz2Vx0();
							pNext[w].y = dx2VyN1_dy2VyN1_dz2Vy0();
							pNext[w].z = dx2VzN1_dy2VzN1_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx-1,ny-1,nz-1)
							pNext[w].x = dx2VxN1_dy2VxN1_dz2VxN1();
							pNext[w].y = dx2VyN1_dy2VyN1_dz2VyN1();
							pNext[w].z = dx2VzN1_dy2VzN1_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx-1,ny-1,nz)
							pNext[w].x = dx2VxN1_dy2VxN1_dz2VxN();
							pNext[w].y = dx2VyN1_dy2VyN1_dz2VyN();
							pNext[w].z = dx2VzN1_dy2VzN1_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(nx-1,ny-1,1<k<nz-1)
							pNext[w].x = dx2VxN1_dy2VxN1_dz2Vx();
							pNext[w].y = dx2VyN1_dy2VyN1_dz2Vy();
							pNext[w].z = dx2VzN1_dy2VzN1_dz2Vz();
						}
					}
					else if (j == ny)
					{
						//y at the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx-1,ny,1)
							pNext[w].x = dx2VxN1_dy2VxN_dz2Vx1();
							pNext[w].y = dx2VyN1_dy2VyN_dz2Vy1();
							pNext[w].z = dx2VzN1_dy2VzN_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx-1,ny,0)
							pNext[w].x = dx2VxN1_dy2VxN_dz2Vx0();
							pNext[w].y = dx2VyN1_dy2VyN_dz2Vy0();
							pNext[w].z = dx2VzN1_dy2VzN_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx-1,ny,nz-1)
							pNext[w].x = dx2VxN1_dy2VxN_dz2VxN1();
							pNext[w].y = dx2VyN1_dy2VyN_dz2VyN1();
							pNext[w].z = dx2VzN1_dy2VzN_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx-1,ny,nz)
							pNext[w].x = dx2VxN1_dy2VxN_dz2VxN();
							pNext[w].y = dx2VyN1_dy2VyN_dz2VyN();
							pNext[w].z = dx2VzN1_dy2VzN_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(nx-1,ny,1<k<nz-1)
							pNext[w].x = dx2VxN1_dy2VxN_dz2Vx();
							pNext[w].y = dx2VyN1_dy2VyN_dz2Vy();
							pNext[w].z = dx2VzN1_dy2VzN_dz2Vz();
						}
					}
					else
					{
						//y inside boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx-1,1<j<ny-1,1)
							pNext[w].x = dx2VxN1_dy2Vx_dz2Vx1();
							pNext[w].y = dx2VyN1_dy2Vy_dz2Vy1();
							pNext[w].z = dx2VzN1_dy2Vz_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx-1,1<j<ny-1,0)
							pNext[w].x = dx2VxN1_dy2Vx_dz2Vx0();
							pNext[w].y = dx2VyN1_dy2Vy_dz2Vy0();
							pNext[w].z = dx2VzN1_dy2Vz_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx-1,1<j<ny-1,nz-1)
							pNext[w].x = dx2VxN1_dy2Vx_dz2VxN1();
							pNext[w].y = dx2VyN1_dy2Vy_dz2VyN1();
							pNext[w].z = dx2VzN1_dy2Vz_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx-1,1<j<ny-1,nz)
							pNext[w].x = dx2VxN1_dy2Vx_dz2VxN();
							pNext[w].y = dx2VyN1_dy2Vy_dz2VyN();
							pNext[w].z = dx2VzN1_dy2Vz_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(nx-1,1<j<ny-1,1<k<nz-1)
							pNext[w].x = dx2VxN1_dy2Vx_dz2Vx();
							pNext[w].y = dx2VyN1_dy2Vy_dz2Vy();
							pNext[w].z = dx2VzN1_dy2Vz_dz2Vz();
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
							pNext[w].x = dx2VxN_dy2Vx1_dz2Vx1();
							pNext[w].y = dx2VyN_dy2Vy1_dz2Vy1();
							pNext[w].z = dx2VzN_dy2Vz1_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx,1,0)
							pNext[w].x = dx2VxN_dy2Vx1_dz2Vx0();
							pNext[w].y = dx2VyN_dy2Vy1_dz2Vy0();
							pNext[w].z = dx2VzN_dy2Vz1_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx,1,nz-1)
							pNext[w].x = dx2VxN_dy2Vx1_dz2VxN1();
							pNext[w].y = dx2VyN_dy2Vy1_dz2VyN1();
							pNext[w].z = dx2VzN_dy2Vz1_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx,1,nz)
							pNext[w].x = dx2VxN_dy2Vx1_dz2VxN();
							pNext[w].y = dx2VyN_dy2Vy1_dz2VyN();
							pNext[w].z = dx2VzN_dy2Vz1_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(nx,1,1<k<nz-1)
							pNext[w].x = dx2VxN_dy2Vx1_dz2Vx();
							pNext[w].y = dx2VyN_dy2Vy1_dz2Vy();
							pNext[w].z = dx2VzN_dy2Vz1_dz2Vz();
						}
					}
					else if (j == 0)
					{
						//y at the lower boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx,0,1)
							pNext[w].x = dx2VxN_dy2Vx0_dz2Vx1();
							pNext[w].y = dx2VyN_dy2Vy0_dz2Vy1();
							pNext[w].z = dx2VzN_dy2Vz0_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx,0,0)
							pNext[w].x = dx2VxN_dy2Vx0_dz2Vx0();
							pNext[w].y = dx2VyN_dy2Vy0_dz2Vy0();
							pNext[w].z = dx2VzN_dy2Vz0_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx,0,nz-1)
							pNext[w].x = dx2VxN_dy2Vx0_dz2VxN1();
							pNext[w].y = dx2VyN_dy2Vy0_dz2VyN1();
							pNext[w].z = dx2VzN_dy2Vz0_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx,0,nz)
							pNext[w].x = dx2VxN_dy2Vx0_dz2VxN();
							pNext[w].y = dx2VyN_dy2Vy0_dz2VyN();
							pNext[w].z = dx2VzN_dy2Vz0_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(nx,0,1<k<nz-1)
							pNext[w].x = dx2VxN_dy2Vx0_dz2Vx();
							pNext[w].y = dx2VyN_dy2Vy0_dz2Vy();
							pNext[w].z = dx2VzN_dy2Vz0_dz2Vz();
						}
					}
					else if (j == nyN1)
					{
						//y near the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx,ny-1,1)
							pNext[w].x = dx2VxN_dy2VxN1_dz2Vx1();
							pNext[w].y = dx2VyN_dy2VyN1_dz2Vy1();
							pNext[w].z = dx2VzN_dy2VzN1_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx,ny-1,0)
							pNext[w].x = dx2VxN_dy2VxN1_dz2Vx0();
							pNext[w].y = dx2VyN_dy2VyN1_dz2Vy0();
							pNext[w].z = dx2VzN_dy2VzN1_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx,ny-1,nz-1)
							pNext[w].x = dx2VxN_dy2VxN1_dz2VxN1();
							pNext[w].y = dx2VyN_dy2VyN1_dz2VyN1();
							pNext[w].z = dx2VzN_dy2VzN1_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx,ny-1,nz)
							pNext[w].x = dx2VxN_dy2VxN1_dz2VxN();
							pNext[w].y = dx2VyN_dy2VyN1_dz2VyN();
							pNext[w].z = dx2VzN_dy2VzN1_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(nx,ny-1,1<k<nz-1)
							pNext[w].x = dx2VxN_dy2VxN1_dz2Vx();
							pNext[w].y = dx2VyN_dy2VyN1_dz2Vy();
							pNext[w].z = dx2VzN_dy2VzN1_dz2Vz();
						}
					}
					else if (j == ny)
					{
						//y at the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx,ny,1)
							pNext[w].x = dx2VxN_dy2VxN_dz2Vx1();
							pNext[w].y = dx2VyN_dy2VyN_dz2Vy1();
							pNext[w].z = dx2VzN_dy2VzN_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx,ny,0)
							pNext[w].x = dx2VxN_dy2VxN_dz2Vx0();
							pNext[w].y = dx2VyN_dy2VyN_dz2Vy0();
							pNext[w].z = dx2VzN_dy2VzN_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx,ny,nz-1)
							pNext[w].x = dx2VxN_dy2VxN_dz2VxN1();
							pNext[w].y = dx2VyN_dy2VyN_dz2VyN1();
							pNext[w].z = dx2VzN_dy2VzN_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx,ny,nz)
							pNext[w].x = dx2VxN_dy2VxN_dz2VxN();
							pNext[w].y = dx2VyN_dy2VyN_dz2VyN();
							pNext[w].z = dx2VzN_dy2VzN_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(nx,ny,1<k<nz-1)
							pNext[w].x = dx2VxN_dy2VxN_dz2Vx();
							pNext[w].y = dx2VyN_dy2VyN_dz2Vy();
							pNext[w].z = dx2VzN_dy2VzN_dz2Vz();
						}
					}
					else
					{
						//y inside boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(nx,1<j<ny-1,1)
							pNext[w].x = dx2VxN_dy2Vx_dz2Vx1();
							pNext[w].y = dx2VyN_dy2Vy_dz2Vy1();
							pNext[w].z = dx2VzN_dy2Vz_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(nx,1<j<ny-1,0)
							pNext[w].x = dx2VxN_dy2Vx_dz2Vx0();
							pNext[w].y = dx2VyN_dy2Vy_dz2Vy0();
							pNext[w].z = dx2VzN_dy2Vz_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(nx,1<j<ny-1,nz-1)
							pNext[w].x = dx2VxN_dy2Vx_dz2VxN1();
							pNext[w].y = dx2VyN_dy2Vy_dz2VyN1();
							pNext[w].z = dx2VzN_dy2Vz_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(nx,1<j<ny-1,nz)
							pNext[w].x = dx2VxN_dy2Vx_dz2VxN();
							pNext[w].y = dx2VyN_dy2Vy_dz2VyN();
							pNext[w].z = dx2VzN_dy2Vz_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(nx,1<j<ny-1,1<k<nz-1)
							pNext[w].x = dx2VxN_dy2Vx_dz2Vx();
							pNext[w].y = dx2VyN_dy2Vy_dz2Vy();
							pNext[w].z = dx2VzN_dy2Vz_dz2Vz();
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
							pNext[w].x = dx2Vx_dy2Vx1_dz2Vx1();
							pNext[w].y = dx2Vy_dy2Vy1_dz2Vy1();
							pNext[w].z = dx2Vz_dy2Vz1_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1<i<nx-1,1,0)
							pNext[w].x = dx2Vx_dy2Vx1_dz2Vx0();
							pNext[w].y = dx2Vy_dy2Vy1_dz2Vy0();
							pNext[w].z = dx2Vz_dy2Vz1_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1<i<nx-1,1,nz-1)
							pNext[w].x = dx2Vx_dy2Vx1_dz2VxN1();
							pNext[w].y = dx2Vy_dy2Vy1_dz2VyN1();
							pNext[w].z = dx2Vz_dy2Vz1_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1<i<nx-1,1,nz)
							pNext[w].x = dx2Vx_dy2Vx1_dz2VxN();
							pNext[w].y = dx2Vy_dy2Vy1_dz2VyN();
							pNext[w].z = dx2Vz_dy2Vz1_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(1<i<nx-1,1,1<k<nz-1)
							pNext[w].x = dx2Vx_dy2Vx1_dz2Vx();
							pNext[w].y = dx2Vy_dy2Vy1_dz2Vy();
							pNext[w].z = dx2Vz_dy2Vz1_dz2Vz();
						}
					}
					else if (j == 0)
					{
						//y at the lower boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1<i<nx-1,0,1)
							pNext[w].x = dx2Vx_dy2Vx0_dz2Vx1();
							pNext[w].y = dx2Vy_dy2Vy0_dz2Vy1();
							pNext[w].z = dx2Vz_dy2Vz0_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1<i<nx-1,0,0)
							pNext[w].x = dx2Vx_dy2Vx0_dz2Vx0();
							pNext[w].y = dx2Vy_dy2Vy0_dz2Vy0();
							pNext[w].z = dx2Vz_dy2Vz0_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1<i<nx-1,0,nz-1)
							pNext[w].x = dx2Vx_dy2Vx0_dz2VxN1();
							pNext[w].y = dx2Vy_dy2Vy0_dz2VyN1();
							pNext[w].z = dx2Vz_dy2Vz0_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1<i<nx-1,0,nz)
							pNext[w].x = dx2Vx_dy2Vx0_dz2VxN();
							pNext[w].y = dx2Vy_dy2Vy0_dz2VyN();
							pNext[w].z = dx2Vz_dy2Vz0_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(1<i<nx-1,0,1<k<nz-1)
							pNext[w].x = dx2Vx_dy2Vx0_dz2Vx();
							pNext[w].y = dx2Vy_dy2Vy0_dz2Vy();
							pNext[w].z = dx2Vz_dy2Vz0_dz2Vz();
						}
					}
					else if (j == nyN1)
					{
						//y near the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1<i<nx-1,ny-1,1)
							pNext[w].x = dx2Vx_dy2VxN1_dz2Vx1();
							pNext[w].y = dx2Vy_dy2VyN1_dz2Vy1();
							pNext[w].z = dx2Vz_dy2VzN1_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1<i<nx-1,ny-1,0)
							pNext[w].x = dx2Vx_dy2VxN1_dz2Vx0();
							pNext[w].y = dx2Vy_dy2VyN1_dz2Vy0();
							pNext[w].z = dx2Vz_dy2VzN1_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1<i<nx-1,ny-1,nz-1)
							pNext[w].x = dx2Vx_dy2VxN1_dz2VxN1();
							pNext[w].y = dx2Vy_dy2VyN1_dz2VyN1();
							pNext[w].z = dx2Vz_dy2VzN1_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1<i<nx-1,ny-1,nz)
							pNext[w].x = dx2Vx_dy2VxN1_dz2VxN();
							pNext[w].y = dx2Vy_dy2VyN1_dz2VyN();
							pNext[w].z = dx2Vz_dy2VzN1_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(1<i<nx-1,ny-1,1<k<nz-1)
							pNext[w].x = dx2Vx_dy2VxN1_dz2Vx();
							pNext[w].y = dx2Vy_dy2VyN1_dz2Vy();
							pNext[w].z = dx2Vz_dy2VzN1_dz2Vz();
						}
					}
					else if (j == ny)
					{
						//y at the upper boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1<i<nx-1,ny,1)
							pNext[w].x = dx2Vx_dy2VxN_dz2Vx1();
							pNext[w].y = dx2Vy_dy2VyN_dz2Vy1();
							pNext[w].z = dx2Vz_dy2VzN_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1<i<nx-1,ny,0)
							pNext[w].x = dx2Vx_dy2VxN_dz2Vx0();
							pNext[w].y = dx2Vy_dy2VyN_dz2Vy0();
							pNext[w].z = dx2Vz_dy2VzN_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1<i<nx-1,ny,nz-1)
							pNext[w].x = dx2Vx_dy2VxN_dz2VxN1();
							pNext[w].y = dx2Vy_dy2VyN_dz2VyN1();
							pNext[w].z = dx2Vz_dy2VzN_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1<i<nx-1,ny,nz)
							pNext[w].x = dx2Vx_dy2VxN_dz2VxN();
							pNext[w].y = dx2Vy_dy2VyN_dz2VyN();
							pNext[w].z = dx2Vz_dy2VzN_dz2VzN();
						}
						else
						{
							//z inside the boundary
							//(1<i<nx-1,ny,1<k<nz-1)
							pNext[w].x = dx2Vx_dy2VxN_dz2Vx();
							pNext[w].y = dx2Vy_dy2VyN_dz2Vy();
							pNext[w].z = dx2Vz_dy2VzN_dz2Vz();
						}
					}
					else
					{
						//y inside boundary
						if (k == 1)
						{
							//z near the lower boundary
							//(1<i<nx-1,1<j<ny-1,1)
							pNext[w].x = dx2Vx_dy2Vx_dz2Vx1();
							pNext[w].y = dx2Vy_dy2Vy_dz2Vy1();
							pNext[w].z = dx2Vz_dy2Vz_dz2Vz1();
						}
						else if (k == 0)
						{
							//z at the lower boundary
							//(1<i<nx-1,1<j<ny-1,0)
							pNext[w].x = dx2Vx_dy2Vx_dz2Vx0();
							pNext[w].y = dx2Vy_dy2Vy_dz2Vy0();
							pNext[w].z = dx2Vz_dy2Vz_dz2Vz0();
						}
						else if (k == nzN1)
						{
							//z near the upper boundary
							//(1<i<nx-1,1<j<ny-1,nz-1)
							pNext[w].x = dx2Vx_dy2Vx_dz2VxN1();
							pNext[w].y = dx2Vy_dy2Vy_dz2VyN1();
							pNext[w].z = dx2Vz_dy2Vz_dz2VzN1();
						}
						else if (k == nz)
						{
							//z at the upper boundary
							//(1<i<nx-1,1<j<ny-1,nz)
							pNext[w].x = dx2Vx_dy2Vx_dz2VxN();
							pNext[w].y = dx2Vy_dy2Vy_dz2VyN();
							pNext[w].z = dx2Vz_dy2Vz_dz2VzN();
						}
						else
						{
							//(x,y,z) inside boundary
							pNext[w].x = dx2Vx_dy2Vx_dz2Vx();
							pNext[w].y = dx2Vy_dy2Vy_dz2Vy();
							pNext[w].z = dx2Vz_dy2Vz_dz2Vz();
						}
					}
				}
				//
				pNext[w].x = -pNext[w].x / 12;
				pNext[w].y = -pNext[w].y / 12;
				pNext[w].z = -pNext[w].z / 12;
				//
				w++;
			}
		}
	}
	return ret;
}

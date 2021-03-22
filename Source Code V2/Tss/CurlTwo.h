#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

calculate the second order curl
********************************************************************/
#include "Tss.h"

class CurlTwo
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
	CurlTwo();
	~CurlTwo();
	void setSpaceRange(unsigned int nx0, unsigned int ny0, unsigned int nz0);
	size_t I(unsigned int i, unsigned int j, unsigned int k){ return k + nz1*(j + ny1*i); }
	//second order
	int _getNextCurlUseFastMethod(Point3Dstruct *pField, Point3Dstruct *pNext);
	//test
	double Dx2Vx(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double Dy2Vy(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double Dz2Vz(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0);
	//
	double Dy2Vx(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double Dz2Vy(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double Dx2Vz(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0);
	//
	double Dz2Vx(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double Dx2Vy(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double Dy2Vz(Point3Dstruct *f, unsigned int i0, unsigned int j0, unsigned int k0);
	//
	//inside boundary
	double dx2Vx();
	double dy2Vx();
	double dz2Vx();
	//
	double dx2Vy();
	double dy2Vy();
	double dz2Vy();
	//
	double dx2Vz();
	double dy2Vz();
	double dz2Vz();
	//near the lower boundary
	double dx2Vx1();
	double dy2Vx1();
	double dz2Vx1();
	//
	double dx2Vy1();
	double dy2Vy1();
	double dz2Vy1();
	//
	double dx2Vz1();
	double dy2Vz1();
	double dz2Vz1();
	//at the lower boundary
	double dx2Vx0();
	double dy2Vx0();
	double dz2Vx0();
	//
	double dx2Vy0();
	double dy2Vy0();
	double dz2Vy0();
	//
	double dx2Vz0();
	double dy2Vz0();
	double dz2Vz0();

	//near the upper boundary
	double dx2VxN1();
	double dy2VxN1();
	double dz2VxN1();
	//
	double dx2VyN1();
	double dy2VyN1();
	double dz2VyN1();
	//
	double dx2VzN1();
	double dy2VzN1();
	double dz2VzN1();

	//at the upper boundary
	double dx2VxN();
	double dy2VxN();
	double dz2VxN();
	//
	double dx2VyN();
	double dy2VyN();
	double dz2VyN();
	//
	double dx2VzN();
	double dy2VzN();
	double dz2VzN();

	//curls
	// Cx=dx2Vx + dy2Vx + dz2Vx============Cx=dx2Vx + dy2Vx + dz2Vx=======================
	//
	//(0,0,0)
	double dx2Vx0_dy2Vx0_dz2Vx0();
	//(0,0,1)
	double dx2Vx0_dy2Vx0_dz2Vx1();
	//(0,0,N1)
	double dx2Vx0_dy2Vx0_dz2VxN1();
	//(0,0,N)
	double dx2Vx0_dy2Vx0_dz2VxN();
	//(0,0,1<k<nz-1)
	double dx2Vx0_dy2Vx0_dz2Vx();
	//
	//(0,1,0)
	double dx2Vx0_dy2Vx1_dz2Vx0();
	//(0,1,1)
	double dx2Vx0_dy2Vx1_dz2Vx1();
	//(0,1,N1)
	double dx2Vx0_dy2Vx1_dz2VxN1();
	//(0,1,N)
	double dx2Vx0_dy2Vx1_dz2VxN();
	//(0,1,1<k<nz-1)
	double dx2Vx0_dy2Vx1_dz2Vx();
	//
	//(0,N1,0)
	double dx2Vx0_dy2VxN1_dz2Vx0();
	//(0,N1,1)
	double dx2Vx0_dy2VxN1_dz2Vx1();
	//(0,N1,N1)
	double dx2Vx0_dy2VxN1_dz2VxN1();
	//(0,N1,N)
	double dx2Vx0_dy2VxN1_dz2VxN();
	//(0,N1,1<k<nz-1)
	double dx2Vx0_dy2VxN1_dz2Vx();
	//
	//(0,N,0)
	double dx2Vx0_dy2VxN_dz2Vx0();
	//(0,N,1)
	double dx2Vx0_dy2VxN_dz2Vx1();
	//(0,N,N1)
	double dx2Vx0_dy2VxN_dz2VxN1();
	//(0,N,N)
	double dx2Vx0_dy2VxN_dz2VxN();
	//(0,N,1<k<nz-1)
	double dx2Vx0_dy2VxN_dz2Vx();
	//
	//(0,1<j<ny-1,0)
	double dx2Vx0_dy2Vx_dz2Vx0();
	//(0,1<j<ny-1,1)
	double dx2Vx0_dy2Vx_dz2Vx1();
	//(0,1<j<ny-1,N1)
	double dx2Vx0_dy2Vx_dz2VxN1();
	//(0,1<j<ny-1,N)
	double dx2Vx0_dy2Vx_dz2VxN();
	//(0,1<j<ny-1,1<k<nz-1)
	double dx2Vx0_dy2Vx_dz2Vx();
	//------------------------------------------
	//
	//(1,0,0)
	double dx2Vx1_dy2Vx0_dz2Vx0();
	//(1,0,1)
	double dx2Vx1_dy2Vx0_dz2Vx1();
	//(1,0,N1)
	double dx2Vx1_dy2Vx0_dz2VxN1();
	//(1,0,N)
	double dx2Vx1_dy2Vx0_dz2VxN();
	//(1,0,1<k<nz-1)
	double dx2Vx1_dy2Vx0_dz2Vx();
	//
	//(1,1,0)
	double dx2Vx1_dy2Vx1_dz2Vx0();
	//(1,1,1)
	double dx2Vx1_dy2Vx1_dz2Vx1();
	//(1,1,N1)
	double dx2Vx1_dy2Vx1_dz2VxN1();
	//(1,1,N)
	double dx2Vx1_dy2Vx1_dz2VxN();
	//(1,1,1<k<nz-1)
	double dx2Vx1_dy2Vx1_dz2Vx();
	//
	//(1,N1,0)
	double dx2Vx1_dy2VxN1_dz2Vx0();
	//(1,N1,1)
	double dx2Vx1_dy2VxN1_dz2Vx1();
	//(1,N1,N1)
	double dx2Vx1_dy2VxN1_dz2VxN1();
	//(1,N1,N)
	double dx2Vx1_dy2VxN1_dz2VxN();
	//(1,N1,1<k<nz-1)
	double dx2Vx1_dy2VxN1_dz2Vx();
	//
	//(1,N,0)
	double dx2Vx1_dy2VxN_dz2Vx0();
	//(1,N,1)
	double dx2Vx1_dy2VxN_dz2Vx1();
	//(1,N,N1)
	double dx2Vx1_dy2VxN_dz2VxN1();
	//(1,N,N)
	double dx2Vx1_dy2VxN_dz2VxN();
	//(1,N,1<k<nz-1)
	double dx2Vx1_dy2VxN_dz2Vx();
	//
	//(1,1<j<ny-1,0)
	double dx2Vx1_dy2Vx_dz2Vx0();
	//(1,1<j<ny-1,1)
	double dx2Vx1_dy2Vx_dz2Vx1();
	//(1,1<j<ny-1,N1)
	double dx2Vx1_dy2Vx_dz2VxN1();
	//(1,1<j<ny-1,N)
	double dx2Vx1_dy2Vx_dz2VxN();
	//(1,1<j<ny-1,1<k<nz-1)
	double dx2Vx1_dy2Vx_dz2Vx();
	//--end of i=1-----------------------------------------
	//
	//---start of i=nx-1-----------------------------------
	//(nx-1,0,0)
	double dx2VxN1_dy2Vx0_dz2Vx0();
	//(nx-1,0,1)
	double dx2VxN1_dy2Vx0_dz2Vx1();
	//(nx-1,0,N1)
	double dx2VxN1_dy2Vx0_dz2VxN1();
	//(nx-1,0,N)
	double dx2VxN1_dy2Vx0_dz2VxN();
	//(nx-1,0,1<k<nz-1)
	double dx2VxN1_dy2Vx0_dz2Vx();
	//
	//(nx-1,1,0)
	double dx2VxN1_dy2Vx1_dz2Vx0();
	//(nx-1,1,1)
	double dx2VxN1_dy2Vx1_dz2Vx1();
	//(nx-1,1,N1)
	double dx2VxN1_dy2Vx1_dz2VxN1();
	//(nx-1,1,N)
	double dx2VxN1_dy2Vx1_dz2VxN();
	//(nx-1,1,1<k<nz-1)
	double dx2VxN1_dy2Vx1_dz2Vx();
	//
	//(nx-1,ny-1,0)
	double dx2VxN1_dy2VxN1_dz2Vx0();
	//(nx-1,N1,1)
	double dx2VxN1_dy2VxN1_dz2Vx1();
	//(nx-1,N1,N1)
	double dx2VxN1_dy2VxN1_dz2VxN1();
	//(nx-1,N1,N)
	double dx2VxN1_dy2VxN1_dz2VxN();
	//(nx-1,N1,1<k<nz-1)
	double dx2VxN1_dy2VxN1_dz2Vx();
	//
	//(nx-1,N,0)
	double dx2VxN1_dy2VxN_dz2Vx0();
	//(nx-1,N,1)
	double dx2VxN1_dy2VxN_dz2Vx1();
	//(nx-1,N,N1)
	double dx2VxN1_dy2VxN_dz2VxN1();
	//(nx-1,N,N)
	double dx2VxN1_dy2VxN_dz2VxN();
	//(nx-1,N,1<k<nz-1)
	double dx2VxN1_dy2VxN_dz2Vx();
	//
	//(nx-1,1<j<ny-1,0)
	double dx2VxN1_dy2Vx_dz2Vx0();
	//(nx-1,1<j<ny-1,1)
	double dx2VxN1_dy2Vx_dz2Vx1();
	//(nx-1,1<j<ny-1,N1)
	double dx2VxN1_dy2Vx_dz2VxN1();
	//(nx-1,1<j<ny-1,N)
	double dx2VxN1_dy2Vx_dz2VxN();
	//(nx-1,1<j<ny-1,1<k<nz-1)
	double dx2VxN1_dy2Vx_dz2Vx();
	//-end of i=N1------------------------------------------
	//
	//-start of i=N-----------------------------------------
	//(nx,0,0)
	double dx2VxN_dy2Vx0_dz2Vx0();
	//(nx,0,1)
	double dx2VxN_dy2Vx0_dz2Vx1();
	//(nx,0,N1)
	double dx2VxN_dy2Vx0_dz2VxN1();
	//(nx,0,N)
	double dx2VxN_dy2Vx0_dz2VxN();
	//(nx,0,1<k<nz-1)
	double dx2VxN_dy2Vx0_dz2Vx();
	//
	//(nx,1,0)
	double dx2VxN_dy2Vx1_dz2Vx0();
	//(nx,1,1)
	double dx2VxN_dy2Vx1_dz2Vx1();
	//(nx,1,N1)
	double dx2VxN_dy2Vx1_dz2VxN1();
	//(nx,1,N)
	double dx2VxN_dy2Vx1_dz2VxN();
	//(nx,1,1<k<nz-1)
	double dx2VxN_dy2Vx1_dz2Vx();
	//
	//(nx,ny-1,0)
	double dx2VxN_dy2VxN1_dz2Vx0();
	//(nx,N1,1)
	double dx2VxN_dy2VxN1_dz2Vx1();
	//(nx,N1,N1)
	double dx2VxN_dy2VxN1_dz2VxN1();
	//(nx,N1,N)
	double dx2VxN_dy2VxN1_dz2VxN();
	//(nx,N1,1<k<nz-1)
	double dx2VxN_dy2VxN1_dz2Vx();
	//
	//(nx,N,0)
	double dx2VxN_dy2VxN_dz2Vx0();
	//(nx,N,1)
	double dx2VxN_dy2VxN_dz2Vx1();
	//(nx,N,N1)
	double dx2VxN_dy2VxN_dz2VxN1();
	//(nx,N,N)
	double dx2VxN_dy2VxN_dz2VxN();
	//(nx,N,1<k<nz-1)
	double dx2VxN_dy2VxN_dz2Vx();
	//
	//(nx,1<j<ny-1,0)
	double dx2VxN_dy2Vx_dz2Vx0();
	//(nx,1<j<ny-1,1)
	double dx2VxN_dy2Vx_dz2Vx1();
	//(nx,1<j<ny-1,N1)
	double dx2VxN_dy2Vx_dz2VxN1();
	//(nx,1<j<ny-1,N)
	double dx2VxN_dy2Vx_dz2VxN();
	//(nx,1<j<ny-1,1<k<nz-1)
	double dx2VxN_dy2Vx_dz2Vx();
	//-end of i=nx==============================
	//
	//==start of 1<i<nx-1========================
	//(1<i<nx-1,0,0)
	double dx2Vx_dy2Vx0_dz2Vx0();
	//(1<i<nx-1,0,1)
	double dx2Vx_dy2Vx0_dz2Vx1();
	//(1<i<nx-1,0,N1)
	double dx2Vx_dy2Vx0_dz2VxN1();
	//(1<i<nx-1,0,N)
	double dx2Vx_dy2Vx0_dz2VxN();
	//(1<i<nx-1,0,1<k<nz-1)
	double dx2Vx_dy2Vx0_dz2Vx();
	//
	//(1<i<nx-1,1,0)
	double dx2Vx_dy2Vx1_dz2Vx0();
	//(1<i<nx-1,1,1)
	double dx2Vx_dy2Vx1_dz2Vx1();
	//(1<i<nx-1,1,N1)
	double dx2Vx_dy2Vx1_dz2VxN1();
	//(1<i<nx-1,1,N)
	double dx2Vx_dy2Vx1_dz2VxN();
	//(1<i<nx-1,1,1<k<nz-1)
	double dx2Vx_dy2Vx1_dz2Vx();
	//
	//(1<i<nx-1,ny-1,0)
	double dx2Vx_dy2VxN1_dz2Vx0();
	//(1<i<nx-1,N1,1)
	double dx2Vx_dy2VxN1_dz2Vx1();
	//(1<i<nx-1,N1,N1)
	double dx2Vx_dy2VxN1_dz2VxN1();
	//(1<i<nx-1,N1,N)
	double dx2Vx_dy2VxN1_dz2VxN();
	//(1<i<nx-1,N1,1<k<nz-1)
	double dx2Vx_dy2VxN1_dz2Vx();
	//
	//(1<i<nx-1,N,0)
	double dx2Vx_dy2VxN_dz2Vx0();
	//(1<i<nx-1,N,1)
	double dx2Vx_dy2VxN_dz2Vx1();
	//(1<i<nx-1,N,N1)
	double dx2Vx_dy2VxN_dz2VxN1();
	//(1<i<nx-1,N,N)
	double dx2Vx_dy2VxN_dz2VxN();
	//(1<i<nx-1,N,1<k<nz-1)
	double dx2Vx_dy2VxN_dz2Vx();
	//
	//(1<i<nx-1,1<j<ny-1,0)
	double dx2Vx_dy2Vx_dz2Vx0();
	//(1<i<nx-1,1<j<ny-1,1)
	double dx2Vx_dy2Vx_dz2Vx1();
	//(1<i<nx-1,1<j<ny-1,N1)
	double dx2Vx_dy2Vx_dz2VxN1();
	//(1<i<nx-1,1<j<ny-1,N)
	double dx2Vx_dy2Vx_dz2VxN();
	//(1<i<nx-1,1<j<ny-1,1<k<nz-1)
	double dx2Vx_dy2Vx_dz2Vx();
	//==end of 1<i<nx-1========================
	// Cx=dx2Vx + dy2Vx + dz2Vx============Cx=dx2Vx + dy2Vx + dz2Vx=======================
	// Cy=dx2Vy + dy2Vy + dz2Vy============Cy=dx2Vy + dy2Vy + dz2Vy=======================
	//
	//(0,0,0)
	double dx2Vy0_dy2Vy0_dz2Vy0();
	//(0,0,1)
	double dx2Vy0_dy2Vy0_dz2Vy1();
	//(0,0,N1)
	double dx2Vy0_dy2Vy0_dz2VyN1();
	//(0,0,N)
	double dx2Vy0_dy2Vy0_dz2VyN();
	//(0,0,1<k<nz-1)
	double dx2Vy0_dy2Vy0_dz2Vy();
	//
	//(0,1,0)
	double dx2Vy0_dy2Vy1_dz2Vy0();
	//(0,1,1)
	double dx2Vy0_dy2Vy1_dz2Vy1();
	//(0,1,N1)
	double dx2Vy0_dy2Vy1_dz2VyN1();
	//(0,1,N)
	double dx2Vy0_dy2Vy1_dz2VyN();
	//(0,1,1<k<nz-1)
	double dx2Vy0_dy2Vy1_dz2Vy();
	//
	//(0,N1,0)
	double dx2Vy0_dy2VyN1_dz2Vy0();
	//(0,N1,1)
	double dx2Vy0_dy2VyN1_dz2Vy1();
	//(0,N1,N1)
	double dx2Vy0_dy2VyN1_dz2VyN1();
	//(0,N1,N)
	double dx2Vy0_dy2VyN1_dz2VyN();
	//(0,N1,1<k<nz-1)
	double dx2Vy0_dy2VyN1_dz2Vy();
	//
	//(0,N,0)
	double dx2Vy0_dy2VyN_dz2Vy0();
	//(0,N,1)
	double dx2Vy0_dy2VyN_dz2Vy1();
	//(0,N,N1)
	double dx2Vy0_dy2VyN_dz2VyN1();
	//(0,N,N)
	double dx2Vy0_dy2VyN_dz2VyN();
	//(0,N,1<k<nz-1)
	double dx2Vy0_dy2VyN_dz2Vy();
	//
	//(0,1<j<ny-1,0)
	double dx2Vy0_dy2Vy_dz2Vy0();
	//(0,1<j<ny-1,1)
	double dx2Vy0_dy2Vy_dz2Vy1();
	//(0,1<j<ny-1,N1)
	double dx2Vy0_dy2Vy_dz2VyN1();
	//(0,1<j<ny-1,N)
	double dx2Vy0_dy2Vy_dz2VyN();
	//(0,1<j<ny-1,1<k<nz-1)
	double dx2Vy0_dy2Vy_dz2Vy();
	//------------------------------------------
	//
	//(1,0,0)
	double dx2Vy1_dy2Vy0_dz2Vy0();
	//(1,0,1)
	double dx2Vy1_dy2Vy0_dz2Vy1();
	//(1,0,N1)
	double dx2Vy1_dy2Vy0_dz2VyN1();
	//(1,0,N)
	double dx2Vy1_dy2Vy0_dz2VyN();
	//(1,0,1<k<nz-1)
	double dx2Vy1_dy2Vy0_dz2Vy();
	//
	//(1,1,0)
	double dx2Vy1_dy2Vy1_dz2Vy0();
	//(1,1,1)
	double dx2Vy1_dy2Vy1_dz2Vy1();
	//(1,1,N1)
	double dx2Vy1_dy2Vy1_dz2VyN1();
	//(1,1,N)
	double dx2Vy1_dy2Vy1_dz2VyN();
	//(1,1,1<k<nz-1)
	double dx2Vy1_dy2Vy1_dz2Vy();
	//
	//(1,N1,0)
	double dx2Vy1_dy2VyN1_dz2Vy0();
	//(1,N1,1)
	double dx2Vy1_dy2VyN1_dz2Vy1();
	//(1,N1,N1)
	double dx2Vy1_dy2VyN1_dz2VyN1();
	//(1,N1,N)
	double dx2Vy1_dy2VyN1_dz2VyN();
	//(1,N1,1<k<nz-1)
	double dx2Vy1_dy2VyN1_dz2Vy();
	//
	//(1,N,0)
	double dx2Vy1_dy2VyN_dz2Vy0();
	//(1,N,1)
	double dx2Vy1_dy2VyN_dz2Vy1();
	//(1,N,N1)
	double dx2Vy1_dy2VyN_dz2VyN1();
	//(1,N,N)
	double dx2Vy1_dy2VyN_dz2VyN();
	//(1,N,1<k<nz-1)
	double dx2Vy1_dy2VyN_dz2Vy();
	//
	//(1,1<j<ny-1,0)
	double dx2Vy1_dy2Vy_dz2Vy0();
	//(1,1<j<ny-1,1)
	double dx2Vy1_dy2Vy_dz2Vy1();
	//(1,1<j<ny-1,N1)
	double dx2Vy1_dy2Vy_dz2VyN1();
	//(1,1<j<ny-1,N)
	double dx2Vy1_dy2Vy_dz2VyN();
	//(1,1<j<ny-1,1<k<nz-1)
	double dx2Vy1_dy2Vy_dz2Vy();
	//--end of i=1-----------------------------------------
	//
	//---start of i=nx-1-----------------------------------
	//(nx-1,0,0)
	double dx2VyN1_dy2Vy0_dz2Vy0();
	//(nx-1,0,1)
	double dx2VyN1_dy2Vy0_dz2Vy1();
	//(nx-1,0,N1)
	double dx2VyN1_dy2Vy0_dz2VyN1();
	//(nx-1,0,N)
	double dx2VyN1_dy2Vy0_dz2VyN();
	//(nx-1,0,1<k<nz-1)
	double dx2VyN1_dy2Vy0_dz2Vy();
	//
	//(nx-1,1,0)
	double dx2VyN1_dy2Vy1_dz2Vy0();
	//(nx-1,1,1)
	double dx2VyN1_dy2Vy1_dz2Vy1();
	//(nx-1,1,N1)
	double dx2VyN1_dy2Vy1_dz2VyN1();
	//(nx-1,1,N)
	double dx2VyN1_dy2Vy1_dz2VyN();
	//(nx-1,1,1<k<nz-1)
	double dx2VyN1_dy2Vy1_dz2Vy();
	//
	//(nx-1,ny-1,0)
	double dx2VyN1_dy2VyN1_dz2Vy0();
	//(nx-1,N1,1)
	double dx2VyN1_dy2VyN1_dz2Vy1();
	//(nx-1,N1,N1)
	double dx2VyN1_dy2VyN1_dz2VyN1();
	//(nx-1,N1,N)
	double dx2VyN1_dy2VyN1_dz2VyN();
	//(nx-1,N1,1<k<nz-1)
	double dx2VyN1_dy2VyN1_dz2Vy();
	//
	//(nx-1,N,0)
	double dx2VyN1_dy2VyN_dz2Vy0();
	//(nx-1,N,1)
	double dx2VyN1_dy2VyN_dz2Vy1();
	//(nx-1,N,N1)
	double dx2VyN1_dy2VyN_dz2VyN1();
	//(nx-1,N,N)
	double dx2VyN1_dy2VyN_dz2VyN();
	//(nx-1,N,1<k<nz-1)
	double dx2VyN1_dy2VyN_dz2Vy();
	//
	//(nx-1,1<j<ny-1,0)
	double dx2VyN1_dy2Vy_dz2Vy0();
	//(nx-1,1<j<ny-1,1)
	double dx2VyN1_dy2Vy_dz2Vy1();
	//(nx-1,1<j<ny-1,N1)
	double dx2VyN1_dy2Vy_dz2VyN1();
	//(nx-1,1<j<ny-1,N)
	double dx2VyN1_dy2Vy_dz2VyN();
	//(nx-1,1<j<ny-1,1<k<nz-1)
	double dx2VyN1_dy2Vy_dz2Vy();
	//-end of i=N1------------------------------------------
	//
	//-start of i=N-----------------------------------------
	//(nx,0,0)
	double dx2VyN_dy2Vy0_dz2Vy0();
	//(nx,0,1)
	double dx2VyN_dy2Vy0_dz2Vy1();
	//(nx,0,N1)
	double dx2VyN_dy2Vy0_dz2VyN1();
	//(nx,0,N)
	double dx2VyN_dy2Vy0_dz2VyN();
	//(nx,0,1<k<nz-1)
	double dx2VyN_dy2Vy0_dz2Vy();
	//
	//(nx,1,0)
	double dx2VyN_dy2Vy1_dz2Vy0();
	//(nx,1,1)
	double dx2VyN_dy2Vy1_dz2Vy1();
	//(nx,1,N1)
	double dx2VyN_dy2Vy1_dz2VyN1();
	//(nx,1,N)
	double dx2VyN_dy2Vy1_dz2VyN();
	//(nx,1,1<k<nz-1)
	double dx2VyN_dy2Vy1_dz2Vy();
	//
	//(nx,ny-1,0)
	double dx2VyN_dy2VyN1_dz2Vy0();
	//(nx,N1,1)
	double dx2VyN_dy2VyN1_dz2Vy1();
	//(nx,N1,N1)
	double dx2VyN_dy2VyN1_dz2VyN1();
	//(nx,N1,N)
	double dx2VyN_dy2VyN1_dz2VyN();
	//(nx,N1,1<k<nz-1)
	double dx2VyN_dy2VyN1_dz2Vy();
	//
	//(nx,N,0)
	double dx2VyN_dy2VyN_dz2Vy0();
	//(nx,N,1)
	double dx2VyN_dy2VyN_dz2Vy1();
	//(nx,N,N1)
	double dx2VyN_dy2VyN_dz2VyN1();
	//(nx,N,N)
	double dx2VyN_dy2VyN_dz2VyN();
	//(nx,N,1<k<nz-1)
	double dx2VyN_dy2VyN_dz2Vy();
	//
	//(nx,1<j<ny-1,0)
	double dx2VyN_dy2Vy_dz2Vy0();
	//(nx,1<j<ny-1,1)
	double dx2VyN_dy2Vy_dz2Vy1();
	//(nx,1<j<ny-1,N1)
	double dx2VyN_dy2Vy_dz2VyN1();
	//(nx,1<j<ny-1,N)
	double dx2VyN_dy2Vy_dz2VyN();
	//(nx,1<j<ny-1,1<k<nz-1)
	double dx2VyN_dy2Vy_dz2Vy();
	//-end of i=nx==============================
	//
	//==start of 1<i<nx-1========================
	//(1<i<nx-1,0,0)
	double dx2Vy_dy2Vy0_dz2Vy0();
	//(1<i<nx-1,0,1)
	double dx2Vy_dy2Vy0_dz2Vy1();
	//(1<i<nx-1,0,N1)
	double dx2Vy_dy2Vy0_dz2VyN1();
	//(1<i<nx-1,0,N)
	double dx2Vy_dy2Vy0_dz2VyN();
	//(1<i<nx-1,0,1<k<nz-1)
	double dx2Vy_dy2Vy0_dz2Vy();
	//
	//(1<i<nx-1,1,0)
	double dx2Vy_dy2Vy1_dz2Vy0();
	//(1<i<nx-1,1,1)
	double dx2Vy_dy2Vy1_dz2Vy1();
	//(1<i<nx-1,1,N1)
	double dx2Vy_dy2Vy1_dz2VyN1();
	//(1<i<nx-1,1,N)
	double dx2Vy_dy2Vy1_dz2VyN();
	//(1<i<nx-1,1,1<k<nz-1)
	double dx2Vy_dy2Vy1_dz2Vy();
	//
	//(1<i<nx-1,ny-1,0)
	double dx2Vy_dy2VyN1_dz2Vy0();
	//(1<i<nx-1,N1,1)
	double dx2Vy_dy2VyN1_dz2Vy1();
	//(1<i<nx-1,N1,N1)
	double dx2Vy_dy2VyN1_dz2VyN1();
	//(1<i<nx-1,N1,N)
	double dx2Vy_dy2VyN1_dz2VyN();
	//(1<i<nx-1,N1,1<k<nz-1)
	double dx2Vy_dy2VyN1_dz2Vy();
	//
	//(1<i<nx-1,N,0)
	double dx2Vy_dy2VyN_dz2Vy0();
	//(1<i<nx-1,N,1)
	double dx2Vy_dy2VyN_dz2Vy1();
	//(1<i<nx-1,N,N1)
	double dx2Vy_dy2VyN_dz2VyN1();
	//(1<i<nx-1,N,N)
	double dx2Vy_dy2VyN_dz2VyN();
	//(1<i<nx-1,N,1<k<nz-1)
	double dx2Vy_dy2VyN_dz2Vy();
	//
	//(1<i<nx-1,1<j<ny-1,0)
	double dx2Vy_dy2Vy_dz2Vy0();
	//(1<i<nx-1,1<j<ny-1,1)
	double dx2Vy_dy2Vy_dz2Vy1();
	//(1<i<nx-1,1<j<ny-1,N1)
	double dx2Vy_dy2Vy_dz2VyN1();
	//(1<i<nx-1,1<j<ny-1,N)
	double dx2Vy_dy2Vy_dz2VyN();
	//(1<i<nx-1,1<j<ny-1,1<k<nz-1)
	double dx2Vy_dy2Vy_dz2Vy();
	//==end of 1<i<nx-1========================
	//end of Cy=dx2Vy + dy2Vy + dz2Vy============end of Cy=dx2Vy + dy2Vy + dz2Vy=======================
	//
	// Cz=dx2Vz + dy2Vz + dz2Vz============Cz=dx2Vz + dy2Vz + dz2Vz=======================
	//
	//(0,0,0)
	double dx2Vz0_dy2Vz0_dz2Vz0();
	//(0,0,1)
	double dx2Vz0_dy2Vz0_dz2Vz1();
	//(0,0,N1)
	double dx2Vz0_dy2Vz0_dz2VzN1();
	//(0,0,N)
	double dx2Vz0_dy2Vz0_dz2VzN();
	//(0,0,1<k<nz-1)
	double dx2Vz0_dy2Vz0_dz2Vz();
	//
	//(0,1,0)
	double dx2Vz0_dy2Vz1_dz2Vz0();
	//(0,1,1)
	double dx2Vz0_dy2Vz1_dz2Vz1();
	//(0,1,N1)
	double dx2Vz0_dy2Vz1_dz2VzN1();
	//(0,1,N)
	double dx2Vz0_dy2Vz1_dz2VzN();
	//(0,1,1<k<nz-1)
	double dx2Vz0_dy2Vz1_dz2Vz();
	//
	//(0,N1,0)
	double dx2Vz0_dy2VzN1_dz2Vz0();
	//(0,N1,1)
	double dx2Vz0_dy2VzN1_dz2Vz1();
	//(0,N1,N1)
	double dx2Vz0_dy2VzN1_dz2VzN1();
	//(0,N1,N)
	double dx2Vz0_dy2VzN1_dz2VzN();
	//(0,N1,1<k<nz-1)
	double dx2Vz0_dy2VzN1_dz2Vz();
	//
	//(0,N,0)
	double dx2Vz0_dy2VzN_dz2Vz0();
	//(0,N,1)
	double dx2Vz0_dy2VzN_dz2Vz1();
	//(0,N,N1)
	double dx2Vz0_dy2VzN_dz2VzN1();
	//(0,N,N)
	double dx2Vz0_dy2VzN_dz2VzN();
	//(0,N,1<k<nz-1)
	double dx2Vz0_dy2VzN_dz2Vz();
	//
	//(0,1<j<ny-1,0)
	double dx2Vz0_dy2Vz_dz2Vz0();
	//(0,1<j<ny-1,1)
	double dx2Vz0_dy2Vz_dz2Vz1();
	//(0,1<j<ny-1,N1)
	double dx2Vz0_dy2Vz_dz2VzN1();
	//(0,1<j<ny-1,N)
	double dx2Vz0_dy2Vz_dz2VzN();
	//(0,1<j<ny-1,1<k<nz-1)
	double dx2Vz0_dy2Vz_dz2Vz();
	//------------------------------------------
	//
	//(1,0,0)
	double dx2Vz1_dy2Vz0_dz2Vz0();
	//(1,0,1)
	double dx2Vz1_dy2Vz0_dz2Vz1();
	//(1,0,N1)
	double dx2Vz1_dy2Vz0_dz2VzN1();
	//(1,0,N)
	double dx2Vz1_dy2Vz0_dz2VzN();
	//(1,0,1<k<nz-1)
	double dx2Vz1_dy2Vz0_dz2Vz();
	//
	//(1,1,0)
	double dx2Vz1_dy2Vz1_dz2Vz0();
	//(1,1,1)
	double dx2Vz1_dy2Vz1_dz2Vz1();
	//(1,1,N1)
	double dx2Vz1_dy2Vz1_dz2VzN1();
	//(1,1,N)
	double dx2Vz1_dy2Vz1_dz2VzN();
	//(1,1,1<k<nz-1)
	double dx2Vz1_dy2Vz1_dz2Vz();
	//
	//(1,N1,0)
	double dx2Vz1_dy2VzN1_dz2Vz0();
	//(1,N1,1)
	double dx2Vz1_dy2VzN1_dz2Vz1();
	//(1,N1,N1)
	double dx2Vz1_dy2VzN1_dz2VzN1();
	//(1,N1,N)
	double dx2Vz1_dy2VzN1_dz2VzN();
	//(1,N1,1<k<nz-1)
	double dx2Vz1_dy2VzN1_dz2Vz();
	//
	//(1,N,0)
	double dx2Vz1_dy2VzN_dz2Vz0();
	//(1,N,1)
	double dx2Vz1_dy2VzN_dz2Vz1();
	//(1,N,N1)
	double dx2Vz1_dy2VzN_dz2VzN1();
	//(1,N,N)
	double dx2Vz1_dy2VzN_dz2VzN();
	//(1,N,1<k<nz-1)
	double dx2Vz1_dy2VzN_dz2Vz();
	//
	//(1,1<j<ny-1,0)
	double dx2Vz1_dy2Vz_dz2Vz0();
	//(1,1<j<ny-1,1)
	double dx2Vz1_dy2Vz_dz2Vz1();
	//(1,1<j<ny-1,N1)
	double dx2Vz1_dy2Vz_dz2VzN1();
	//(1,1<j<ny-1,N)
	double dx2Vz1_dy2Vz_dz2VzN();
	//(1,1<j<ny-1,1<k<nz-1)
	double dx2Vz1_dy2Vz_dz2Vz();
	//--end of i=1-----------------------------------------
	//
	//---start of i=nx-1-----------------------------------
	//(nx-1,0,0)
	double dx2VzN1_dy2Vz0_dz2Vz0();
	//(nx-1,0,1)
	double dx2VzN1_dy2Vz0_dz2Vz1();
	//(nx-1,0,N1)
	double dx2VzN1_dy2Vz0_dz2VzN1();
	//(nx-1,0,N)
	double dx2VzN1_dy2Vz0_dz2VzN();
	//(nx-1,0,1<k<nz-1)
	double dx2VzN1_dy2Vz0_dz2Vz();
	//
	//(nx-1,1,0)
	double dx2VzN1_dy2Vz1_dz2Vz0();
	//(nx-1,1,1)
	double dx2VzN1_dy2Vz1_dz2Vz1();
	//(nx-1,1,N1)
	double dx2VzN1_dy2Vz1_dz2VzN1();
	//(nx-1,1,N)
	double dx2VzN1_dy2Vz1_dz2VzN();
	//(nx-1,1,1<k<nz-1)
	double dx2VzN1_dy2Vz1_dz2Vz();
	//
	//(nx-1,ny-1,0)
	double dx2VzN1_dy2VzN1_dz2Vz0();
	//(nx-1,N1,1)
	double dx2VzN1_dy2VzN1_dz2Vz1();
	//(nx-1,N1,N1)
	double dx2VzN1_dy2VzN1_dz2VzN1();
	//(nx-1,N1,N)
	double dx2VzN1_dy2VzN1_dz2VzN();
	//(nx-1,N1,1<k<nz-1)
	double dx2VzN1_dy2VzN1_dz2Vz();
	//
	//(nx-1,N,0)
	double dx2VzN1_dy2VzN_dz2Vz0();
	//(nx-1,N,1)
	double dx2VzN1_dy2VzN_dz2Vz1();
	//(nx-1,N,N1)
	double dx2VzN1_dy2VzN_dz2VzN1();
	//(nx-1,N,N)
	double dx2VzN1_dy2VzN_dz2VzN();
	//(nx-1,N,1<k<nz-1)
	double dx2VzN1_dy2VzN_dz2Vz();
	//
	//(nx-1,1<j<ny-1,0)
	double dx2VzN1_dy2Vz_dz2Vz0();
	//(nx-1,1<j<ny-1,1)
	double dx2VzN1_dy2Vz_dz2Vz1();
	//(nx-1,1<j<ny-1,N1)
	double dx2VzN1_dy2Vz_dz2VzN1();
	//(nx-1,1<j<ny-1,N)
	double dx2VzN1_dy2Vz_dz2VzN();
	//(nx-1,1<j<ny-1,1<k<nz-1)
	double dx2VzN1_dy2Vz_dz2Vz();
	//-end of i=N1------------------------------------------
	//
	//-start of i=N-----------------------------------------
	//(nx,0,0)
	double dx2VzN_dy2Vz0_dz2Vz0();
	//(nx,0,1)
	double dx2VzN_dy2Vz0_dz2Vz1();
	//(nx,0,N1)
	double dx2VzN_dy2Vz0_dz2VzN1();
	//(nx,0,N)
	double dx2VzN_dy2Vz0_dz2VzN();
	//(nx,0,1<k<nz-1)
	double dx2VzN_dy2Vz0_dz2Vz();
	//
	//(nx,1,0)
	double dx2VzN_dy2Vz1_dz2Vz0();
	//(nx,1,1)
	double dx2VzN_dy2Vz1_dz2Vz1();
	//(nx,1,N1)
	double dx2VzN_dy2Vz1_dz2VzN1();
	//(nx,1,N)
	double dx2VzN_dy2Vz1_dz2VzN();
	//(nx,1,1<k<nz-1)
	double dx2VzN_dy2Vz1_dz2Vz();
	//
	//(nx,ny-1,0)
	double dx2VzN_dy2VzN1_dz2Vz0();
	//(nx,N1,1)
	double dx2VzN_dy2VzN1_dz2Vz1();
	//(nx,N1,N1)
	double dx2VzN_dy2VzN1_dz2VzN1();
	//(nx,N1,N)
	double dx2VzN_dy2VzN1_dz2VzN();
	//(nx,N1,1<k<nz-1)
	double dx2VzN_dy2VzN1_dz2Vz();
	//
	//(nx,N,0)
	double dx2VzN_dy2VzN_dz2Vz0();
	//(nx,N,1)
	double dx2VzN_dy2VzN_dz2Vz1();
	//(nx,N,N1)
	double dx2VzN_dy2VzN_dz2VzN1();
	//(nx,N,N)
	double dx2VzN_dy2VzN_dz2VzN();
	//(nx,N,1<k<nz-1)
	double dx2VzN_dy2VzN_dz2Vz();
	//
	//(nx,1<j<ny-1,0)
	double dx2VzN_dy2Vz_dz2Vz0();
	//(nx,1<j<ny-1,1)
	double dx2VzN_dy2Vz_dz2Vz1();
	//(nx,1<j<ny-1,N1)
	double dx2VzN_dy2Vz_dz2VzN1();
	//(nx,1<j<ny-1,N)
	double dx2VzN_dy2Vz_dz2VzN();
	//(nx,1<j<ny-1,1<k<nz-1)
	double dx2VzN_dy2Vz_dz2Vz();
	//-end of i=nx==============================
	//
	//==start of 1<i<nx-1========================
	//(1<i<nx-1,0,0)
	double dx2Vz_dy2Vz0_dz2Vz0();
	//(1<i<nx-1,0,1)
	double dx2Vz_dy2Vz0_dz2Vz1();
	//(1<i<nx-1,0,N1)
	double dx2Vz_dy2Vz0_dz2VzN1();
	//(1<i<nx-1,0,N)
	double dx2Vz_dy2Vz0_dz2VzN();
	//(1<i<nx-1,0,1<k<nz-1)
	double dx2Vz_dy2Vz0_dz2Vz();
	//
	//(1<i<nx-1,1,0)
	double dx2Vz_dy2Vz1_dz2Vz0();
	//(1<i<nx-1,1,1)
	double dx2Vz_dy2Vz1_dz2Vz1();
	//(1<i<nx-1,1,N1)
	double dx2Vz_dy2Vz1_dz2VzN1();
	//(1<i<nx-1,1,N)
	double dx2Vz_dy2Vz1_dz2VzN();
	//(1<i<nx-1,1,1<k<nz-1)
	double dx2Vz_dy2Vz1_dz2Vz();
	//
	//(1<i<nx-1,ny-1,0)
	double dx2Vz_dy2VzN1_dz2Vz0();
	//(1<i<nx-1,N1,1)
	double dx2Vz_dy2VzN1_dz2Vz1();
	//(1<i<nx-1,N1,N1)
	double dx2Vz_dy2VzN1_dz2VzN1();
	//(1<i<nx-1,N1,N)
	double dx2Vz_dy2VzN1_dz2VzN();
	//(1<i<nx-1,N1,1<k<nz-1)
	double dx2Vz_dy2VzN1_dz2Vz();
	//
	//(1<i<nx-1,N,0)
	double dx2Vz_dy2VzN_dz2Vz0();
	//(1<i<nx-1,N,1)
	double dx2Vz_dy2VzN_dz2Vz1();
	//(1<i<nx-1,N,N1)
	double dx2Vz_dy2VzN_dz2VzN1();
	//(1<i<nx-1,N,N)
	double dx2Vz_dy2VzN_dz2VzN();
	//(1<i<nx-1,N,1<k<nz-1)
	double dx2Vz_dy2VzN_dz2Vz();
	//
	//(1<i<nx-1,1<j<ny-1,0)
	double dx2Vz_dy2Vz_dz2Vz0();
	//(1<i<nx-1,1<j<ny-1,1)
	double dx2Vz_dy2Vz_dz2Vz1();
	//(1<i<nx-1,1<j<ny-1,N1)
	double dx2Vz_dy2Vz_dz2VzN1();
	//(1<i<nx-1,1<j<ny-1,N)
	double dx2Vz_dy2Vz_dz2VzN();
	//(1<i<nx-1,1<j<ny-1,1<k<nz-1)
	double dx2Vz_dy2Vz_dz2Vz();
	//==end of 1<i<nx-1========================
	//end of Cz=dx2Vz + dy2Vz + dz2Vz============Cz=dx2Vz + dy2Vz + dz2Vz=======================

};


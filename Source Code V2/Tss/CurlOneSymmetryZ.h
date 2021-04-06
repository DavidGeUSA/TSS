#pragma once

/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 04/02/2021
Allrights reserved by David Ge

calculate first order curl using rotate symmetry
********************************************************************/

#include "Tss.h"
#include "RotateSymmetryField.h"

class CurlOneSymmetryZ
{
private:
	RotateSymmetryField *Field;
	RotateSymmetryField *pNext;
	unsigned int nx, ny, nz;       //computation domain. 0...nx; 0...ny; 0...nz
	unsigned int nxN1, nyN1, nzN1; //nx-1, ny-1, nz-1
	unsigned int nx1, ny1, nz1;    //nx+1, ny+1, nz+1
	//
	//work variables
	unsigned int i, j, k;
	//size_t w;
	//prepare for threading
	unsigned int th;
	unsigned int i0, i1;
	//size_t w0;
	Point3Dstruct v0;
public:
	CurlOneSymmetryZ();
	~CurlOneSymmetryZ();
	void setSpaceRange(unsigned int nx0, unsigned int ny0, unsigned int nz0);
	size_t I(unsigned int i, unsigned int j, unsigned int k){ return (k) + nz1*((j) + ny1*(i)); }
	void setThreadWork(unsigned int startIndex, unsigned int endIndex, size_t startIndexOneD, unsigned int t)
	{
		i0 = startIndex; i1 = endIndex; /* w0 = startIndexOneD;*/ th = t;
	}
	void setFieldsMemory(RotateSymmetryField *f, RotateSymmetryField *pNextCurl){ Field = f; pNext = pNextCurl; }
	void calculateCurl();
	//
	double dxVx();   //insize boundary
	double dxVx0();  //at lower boundary
	double dxVx1();  //near lower boundary
	double dxVxN1(); //near upper boundary
	double dxVxN();  //at upper boundary
	//
	double dyVy();   //insize boundary
	double dyVy0();  //at lower boundary
	double dyVy1();  //near lower boundary
	double dyVyN1(); //near upper boundary
	double dyVyN();  //at upper boundary
	//
	double dzVz();   //insize boundary
	double dzVz0();  //at lower boundary
	double dzVz1();  //near lower boundary
	double dzVzN1(); //near upper boundary
	double dzVzN();  //at upper boundary
	//
	double dyVz();   //insize boundary
	double dyVz0();  //at lower boundary
	double dyVz1();  //near lower boundary
	double dyVzN1(); //near upper boundary
	double dyVzN();  //at upper boundary
	//
	double dzVy();   //insize boundary
	double dzVy0();  //at lower boundary
	double dzVy1();  //near lower boundary
	double dzVyN1(); //near upper boundary
	double dzVyN();  //at upper boundary
	//
	double dzVx();   //insize boundary
	double dzVx0();  //at lower boundary
	double dzVx1();  //near lower boundary
	double dzVxN1(); //near upper boundary
	double dzVxN();  //at upper boundary
	//
	double dxVz();   //insize boundary
	double dxVz0();  //at lower boundary
	double dxVz1();  //near lower boundary
	double dxVzN1(); //near upper boundary
	double dxVzN();  //at upper boundary
	//
	double dxVy();   //insize boundary
	double dxVy0();  //at lower boundary
	double dxVy1();  //near lower boundary
	double dxVyN1(); //near upper boundary
	double dxVyN();  //at upper boundary
	//
	double dyVx();   //insize boundary
	double dyVx0();  //at lower boundary
	double dyVx1();  //near lower boundary
	double dyVxN1(); //near upper boundary
	double dyVxN();  //at upper boundary
	//
	double dzVx1_dxVz1();  //Cy at (1,*,1)
	double dyVz0_dzVy0();  //Cx at (*,0,0)
	double dzVxN1_dxVz1(); //Cy at (1,*,nz-1)
	double dyVz0_dzVyN();  //Cx at (*,0,nz)
	double dyVzN1_dzVy1(); //Cx at (*,ny-1,1)
	double dxVy1_dyVxN1(); //Cz at (1,ny-1,*)
	double dyVzN1_dzVyN1(); //Cx at (*,ny-1,nz-1)
	double dyVzN_dzVy0();   //Cx at (*,ny,0)
	double dyVzN_dzVyN();   //Cx at (*,ny,nz)
	double dyVz_dzVy();     //Cx at (*,1<j<ny-1,1<k<nz-1)
	double dyVz1_dzVy1();   //Cx at (*,1,1)
	double dzVx0_dxVz0();   //Cy at (0,*,0)
	double dyVz1_dzVyN1();  //Cx at (*,1,nz-1)
	double dzVxN_dxVz0();   //Cy at (0,*,nz)
	double dxVy0_dyVx0();   //Cz at (0,0,*)
	double dxVy0_dyVxN();   //Cz at (0,ny,*)
	double dzVx1_dxVzN1();  //Cy at (nx-1,*,1)
	double dxVyN1_dyVx1();  //Cz at (nx-1,1,*)
	double dzVxN1_dxVzN1(); //Cy at (nx-1,*,nz-1)
	double dxVyN1_dyVxN1(); //Cz at (nx-1,ny-1,*)
	double dzVx0_dxVzN();   //Cy at (nx,*,0)
	double dzVxN_dxVzN();   //Cy at (nx, *, nz)
	double dxVyN_dyVx0();   //Cz at (nx, 0, *)
	double dxVyN_dyVxN();   //Cz at (nx, ny, *) 
	double dzVx_dxVz();     //Cy at (1<i<nx-1,*,1<k<nz-1)
	double dxVy_dyVx();     //Cz at (1<i<nx-1,1<j<ny-1,*)
	double dxVy1_dyVx1();   //Cz at (1,1,*)
	//test
	double DxVx(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double DyVy(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double DzVz(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0);
	//
	double DyVx(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double DzVy(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double DxVz(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0);
	//
	double DzVx(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double DxVy(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0);
	double DyVz(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0);
	//
	double Divergence(RotateSymmetryField *f, unsigned int i0, unsigned int j0, unsigned int k0, size_t w0);
	int _getNextCurlUseFastMethod(RotateSymmetryField *pCurrent, RotateSymmetryField *pNextCurl);
};


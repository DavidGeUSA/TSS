#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/22/2020
Allrights reserved by David Ge

********************************************************************/
#include "Tss.h"
#include "Space.h"
#include "../EMField/Plugin.h"
/*
  boundary condition. default is PEMC
*/
class BoundaryTss:virtual public Plugin
{
private:
	bool _initialized;           //for detecting coding error
	double ds;                   //space step size
	unsigned int nx, ny, nz;     //computation domain. 0...nx; 0...ny; 0...nz
	unsigned int smax;           //half estimatin order. Estimation order = 2*smax
	unsigned int M;              //Estimation order = 2*smax
	unsigned int nx1, ny1, nz1;  //nx1=nx+1; ny1=ny+1; nz1=nz+1
	Array3Dstruct *A;            //space estimation matrixes
	double factor0, factor1;     //sum of row 1 of A[smax] and A[M]
	Space *space;                //space module, all above are from it
	bool useDivergence;          //true:use divergence constraint
	Point3Dstruct *efield;
	Point3Dstruct* hfield;
	//work variables
	size_t a0, am, aw;
protected:
	virtual void setXboundary(unsigned int j, unsigned int k);
	virtual void setYboundary(unsigned int i, unsigned int k);
	virtual void setZboundary(unsigned int i, unsigned int j);
public:
	BoundaryTss();
	~BoundaryTss();
	bool Initialized(){ return _initialized; }
	//
	size_t Idx(unsigned i, unsigned j, unsigned k);
	//give derived classes a chance to read configurations from the task file
	virtual int initialize(TaskFile *configs){ return 0; }
	//initialize
	virtual int setSpace(Space *spaceModule);
	//
	virtual int apply(Point3Dstruct *E, Point3Dstruct* H);
};


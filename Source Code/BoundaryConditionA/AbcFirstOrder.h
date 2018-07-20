#ifndef __ABCFIRSTORDER_H__
#define __ABCFIRSTORDER_H__

/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by Bob Limnor

********************************************************************/

#include "..\EMField\EMField.h"
#include "..\EMField\BoundaryCondition.h"

/*
	implement first order ABC
	the code/algorithm is taken from a book "understanding FDTD method" by John B. Schneider
	see https://github.com/john-b-schneider/uFDTD/blob/master/Code/Fdtd-3d/abc3dfirst.c
*/
class AbcFirstOrder: public BoundaryCondition
{
private:
	//remember the boundary value at current time
	double *exy0, *exy1, *exz0, *exz1;
	double *eyx0, *eyx1, *eyz0, *eyz1;
	double *ezx0, *ezx1, *ezy0, *ezy1;
	double abccoef; 
public:
	AbcFirstOrder(void);
	~AbcFirstOrder(void);
	void cleanup();
	//
	//initialize boundary condition module
	virtual int initialize(double Courant, int maximumRadius, TaskFile *taskParameters);
	//
	//apply boundary condition at the space point identified by radius indexes (m,n,p)
	virtual void handleData(int m, int n, int p);
};

//get memory index from plane index
#define PLANEINDEX(idx1, idx2) (CUBICINDEX(idx2) + gridSize * CUBICINDEX(idx1))

#endif
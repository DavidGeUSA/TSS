#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/02/2020
Allrights reserved by David Ge

Modifications
Date            Author      Description
---------------------------------------------
2021-03-06      David Ge    Linux compatibility
********************************************************************/
#include "../EMField/BoundaryCondition.h"

/*
	perfect electromagnetic conducting boundary for TSS
*/
class PemcTss :
	virtual public BoundaryCondition
{
public:
	PemcTss();
	~PemcTss();
	//
	virtual int initialize(TaskFile *configs){ return 0; }
	//initialize boundary condition module
	virtual int initialize(double Courant, int maximumRadius, TaskFile *taskParameters);
	//
	virtual RadiusHandleType setRadius(int radius);
	//
	//apply boundary condition at the space point identified by radius indexes (m,n,p)
	virtual void handleData(int m, int n, int p);
};


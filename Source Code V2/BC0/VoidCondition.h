#ifndef __VOIDCONDITION_H__
#define __VOIDCONDITION_H__

/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility
********************************************************************/

#include "../EMField/EMField.h"
#include "../EMField/BoundaryCondition.h"

/*
	it does not apply any boundary conditions
*/
class VoidCondition: virtual public BoundaryCondition
{
public:
	VoidCondition(void){}
	~VoidCondition(void){}
	//
	virtual RadiusHandleType setRadius(int radius){return Finish;}
	virtual int initialize(TaskFile *configs){ return 0; }
	virtual int initialize(double Courant, int maximumRadius){return ERR_OK;}
	virtual int applyBoundaryCondition(int m, int n, int p){return ERR_OK;}
	virtual void handleData(int m, int n, int p){};
};


#endif
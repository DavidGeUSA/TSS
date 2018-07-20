#ifndef __VOIDCONDITION_H__
#define __VOIDCONDITION_H__

/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by Bob Limnor

********************************************************************/

#include "..\EMField\EMField.h"
#include "..\EMField\BoundaryCondition.h"

/*
	it does not apply any boundary conditions
*/
class VoidCondition: public BoundaryCondition
{
public:
	VoidCondition(void){}
	~VoidCondition(void){}
	//
	virtual RadiusHandleType setRadius(int radius){return Finish;}
	virtual int initialize(double Courant, int maximumRadius){return ERR_OK;}
	virtual int applyBoundaryCondition(int m, int n, int p){return ERR_OK;}
	virtual void handleData(int m, int n, int p){};
};


#endif
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/

#include "BoundaryCondition.h"
#include "..\MemoryMan\memman.h"

BoundaryCondition::BoundaryCondition(void)
{
	thickness = 1;
	_fields = NULL;
}

int BoundaryCondition::initialize(double Courant, int maximumRadius, TaskFile *taskParameters)
{
	Cdtds = Courant;
	maxRadius = maximumRadius;
	gridSize = 2 * maxRadius + 1;
	return ERR_OK;
}

int BoundaryCondition::getThickNess()
{
	return thickness;
}
void BoundaryCondition::setFields(FieldPoint3D *fields)
{
	_fields = fields;
}
//////////////////////////////////////////////////////
/*
	a simulation system goes through computation domain
	radius by radius; before working on a radius, it calls 
	this function to inform this calss.
	The return value tells the simulation system if this
	class will work on a radius or not.
	It returns NeedProcess if it wants to process space
	points at the radius. The simulation system will call
	handleData repeatedly for every space point at
	the radius.
	It returns DoNotProcess to tell the simulation system 
	it doesnot want to work on the radius.
*/
//////////////////////////////////////////////////////
RadiusHandleType BoundaryCondition::setRadius(int radius)
{
	r = radius;
	if(r+thickness > maxRadius)
	{
		return NeedProcess;
	}
	return DoNotProcess;
}

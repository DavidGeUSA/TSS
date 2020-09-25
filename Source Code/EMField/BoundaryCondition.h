#ifndef __BOUNDARYCONDITION_H__
#define __BOUNDARYCONDITION_H__

/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/

#include "EMField.h"
#include "RadiusIndex.h"
#include "..\FileUtil\taskFile.h"
#include "..\MemoryMan\MemoryManager.h"

/*
	abstract class for implementing boundary conditions
	A boundary condition should be implemented in a dynamic link library 
	to be plugged into a simulation system at runtime.
*/
class BoundaryCondition:public virtual GoThroughSphereByIndexes, public virtual RadiusIndexCacheUser, public virtual MemoryManUser, public Plugin
{
protected:
	double Cdtds; //Courant number = 1.0 / sqrt(3.0)
	FieldPoint3D *_fields; //fields to work on
	int thickness; //boundary thickness
	int gridSize;  //number of space points on one axis, it is 2*maxRadius + 1
public:
	BoundaryCondition(void);
	//int SetMemoryMappingFolder(const wchar_t *folder);
	int getThickNess();
	void setFields(FieldPoint3D *fields);
	//
	/*
		initialize boundary condition object
		if a derived class overrides this function then it should the base function first.
		a derived class may define runtime parameters. each parameter appears in a task file as a line of name=value
		in the derived function the parameter values are read back via taskParameters
	*/
	virtual int initialize(double Courant, int maximumRadius, TaskFile *taskParameters);
	/*
		the following overridable functions will be called when applying boundary conditions.
		a simulation system calls gothroughSphere() to go through space points radius by radius.
	*/
	/*
		the function setRadius(...) is called when a new radius is to be processed.
		this function sets member r to radius.
		usually you do not need to override this function.
	*/
	virtual RadiusHandleType setRadius(int radius);
	/*
		a simulation system goes through space points radius by radius.
		the function handleData(...) is called for each space point.
			m,n,p=0,+-1,+-2,...,+-maxRadius
			radius = max(|m|,|n|,|p|)
			radius = 0, 1, 2, ..., maxRadius
		you must override this function to handle each space point.
	*/
	virtual void handleData(int m, int n, int p)=0;
};

#endif


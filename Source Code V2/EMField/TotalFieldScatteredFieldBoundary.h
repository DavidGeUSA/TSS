#ifndef __TotalFieldScatteredFieldBoundary_H__
#define __TotalFieldScatteredFieldBoundary_H__

/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/

#include "EMField.h"
#include "RadiusIndex.h"
#include "../FileUtil/taskFile.h"
#include "../MemoryMan/MemoryManager.h"

/*
	abstract class for implementing Total Field/Scattered Field Boundary
	A TF/SF boundary should be implemented in a dynamic link library 
	to be plugged into a simulation system at runtime.
*/
class TotalFieldScatteredFieldBoundary: public virtual RadiusIndexCacheUser, public virtual MemoryManUser, public Plugin
{
protected:
	double Cdtds; //Courant number = 1.0 / sqrt(3.0)
	FieldPoint3D *_fields; //fields to work on
	int maxRadius; //maximum radius
	int gridSize;  //number of space points on one axis, it is 2*maxRadius + 1
	//TFSF boundary
	int firstX, firstY, firstZ, // indices for first point in TF region
		lastX, lastY, lastZ;    // indices for last point in TF region
	//
public:
	TotalFieldScatteredFieldBoundary(void);
	//
	/*
		initialize TF/SF boundary object
		if a derived class overrides this function then it should call the base function first.
		a derived class may define runtime parameters. each parameter appears in a task file as a line of name=value
		in the derived function the parameter values are read back via taskParameters
	*/
	virtual int initialize(double Courant, int maximumRadius, TaskFile *taskParameters);
	virtual int applyTFSF(FieldPoint3D *fields);
	//relationship of radius indexing m,n,p and cubic indexing i,j,k:
	//i = m + gridSize; j = n + gridSize; k = p + gridSize
	//m,n,p=0,+-1,+-2,...,+-maxRadius
	//i,j,k=0,1,2,...,2*maxRadius, use _fields[CINDEX(i,j,k)] to access fields
	virtual void applyOnPlaneX0(int j, int k)=0;
	virtual void applyOnPlaneX1(int j, int k)=0;
	virtual void applyOnPlaneY0(int i, int k)=0;
	virtual void applyOnPlaneY1(int i, int k)=0;
	virtual void applyOnPlaneZ0(int i, int j)=0;
	virtual void applyOnPlaneZ1(int i, int j)=0;
};


#endif
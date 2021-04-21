#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 04/02/2021
Allrights reserved by David Ge

virtual fields with rotating symmetry, replacing real field represented by memory Point3Dstruct*

3D: i=0,1,...,nx; j=0,1,...,ny; k=0,1,...,nz
rotating symmetry:
	ny=nx; nx even; center line at ic=nx/2: (ic, ic, k) -> (0,0,k*ds), ic*ds=xmin
	field(i,j,k)=field(sqrt((i-ic)^2,(j-ic)^2),k)
use a 2D half plane to represent 3D:
	2D plane (i=0,1,...,ic; j=ic; k=0,1,...,nz)
********************************************************************/

#include "VirtualField.h"
#include "../MemoryMan/MemoryManager.h"

class RotateSymmetryField :
	virtual public VirtualField
{
private:
	unsigned int ic;      //center point = nx/2
	unsigned int ic1;     //ic+1
	unsigned int nz1;     //nz+1
	size_t cellCount;     //ic1*nz1
	size_t fieldMemorySize; //cellCount*sizeof(Point3Dstruct)
	Point3Dstruct *plane;   //(i,ic,k) plane, i=0,1,...,ic; j=ic; k=0,1,...,nz
	bool memAllocated;      //allocated plane, need to free plane
	//
	//Point3Dstruct val;
	//
public:
	RotateSymmetryField();
	RotateSymmetryField(SimStruct *simp, Point3Dstruct *field);
	~RotateSymmetryField();
	void cleanup();
	Point3Dstruct *getRawMemory(){ return plane; }
	void setRawMemory(Point3Dstruct *f){ plane = f; }
	//static utilities
	static int IsZrotateSymmetry(SimStruct *pams);
	static void map(double fx0, double fy0, double x, double y, double *pfx, double *pfy);
	static void mapToNegativeY(double fx0, double fy0, double *pfx, double *pfy);
	static void mapToPositiveY(double fx0, double fy0, double *pfx, double *pfy);
	static void mapToPositiveX(double fx0, double fy0, double *pfx, double *pfy);
	//
	//plane index
	size_t Idx(unsigned int i, unsigned int k){ return k + nz1*i; }
	Point3Dstruct *getFieldOnPlane(size_t w){ return &(plane[w]); }
	Point3Dstruct *getFieldOnPlane(unsigned int i, unsigned int k){ return &(plane[Idx(i,k)]); }
	void setFieldOnPlane(size_t w, Point3Dstruct *v){ plane[w].x = v->x; plane[w].y = v->y; plane[w].z = v->z; }
	//
	virtual int initialVirtualField(SimStruct *simp, MemoryManager *mem);
	virtual void getField(unsigned int i, unsigned int j, unsigned int k, Point3Dstruct *val);
	virtual double getFieldx(unsigned int i, unsigned int j, unsigned int k);
	virtual double getFieldy(unsigned int i, unsigned int j, unsigned int k);
	virtual double getFieldz(unsigned int i, unsigned int j, unsigned int k);
	virtual void setField(unsigned int i, unsigned int j, unsigned int k, Point3Dstruct *val);
};


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
	Point3Dstruct *plane; //(i,ic,k) plane, i=0,1,...,ic; j=ic; k=0,1,...,nz
	//
	Point3Dstruct val;
	//
	//work variables
	int ir, jr;          //for calculate radius, ir = ic-i, jr = ic-j
	double r;            //radius = sqrt(ir^2+jr^2)
	unsigned int n0, n1; //integers covering r; n1=ceiling(r)>=r; n0=floor(r)<=r
	unsigned int dn;     //dn = n1-n0; if dn==0 then index=(ic-n0,ic,k) 
						 //if dn !=0 then use weighted average between indexes (ic-n0,ic,k) and (ic-n1,ic,k) 
	double f1, f0;       //weights, f1=(n1-r)/dn, f0=(r-n0)/dn
						 //Field(i,j,k) = f1 * Field(ic-n0,ic,k) + f0 * Field(ic-n1,ic,k)
	size_t idx1, idx0;
public:
	RotateSymmetryField();
	~RotateSymmetryField();
	void cleanup();
	Point3Dstruct *getRawMemory(){ return plane; }
	//
	//plane index
	size_t Idx(unsigned int i, unsigned int k){ return k + ic1*i; }
	Point3Dstruct *getFieldOnPlane(size_t w){ return &(plane[w]); }
	Point3Dstruct *getFieldOnPlane(unsigned int i, unsigned int k){ return &(plane[Idx(i,k)]); }
	void setFieldOnPlane(size_t w, Point3Dstruct *v){ plane[w].x = v->x; plane[w].y = v->y; plane[w].z = v->z; }
	//
	virtual int initialVirtualField(SimStruct *simp, MemoryManager *mem);
	virtual Point3Dstruct *getField(unsigned int i, unsigned int j, unsigned int k);
	virtual double getFieldx(unsigned int i, unsigned int j, unsigned int k);
	virtual double getFieldy(unsigned int i, unsigned int j, unsigned int k);
	virtual double getFieldz(unsigned int i, unsigned int j, unsigned int k);
	virtual void setField(unsigned int i, unsigned int j, unsigned int k, Point3Dstruct *val);
};


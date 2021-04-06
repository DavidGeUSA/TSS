#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 04/02/2021
Allrights reserved by David Ge

base class for virtual fields, replacing real field represented by memory Point3Dstruct*
********************************************************************/

#include "../EMField/EMField.h"
#include "Tss.h"
#include "../MemoryMan/MemoryManager.h"

class VirtualField
{
protected:
	SimStruct *pams;
	MemoryManager *_mem;
public:
	VirtualField();
	~VirtualField();
	//
	virtual int initialVirtualField(SimStruct *simp, MemoryManager *mem){ pams = simp; _mem = mem; return 0; }
	virtual Point3Dstruct *getField(unsigned int i, unsigned int j, unsigned int k)=0;
	virtual double getFieldx(unsigned int i, unsigned int j, unsigned int k) = 0;
	virtual double getFieldy(unsigned int i, unsigned int j, unsigned int k) = 0;
	virtual double getFieldz(unsigned int i, unsigned int j, unsigned int k) = 0;
	virtual void setField(unsigned int i, unsigned int j, unsigned int k, Point3Dstruct *val) = 0;
};


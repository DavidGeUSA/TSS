#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

field source of Ricker function
********************************************************************/

#include "../Tss/FieldSourceTss.h"
#include "../FileUtil/taskFile.h"

class RickerSource : virtual public FieldSourceTss
{
private:
	double ppw;
	double courant;
	unsigned int i0, j0, k0;
	unsigned int iN, iP, jN, jP, kN, kP;
	size_t w0;
public:
	RickerSource();
	virtual ~RickerSource();
	virtual int initialize(TaskFile *configs);
	virtual int initialize(SimStruct *params);
	virtual bool isInSource(unsigned int i, unsigned int j, unsigned int k);
	virtual int applySources(double t, size_t tIndex, Point3Dstruct *efile, Point3Dstruct *hfile);
	virtual int applyToZrotateSymmetry(double t, size_t tIndex, RotateSymmetryField *efile, RotateSymmetryField *hfile);
};


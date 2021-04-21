#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

void boundary condition 
********************************************************************/
#include "../Tss/BoundaryTss.h"
class BoundaryTssVoid : virtual public BoundaryTss
{
public:
	BoundaryTssVoid();
	~BoundaryTssVoid();
	virtual int apply(Point3Dstruct *E, Point3Dstruct* H){ return 0; }
	virtual int applyToZrotateSymmetry(RotateSymmetryField *E, RotateSymmetryField *H){ return 0; }
};


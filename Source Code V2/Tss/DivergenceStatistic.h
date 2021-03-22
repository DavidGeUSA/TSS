#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

use field divergence to evaluate simulation accuracy
********************************************************************/
#include "Space.h"
#include "../FileUtil/taskFile.h"
#include "FieldSourceTss.h"

class DivergenceStatistic
{
protected:
	Space *space;
	SimStruct *pams;
	FieldStatisticStruct *fs;
	FieldSourceTss *src;
	unsigned int s0;
	unsigned int endX, endY, endZ;
public:
	DivergenceStatistic();
	~DivergenceStatistic();
	virtual int initializeByConfig(TaskFile *taskConfig){ return 0; }
	virtual void initialize(Space *sp, SimStruct *ps, FieldSourceTss *fieldSource, bool excluseBoundary);
	virtual size_t calculate(Point3Dstruct *efile, Point3Dstruct *hfile, FieldStatisticStruct *f);
};


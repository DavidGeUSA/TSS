#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

use field divergence to evaluate simulation accuracy
********************************************************************/
#include "Space.h"
#include "CurlOne.h"
#include "CurlOneSymmetryZ.h"
#include "../FileUtil/taskFile.h"
#include "FieldSourceTss.h"

class DivergenceStatistic
{ 
protected:
	Space *space;             //calculate space derivatives
	SimStruct *pams;          //simulation parameters
	FieldStatisticStruct *fs; //statistics holder
	FieldSourceTss *src;      //to exclude space points where field sources are applied
	unsigned int s0;                //start index
	unsigned int endX, endY, endZ;  //end indexes
	bool _excludeBoundary;
public:
	DivergenceStatistic(){ pams = NULL; fs = NULL; src = NULL; s0 = 0; endX = endY = endZ = 0; }
	~DivergenceStatistic(){}
	void initialize(Space *sp, SimStruct *ps, FieldSourceTss *fieldSource, bool excludeBoundary);
	//
	virtual int initializeByConfig(TaskFile *taskConfig){ return 0; }
	virtual int onInitialize(){ return 0; }
	virtual size_t calculate(Point3Dstruct *efile, Point3Dstruct *hfile, FieldStatisticStruct *f)=0;
};

class DivergenceStatistic3D :public virtual DivergenceStatistic
{
protected:
	//Space module can be used to calculate space derivatives in a generic way
	//CurlOne module calculate space derivatives, optimized in speed
	//these modules should give the same results. they can be used to verify each other
	CurlOne c1;               //calculate space derivatives	
	virtual int onInitialize();
public:
	DivergenceStatistic3D();
	~DivergenceStatistic3D();
	
	virtual size_t calculate(Point3Dstruct *efile, Point3Dstruct *hfile, FieldStatisticStruct *f);
};

class DivergenceStatisticZRotateSymmetry :public virtual DivergenceStatistic
{
private:
	unsigned int ic;  //center
protected:
	//Space module can be used to calculate space derivatives in a generic way
	//CurlOne module calculate space derivatives, optimized in speed
	//these modules should give the same results. they can be used to verify each other
	CurlOneSymmetryZ c1;               //calculate space derivatives	
	virtual int onInitialize();
public:
	DivergenceStatisticZRotateSymmetry(){}
	~DivergenceStatisticZRotateSymmetry(){}

	virtual size_t calculate(Point3Dstruct *efile, Point3Dstruct *hfile, FieldStatisticStruct *f);
};

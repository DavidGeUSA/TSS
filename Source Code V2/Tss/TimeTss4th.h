#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

time advancement estimation module, optimized for 4-th order estimation
coding not finished
********************************************************************/
#include "TimeTssBase.h"
#include "CurlOne.h"
#include "CurlTwo.h"

/*
	4th order estimation, optimized
*/
class TimeTss4th :
	virtual public TimeTssBase
{
protected:
	CurlTwo crE2;
	CurlTwo crH2;
	CurlOne cr1a, cr1b;
	Point3Dstruct *work;
	int curlNumber;
	//
	double gee1,ghe0,ghe1;
	double ghh1, geh0, geh1;
	//
	virtual void cleanup();
public:
	TimeTss4th();
	virtual ~TimeTss4th();
	
	virtual int initFields(FieldsSetter* f0);
	virtual int GetFirstCurls();
	virtual int GetNextCurls();
	virtual int AdvanceOneStep();
};


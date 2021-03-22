#pragma once
#include "TimeTssBase.h"
/*
	Yee FDTD algorithm
*/
class TimeYee : virtual public TimeTssBase
{
private:
	unsigned int ny1, nz1;
public:
	TimeYee();
	virtual ~TimeYee();
	//
	virtual int GetFirstCurls() { return 0; }
	virtual int GetNextCurls(){ return 0; }
	//
	size_t I(unsigned int i, unsigned int j, unsigned int k){ return (k) + nz1*((j) + ny1*(i)); }
	//
	virtual int initFields(FieldsSetter* f0);
	virtual int AdvanceOneStep();
	//
	virtual bool needModifyFieldsForStatistics(){ return true; }
	virtual void modifyFieldForStatistics_E(Point3Dstruct *src, Point3Dstruct *dest);
	virtual void modifyFieldForStatistics_H(Point3Dstruct *src, Point3Dstruct *dest);

};


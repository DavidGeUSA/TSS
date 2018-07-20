#pragma once
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/

#include "..\EMField\Radiusindex.h"
class FieldSourceSphereCurrent :
	public GoThroughSphereByIndexes
{
protected:
	double _magnitude;
	double _densityAtPoint;
	FieldPoint3D *_fields;
	size_t _timeIndex;
public:
	FieldSourceSphereCurrent(void);
	~FieldSourceSphereCurrent(void);
	void reset(FieldPoint3D *fields, double magnitude, size_t timeIndex);
	//
	virtual void handleData(int m, int n, int p);

};


#pragma once
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include "../EMField/RadiusIndex.h"
#include "../EMField/EMField.h"
/*
	apply curls for advancing fields in time
*/
class ApplyCurls:public virtual GoThroughSphereByIndexes
{
protected:
	FieldPoint3D *_fields; //fields for applying curls to it
	FieldPoint3D *_curls;  //curls to apply to _fields
	double *_factorE, *_factorH;
public:
	ApplyCurls();
	virtual void SetFields(FieldPoint3D *fields, FieldPoint3D *curls, double *factorE, double *factorH);
	virtual void handleData(int m, int n, int p)=0;
};
/*
	apply curls for advancing fields in time at order 2k
*/
class ApplyCurlsEven:public virtual ApplyCurls
{
public:
	virtual void handleData(int m, int n, int p);
};
/*
	apply curls for advancing fields in time at order 2k+1
*/
class ApplyCurlsOdd:public ApplyCurls
{
public:
	virtual void handleData(int m, int n, int p);
};


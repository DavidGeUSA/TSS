#pragma once
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "../EMField/EMField.h"
#include "../EMField/RadiusIndex.h"

///////////////////////////////////////////////////////////////////
/*
	advance field in time
*/
class UpdateField:public virtual GoThroughSphereByIndexes, public virtual RadiusIndexCacheUser
{
protected:
	double ch, ce; //ch = (dt/ds)/mu0, ce = (dt/ds)/eps0
	FieldPoint3D *_field;
	int maxRadius, maxRadiusNeg;
public:
	UpdateField();
	void setMaxRadius(RadiusIndexToSeriesIndex *cache, int maxR, double ch_i, double ce_i);
	void reset(FieldPoint3D *field);
	//
	virtual void handleData(int m, int n, int p)=0;
};

/*
	update magnetic field
*/
class UpdateHField:public virtual UpdateField
{
public:
	virtual void handleData(int m, int n, int p);
};

/*
	update electric field
*/
class UpdateEField:public UpdateField
{
public:
	virtual void handleData(int m, int n, int p);
};

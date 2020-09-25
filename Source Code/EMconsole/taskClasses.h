#pragma once
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "..\EMField\RadiusIndex.h"

/*
	for testing speed of going through sphere index by GoThroughSphereByIndexes
*/
class SphereIndexSpeedTest: public GoThroughSphereByIndexes
{
private:
	int displayCount;
	int displayInterval;
	size_t totalIndex;
protected:
	virtual void handleData(int m, int n, int p);
public:
	SphereIndexSpeedTest();
	void setTotalIndex(size_t total, int interval);
};

/*
	
*/
class PickFieldPoints: public GoThroughSphereBySpaces, public virtual MemoryManUser
{
private:
	bool inMonotonic;
	size_t _picks;
	FieldPoint3D *_fields;
	size_t index;
	FieldItem3D *_items;
	double *_magnitudes;
	size_t *_collectedIndexes;
	size_t startIndex;
	size_t pos;
protected:
	virtual void handleData(double x, double y, double z);
	virtual void onFinish();
public:
	PickFieldPoints();
	~PickFieldPoints();
	bool IsInMonotonic(){return inMonotonic;}
	int initialize(size_t picks, FieldPoint3D *fields, FieldItem3D *pickedItems);
};


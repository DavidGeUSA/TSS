#pragma once
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/

#include "..\EMField\EMField.h"
#include "..\EMField\RadiusIndex.h"
#include "FieldCompareResult.h"

/*
	compare two fields
*/
class FieldDataComparer: public virtual GoThroughSphereByIndexes, public virtual RadiusIndexCacheUser
{
private:
	//the two fields to be compared
	FieldPoint3D *_FieldsA;
	FieldPoint3D *_FieldsB;
	//
	FieldCompareResult *_results;
public:
	FieldDataComparer(RadiusIndexToSeriesIndex *idxCache, int maxR);
	~FieldDataComparer(void);
	void setFields(FieldPoint3D *A, FieldPoint3D *B);
	int compareDataByRadius();
	FieldCompareResult *GetResults();
	//
	virtual void handleData(int m, int n, int p);
};

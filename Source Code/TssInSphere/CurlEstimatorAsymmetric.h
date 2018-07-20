#pragma once
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/
#include "..\EMField\EMField.h"
#include "..\EMField\RadiusIndex.h"
#include "DerivativeEstimator.h"
/*
	estimate curls using asymmetric derivative estimation
*/
class CurlEstimatorAsymmetric:public virtual GoThroughSphereByIndexes, public virtual RadiusIndexCacheUser
{
private:
	FieldPoint3D *_fields; //fields for calculating curls of it
	FieldPoint3D *_curls;  //curls of _fields
	DerivativeEstimatorAsymmetric *_derivative;
	size_t index;
	int r;
protected:
	int ret;
	size_t idx, idx2;
public:
	CurlEstimatorAsymmetric(DerivativeEstimatorAsymmetric *derivative);
	void SetFields(FieldPoint3D *fields, FieldPoint3D *curls);
	virtual void handleData(int m, int n, int p);
};


#pragma once
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/
#include "FieldStatistics.h"
#include "..\EMField\RadiusIndex.h"
#include "DerivativeEstimator.h"
/*
	check field validity by divergence=0
*/
class FieldStatisticsByDivergence: public virtual FieldStatistics, public virtual GoThroughSphereByIndexes, public virtual RadiusIndexCacheUser
{
protected:
	FieldPoint3D *_fields;
	double ds2;
public:
	FieldStatisticsByDivergence(FieldPoint3D *fields, double halfSpaceStep);
	~FieldStatisticsByDivergence();
	void SetField(FieldPoint3D *fields);
	
	virtual void handleData(int m, int n, int p)=0;
};

/*
	use asymmetric estimation for divergence
*/

////////////////////////////////////////////////////////////////////////
/*
	check field validity by divergence=0. use asymmetric estimations near and at the boundary
*/
class FieldStatisticsByDivergenceAsymmetric: public virtual FieldStatisticsByDivergence
{
private:
	DerivativeEstimatorAsymmetric *_derivative;
	size_t idx,idx2;
public:
	FieldStatisticsByDivergenceAsymmetric(DerivativeEstimatorAsymmetric *derivative, FieldPoint3D *fields, double spaceStep);
	~FieldStatisticsByDivergenceAsymmetric();
	virtual void handleData(int m, int n, int p);
};
////////////////////////////////////////////////////////////////////////

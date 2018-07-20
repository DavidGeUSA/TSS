#pragma once
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/

#include "..\TssInSphere\FieldStatisticsByDivergence.h"

/*
	get summations of divergence of E and H for each radius
*/
class DivergenceComparer:public virtual FieldStatisticsByDivergenceAsymmetric
{
private:
	double *_divE;
	double *_divH;
public:
	DivergenceComparer(double *divgE, double *divgH, DerivativeEstimatorAsymmetric *owner, FieldPoint3D *fields, double halfSpaceStep);
	//
	virtual void MakeStatistics(FieldPoint3D *_fields, size_t index, int r);
};


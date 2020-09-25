#pragma once
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "..\EMField\EMField.h"
#include "..\EMField\RadiusIndex.h"
#include "DivergenceComparer.h"

/*
	compare divergences of two fields
*/
class FieldDivergenceComparer:public virtual RadiusIndexCacheUser
{
private:
	int ret;
	FieldPoint3D *_FieldsA;
	FieldPoint3D *_FieldsB;
	DerivativeEstimatorAsymmetric *_derivativeAsymmetric; //estimating derivative, using asymmetric estimation near boundary
	DivergenceComparer *_divergenceEstimatorA; //check field by divergence=0
	DivergenceComparer *_divergenceEstimatorB;
	//
	double *DivMagnitudeE_A; //sum of divergence magnitude on r
	double *DivMagnitudeH_A; //sum of divergence magnitude on r
	double *DivMagnitudeE_B; //sum of divergence magnitude on r
	double *DivMagnitudeH_B; //sum of divergence magnitude on r
	//
	double *diffE; //difference of DivMagnitudeE_A[r]-DivMagnitudeE_B[r]
	double *diffH; //difference of DivMagnitudeH_A[r]-DivMagnitudeH_B[r]
	//
	int maxRadius;
	double ds;
	int internalRadius;
	//
	double minDifE, minDifH;
	double maxDifE, maxDifH;
	double avgDifE, avgDifH;
	double MinE_A, MinE_B, MinH_A, MinH_B;
	double MaxE_A, MaxE_B, MaxH_A, MaxH_B;
	//
	void cleanup();
public:
	FieldDivergenceComparer(RadiusIndexToSeriesIndex *idxCache);
	~FieldDivergenceComparer(void);
	int setFields(FieldPoint3D *fieldA, FieldPoint3D *fieldB, int maxR, double stepSize, int radiusLimit);
	int compareDivergenceByRadius();
	double GetMinimumDiffE(){return minDifE;}
	double GetMinimumDiffH(){return minDifH;}
	double GetMaximumDiffE(){return maxDifE;}
	double GetMaximumDiffH(){return maxDifH;}
	double GetAverageDiffE(){return avgDifE;}
	double GetAverageDiffH(){return avgDifH;}
	double GetMinimumE_A(){return MinE_A;}
	double GetMinimumH_A(){return MinH_A;}
	double GetMinimumE_B(){return MinE_B;}
	double GetMinimumH_B(){return MinH_B;}

	double GetMaximumE_A(){return MaxE_A;}
	double GetMaximumH_A(){return MaxH_A;}
	double GetMaximumE_B(){return MaxE_B;}
	double GetMaximumH_B(){return MaxH_B;}
};

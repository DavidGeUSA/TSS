#pragma once
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include "../EMField/RadiusIndex.h"
/*
	estimate derivative using specified order
*/
class DerivativeEstimator:public virtual RadiusIndexCacheUser
{
protected:
	int ret;
	//
	int _maxOrder;
	int maxRadius;
	int maxRadiusNeg;
	int _radiusOfInterior; //maxRadius - _maxOrder
	//
	int M2; //2*_maxOrder;
	//
	virtual void cleanup()=0;
public:
	DerivativeEstimator(int maxEstimationOrder, int maxR, RadiusIndexToSeriesIndex *indexCache);
	virtual ~DerivativeEstimator();
	virtual void prepareCoefficeints()=0;
	int GetLastHandlerError(){return ret;}
	virtual int checkBoundary(int idx)=0;
	//
	size_t Index(int m, int n, int p);
	inline void SetIndex(int m, int n, int p, size_t index);
};
/////////////////////////////////////////////////////////////////////////
/*
	estimate derivatives. use asymmetric estimations near and at the boundary
*/
class DerivativeEstimatorAsymmetric: public DerivativeEstimator
{
private:
	int _numEstimatins; //1+2*_maxOrder
	void fillTaylorMatrixAsymmetric(double *a, int positiveEnd, int negativeEnd);
protected:
	virtual void cleanup();
public:
	DerivativeEstimatorAsymmetric(int maxEstimationOrder, int maxR, RadiusIndexToSeriesIndex* indexCache);
	virtual ~DerivativeEstimatorAsymmetric();
	//
	virtual void prepareCoefficeints();
	virtual int checkBoundary(int idx);
	//array used by "asymmetric estimation" approach 
	double **coefficientByEdge; //[2M+1] pointer of 2M doubles
	double *coefficients; //findCoeeficients sets coefficients to one of pointers in coefficientByEdge
	int _positiveEnd, _negativeEnd; //indexes for getting samplings, set by findCoeeficients

};


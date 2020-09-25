/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "FieldDivergenceComparer.h"
#include "DivergenceComparer.h"

#include <float.h>
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>

FieldDivergenceComparer::FieldDivergenceComparer(RadiusIndexToSeriesIndex *idxCache)
{
	ret = ERR_OK;
	seriesIndex = idxCache;
	_FieldsA = NULL;
	_FieldsB = NULL;
	_divergenceEstimatorA = NULL;
	_divergenceEstimatorB = NULL;
	_derivativeAsymmetric = NULL;
	DivMagnitudeE_A = NULL;
	DivMagnitudeH_A = NULL;
	DivMagnitudeE_B = NULL;
	DivMagnitudeH_B = NULL;
	diffE = NULL;
	diffH = NULL;
}


FieldDivergenceComparer::~FieldDivergenceComparer(void)
{
	cleanup();
}

void FieldDivergenceComparer::cleanup()
{
	if(_divergenceEstimatorA != NULL)
	{
		delete _divergenceEstimatorA;
		_divergenceEstimatorA = NULL;
	}
	if(_divergenceEstimatorB != NULL)
	{
		delete _divergenceEstimatorB;
		_divergenceEstimatorB = NULL;
	}
	if(_derivativeAsymmetric != NULL)
	{
		delete _derivativeAsymmetric;
		_derivativeAsymmetric = NULL;
	}
	if(DivMagnitudeE_A != NULL)
	{
		delete[] DivMagnitudeE_A;
		DivMagnitudeE_A = NULL;
	}
	if(DivMagnitudeH_A != NULL)
	{
		delete[] DivMagnitudeH_A;
		DivMagnitudeH_A = NULL;
	}
	if(DivMagnitudeE_B != NULL)
	{
		delete[] DivMagnitudeE_B;
		DivMagnitudeE_B = NULL;
	}
	if(DivMagnitudeH_B != NULL)
	{
		delete[] DivMagnitudeH_B;
		DivMagnitudeH_B = NULL;
	}
	if(diffE != NULL)
	{
		delete[] diffE;
		diffE = NULL;
	}
	if(diffH != NULL)
	{
		delete[] diffH;
		diffH = NULL;
	}
	_FieldsA = NULL;
	_FieldsB = NULL;
}

int FieldDivergenceComparer::setFields(FieldPoint3D *fieldA, FieldPoint3D *fieldB, int maxR, double stepSize, int radiusLimit)
{
	cleanup();
	_FieldsA = fieldA;
	_FieldsB = fieldB;
	maxRadius = maxR;
	ds = stepSize;
	internalRadius = radiusLimit;
	//
	DivMagnitudeE_A = new double[maxRadius+1];
	if(DivMagnitudeE_A == NULL)
	{
		ret = ERR_OUTOFMEMORY;
	}
	if(ret == ERR_OK)
	{
		DivMagnitudeH_A = new double[maxRadius+1];
		if(DivMagnitudeH_A == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}
	if(ret == ERR_OK)
	{
		DivMagnitudeE_B = new double[maxRadius+1];
		if(DivMagnitudeE_B == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}
	if(ret == ERR_OK)
	{
		DivMagnitudeH_B = new double[maxRadius+1];
		if(DivMagnitudeH_B == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}
	if(ret == ERR_OK)
	{
		diffE = new double[maxRadius+1];
		if(diffE == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}
	if(ret == ERR_OK)
	{
		diffH = new double[maxRadius+1];
		if(diffH == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}
	//
	if(ret == ERR_OK)
	{
		for(int i = 0; i<=maxRadius;i++)
		{
			DivMagnitudeE_A[i] = DivMagnitudeH_A[i] = DivMagnitudeE_B[i] = DivMagnitudeH_B[i] = 0.0;
		}
		//
		_derivativeAsymmetric = new DerivativeEstimatorAsymmetric(1, maxRadius, seriesIndex);
		_derivativeAsymmetric->prepareCoefficeints();
		ret = _derivativeAsymmetric->GetLastHandlerError();
		if(ret == ERR_OK)
		{
			_divergenceEstimatorA = new DivergenceComparer(DivMagnitudeE_A, DivMagnitudeH_A, _derivativeAsymmetric, _FieldsA, ds);
			ret = _divergenceEstimatorA->AllocateList(maxRadius);
			if(ret == ERR_OK)
			{
				_divergenceEstimatorB = new DivergenceComparer(DivMagnitudeE_B, DivMagnitudeH_B, _derivativeAsymmetric, _FieldsB, ds);
				ret = _divergenceEstimatorB->AllocateList(maxRadius);
			}
		}
	}
	return ret;
}
int FieldDivergenceComparer::compareDivergenceByRadius()
{
	_divergenceEstimatorA->SetField(_FieldsA);
	ret = _divergenceEstimatorA->gothroughSphere(maxRadius);
	if(ret == ERR_OK)
	{
		_divergenceEstimatorB->SetField(_FieldsB);
		ret = _divergenceEstimatorB->gothroughSphere(maxRadius);
	}
	if(ret == ERR_OK)
	{
		size_t points;
		for(int r=1;r<=maxRadius;r++)
		{
			points = pointsAt(r);
			DivMagnitudeE_A[r] /= (double)points;
			DivMagnitudeE_B[r] /= (double)points;
			DivMagnitudeH_A[r] /= (double)points;
			DivMagnitudeH_B[r] /= (double)points;
			diffE[r] = (DivMagnitudeE_A[r] - DivMagnitudeE_B[r]);
			diffH[r] = (DivMagnitudeH_A[r] - DivMagnitudeH_B[r]);
		}
		//make statistics
		minDifE = minDifH = DBL_MAX;
		maxDifE = maxDifH = -DBL_MAX;
		avgDifE = avgDifH = 0.0;
		points = 0;
		for(int r=1;r<=internalRadius;r++)
		{
			points += pointsAt(r);
			if(diffE[r] < minDifE)
			{
				minDifE = diffE[r];
				MinE_A = DivMagnitudeE_A[r];
				MinE_B = DivMagnitudeE_B[r];
			}
			if(diffH[r] < minDifH)
			{
				minDifH = diffH[r];
				MinH_A = DivMagnitudeH_A[r];
				MinH_B = DivMagnitudeH_B[r];
			}
			if(diffE[r] > maxDifE)
			{
				maxDifE = diffE[r];
				MaxE_A = DivMagnitudeE_A[r];
				MaxE_B = DivMagnitudeE_B[r];
			}
			if(diffH[r] > maxDifH)
			{
				maxDifH = diffH[r];
				MaxH_A = DivMagnitudeH_A[r];
				MaxH_B = DivMagnitudeH_B[r];
			}
			avgDifE += diffE[r];
			avgDifH += diffH[r];
		}
		avgDifE = avgDifE / (double)(internalRadius);
		avgDifH = avgDifH / (double)(internalRadius);
	}
	return ret;
}


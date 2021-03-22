/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include <malloc.h>
#include "DerivativeEstimator.h"
#include "../MathTools/MathTools.h"
#include "../MathTools/Matrix.h"

DerivativeEstimator::DerivativeEstimator(int maxEstimationOrder, int maxR, RadiusIndexToSeriesIndex *indexCache)
{
	ret = ERR_OK;
	_maxOrder = maxEstimationOrder;
	maxRadius = maxR;
	maxRadiusNeg = -maxR;
	M2 = 2 * _maxOrder;
	_radiusOfInterior = maxRadius - _maxOrder;
	
	seriesIndex = indexCache;
}
DerivativeEstimator::~DerivativeEstimator()
{
}

size_t DerivativeEstimator::Index(int m, int n, int p)
{
	return seriesIndex->Index(m,n,p);
}

////////////////////////////////////////////////////////////////
/*
	derivative estimator using asymmetric estimations near or at the edges
*/
DerivativeEstimatorAsymmetric::DerivativeEstimatorAsymmetric(int maxEstimationOrder, int maxR, RadiusIndexToSeriesIndex* indexCache):DerivativeEstimator(maxEstimationOrder, maxR, indexCache)
{
	ret = ERR_OK;
	coefficientByEdge = NULL;
	coefficients = NULL;
	_numEstimatins = 2 * maxEstimationOrder + 1;
}
DerivativeEstimatorAsymmetric::~DerivativeEstimatorAsymmetric()
{
	cleanup();
}
void DerivativeEstimatorAsymmetric::cleanup()
{
	if(coefficientByEdge != NULL)
	{
		for(int i=0;i<_numEstimatins;i++)
		{
			if(coefficientByEdge[i] != NULL)
			{
				delete coefficientByEdge[i];
				coefficientByEdge[i] = NULL;
			}
		}
		delete[] coefficientByEdge;
	}
}
/*
	Fill Taylor series for asymmetric samplings
	a - the array to be filled. It is a 2D array in 1D indexing
	edge - 
*/
void DerivativeEstimatorAsymmetric::fillTaylorMatrixAsymmetric(double *a, int positiveEnd, int negativeEnd)
{
	double kp,nd;
	int ix = 0;
	int rIdx;
	//
	rIdx = 1; //always starts from 1, the 0-th sampling is the function at current location and will be used together with each sampling at rIdx-th deviated location
	while(rIdx <= positiveEnd)
	{
		kp = 1.0; nd = 1.0;
		for(int k=1;k<=M2;k++)
		{
			kp *= (double)k;
			nd *= (double)rIdx;
			a[ix + k - 1]   = nd / kp; //Taylor coefficient for k-th derivative
		}
		ix += M2; //next row
		rIdx++; //next deviation location for sampling
	}
	rIdx = -1; //negative deviation samplings
	while(rIdx >= negativeEnd)
	{
		kp = 1.0; nd = 1.0;
		for(int k=1;k<=M2;k++)
		{
			kp *= (double)k;
			nd *= (double)rIdx;
			a[ix + k - 1]   = nd / kp; //Taylor coefficient for k-th derivative
		}
		ix += M2; //next row
		rIdx--; //next deviation location for sampling
	}
}
///////////////////////////////////////////////////////////////
/*
	determine Taylor coefficients to be used and the index ranges, by relative distance of idx with maxRadius or maxRadiusNeg
	idx - space location index. 
		idx >= 0 
			idx <= maxRadius - _maxOrder : it is not near the boundary
				coefficients = coefficientByEdge[0]
				_positiveEnd=_maxOrder,_negativeEnd=_maxOrder
			idx > maxRadius - _maxOrder : it is near the boundary
				h = maxRadius - idx (h=0,1,2,...,_maxOrder-1)
				coefficients = coefficientByEdge[h+1]
				_positiveEnd=h, _negativeEnd=2 * _maxOrder - h
		idx < 0
			idx >= _maxOrder + maxRadiusNeg : it is not near the boundary
				coefficients = coefficientByEdge[0]
				_positiveEnd=_maxOrder,_negativeEnd=_maxOrder
			idx < _maxOrder + maxRadiusNeg : it is near the boundary
				h = maxRadius + idx (h=0,1,2,...,_maxOrder-1)
				coefficients = coefficientByEdge[_maxOrder + h + 1]
				_positiveEnd=2 * _maxOrder - h, _negativeEnd = h
*/
int DerivativeEstimatorAsymmetric::checkBoundary(int idx)
{
	int h;
	if(idx >= 0)
	{
		if(idx <= _radiusOfInterior)
		{
			h = 0;
			coefficients = coefficientByEdge[0];
			_positiveEnd =  _maxOrder;
			_negativeEnd = -_maxOrder;
		}
		else
		{
			h = maxRadius - idx + 1;                 //1(idx=maxRadius:maximum index), 2(maxRadius-1), 3, ..., _maxOrder(idx=maxRadius-_maxOrder+1:minimum boundary index)
			coefficients = coefficientByEdge[h];
			_positiveEnd =  h - 1;                   //0              (idx at edge), 1,              2,            ..., _maxOrder-1 (idx just enters boundary)
			_negativeEnd = -(2 * _maxOrder - h + 1); //-2 * _maxOrder (idx at edge), -2*_maxOrder-1, -2*_maxOrder-2, ..., -_maxOrder-1 
		}
	}
	else
	{
		if(idx >= -_radiusOfInterior)
		{
			h = 0;
			coefficients = coefficientByEdge[0];
			_positiveEnd =  _maxOrder;
			_negativeEnd = -_maxOrder;
		}
		else
		{
			h = maxRadius + idx + 1;                         //1(idx=-maxRadius),          2(-maxRadius+1),3(-maxRadius+2),...,_maxOrder(-maxRadius+_maxOrder-1)
			coefficients = coefficientByEdge[_maxOrder + h];
			_positiveEnd =  2 * _maxOrder - h + 1;           //2 * _maxOrder (idx at edge), 2*_maxOrder-1, 2*_maxOrder-2, ..., _maxOrder+1
			_negativeEnd = -h + 1;                           //0             (idx at edge), -1,            -2,            ..., -_maxOrder+1
		}
	}
	return h;
}
void DerivativeEstimatorAsymmetric::prepareCoefficeints()
{
	int i;
	int M22 = M2 * M2;
	cleanup();
	coefficientByEdge = new double*[_numEstimatins];
	if(coefficientByEdge == NULL)
		ret = ERR_OUTOFMEMORY;
	else
	{
		for(i=0;i<_numEstimatins;i++)
		{
			coefficientByEdge[i] = NULL;
		}
		for(i=0;i<_numEstimatins;i++)
		{
			coefficientByEdge[i] = new double[M2];
			if(coefficientByEdge[i] == NULL)
			{
				ret = ERR_OUTOFMEMORY;
				break;
			}
		}
	}
	if(ret == ERR_OK)
	{
		double *A;
		double *Q;
		A = Q = NULL;
		A = (double *)malloc(sizeof(double)*M22);
		if(A == NULL)
			ret = ERR_OUTOFMEMORY;
		else
		{
			Q = (double *)malloc(sizeof(double)*M22);
			if(Q == NULL)
				ret = ERR_OUTOFMEMORY;
		}
		if(ret == ERR_OK)
		{
			//prepare coefficientByEdge[0]
			fillTaylorMatrixAsymmetric(A, _maxOrder, -_maxOrder);
			if(inverse(A, Q, M2))
			{
				coefficients = coefficientByEdge[0];
				for(i=0;i<M2;i++)
				{
					coefficients[i] = Q[i];
				}
			}
			else
			{
				ret = ERR_MATH_NOT_INVERSE;
			}
			//prepare for near or at the positive edge
			if(ret == ERR_OK)
			{
				for(int h=1;h<=_maxOrder;h++)
				{
					fillTaylorMatrixAsymmetric(A, h - 1, -(2 * _maxOrder - h + 1));
					if(inverse(A, Q, M2))
					{
						coefficients = coefficientByEdge[h];
						for(i=0;i<M2;i++)
						{
							coefficients[i] = Q[i];
						}
					}
					else
					{
						ret = ERR_MATH_NOT_INVERSE;
						break;
					}
				}
			}
			//prepare for near or at the negative edge
			if(ret == ERR_OK)
			{
				for(int h=1;h<=_maxOrder;h++)
				{
					fillTaylorMatrixAsymmetric(A, 2 * _maxOrder - h + 1, -h + 1);
					if(inverse(A, Q, M2))
					{
						coefficients = coefficientByEdge[_maxOrder + h];
						for(i=0;i<M2;i++)
						{
							coefficients[i] = Q[i];
						}
					}
					else
					{
						ret = ERR_MATH_NOT_INVERSE;
						break;
					}
				}
			}
		}
		if(A != NULL)
		{
			free(A);
		}
		if(Q != NULL)
		{
			free(Q);
		}
	}
	//
}
//////////////////////////////////////////////////////////////

/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include "FieldStatisticsByDivergence.h"
#include "TssInSphere.h"
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  


FieldStatisticsByDivergence::FieldStatisticsByDivergence(FieldPoint3D *fields, double halfSpaceStep)
{
	ret = ERR_OK;
	_fields = fields;
	ds2 = halfSpaceStep;
	index = 0;
}
FieldStatisticsByDivergence::~FieldStatisticsByDivergence()
{
	_fields = NULL;
}
void FieldStatisticsByDivergence::SetField(FieldPoint3D *fields)
{
	_fields = fields;
	Reset();
	index = 0;
}
/////////////////////////////////////////////////////////////////
FieldStatisticsByDivergenceAsymmetric::FieldStatisticsByDivergenceAsymmetric(DerivativeEstimatorAsymmetric *derivative, FieldPoint3D *fields, double spaceStep):FieldStatisticsByDivergence(fields, spaceStep)
{
	_derivative = derivative;
	if(_derivative == NULL)
	{
		ret = ERR_TSS_DERIVATIVE;
	}
	else
	{
		_derivative->shareIndexCacheTo(this);
		ret = ERR_OK;
	}
}
FieldStatisticsByDivergenceAsymmetric::~FieldStatisticsByDivergenceAsymmetric()
{
}
void FieldStatisticsByDivergenceAsymmetric::handleData(int m, int n, int p)
{
	int h;
	int k,i;
	_divergE = _divergH = 0.0;
	//dFx/dx
	h = _derivative->checkBoundary(m);
	if(h == 0)
	{
		i=0;
		k=1;
		while(k <= _derivative->_positiveEnd)
		{
			idx  = seriesIndex->Index(m+k,n,p);
			idx2 = seriesIndex->Index(m-k,n,p);
			_divergE += _derivative->coefficients[i] * (_fields[idx].E.x - _fields[idx2].E.x);
			_divergH += _derivative->coefficients[i] * (_fields[idx].H.x - _fields[idx2].H.x);
			k++;
			i++;
		}
	}
	else
	{
		i=0;
		k=1;
		while(k <= _derivative->_positiveEnd)
		{
			idx = seriesIndex->Index(m+k,n,p);
			_divergE += _derivative->coefficients[i] * (_fields[idx].E.x - _fields[index].E.x);
			_divergH += _derivative->coefficients[i] * (_fields[idx].H.x - _fields[index].H.x);
			k++;
			i++;
		}
		k=-1;
		while(k >= _derivative->_negativeEnd)
		{
			idx = seriesIndex->Index(m+k,n,p);
			_divergE += _derivative->coefficients[i] * (_fields[idx].E.x - _fields[index].E.x);
			_divergH += _derivative->coefficients[i] * (_fields[idx].H.x - _fields[index].H.x);
			k--;
			i++;
		}
	}
	//dFy/dy
	h = _derivative->checkBoundary(n);
	if(h == 0)
	{
		i=0;
		k=1;
		while(k <= _derivative->_positiveEnd)
		{
			idx  = seriesIndex->Index(m,n+k,p);
			idx2 = seriesIndex->Index(m,n-k,p);
			_divergE += _derivative->coefficients[i] * (_fields[idx].E.y - _fields[idx2].E.y);
			_divergH += _derivative->coefficients[i] * (_fields[idx].H.y - _fields[idx2].H.y);
			k++;
			i++;
		}
	}
	else
	{
		i=0;
		k=1;
		while(k <= _derivative->_positiveEnd)
		{
			idx = seriesIndex->Index(m,n+k,p);
			_divergE += _derivative->coefficients[i] * (_fields[idx].E.y - _fields[index].E.y);
			_divergH += _derivative->coefficients[i] * (_fields[idx].H.y - _fields[index].H.y);
			k++;
			i++;
		}
		k=-1;
		while(k >= _derivative->_negativeEnd)
		{
			idx = seriesIndex->Index(m,n+k,p);
			_divergE += _derivative->coefficients[i] * (_fields[idx].E.y - _fields[index].E.y);
			_divergH += _derivative->coefficients[i] * (_fields[idx].H.y - _fields[index].H.y);
			k--;
			i++;
		}
	}
	//dFz/dz
	h = _derivative->checkBoundary(p);
	if(h == 0)
	{
		i=0;
		k=1;
		while(k <= _derivative->_positiveEnd)
		{
			idx  = seriesIndex->Index(m,n,p+k);
			idx2 = seriesIndex->Index(m,n,p-k);
			_divergE += _derivative->coefficients[i] * (_fields[idx].E.z - _fields[idx2].E.z);
			_divergH += _derivative->coefficients[i] * (_fields[idx].H.z - _fields[idx2].H.z);
			k++;
			i++;
		}
	}
	else
	{
		i=0;
		k=1;
		while(k <= _derivative->_positiveEnd)
		{
			idx = seriesIndex->Index(m,n,p+k);
			_divergE += _derivative->coefficients[i] * (_fields[idx].E.z - _fields[index].E.z);
			_divergH += _derivative->coefficients[i] * (_fields[idx].H.z - _fields[index].H.z);
			k++;
			i++;
		}
		k=-1;
		while(k >= _derivative->_negativeEnd)
		{
			idx = seriesIndex->Index(m,n,p+k);
			_divergE += _derivative->coefficients[i] * (_fields[idx].E.z - _fields[index].E.z);
			_divergH += _derivative->coefficients[i] * (_fields[idx].H.z - _fields[index].H.z);
			k--;
			i++;
		}
	}
	//
	_divergE /= ds2;
	_divergH /= ds2;
	//
	_sumDivgE += abs(_divergE);
	_sumDivgH += abs(_divergH);
	//
	MakeStatistics(_fields, index, r);
	//
	index++;
}
///////////////////////////////////////////////////////////////

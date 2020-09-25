/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "CurlEstimatorAsymmetric.h"
#include "TssInSphere.h"

CurlEstimatorAsymmetric::CurlEstimatorAsymmetric(DerivativeEstimatorAsymmetric *derivative)
{
	_derivative = derivative;
	_fields = NULL;
	_curls = NULL;
	if(_derivative != NULL)
	{
		_derivative->shareIndexCacheTo(this);
		ret = ERR_OK;
	}
	else
	{
		ret = ERR_TSS_DERIVATIVE;
	}
}
void CurlEstimatorAsymmetric::SetFields(FieldPoint3D *fields, FieldPoint3D *curls)
{
	_fields = fields;
	_curls = curls;
	index = 0;
}
void CurlEstimatorAsymmetric::handleData(int m, int n, int p)
{
	int h;
	int k,i;
	_curls[index].E.x = _curls[index].H.x = _curls[index].E.y = _curls[index].H.y = _curls[index].E.z = _curls[index].H.z = 0.0;
	//
	//get dy
	h = _derivative->checkBoundary(n);
	if(h == 0)
	{
		//_positiveEnd = M, _negativeEnd=-M
		//_derivative->coefficients[i] = - _derivative->coefficients[i+M], i=0,1,2,...,M-1
		i=0; //i=0,1,2,...,_positiveEnd-1
		k=1;
		while(k <= _derivative->_positiveEnd)
		{
			idx  = seriesIndex->Index(m,n+k,p);
			idx2 = seriesIndex->Index(m,n-k,p);
			_curls[index].E.x += _derivative->coefficients[i] * (_fields[idx].E.z - _fields[idx2].E.z);
			_curls[index].H.x += _derivative->coefficients[i] * (_fields[idx].H.z - _fields[idx2].H.z);
			_curls[index].E.z -= _derivative->coefficients[i] * (_fields[idx].E.x - _fields[idx2].E.x);
			_curls[index].H.z -= _derivative->coefficients[i] * (_fields[idx].H.x - _fields[idx2].H.x);
			k++;
			i++;
		}
	}
	else
	{
		i=0; //i=0,1,2,...,_positiveEnd-1
		k=1;
		while(k <= _derivative->_positiveEnd)
		{
			idx = seriesIndex->Index(m,n+k,p);
			_curls[index].E.x += _derivative->coefficients[i] * (_fields[idx].E.z - _fields[index].E.z);
			_curls[index].H.x += _derivative->coefficients[i] * (_fields[idx].H.z - _fields[index].H.z);
			_curls[index].E.z -= _derivative->coefficients[i] * (_fields[idx].E.x - _fields[index].E.x);
			_curls[index].H.z -= _derivative->coefficients[i] * (_fields[idx].H.x - _fields[index].H.x);
			k++;
			i++;
		}
		//i=_positiveEnd, _positiveEnd+1, _positiveEnd+2,...,_positiveEnd+(-_negativeEnd-1)=2M-1
		k=-1;
		while(k >= _derivative->_negativeEnd)
		{
			idx = seriesIndex->Index(m,n+k,p);
			_curls[index].E.x += _derivative->coefficients[i] * (_fields[idx].E.z - _fields[index].E.z);
			_curls[index].H.x += _derivative->coefficients[i] * (_fields[idx].H.z - _fields[index].H.z);
			_curls[index].E.z -= _derivative->coefficients[i] * (_fields[idx].E.x - _fields[index].E.x);
			_curls[index].H.z -= _derivative->coefficients[i] * (_fields[idx].H.x - _fields[index].H.x);
			k--;
			i++;
		}
	}
	//get dz
	h = _derivative->checkBoundary(p);
	if(h == 0)
	{
		i=0;
		k=1;
		while(k <= _derivative->_positiveEnd)
		{
			idx  = seriesIndex->Index(m,n,p+k);
			idx2 = seriesIndex->Index(m,n,p-k);
			_curls[index].E.x -= _derivative->coefficients[i] * (_fields[idx].E.y - _fields[idx2].E.y);
			_curls[index].H.x -= _derivative->coefficients[i] * (_fields[idx].H.y - _fields[idx2].H.y);
			_curls[index].E.y += _derivative->coefficients[i] * (_fields[idx].E.x - _fields[idx2].E.x);
			_curls[index].H.y += _derivative->coefficients[i] * (_fields[idx].H.x - _fields[idx2].H.x);
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
			_curls[index].E.x -= _derivative->coefficients[i] * (_fields[idx].E.y - _fields[index].E.y);
			_curls[index].H.x -= _derivative->coefficients[i] * (_fields[idx].H.y - _fields[index].H.y);
			_curls[index].E.y += _derivative->coefficients[i] * (_fields[idx].E.x - _fields[index].E.x);
			_curls[index].H.y += _derivative->coefficients[i] * (_fields[idx].H.x - _fields[index].H.x);
			k++;
			i++;
		}
		k=-1;
		while(k >= _derivative->_negativeEnd)
		{
			idx = seriesIndex->Index(m,n,p+k);
			_curls[index].E.x -= _derivative->coefficients[i] * (_fields[idx].E.y - _fields[index].E.y);
			_curls[index].H.x -= _derivative->coefficients[i] * (_fields[idx].H.y - _fields[index].H.y);
			_curls[index].E.y += _derivative->coefficients[i] * (_fields[idx].E.x - _fields[index].E.x);
			_curls[index].H.y += _derivative->coefficients[i] * (_fields[idx].H.x - _fields[index].H.x);
			k--;
			i++;
		}
	}
	//get dx
	h = _derivative->checkBoundary(m);
	if(h == 0)
	{
		i=0;
		k=1;
		while(k <= _derivative->_positiveEnd)
		{
			idx  = seriesIndex->Index(m+k,n,p);
			idx2 = seriesIndex->Index(m-k,n,p);
			_curls[index].E.y -= _derivative->coefficients[i] * (_fields[idx].E.z - _fields[idx2].E.z);
			_curls[index].H.y -= _derivative->coefficients[i] * (_fields[idx].H.z - _fields[idx2].H.z);
			_curls[index].E.z += _derivative->coefficients[i] * (_fields[idx].E.y - _fields[idx2].E.y);
			_curls[index].H.z += _derivative->coefficients[i] * (_fields[idx].H.y - _fields[idx2].H.y);
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
			_curls[index].E.y -= _derivative->coefficients[i] * (_fields[idx].E.z - _fields[index].E.z);
			_curls[index].H.y -= _derivative->coefficients[i] * (_fields[idx].H.z - _fields[index].H.z);
			_curls[index].E.z += _derivative->coefficients[i] * (_fields[idx].E.y - _fields[index].E.y);
			_curls[index].H.z += _derivative->coefficients[i] * (_fields[idx].H.y - _fields[index].H.y);
			k++;
			i++;
		}
		k=-1;
		while(k >= _derivative->_negativeEnd)
		{
			idx = seriesIndex->Index(m+k,n,p);
			_curls[index].E.y -= _derivative->coefficients[i] * (_fields[idx].E.z - _fields[index].E.z);
			_curls[index].H.y -= _derivative->coefficients[i] * (_fields[idx].H.z - _fields[index].H.z);
			_curls[index].E.z += _derivative->coefficients[i] * (_fields[idx].E.y - _fields[index].E.y);
			_curls[index].H.z += _derivative->coefficients[i] * (_fields[idx].H.y - _fields[index].H.y);
			k--;
			i++;
		}
	}
	//
	index++;
}


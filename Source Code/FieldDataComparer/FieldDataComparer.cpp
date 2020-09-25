/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "FieldDataComparer.h"
#include "FieldCompareResult.h"
#include <malloc.h>
#include <float.h>
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>

FieldDataComparer::FieldDataComparer(RadiusIndexToSeriesIndex *idxCache, int maxR)
{
	seriesIndex = idxCache;
	maxRadius = maxR;
	_results = (FieldCompareResult *)malloc((maxR+1) * sizeof(FieldCompareResult));
	if(_results == NULL)
		ret = ERR_OUTOFMEMORY;
	else
	{
		ret = ERR_OK;
	}
}
FieldDataComparer::~FieldDataComparer()
{
	if(_results != NULL)
	{
		free(_results);
	}
}

void FieldDataComparer::setFields(FieldPoint3D *A, FieldPoint3D *B)
{
	_FieldsA = A;
	_FieldsB = B;
}


int FieldDataComparer::compareDataByRadius()
{
	for(r=0;r<=maxRadius;r++)
	{
		_results[r].sumDiff.E.x = 0.0;
		_results[r].sumDiff.E.y = 0.0;
		_results[r].sumDiff.E.z = 0.0;
		_results[r].sumDiff.H.x = 0.0;
		_results[r].sumDiff.H.y = 0.0;
		_results[r].sumDiff.H.z = 0.0;
		_results[r].sumField.E.x = 0.0;
		_results[r].sumField.E.y = 0.0;
		_results[r].sumField.E.z = 0.0;
		_results[r].sumField.H.x = 0.0;
		_results[r].sumField.H.y = 0.0;
		_results[r].sumField.H.z = 0.0;
	}
	ret = gothroughSphere(maxRadius);
	return ret;
}

FieldCompareResult *FieldDataComparer::GetResults()
{
	return _results;
}
//
void FieldDataComparer::handleData(int m, int n, int p)
{
	_results[r].sumDiff.E.x += abs(_FieldsA[index].E.x - _FieldsB[index].E.x);
	_results[r].sumDiff.E.y += abs(_FieldsA[index].E.y - _FieldsB[index].E.y);
	_results[r].sumDiff.E.z += abs(_FieldsA[index].E.z - _FieldsB[index].E.z);
	_results[r].sumDiff.H.x += abs(_FieldsA[index].H.x - _FieldsB[index].H.x);
	_results[r].sumDiff.H.y += abs(_FieldsA[index].H.y - _FieldsB[index].H.y);
	_results[r].sumDiff.H.z += abs(_FieldsA[index].H.z - _FieldsB[index].H.z);
	//
	_results[r].sumField.E.x += abs(_FieldsA[index].E.x);
	_results[r].sumField.E.y += abs(_FieldsA[index].E.y);
	_results[r].sumField.E.z += abs(_FieldsA[index].E.z);
	_results[r].sumField.H.x += abs(_FieldsA[index].H.x);
	_results[r].sumField.H.y += abs(_FieldsA[index].H.y);
	_results[r].sumField.H.z += abs(_FieldsA[index].H.z);
	//
	index ++;
}



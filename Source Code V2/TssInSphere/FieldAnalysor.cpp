/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "FieldAnalysor.h"

#include <malloc.h>
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  

FieldAnalysor::FieldAnalysor(RadiusIndexToSeriesIndex* indexCache)
{
	ret = ERR_OK;
	//physics constants ==============
	c0 = 299792458; //speed of light
	mu0 = 4.0 * M_PI * 1.0e-7;
	eps0 = 1.0 /(mu0 * c0 * c0);
	//=================================
	seriesIndex = indexCache;
	_fields = NULL;
	_divergenceEstimator = NULL;
	_derivativeAsymmetric = NULL;
	index = 0;
}
FieldAnalysor::~FieldAnalysor(void)
{
	cleanup();
}
void FieldAnalysor::cleanup()
{
	if(_divergenceEstimator != NULL)
	{
		delete _divergenceEstimator;
		_divergenceEstimator = NULL;
	}
	if(_derivativeAsymmetric != NULL)
	{
		delete _derivativeAsymmetric;
		_derivativeAsymmetric = NULL;
	}
	_fields = NULL;
}
int FieldAnalysor::setFields(FieldPoint3D *fields, int maxR, double spaceStep, int halfOrder)
{
	cleanup();
	_fields = fields;
	if (maxR <= 0)
	{
		ret = ERR_INVALID_SIZE;
	}
	else
	{
		maxRadius = maxR;
		ds = spaceStep;
		_halfOrder = halfOrder;
		_derivativeAsymmetric = new DerivativeEstimatorAsymmetric(_halfOrder, maxRadius, seriesIndex);
		_divergenceEstimator = new FieldStatisticsByDivergenceAsymmetric(_derivativeAsymmetric, _fields, ds);
		_derivativeAsymmetric->prepareCoefficeints();
		ret = _derivativeAsymmetric->GetLastHandlerError();
		if (ret == ERR_OK)
		{
			ret = _divergenceEstimator->AllocateList(maxRadius);
		}
	}
	return ret;
}
int FieldAnalysor::execute()
{
	ret = RADIUSINDEXMAPEXIST;
	if(ret == ERR_OK)
	{
		_divergenceEstimator->SetField(_fields);
		ret =_divergenceEstimator->gothroughSphere(maxRadius);
		if(ret == ERR_OK)
		{
			dataByRadius = _divergenceEstimator->GetList();
			if(dataByRadius != NULL)
			{
				index = 0;
				ret = this->gothroughSphere(maxRadius, ds);
			}
		}
	}
	return ret;
}
void FieldAnalysor::handleData(double x, double y, double z)
{
	/*
	Field energy = (eps0 *||E||^2 + mu0*||H||^2) / 2
	*/
	se =
		(eps0 * (_fields[index].E.x * _fields[index].E.x + _fields[index].E.y * _fields[index].E.y + _fields[index].E.z * _fields[index].E.z)
		+ mu0 * (_fields[index].H.x * _fields[index].H.x + _fields[index].H.y * _fields[index].H.y + _fields[index].H.z * _fields[index].H.z))/2.0;
	dataByRadius[r].sumEnergy += se;
	//S = E X H
	sx = _fields[index].E.y * _fields[index].H.z - _fields[index].H.y * _fields[index].E.z;
	sy = _fields[index].E.z * _fields[index].H.x - _fields[index].H.z * _fields[index].E.x;
	sz = _fields[index].E.x * _fields[index].H.y - _fields[index].H.x * _fields[index].E.y;
	//
	sm = sqrt(sx * sx + sy * sy + sz * sz); //||S||
	sa = x * sx + y * sy + z * sz;          // (x, y, z) dot S
	//use energy level to set a magnitude threhold for determining orthogonal energy
	if(se > 1.0)
		se = 1.0e-8;
	else
		se = 1.0e-8 * se;
	//
	if(sa > se) //(x, y, z) dot S > 0, S is in the same direction of (x,y,z) -> (0,0,0)
	{
		dataByRadius[r].sumEnergyInwards += sm;
	}
	else if(sa < -se) //(x, y, z) dot S < 0, S is in the opposit direction of (x,y,z) -> (0,0,0)
	{
		dataByRadius[r].sumEnergyOutwards += sm;
	}
	else //S is orthogonal to (x,y,z) -> (0,0,0)
	{
		dataByRadius[r].sumEnergyCircular += sm;
	}
	//
	index++;
}
void FieldAnalysor::ShowDetails(fnProgressReport reporter)
{
	_divergenceEstimator->ShowDetails(reporter);
}
void FieldAnalysor::ShowReport(fnProgressReport reporter)
{
	_divergenceEstimator->ShowReport(reporter);
}
int FieldAnalysor::WriteDivegenceToFile(FILE *filehandle)
{
	return _divergenceEstimator->WriteDivegenceToFile(filehandle);
}
double FieldAnalysor::getAverageDivergenceE()
{
	return _divergenceEstimator->GetAverageDivergenceE();
}
double FieldAnalysor::getAverageDivergenceH()
{
	return _divergenceEstimator->GetAverageDivergenceH();
}
//
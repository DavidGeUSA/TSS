
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/
#include "FieldSourceSphereCurrent.h"
#include <malloc.h>
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  

FieldSourceSphereCurrent::FieldSourceSphereCurrent(void)
{
	_fields = NULL;
	_magnitude = 0.0;
	_densityAtPoint = 0;
}


FieldSourceSphereCurrent::~FieldSourceSphereCurrent(void)
{
}

void FieldSourceSphereCurrent::reset(FieldPoint3D *fields, double magnitude, size_t timeIndex)
{
	_fields = fields;
	_magnitude = magnitude;
	_timeIndex = timeIndex;
	_densityAtPoint = 0.0;
	ret = ERR_OK;
	index = 0;
}
void FieldSourceSphereCurrent::handleData(int m, int n, int p)
{
	if(r > _timeIndex)
	{
		_densityAtPoint = (double)m * (double)m + (double)n * (double)n + (double)p * (double)p;
		_densityAtPoint = _magnitude /(_densityAtPoint * sqrt(_densityAtPoint));
		_fields[index].E.x -= _densityAtPoint * (double)m;
		_fields[index].E.y -= _densityAtPoint * (double)n;
		_fields[index].E.z -= _densityAtPoint * (double)p;
	}
	index++;
}


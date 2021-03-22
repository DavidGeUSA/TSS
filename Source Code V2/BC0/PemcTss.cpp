/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/02/2020
Allrights reserved by David Ge

Modifications
Date            Author      Description
---------------------------------------------
2021-03-06      David Ge    Linux compatibility
********************************************************************/
#include "PemcTss.h"


PemcTss::PemcTss()
{
}


PemcTss::~PemcTss()
{
}

int PemcTss::initialize(double Courant, int maximumRadius, TaskFile *taskParameters)
{
	return ERR_OK;
}
RadiusHandleType PemcTss::setRadius(int radius)
{
	if (radius < maxRadius)
		return DoNotProcess;
	return NeedProcess;
}
void PemcTss::handleData(int m, int n, int p)
{
	size_t idx = SINDEX(m, n, p);
	if (m == -maxRadius || m == maxRadius)
	{
		if (n == -maxRadius || n == maxRadius)
		{
			_fields[idx].E.x = 0.0; _fields[idx].E.y = 0.0; _fields[idx].E.z = 0.0;
			_fields[idx].H.x = 0.0; _fields[idx].H.y = 0.0; _fields[idx].H.z = 0.0;
		}
		else if (p == -maxRadius || p == maxRadius)
		{
			_fields[idx].E.x = 0.0; _fields[idx].E.y = 0.0; _fields[idx].E.z = 0.0;
			_fields[idx].H.x = 0.0; _fields[idx].H.y = 0.0; _fields[idx].H.z = 0.0;
		}
		else
		{
			 _fields[idx].E.y = 0.0; _fields[idx].E.z = 0.0;
			 _fields[idx].H.y = 0.0; _fields[idx].H.z = 0.0;
		}
	}
	else if (n == -maxRadius || n == maxRadius)
	{
		if (p == -maxRadius || p == maxRadius)
		{
			_fields[idx].E.x = 0.0; _fields[idx].E.y = 0.0; _fields[idx].E.z = 0.0;
			_fields[idx].H.x = 0.0; _fields[idx].H.y = 0.0; _fields[idx].H.z = 0.0;
		}
		else
		{
			_fields[idx].E.x = 0.0; _fields[idx].E.z = 0.0;
			_fields[idx].H.x = 0.0; _fields[idx].H.z = 0.0;
		}
	}
	else
	{
		_fields[idx].E.x = 0.0; _fields[idx].E.y = 0.0;
		_fields[idx].H.x = 0.0; _fields[idx].H.y = 0.0;
	}
}
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "ApplyCurlsInhomogeneous.h"

void ApplyCurlsEvenInhomogeneous::handleData(int m, int n, int p)
{
	_fields[index].E.x += _factorE[index] * _curls[index].E.x;
	_fields[index].E.y += _factorE[index] * _curls[index].E.y;
	_fields[index].E.z += _factorE[index] * _curls[index].E.z;
	_fields[index].H.x += _factorH[index] * _curls[index].H.x;
	_fields[index].H.y += _factorH[index] * _curls[index].H.y;
	_fields[index].H.z += _factorH[index] * _curls[index].H.z;
	//
	index++;
}
void ApplyCurlsOddInhomogeneous::handleData(int m, int n, int p)
{
	_fields[index].E.x += _factorH[index] * _curls[index].H.x;
	_fields[index].E.y += _factorH[index] * _curls[index].H.y;
	_fields[index].E.z += _factorH[index] * _curls[index].H.z;
	_fields[index].H.x += _factorE[index] * _curls[index].E.x;
	_fields[index].H.y += _factorE[index] * _curls[index].E.y;
	_fields[index].H.z += _factorE[index] * _curls[index].E.z;
	//
	index++;
}


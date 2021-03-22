/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "ApplyCurls.h"

ApplyCurls::ApplyCurls()
{
}
void ApplyCurls::SetFields(FieldPoint3D *fields, FieldPoint3D *curls, double *factorE, double *factorH)
{
	_fields = fields;
	_curls = curls;
	_factorE = factorE;
	_factorH = factorH;
	index = 0;
}
void ApplyCurlsEven::handleData(int m, int n, int p)
{
	_fields[index].E.x += *_factorE * _curls[index].E.x;
	_fields[index].E.y += *_factorE * _curls[index].E.y;
	_fields[index].E.z += *_factorE * _curls[index].E.z;
	_fields[index].H.x += *_factorH * _curls[index].H.x;
	_fields[index].H.y += *_factorH * _curls[index].H.y;
	_fields[index].H.z += *_factorH * _curls[index].H.z;
	//
	index++;
}
void ApplyCurlsOdd::handleData(int m, int n, int p)
{
	_fields[index].E.x += *_factorH * _curls[index].H.x;
	_fields[index].E.y += *_factorH * _curls[index].H.y;
	_fields[index].E.z += *_factorH * _curls[index].H.z;
	_fields[index].H.x += *_factorE * _curls[index].E.x;
	_fields[index].H.y += *_factorE * _curls[index].E.y;
	_fields[index].H.z += *_factorE * _curls[index].E.z;
	//
	index++;
}


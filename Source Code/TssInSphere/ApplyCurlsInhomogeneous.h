#pragma once
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "ApplyCurls.h"

/*
	apply curls for advancing fields in time at order 2k
*/
class ApplyCurlsEvenInhomogeneous:public ApplyCurlsEven
{
public:
	virtual void handleData(int m, int n, int p);
};

/*
	apply curls for advancing fields in time at order 2k+1
*/
class ApplyCurlsOddInhomogeneous:public ApplyCurlsOdd
{
public:
	virtual void handleData(int m, int n, int p);
};


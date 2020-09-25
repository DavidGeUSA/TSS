/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/

#include "TssFDTD.h"

TssFDTD::TssFDTD(void)
{
}

TssFDTD::~TssFDTD(void)
{
}
///////////////////////////////////////////////////////
TssFDTDinhomo::TssFDTDinhomo(void)
{
}

TssFDTDinhomo::~TssFDTDinhomo(void)
{
}
/*
	assign values to space-location-dependent Permeability and Permittivity
*/
int TssFDTDinhomo::initializeInhomogeneous()
{
	InitInhomogeneousParameters iip(mu, eps);
	int ret = iip.gothroughSphere(maxRadius);
	return ret;
}

/*
	it is called after onInitInhomogeneous. default is doing nothing.
	for this sample, free memories mu and eps because they are no longer need
*/
int TssFDTDinhomo::onPreparedInhomogeneous()
{
	if(mu != NULL)
	{
		FreeMemory(mu);
		mu = NULL;
	}
	if(eps != NULL)
	{
		FreeMemory(eps);
		eps = NULL;
	}
	return ERR_OK;
}
//////////////////////////////////////////////
InitInhomogeneousParameters::InitInhomogeneousParameters(double *mu, double *eps)
{
	//physics constants ==============
	c0 = 299792458; //speed of light
	mu0 = 4.0 * M_PI * 1.0e-7;
	eps0 = 1.0 /(mu0 * c0 * c0);
	//=================================
	//
	_mu = mu;
	_eps = eps;
	index = 0;
}

/*
	a sample inhomogeneous space.
	this function defines what the inhomogeneous environments are.
*/
void InitInhomogeneousParameters::handleData(int m, int n, int p)
{
	if(m > 0) //the space location is at positive x-axis
	{
		_mu[index] = mu0;
		_eps[index] = eps0 * 1.5; //this is where inhomogeneous presents
	}
	else //the space location is at 0 or negative x-axis
	{
		_mu[index] = mu0;
		_eps[index] = eps0;
	}
	index++;
}
//////////////////////////////////////////////
//


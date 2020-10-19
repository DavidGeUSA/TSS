/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 03/31/2018
Allrights reserved by David Ge

********************************************************************/
#include "ArbitraryTimeOrder.h"
#include "..\MathTools\MathTools.h"
#include "..\EMField\EMField.h"
#include <malloc.h>
#include <stdio.h>

ArbitraryTimeOrder::ArbitraryTimeOrder()
{
	//physics constants ==============
	c0 = 299792458.0; //speed of light
	mu0 = 4.0 * M_PI * 1.0e-7;
	eps0 = 1.0 / (mu0 * c0 * c0);
	//=================================
	ht = 0;
	ae = NULL;
	ah = NULL;
}


ArbitraryTimeOrder::~ArbitraryTimeOrder()
{
	cleanup();
}

void ArbitraryTimeOrder::cleanup()
{
	if (ae != NULL){ free(ae); ae = NULL; }
	if (ah != NULL){ free(ah); ah = NULL; }
	ht = 0;
}

int ArbitraryTimeOrder::allocateMemory(unsigned int halfOrder)
{
	int ret = ERR_OK;
	if (halfOrder == 0)
	{
		ret = ERR_MATH_INV_DIM;
	}
	else
	{
		cleanup();
		ht = halfOrder;
		ae = (double *)malloc(2*ht * sizeof(double));
		if (ae == NULL)
		{
			ret = ERR_MATH_OUTOFMEMORY;
		}
		else
		{
			ah = (double *)malloc(2*ht * sizeof(double));
			if (ah == NULL)
			{
				ret = ERR_MATH_OUTOFMEMORY;
			}
		}
	}
	return ret;
}

int ArbitraryTimeOrder::CalculateCoefficients(unsigned int halfOrder, double courant)
{
	int ret = allocateMemory(halfOrder);
	if (ret == ERR_OK)
	{
		int m = 2 * ht;
		/*
		β_e=-∆_t/(μ∆_s ), β_h=∆_t/(ε∆_s )
		a_e[0] = 1, a_h[0] = 1
		a_e[k] = (β_e / k) a_h[k - 1], a_h[k] = (β_h / k) a_e[k - 1]

		*/
		//dt = (ds / c0) * courant;
		double be = -courant / (c0 * mu0);  //dt / (mu0 * ds) ;
		double bh =  courant / (c0 * eps0); //dt / (eps0 * ds);
		//
		ae[0] = 1.0; ah[0] = 1.0;
		for (int i = 1; i < m; i++)
		{
			ae[i] = (be / (double)i)*ah[i - 1];
			ah[i] = (bh / (double)i)*ae[i - 1];
		}
	}
	return ret;
}
///////////////////////////////////////////////////////////////////
ArbitraryOrderTimeAdvance::ArbitraryOrderTimeAdvance()
{
	_field = NULL;
	ret = ERR_OK;
	index = 0;
	//
	//dxp=dxn=dyp=dyn=dzp=dzn = NULL;
	curlOdd = NULL;
	curlEven = NULL;
	//
	curl = NULL;
	field = NULL;
}
ArbitraryOrderTimeAdvance::~ArbitraryOrderTimeAdvance()
{

}
void ArbitraryOrderTimeAdvance::setMaxRadius(RadiusIndexToSeriesIndex *cache, int maxR, ArbitraryTimeOrder* i_timeCoefficients, ArbitrarySpaceOrder* i_spaceCoefficients)
{
	seriesIndex = cache;
	maxRadius = maxR;
	maxRadiusNeg = -maxR;
	timeCoefficients = i_timeCoefficients;
	spaceCoefficients = i_spaceCoefficients;
	hs = spaceCoefficients->ArraySize();
	a = spaceCoefficients->coefficients();
}
void ArbitraryOrderTimeAdvance::reset(FieldPoint3D *field, FieldPoint3D *i_curlOdd, FieldPoint3D *i_curlEven)
{
	_field = field;
	curlOdd = i_curlOdd;
	curlEven = i_curlEven;
	index = 0;
}

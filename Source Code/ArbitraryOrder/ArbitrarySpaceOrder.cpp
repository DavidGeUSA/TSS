/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 03/31/2018
Allrights reserved by David Ge

********************************************************************/
#include "ArbitrarySpaceOrder.h"
#include "..\MathTools\MathTools.h"
#include "..\MathTools\Matrix.h"
#include <stdio.h>
#include <malloc.h>

ArbitrarySpaceOrder::ArbitrarySpaceOrder()
{
	hs = 0;
	a = NULL;
	A = NULL;
}


ArbitrarySpaceOrder::~ArbitrarySpaceOrder()
{
	cleanup();
}
void ArbitrarySpaceOrder::cleanup()
{
	if (a != NULL){ free(a); a = NULL; }
	if (A != NULL){ free(A); A = NULL; }
	hs = 0;
}
int ArbitrarySpaceOrder::allocateMemory(unsigned int halfOrder)
{
	int ret = ERR_OK;
	if (halfOrder == 0)
	{
		ret = ERR_MATH_INV_DIM;
	}
	else
	{
		cleanup();
		hs = halfOrder;
		a = (double *)malloc(hs * sizeof(double));
		if (a == NULL)
		{
			ret = ERR_MATH_OUTOFMEMORY;
		}
		else
		{
			A = (double *)malloc(hs * hs * sizeof(double));
			if (A == NULL)
			{
				ret = ERR_MATH_OUTOFMEMORY;
			}
		}
	}
	return ret;
}

int ArbitrarySpaceOrder::CalculateCoefficients(unsigned int halfOrder)
{
	int ret = allocateMemory(halfOrder);
	if (ret == ERR_OK)
	{
		ret = fillMatrix();
	}
	if (ret == ERR_OK)
	{
		double *AI = (double *)malloc(hs*hs*sizeof(double));
		if (AI == NULL)
		{
			ret = ERR_MATH_OUTOFMEMORY;
		}
		else
		{
			if (inverse(A, AI, hs))
			{
				for (unsigned int i = 0; i < hs; i++)
				{
					a[i] = AI[i];
				}
			}
			else
			{
				ret = ERR_MATH_NOT_INVERSE;
			}
		}
	}
	return ret;
}
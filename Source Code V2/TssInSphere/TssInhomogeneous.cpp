/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "TssInhomogeneous.h"
#include "ApplyCurlsInhomogeneous.h"

TssInhomogeneous::TssInhomogeneous(void)
{
}

TssInhomogeneous::~TssInhomogeneous(void)
{
}

void TssInhomogeneous::cleanup()
{
	TssInSphere::cleanup();
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
	if(ae0_a != NULL)
	{
		FreeMemory(ae0_a);
	}
	if(ae_a != NULL)
	{
		FreeMemory(ae_a);
	}
	if(ah0_a != NULL)
	{
		FreeMemory(ah0_a);
	}
	if(ah_a != NULL)
	{
		FreeMemory(ah_a);
	}
}

int TssInhomogeneous::onInitInhomogeneous()
{
	int ret = ERR_OK;
	//non-inhomogeneous:
	//dtmu  = -(dt/mu0) / ds;
	//dteps =  (dt/eps0) / ds;
	//inhomogeneous:
	for(size_t i = 0;i<fieldItems;i++)
	{
		dtmu[i] = -(dt/mu[i]) / ds;
		dteps[i] = (dt/eps[i]) / ds;
	}
	ret = onPreparedInhomogeneous();
	return ret;
}

/*
	initialize memories for space-location-dependent Permeability, Permittivity, and coefficients
*/
int TssInhomogeneous::onInitialized(TaskFile *taskParameters)
{
	int ret = TssInSphere::onInitialized(taskParameters);
	if(ret == ERR_OK)
	{
		size_t sz = fieldItems * sizeof(double); //memory size for space-location-dependent doubles
		//space-location-dependent Permeability
		mu = (double *)AllocateMemory(sz);
		if(mu == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		else
		{
			//space-location-dependent Permittivity
			eps = (double *)AllocateMemory(sz);
			if(eps == NULL)
			{
				ret = ERR_OUTOFMEMORY;
			}
			else
			{
				//coefficients computed from Permeability, Permittivity and Courant number
				dtmu = (double *)AllocateMemory(sz);
				if(dtmu == NULL)
				{
					ret = ERR_OUTOFMEMORY;
				}
				else
				{
					dteps = (double *)AllocateMemory(sz);
					if(dteps == NULL)
					{
						ret = ERR_OUTOFMEMORY;
					}
				}
			}
			if(ret == ERR_OK)
			{
				//TSS algorithm factors to update fields
				ae0_a = (double *)AllocateMemory(sz);
				if(ae0_a == NULL)
				{
					ret = ERR_OUTOFMEMORY;
				}
				else
				{
					ae_a = (double *)AllocateMemory(sz);
					if(ae_a == NULL)
					{
						ret = ERR_OUTOFMEMORY;
					}
					else
					{
						ah_a = (double *)AllocateMemory(sz);
						if(ah_a == NULL)
						{
							ret = ERR_OUTOFMEMORY;
						}
						else
						{
							ah0_a = (double *)AllocateMemory(sz);
							if(ah0_a == NULL)
							{
								ret = ERR_OUTOFMEMORY;
							}
						}
					}
				}
			}
			if(ret == ERR_OK)
			{
				ret = initializeInhomogeneous();
				if(ret == ERR_OK)
				{
					ret = onInitInhomogeneous();
				}
			}
		}
	}
	return ret;
}

void TssInhomogeneous::createCurlGenerators()
{
	_applyCurlsEven = new ApplyCurlsEvenInhomogeneous();
	_applyCurlsOdd = new ApplyCurlsOddInhomogeneous();
}
/*
	apply curls of orders 2k and 2k+1
	to time advancement
	k>=0
*/
int TssInhomogeneous::applyCurls(int k)
{
	int ret = ERR_OK;
	double kd = 2.0 * (double)k;
	size_t i;
	//curl estimation of order 2k, it is even order
	if(k == 0) //order 0
	{
		for(i=0;i<fieldItems;i++)
		{
			ae_a[i] = ah_a[i] = 1.0;
		}
		curl1 = HE; //order 0 curl estimation is the field itself
	}
	else
	{
		//kd is the estimation order, it can be 2, 4, 6, ...
		for(i=0;i<fieldItems;i++)
		{
			ae0_a[i] = dtmu[i] * ah_a[i] / kd;
			ah0_a[i] = dteps[i] * ae_a[i] / kd;
		}
		for(i=0;i<fieldItems;i++)
		{
			ae_a[i] = ae0_a[i];
			ah_a[i] = ah0_a[i];
		}
		curl1 = Curls[1]; //Curls[1] holds curls from an even estimation order
		//from curl0 to get curl1, it is in Curl[1]
		_curlEstimate->SetFields(curl0, curl1);
		ret = _curlEstimate->gothroughSphere(maxRadius);
		if(ret == ERR_OK)
		{
			//use curl1 to get a time advance estimation
			_applyCurlsEven->SetFields(HE, curl1, ae_a, ah_a);
			ret = _applyCurlsEven->gothroughSphere(maxRadius);
		}
	}
	if(ret == ERR_OK)
	{
		//curl estimation of order 2k+1, it is odd order
		kd += 1.0;
		for(i=0;i<fieldItems;i++)
		{
			ae0_a[i] = dtmu[i] * ah_a[i] / kd;
			ah0_a[i] = dteps[i] * ae_a[i] / kd;
		}
		for(i=0;i<fieldItems;i++)
		{
			ae_a[i] = ae0_a[i];
			ah_a[i] = ah0_a[i];
		}

		curl0 = Curls[0]; //Curls[0] holds curls from an odd estimation order
		//from curl1 to get curl0
		_curlEstimate->SetFields(curl1, curl0);
		//estimating curl0, it is in Curls[0]
		ret = _curlEstimate->gothroughSphere(maxRadius);
		if(ret == ERR_OK)
		{
			//use curl0 to make time advance estimation
			_applyCurlsOdd->SetFields(HE, curl0, ae_a, ah_a);
			ret = _applyCurlsOdd->gothroughSphere(maxRadius);
		}
	}
	return ret;
}

////////////////////
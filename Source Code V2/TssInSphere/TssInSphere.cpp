﻿/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "TssInSphere.h"
#include <malloc.h>
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>
#include "../MemoryMan/memman.h"
#include "../FileUtil/fileutil.h"

TssInSphere::TssInSphere(void)
{
	//physics constants ==============
	c0 = 299792458.0; //speed of light
	mu0 = 4.0 * M_PI * 1.0e-7;
	eps0 = 1.0 /(mu0 * c0 * c0);
	//=================================
	//
	dt = 0.0;   //time step
	ds = 0.0;   //space step
	N = 0;
	maxN = 0;
	maxRadius = 0;
	fieldMemorySize = 0;
	fieldItems = 0;
	_timeIndex = 0;
	_showSummaryOnly = false;
	//
	HE = NULL;
	Curls = NULL;
	curlCount = 0;
	//
	_basefilename = NULL;
	_reporter = NULL;
	//
	_derivative = NULL;
	_curlEstimate = NULL;
	_fieldStatistics = NULL;
	//
}

TssInSphere::~TssInSphere(void)
{
	cleanup();
}
void TssInSphere::cleanup()
{
	if(_derivative != NULL)
	{
		delete _derivative;
		_derivative = NULL;
	}
	if(_curlEstimate != NULL)
	{
		delete _curlEstimate;
		_curlEstimate = NULL;
	}
	if(_fieldStatistics != NULL)
	{
		delete _fieldStatistics;
		_fieldStatistics = NULL;
	}
	if(HE != NULL)
	{
		FreeMemory(HE);
		HE = NULL;
	}
	if(Curls != NULL)
	{
		for(int i=0;i<curlCount;i++)
		{
			if(Curls[i] != NULL)
			{
				FreeMemory(Curls[i]);
				Curls[i] = NULL;
			}
		}
		free(Curls);
		Curls = NULL;
	}

}
void TssInSphere::OnFinishSimulation()
{
	cleanup();
}
void TssInSphere::setReporter(fnProgressReport reporter, bool showSummaryOnly)
{
	_reporter = reporter;
	_showSummaryOnly = showSummaryOnly;
}

int TssInSphere::onInitialized(TaskFile *taskParameters)
{
	int ret = ERR_OK;
	dtmu  = -(dt/mu0) / ds;
	dteps =  (dt/eps0) / ds;
	curlCount = 2;
	Curls = (FieldPoint3D **)malloc(curlCount * sizeof(FieldPoint3D *));
	if(Curls == NULL)
	{
		ret = ERR_OUTOFMEMORY;
	}
	else
	{
		for(int i=0;i<curlCount;i++)
		{
			Curls[i] = NULL;
		}
		for(int i=0;i<curlCount;i++)
		{
			Curls[i] = (FieldPoint3D *)AllocateMemory(fieldMemorySize);
			if(Curls[i] == NULL)
			{
				ret = ERR_OUTOFMEMORY;
				break;
			}
		}
	}
	if(ret == ERR_OK)
	{
		if(_derivative != NULL)
		{
			delete _derivative;
			_derivative = NULL;
		}
		if(_curlEstimate != NULL)
		{
			delete _curlEstimate;
			_curlEstimate = NULL;
		}
		if(_fieldStatistics != NULL)
		{
			delete _fieldStatistics;
			_fieldStatistics = NULL;
		}
		//
		_derivative = new DerivativeEstimatorAsymmetric(_maxOrderSpaceDerivative, maxRadius, seriesIndex);
		_fieldStatistics = new FieldStatisticsByDivergenceAsymmetric(_derivative, HE, ds);
		_curlEstimate = new CurlEstimatorAsymmetric(_derivative);
		if(_derivative->GetLastHandlerError() == ERR_OK && _fieldStatistics->GetLastHandlerError() == ERR_OK)
		{
			_derivative->prepareCoefficeints();
			if(_derivative->GetLastHandlerError() == ERR_OK)
			{
				ret = _fieldStatistics->AllocateList(maxRadius);
				if(ret == ERR_OK)
				{
					createCurlGenerators();
				}
			}
			else
			{
				ret = _derivative->GetLastHandlerError();
			}
		}
		else
		{
			ret = _derivative->GetLastHandlerError();
			if(ret == ERR_OK)
			{
				ret = _fieldStatistics->GetLastHandlerError();
			}
		}
	}
	return ret;
}
void TssInSphere::createCurlGenerators()
{
	_applyCurlsEven = new ApplyCurlsEven();
	_applyCurlsOdd = new ApplyCurlsOdd();
}
int TssInSphere::verifyFieldsByDivergence(FieldPoint3D *fields)
{
	int ret = ERR_OK;
	if(_reporter == NULL)
	{
		ret = ERR_TSS_REPORTER;
	}
	else
	{
		if(_fieldStatistics != NULL)
		{
			_fieldStatistics->SetField(fields==NULL?HE:fields);
			ret = _fieldStatistics->gothroughSphere(maxRadius);
		}
		else
		{
			ret = ERR_NOTINITIALIZED;
		}
		if(ret == ERR_OK)
		{
			if(!_showSummaryOnly)
			{
				_fieldStatistics->ShowDetails(_reporter);
			}
			_fieldStatistics->ShowSummary(_reporter);
		}
	}
	return ret;
}
int TssInSphere::verifyCurls()
{
	int ret = ERR_OK;
	curl1 = HE;
	curl0 = Curls[0];
	_curlEstimate->SetFields(curl1, curl0);
	ret = _curlEstimate->gothroughSphere(maxRadius);
	if(ret == ERR_OK)
	{
		ret = verifyFieldsByDivergence(curl0);
	}
	return ret;
}

/*
	apply curls of orders 2k and 2k+1
	to time advancement
	k>=0
*/
int TssInSphere::applyCurls(int k)
{
	int ret = ERR_OK;
	double kd = 2.0 * (double)k;
	//curl estimation of order 2k, it is even order
	if(k == 0) //order 0
	{
		ae = ah = 1.0;
		curl1 = HE; //order 0 curl estimation is the field itself
	}
	else
	{
		//kd is the estimation order, it can be 2, 4, 6, ...
		ae0 = dtmu * ah / kd;
		ah0 = dteps * ae / kd;
		ae = ae0;
		ah = ah0;
		curl1 = Curls[1]; //Curls[1] holds curls from an even estimation order
		//from curl0 to get curl1, it is in Curl[1]
		_curlEstimate->SetFields(curl0, curl1);
		ret = _curlEstimate->gothroughSphere(maxRadius);
		if(ret == ERR_OK)
		{
			//use curl1 to get a time advance estimation
			_applyCurlsEven->SetFields(HE, curl1, &ae, &ah);
			ret = _applyCurlsEven->gothroughSphere(maxRadius);
		}
	}
	if(ret == ERR_OK)
	{
		//curl estimation of order 2k+1, it is odd order
		kd += 1.0;
		ae0 = dtmu * ah / kd;
		ah0 = dteps * ae / kd;
		ae = ae0;
		ah = ah0;
		curl0 = Curls[0]; //Curls[0] holds curls from an odd estimation order
		//from curl1 to get curl0
		_curlEstimate->SetFields(curl1, curl0);
		//estimating curl0, it is in Curls[0]
		ret = _curlEstimate->gothroughSphere(maxRadius);
		if(ret == ERR_OK)
		{
			//use curl0 to make time advance estimation
			_applyCurlsOdd->SetFields(HE, curl0, &ae, &ah);
			ret = _applyCurlsOdd->gothroughSphere(maxRadius);
		}
	}
	return ret;
}

//uncomment it to do debug
//#define DEBUG2NDORDER
/*
	advance time forward by one step of dt
*/
int TssInSphere::updateFieldsToMoveForward()
{
	int ret = ERR_OK;
	//advance time indicators
	_timeIndex++;
	_time += dt;
	//save existing fields to a file and allocating new memory
	//it will copy the existing fields to new memory
	//fields are still at a time of _time-dt
	ret = allocateFieldMemory();
	if(ret == ERR_OK)
	{
		if(_recordFDTDStepTimes)
		{
			startTime = getTimeTick();
		}
		//DEBUG verify TSS via Ricker source ///////////////
#ifdef DEBUG2NDORDER
		size_t debugTime = 1;
		FieldPoint3D *f1 = NULL, *f2=NULL;
		if (_timeIndex == debugTime)
		{
			f1 = (FieldPoint3D *)AllocateMemory(fieldMemorySize);
			f2 = (FieldPoint3D *)AllocateMemory(fieldMemorySize);
			for (size_t i = 0; i<fieldItems; i++)
			{
				f1[i] = HE[i];
				f2[i] = HE[i];
			}
		}
#endif
		////////////////////////////////////////////////////
		//bring fields to _time
		//use each order of space curls to get each order of temporal derivative for advancing fields in time
		for(int k = 0; k < _maxOrderTimeAdvance; k++)
		{
#ifdef DEBUG2NDORDER
			if (_timeIndex == debugTime)
			{
				ret = ret;//break here
			}
#endif
			//for k=0, one first-order curl estimation is applied, resulting in a second-order time advance estimation
			//for k>0, one even order curl estimation is applied then one odd order curl estimation is applied, 
			//resulting in a 2-orders increase in time advance estimation
			ret = applyCurls(k);
			if(ret != ERR_OK)
			{
				break;
			}
		}
		//DEBUG verify 2nd order TSS  //////////////////
#ifdef DEBUG2NDORDER
#define IDX(m,n,p) seriesIndex->Index((m), (n), (p))

		if (_timeIndex == debugTime)
		{
			int rMax = (int)maxRadius;
			double mag;
			double aev = dt / (2.0*eps0*ds);//((1.0 / 2.0) / sqrt(3.0))*sqrt(mu0 / eps0); //1/(2√3) √(μ/ε)
			double ahv = dt / (2.0*mu0*ds);// ((1.0 / 2.0) / sqrt(3.0))*sqrt(eps0 / mu0); //1/(2√3) √(ε/μ)
			double errSum = 0.0, err;
			int bound = 3;
			for (int m = bound - rMax; m <= rMax - bound; m++)
			{
				for (int n = bound - rMax; n <= rMax - bound; n++)
				{
					for (int p = bound - rMax; p <= rMax - bound; p++)
					{
						size_t idx = seriesIndex->Index(m, n, p);
						//check ret here
						//Ex <- H_z (m,n+1,p)-H_z (m,n-1,p)-H_y (m,n,p+1)+H_y (m,n,p-1)
						//Ey <- H_x (m,n,p+1)-H_x (m,n,p-1)-H_z (m+1,n,p)+H_z (m-1,n,p)
						//Ez <- H_y (m+1,n,p)-H_y (m-1,n,p)-H_x (m,n+1,p)+H_x (m,n-1,p)
						f2[idx].E.x = f1[idx].E.x + (f1[IDX(m, n + 1, p)].H.z - f1[IDX(m, n - 1, p)].H.z - f1[IDX(m, n, p + 1)].H.y + f1[IDX(m, n, p - 1)].H.y)*aev;
						f2[idx].E.y = f1[idx].E.y + (f1[IDX(m, n, p + 1)].H.x - f1[IDX(m, n, p - 1)].H.x - f1[IDX(m + 1, n, p)].H.z + f1[IDX(m - 1, n, p)].H.z)*aev;
						f2[idx].E.z = f1[idx].E.z + (f1[IDX(m + 1, n, p)].H.y - f1[IDX(m - 1, n, p)].H.y - f1[IDX(m, n + 1, p)].H.x + f1[IDX(m, n - 1, p)].H.x)*aev;
						//Hx <- E_z (m,n+1,p)-E_z (m,n-1,p)-E_y (m,n,p+1)+E_y (m,n,p-1)
						//Hy <- E_x (m,n,p+1)-E_x (m,n,p-1)-E_z (m+1,n,p)+E_z (m-1,n,p)
						//Hz <- E_y (m+1,n,p)-E_y (m-1,n,p)-E_x (m,n+1,p)+E_x (m,n-1,p)
						f2[idx].H.x = f1[idx].H.x - (f1[IDX(m, n + 1, p)].E.z - f1[IDX(m, n - 1, p)].E.z - f1[IDX(m, n, p + 1)].E.y + f1[IDX(m, n, p - 1)].E.y)*ahv;
						f2[idx].H.y = f1[idx].H.y - (f1[IDX(m, n, p + 1)].E.x - f1[IDX(m, n, p - 1)].E.x - f1[IDX(m + 1, n, p)].E.z + f1[IDX(m - 1, n, p)].E.z)*ahv;
						f2[idx].H.z = f1[idx].H.z - (f1[IDX(m + 1, n, p)].E.y - f1[IDX(m - 1, n, p)].E.y - f1[IDX(m, n + 1, p)].E.x + f1[IDX(m, n - 1, p)].E.x)*ahv;
						//
						err = (f2[idx].E.x - HE[idx].E.x)*(f2[idx].E.x - HE[idx].E.x) + (f2[idx].E.y - HE[idx].E.y)*(f2[idx].E.y - HE[idx].E.y) + (f2[idx].E.z - HE[idx].E.z)*(f2[idx].E.z - HE[idx].E.z);
						mag = f2[idx].E.x*f2[idx].E.x + f2[idx].E.y*f2[idx].E.y + f2[idx].E.z*f2[idx].E.z;
						if (m == 0 && n == 1 && p == 0)
						{
							size_t ip1 = seriesIndex->Index(m, n, p + 1);
							size_t ip2 = seriesIndex->Index(m, n, p - 1);
							size_t im1 = seriesIndex->Index(m + 1, n, p);
							size_t im2 = seriesIndex->Index(m - 1, n, p);
							size_t in1 = seriesIndex->Index(m, n + 1, p);
							size_t in2 = seriesIndex->Index(m, n - 1, p);
							size_t kp1 = seriesIndex->Index(m, n, p + 2);
							size_t kp2 = seriesIndex->Index(m, n, p - 2);
							size_t km1 = seriesIndex->Index(m + 2, n, p);
							size_t km2 = seriesIndex->Index(m - 2, n, p);
							size_t kn1 = seriesIndex->Index(m, n + 2, p);
							size_t kn2 = seriesIndex->Index(m, n - 2, p);
							err = err;
						}
						if (mag > 1.0)
						{
							err = err / mag;
						}
						if (err > 1.0e-10)
						{
							p = p; //break here
						}
						errSum += err;
						err = (f2[idx].H.x - HE[idx].H.x)*(f2[idx].H.x - HE[idx].H.x) + (f2[idx].H.y - HE[idx].H.y)*(f2[idx].H.y - HE[idx].H.y) + (f2[idx].H.z - HE[idx].H.z)*(f2[idx].H.z - HE[idx].H.z);
						mag = f2[idx].E.x*f2[idx].E.x + f2[idx].E.y*f2[idx].E.y + f2[idx].E.z*f2[idx].E.z;
						if (mag > 1.0)
						{
							err = err / mag;
						}
						if (err > 1.0e-10)
						{
							p = p; //break here
						}
						errSum += err;
					}
				}
			}
			if (errSum > 1.0e-10)
			{
				err = err; //break;
			}
		}
		if (f1 != NULL)
		{
			FreeMemory(f1);
		}
		if (f2 != NULL)
		{
			FreeMemory(f2);
		}
#endif
		///////////////////////////////////////////////////////
		if(_recordFDTDStepTimes)
		{
			endTime = getTimeTick(); timeUsed = endTime - startTime;
			_sumtimeused += timeUsed;
			_timesteps++;
		}
	}
	return ret;
}



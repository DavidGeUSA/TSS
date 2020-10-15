/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 10/13/2020
Allrights reserved by David Ge

********************************************************************/
#include "FourthSpaceOrderTss.h"


FourthSpaceOrderTss::FourthSpaceOrderTss()
{
	//physics constants ==============
	c0 = 299792458.0; //speed of light
	mu0 = 4.0 * M_PI * 1.0e-7;
	eps0 = 1.0 / (mu0 * c0 * c0);
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
	//
	HE = NULL;
	//
	_basefilename = NULL;
	//
}


FourthSpaceOrderTss::~FourthSpaceOrderTss()
{
	cleanup();
}

void FourthSpaceOrderTss::cleanup()
{
	if (e_even != NULL) { free(e_even); e_even = NULL; }
	if (e_odd != NULL) { free(e_odd); e_odd = NULL; }
	if (h_even != NULL) { free(h_even); h_even = NULL; }
	if (h_odd != NULL) { free(h_odd); h_odd = NULL; }
	if (curlEven != NULL) { FreeMemory(curlEven); curlEven = NULL; }
	if (curlOdd != NULL) { FreeMemory(curlOdd); curlOdd = NULL; }
}

void FourthSpaceOrderTss::OnFinishSimulation()
{
	cleanup();
}

int FourthSpaceOrderTss::onInitialized(TaskFile *taskParameters)
{
	int ret = ERR_OK;
	//
	e_even = (double*)malloc((_maxOrderTimeAdvance + 1)*sizeof(double));
	e_odd = (double*)malloc((_maxOrderTimeAdvance + 1)*sizeof(double));
	h_even = (double*)malloc((_maxOrderTimeAdvance + 1)*sizeof(double));
	h_odd = (double*)malloc((_maxOrderTimeAdvance + 1)*sizeof(double));
	//
	if (e_even == NULL || e_odd == NULL || h_even == NULL || h_odd == NULL)
	{
		ret = ERR_OUTOFMEMORY;
	}
	else
	{
		curlEven = (FieldPoint3D*)AllocateMemory(GetMemorySize());
		if (curlEven == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		else
		{
			curlOdd = (FieldPoint3D*)AllocateMemory(GetMemorySize());
			if (curlOdd == NULL)
			{
				ret = ERR_OUTOFMEMORY;
			}
		}
	}
	if (ret == ERR_OK)
	{
		double dk = 0.0;
		double ae = sqrt(mu0 / eps0);
		double ah = sqrt(eps0 / mu0);
		double v_even = 1.0, v_odd = 1.0;

		//double eo0 = ae / (12.0*sqrt(3.0));
		//double ho0 = -ah / (12.0*sqrt(3.0));
		//double ee1 = -1.0 / (6.0*12.0*12.0);
		//double eo1 = -ae / (sqrt(3.0)*18.0*12.0 * 12.0 * 12.0);
		//double ho1 = ah / (sqrt(3.0)*18.0*12.0 * 12.0 * 12.0);

		e_even[0] = 1.0; h_even[0] = 1.0;
		for (int i = 0; i < _maxOrderTimeAdvance; i++)
		{
			if (i > 0)
			{
				dk = dk + 1.0;
				v_even = -(v_odd * courant / dk) / 12.0;
				e_even[i] = v_even;
				h_even[i] = v_even;
			}
			dk = dk + 1.0;
			v_odd = (v_even * courant / dk) / 12.0;
			e_odd[i] = ae * v_odd; 
			h_odd[i] = -ah * v_odd;
		}
		//v = abs(eo0 - e_odd[0]);
		//if (v > 0)
		//{
		//}
		//v = abs(ho0 - h_odd[0]);
		//v = abs(ee1 - e_even[1]);
		//v = abs(eo1 - e_odd[1]);
		//v = abs(ho1 - h_odd[1]);
		curlEstimator.setIndexCache(seriesIndex);
	}
	//
	return ret;
}

int FourthSpaceOrderTss::updateFieldsToMoveForward()
{
	int ret = ERR_OK;
	//advance time indicators
	_timeIndex++;
	_time += dt;
	//save existing fields to a file and allocating new memory.
	//it will copy the existing fields to new memory.
	//fields are still at a time of _time-dt2.
	ret = allocateFieldMemory();
	if (ret == ERR_OK)
	{
		//
		if (_recordFDTDStepTimes)
		{
			startTime = getTimeCount();
		}
		curlEstimator.SetFields(HE, curlOdd);
		//get curl(H)
		curlEstimator.setParams(0, maxRadius - 3);
		curlEstimator.gothroughSphere(maxRadius - 3);
		//get curl(E)
		curlEstimator.setParams(1, maxRadius - 3);
		curlEstimator.gothroughSphere(maxRadius - 3);
		//
		//apply curl(H) and curl(E)
		for (size_t i = 0; i<fieldItems; i++)
		{
			HE[i].E.x += e_odd[0] * curlOdd[i].H.x;
			HE[i].E.y += e_odd[0] * curlOdd[i].H.y;
			HE[i].E.z += e_odd[0] * curlOdd[i].H.z;
			HE[i].H.x += h_odd[0] * curlOdd[i].E.x;
			HE[i].H.y += h_odd[0] * curlOdd[i].E.y;
			HE[i].H.z += h_odd[0] * curlOdd[i].E.z;
		}
		//
		curlEstimator.SetFields(curlOdd, curlEven);
		//get curl(curl(H))
		curlEstimator.setParams(0, maxRadius - 3);
		curlEstimator.gothroughSphere(maxRadius - 3);
		//get curl(curl(E))
		curlEstimator.setParams(1, maxRadius - 3);
		curlEstimator.gothroughSphere(maxRadius - 3);
		//
		//apply curl(curl(E)) and curl(curl(H))
		for (size_t i = 0; i<fieldItems; i++)
		{
			HE[i].E.x += e_even[1] * curlEven[i].E.x;
			HE[i].E.y += e_even[1] * curlEven[i].E.y;
			HE[i].E.z += e_even[1] * curlEven[i].E.z;
			HE[i].H.x += h_even[1] * curlEven[i].H.x;
			HE[i].H.y += h_even[1] * curlEven[i].H.y;
			HE[i].H.z += h_even[1] * curlEven[i].H.z;
		}
		//
		curlEstimator.SetFields(curlEven, curlOdd);
		//get curl(curl(curl(H)))
		curlEstimator.setParams(0, maxRadius - 3);
		curlEstimator.gothroughSphere(maxRadius - 3);
		//get curl(curl(curl(E)))
		curlEstimator.setParams(1, maxRadius - 3);
		curlEstimator.gothroughSphere(maxRadius - 3);
		//
		//apply curl(curl(curl(H))) and curl(curl(curl(E)))
		for (size_t i = 0; i<fieldItems; i++)
		{
			HE[i].E.x += e_odd[1] * curlOdd[i].H.x;
			HE[i].E.y += e_odd[1] * curlOdd[i].H.y;
			HE[i].E.z += e_odd[1] * curlOdd[i].H.z;
			HE[i].H.x += h_odd[1] * curlOdd[i].E.x;
			HE[i].H.y += h_odd[1] * curlOdd[i].E.y;
			HE[i].H.z += h_odd[1] * curlOdd[i].E.z;
		}
		if (_recordFDTDStepTimes)
		{
			endTime = getTimeCount(); timeUsed = endTime - startTime;
			_sumtimeused += timeUsed;
		}
		if (ret == ERR_OK)
		{
			if (_tfsf != NULL)
			{
				ret = _tfsf->applyTFSF(HE);
			}
		}
	}
	return ret;
}

FourthOrderCurlEstimator::FourthOrderCurlEstimator()
{
	_fields = NULL;
	_curls = NULL;
	ret = ERR_OK;
}
void FourthOrderCurlEstimator::SetFields(FieldPoint3D *fields, FieldPoint3D *curls)
{
	_fields = fields;
	_curls = curls;
	index = 0;
}

void FourthOrderCurlEstimator::handleData(int m, int n, int p)
{
	double ax, bx, ay, by, az, bz;
	size_t i1 = seriesIndex->Index(m, n + 1, p);
	size_t i2 = seriesIndex->Index(m, n - 1, p);
	size_t i3 = seriesIndex->Index(m, n, p + 1);
	size_t i4 = seriesIndex->Index(m, n, p - 1);
	size_t k1 = seriesIndex->Index(m, n + 2, p);
	size_t k2 = seriesIndex->Index(m, n - 2, p);
	size_t k3 = seriesIndex->Index(m, n, p + 2);
	size_t k4 = seriesIndex->Index(m, n, p - 2);
	size_t i5 = seriesIndex->Index(m + 1, n, p);
	size_t i6 = seriesIndex->Index(m - 1, n, p);
	size_t k5 = seriesIndex->Index(m + 2, n, p);
	size_t k6 = seriesIndex->Index(m - 2, n, p);
	//
	if (forElectric)
	{
		//
		//8(E_z (m,n+1,p)-E_z (m,n-1,p)-E_y (m,n,p+1)+E_y (m,n,p-1)) - E_z (m,n+2,p)+E_z (m,n-2,p)+E_y (m,n,p+2)-E_y (m,n,p-2)
		ax = _fields[i1].E.z - _fields[i2].E.z - _fields[i3].E.y + _fields[i4].E.y;
		bx = -_fields[k1].E.z + _fields[k2].E.z + _fields[k3].E.y - _fields[k4].E.y;
		//
		//8(E_x (m,n,p+1)-E_x (m,n,p-1)-E_z (m+1,n,p)+E_z (m-1,n,p)) - E_x (m,n,p+2)+E_x (m,n,p-2)+E_z (m+2,n,p)-E_z (m-2,n,p)
		ay = _fields[i3].E.x - _fields[i4].E.x - _fields[i5].E.z + _fields[i6].E.z;
		by = -_fields[k3].E.x + _fields[k4].E.x + _fields[k5].E.z - _fields[k6].E.z;
		//
		//8(E_y (m+1,n,p)-E_y (m-1,n,p)-E_x (m,n+1,p)+E_x (m,n-1,p)) - E_y (m+2,n,p)+E_y (m-2,n,p)+E_x (m,n+2,p)-E_x (m,n-2,p)
		az = _fields[i5].E.y - _fields[i6].E.y - _fields[i1].E.x + _fields[i2].E.x;
		bz = _fields[k5].E.y + _fields[k6].E.y + _fields[k1].E.x - _fields[k2].E.x;
		//
		_curls[index].E.x = bx + ax + ax + ax + ax + ax + ax + ax + ax;
		_curls[index].E.y = by + ay + ay + ay + ay + ay + ay + ay + ay;
		_curls[index].E.z = bz + az + az + az + az + az + az + az + az;
	}
	else
	{
		//8(E_z (m,n+1,p)-E_z (m,n-1,p)-E_y (m,n,p+1)+E_y (m,n,p-1)) - E_z (m,n+2,p)+E_z (m,n-2,p)+E_y (m,n,p+2)-E_y (m,n,p-2)
		ax = _fields[i1].H.z - _fields[i2].H.z - _fields[i3].H.y + _fields[i4].H.y;
		bx = -_fields[k1].H.z + _fields[k2].H.z + _fields[k3].H.y - _fields[k4].H.y;
		//
		//8(E_x (m,n,p+1)-E_x (m,n,p-1)-E_z (m+1,n,p)+E_z (m-1,n,p)) - E_x (m,n,p+2)+E_x (m,n,p-2)+E_z (m+2,n,p)-E_z (m-2,n,p)
		ay = _fields[i3].H.x - _fields[i4].H.x - _fields[i5].H.z + _fields[i6].H.z;
		by = -_fields[k3].H.x + _fields[k4].H.x + _fields[k5].H.z - _fields[k6].H.z;
		//
		//8(E_y (m+1,n,p)-E_y (m-1,n,p)-E_x (m,n+1,p)+E_x (m,n-1,p)) - E_y (m+2,n,p)+E_y (m-2,n,p)+E_x (m,n+2,p)-E_x (m,n-2,p)
		az = _fields[i5].H.y - _fields[i6].H.y - _fields[i1].H.x + _fields[i2].H.x;
		bz = _fields[k5].H.y + _fields[k6].H.y + _fields[k1].H.x - _fields[k2].H.x;
		//
		_curls[index].H.x = bx + ax + ax + ax + ax + ax + ax + ax + ax;
		_curls[index].H.y = by + ay + ay + ay + ay + ay + ay + ay + ay;
		_curls[index].H.z = bz + az + az + az + az + az + az + az + az;
	}
	//
	index++;
}


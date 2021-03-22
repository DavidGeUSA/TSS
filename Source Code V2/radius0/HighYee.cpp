/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 10/15/2020
Allrights reserved by David Ge

********************************************************************/
#include "HighYee.h"
#include "PopulateFields.h"

SpaceOrderYee::SpaceOrderYee()
{
}


SpaceOrderYee::~SpaceOrderYee()
{

}

/*
	fill the estimaton matrix in row-major format
*/
int SpaceOrderYee::fillMatrix()
{
	int ret = ERR_OK;
	if (hs == 0 || a == NULL || A == NULL)
	{
		ret = ERR_INVALID_CALL;
	}
	else
	{
		//2[ 1/2           (1/2)^3 / 3!          ...  (1/2)^(2(hs-1) + 1) / (2(hs-1) + 1)! 
		//   3/2           (3/2)^3 / 3!          ...  (3/2)^(2(hs-1) + 1) / (2(hs-1) + 1)!
		//                                       ...
		//  (2(hs-1)+1)/2 ((2(hs-1)+1)/2)^3 / 3! ... ((2(hs-1) + 1) / 2)^(2(hs-1) + 1) / (2(hs-1) + 1)!))]
		int ij = 0;
		double c = -0.5;
		double d;
		double s,g;
		for (unsigned int i = 0; i < hs; i++)
		{
			c += 1.0;
			d = c;
			s = 1.0;
			g = 1.0;
			for (unsigned int j = 0; j < hs; j++)
			{
				A[ij + j] = 2.0 * d/s;
				d = d*c*c;
				g = g + 1.0; //g+1
				s = s * g;   //(g+1)!
				g = g + 1.0; //g+2
				s = s * g;   //(g+2)!
			}
			ij += hs;
		}
	}
	return ret;
}
////////////////////////////////////////////////////////////////////////////////////
TimeOrderYee::TimeOrderYee()
{

}
TimeOrderYee::~TimeOrderYee()
{

}

int TimeOrderYee::CalculateCoefficients(unsigned int halfOrder, double courant)
{
	double v = 4.0;
	int ret = ArbitraryTimeOrder::CalculateCoefficients(halfOrder, courant);
	if (ret == ERR_OK)
	{
		/*
		a_ey^{2k+1} = (1/4^k)  a_e^{2k+1}
		a_hy^{2k+1} = (1/4^k)  a_h^{2k+1}
		k = 0, 1, 2, ..., ht-1
		*/
		for (unsigned int i = 1; i < ht; i++)
		{
			ae[2 * i + 1] = ae[2 * i + 1] / v;
			ah[2 * i + 1] = ah[2 * i + 1] / v;
			v = v * 4.0;
		}
	}
	return ret;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//=========Advance H field=========================================
int YeeTimeAdvanceHField::moveForward()
{
	int ret = ERR_OK;
	unsigned int ht = timeCoefficients->HalfOrder();
	double *ae = timeCoefficients->coefficientsE();
	unsigned int k2 = 1;
	//k_max = ht-1
	//H(q∆_t+1/2 ∆_t )=H(q∆_t-1/2 ∆_t )+∑_(k=0)^(k_max)▒〖a_ey^(2k+1) C_e^{2k+1}  〗(q∆_t)
	//
	//calculate Ce^{1} from E
	//Curl_x = a0(E_z (m,n+1,p)-E_z (m,n,p)-E_y (m,n,p+1)+E_y (m,n,p)) + a1(E_z (m,n+2,p)-E_z (m,n-1,p)-E_y (m,n,p+2)+E_y (m,n,p-1))+...
	//Curl_y = a0(E_x (m,n,p+1)-E_x (m,n,p)-E_z (m+1,n,p)+E_z (m,n,p)) + a1(E_x (m,n,p+2)-E_x (m,n,p-1)-E_z (m+2,n,p)+E_z (m-1,n,p))+...
	//Curl_z = a0(E_y (m+1,n,p)-E_y (m,n,p)-E_x (m,n+1,p)+E_x (m,n,p)) + a1(E_y (m+2,n,p)-E_y (m-1,n,p)-E_x (m,n+2,p)+E_x (m,n-1,p)) + ...
	//indexes used: dy->
	//              [(m,n+1,p), (m,n-0,p)], [(m,n+2,p), (m,n-1,p)], ..., [(m,n+hs,p), (m,n-hs-1,p)]
	//              dz->
	//              [(m,n,p+1), (m,n,p-0)], [(m,n,p+2), (m,n,p-1)], ..., [(m,n,p+hs), (m,n,p-hs-1)]
	//              dx->
	//              [(m+1,n,p), (m-0,n,p)], [(m+2,n,p), (m-1,n,p)], ..., [(m+hs,n,p), (m-hs-1,n,p)]
	//
	field = _field;
	curl = curlOdd;
	for (unsigned int k = 0; k < ht; k++)
	{
		ret = this->gothroughSphere(maxRadius);
		if (ret == ERR_OK)
		{
			for (size_t i = 0; i<index;i++)
			{
				_field[i].H.x += ae[k2] * curl[i].E.x;
				_field[i].H.y += ae[k2] * curl[i].E.y;
				_field[i].H.z += ae[k2] * curl[i].E.z;
			}
			if (k < ht - 1)
			{
				k2 += 2;
				field = curlOdd;
				curl = curlEven;
				ret = this->gothroughSphere(maxRadius);
				if (ret == ERR_OK)
				{
					field = curlEven;
					curl = curlOdd;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return ret;
}
RadiusHandleType YeeTimeAdvanceHField::setRadius(int radius)
{
	if (radius < (int)(maxRadius - hs))
	{
		return NeedProcess;
	}
	return Finish;
}
void YeeTimeAdvanceHField::handleData(int m, int n, int p)
{
	m1 = seriesIndex->Index(m + 1, n, p);
	n1 = seriesIndex->Index(m, n + 1, p);
	p1 = seriesIndex->Index(m, n, p + 1);
	curl[index].E.x = a[0] * (field[n1].E.z - field[index].E.z - field[p1].E.y + field[index].E.y);
	curl[index].E.y = a[0] * (field[p1].E.x - field[index].E.x - field[m1].E.z + field[index].E.z);
	curl[index].E.z = a[0] * (field[m1].E.y - field[index].E.y - field[n1].E.x + field[index].E.x);
	if (hs > 1)
	{
		for (unsigned int i = 0; i < hs; i++)
		{
			m1 = seriesIndex->Index(m + 1 + i, n, p);
			n1 = seriesIndex->Index(m, n + 1 + i, p);
			p1 = seriesIndex->Index(m, n, p + 1 + i);
			m2 = seriesIndex->Index(m - i, n, p);
			n2 = seriesIndex->Index(m, n - i, p);
			p2 = seriesIndex->Index(m, n, p - i);
			curl[index].E.x = a[i] * (field[n1].E.z - field[n2].E.z - field[p1].E.y + field[p2].E.y);
			curl[index].E.y = a[i] * (field[p1].E.x - field[p2].E.x	- field[m1].E.z + field[m2].E.z);
			curl[index].E.z = a[i] * (field[m1].E.y - field[m2].E.y	- field[n1].E.x + field[n2].E.x);
		}
	}
	index++;
}
//=========Advance E field=========================================
int YeeTimeAdvanceEField::moveForward()
{
	int ret = ERR_OK;
	unsigned int ht = timeCoefficients->HalfOrder();
	double *ah = timeCoefficients->coefficientsH();
	unsigned int k2 = 1;
	/*
	E(q∆_t+∆_t )=E(q∆_t )+∑_(k=0)^∞▒〖a_hy^(2k+1) C_h^{2k+1}  〗(q∆_t+1/2 ∆_t)
	*/
	field = _field;
	curl = curlOdd;
	for (unsigned int k = 0; k < ht; k++)
	{
		ret = this->gothroughSphere(maxRadius);
		if (ret == ERR_OK)
		{
			for (size_t i = 0; i<index; i++)
			{
				_field[i].E.x += ah[k2] * curl[i].H.x;
				_field[i].E.y += ah[k2] * curl[i].H.y;
				_field[i].E.z += ah[k2] * curl[i].H.z;
			}
			if (k < ht - 1)
			{
				k2 += 2;
				field = curlOdd;
				curl = curlEven;
				ret = this->gothroughSphere(maxRadius);
				if (ret == ERR_OK)
				{
					field = curlEven;
					curl = curlOdd;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return ret;
}
RadiusHandleType YeeTimeAdvanceEField::setRadius(int radius)
{
	if (radius < (int)(maxRadius - hs))
	{
		return NeedProcess;
	}
	return Finish;
}
void YeeTimeAdvanceEField::handleData(int m, int n, int p)
{
	m1 = seriesIndex->Index(m - 1, n, p);
	n1 = seriesIndex->Index(m, n - 1, p);
	p1 = seriesIndex->Index(m, n, p - 1);
	curl[index].H.x = a[0] * (field[index].H.z - field[n1].H.z - field[index].H.y + field[p1].H.y);
	curl[index].H.y = a[0] * (field[index].H.x - field[p1].H.x - field[index].H.z + field[m1].H.z);
	curl[index].H.z = a[0] * (field[index].H.y - field[m1].H.y - field[index].H.x + field[n1].H.x);
	if (hs > 1)
	{
		for (unsigned int i = 0; i < hs; i++)
		{
			m1 = seriesIndex->Index(m + i, n, p);
			n1 = seriesIndex->Index(m, n + i, p);
			p1 = seriesIndex->Index(m, n, p + i);
			m2 = seriesIndex->Index(m - i - 1, n, p);
			n2 = seriesIndex->Index(m, n - i - 1, p);
			p2 = seriesIndex->Index(m, n, p - i - 1);

			curl[index].H.x = a[i] * (field[n1].H.z - field[n2].H.z - field[p1].H.y + field[p2].H.y);
			curl[index].H.y = a[i] * (field[p1].H.x - field[p2].H.x - field[m1].H.z + field[m2].H.z);
			curl[index].H.z = a[i] * (field[m1].H.y - field[m2].H.y - field[n1].H.x + field[n2].H.x);
		}
	}

	index++;
}
////////////////////////////////////////////////////////////////////////////////////////////////
HighYee::HighYee()
{
	mu0 = 4.0 * M_PI * 1.0e-7;
	eps0 = 1.0 / (mu0 * c0 * c0);
	//
	//dxp = dxn = dyp = dyn = dzp = dzn = NULL;
}


HighYee::~HighYee()
{
	cleanup();
}
///////
void HighYee::cleanup()
{
	if (curlEven != NULL) { FreeMemory(curlEven); curlEven = NULL; }
	if (curlOdd != NULL) { FreeMemory(curlOdd); curlOdd = NULL; }
	//if (dxp != NULL) { free(dxp); dxp = NULL; }
	//if (dxn != NULL) { free(dxn); dxn = NULL; }
	//if (dyp != NULL) { free(dyp); dyp = NULL; }
	//if (dyn != NULL) { free(dyn); dyn = NULL; }
	//if (dzp != NULL) { free(dzp); dzp = NULL; }
	//if (dzn != NULL) { free(dzn); dzn = NULL; }
}
//int HighYee::createCurlIndex()
//{
//	int ret = ERR_OK;
//	if (dxp != NULL || dxn != NULL || dyp != NULL || dyn != NULL || dzp != NULL || dzn != NULL || spaceCoefficients == NULL)
//	{
//		ret = ERR_INVALID_CALL;
//	}
//	else
//	{
//		unsigned int hs = spaceCoefficients.ArraySize();
//		if (hs == 0)
//		{
//			ret = ERR_INVALID_CALL;
//		}
//		else
//		{
//			dxp = (size_t *)malloc(hs * sizeof(size_t));
//			dxn = (size_t *)malloc(hs * sizeof(size_t));
//			dyp = (size_t *)malloc(hs * sizeof(size_t));
//			dyn = (size_t *)malloc(hs * sizeof(size_t));
//			dzp = (size_t *)malloc(hs * sizeof(size_t));
//			dzn = (size_t *)malloc(hs * sizeof(size_t));
//			if (dxp == NULL || dxn == NULL || dyp == NULL || dyn == NULL || dzp == NULL || dzn == NULL)
//			{
//				ret = ERR_OUTOFMEMORY;
//			}
//			else
//			{
//				//indexes used: dy->
//				//              [(m,n+1,p), (m,n-0,p)], [(m,n+2,p), (m,n-1,p)], ..., [(m,n+hs,p), (m,n-hs-1,p)]
//				//              dz->
//				//              [(m,n,p+1), (m,n,p-0)], [(m,n,p+2), (m,n,p-1)], ..., [(m,n,p+hs), (m,n,p-hs-1)]
//				//              dx->
//				//              [(m+1,n,p), (m-0,n,p)], [(m+2,n,p), (m-1,n,p)], ..., [(m+hs,n,p), (m-hs-1,n,p)]
//				for (unsigned int i = 0; i < hs; i++)
//				{
//					dxp[i] = seriesIndex->Index()
//				}
//			}
//		}
//	}
//	return ret;
//}
void HighYee::OnFinishSimulation()
{
	cleanup();
}

int HighYee::PopulateFields(FieldsInitializer *fieldValues)
{
	int ret = ERR_OK;
	PopulateYeeFieldsTime0 p(fieldValues, HE, ds);
	ret = p.gothroughSphere(maxRadius, ds);
	return ret;

}

int HighYee::setFieldValues(FieldsInitializer *fieldValues, double shiftX, double shiftY, double shiftZ)
{
	int ret = ERR_OK;
	PopulateYeeFieldsTime0 p(fieldValues, HE, ds);
	p.setShifted(shiftX, shiftY, shiftZ);
	ret = p.gothroughSphere(maxRadius, ds);
	return ret;
}

int HighYee::onInitialized(TaskFile *taskParameters)
{
	int ret = ERR_OK;
	ret = timeCoefficients.CalculateCoefficients(_maxOrderTimeAdvance, courant);
	if (ret == ERR_OK)
	{
		ret = spaceCoefficients.CalculateCoefficients(_maxOrderSpaceDerivative);
	}
	if (ret == ERR_OK)
	{
		curlOdd = (FieldPoint3D *)AllocateMemory(fieldMemorySize);
		if (curlOdd == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		else
		{
			curlEven = (FieldPoint3D *)AllocateMemory(fieldMemorySize);
			if (curlEven == NULL)
			{
				ret = ERR_OUTOFMEMORY;
			}
		}
		if (ret == ERR_OK)
		{
			hfield.setMaxRadius(seriesIndex, maxRadius, &timeCoefficients, &spaceCoefficients);
			efield.setMaxRadius(seriesIndex, maxRadius, &timeCoefficients, &spaceCoefficients);
		}
	}
	//
	return ret;
}

int HighYee::updateFieldsToMoveForward()
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
		//advance H first
		if (_recordFDTDStepTimes)
		{
			startTime = getTimeCount();
		}
		hfield.reset(HE, curlOdd, curlEven);
		ret = hfield.moveForward();
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
		//advance E next
		if (ret == ERR_OK)
		{
			if (_recordFDTDStepTimes)
			{
				startTime = getTimeCount();
			}
			efield.reset(HE, curlOdd, curlEven);
			ret = efield.moveForward();
			if (_recordFDTDStepTimes)
			{
				endTime = getTimeCount();
				timeUsed += (endTime - startTime);
			}
		}
		if (_recordFDTDStepTimes)
		{
			_sumtimeused += timeUsed;
			_timesteps++;
		}
	}
	return ret;
}


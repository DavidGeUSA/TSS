/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include <malloc.h>
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  
#include <stdlib.h>
#include <float.h>
#include <stdio.h>

#include "../FileUtil/fileutil.h"
#include "FieldStatistics.h"
#include "../EMField/RadiusIndex.h"
#include "../ProcessMonitor/ProcessMonitor.h"


FieldStatistics::FieldStatistics(void)
{
	_dvgByRadius = NULL;
	Reset();
}
FieldStatistics::~FieldStatistics()
{
	if(_dvgByRadius != NULL)
	{
		delete _dvgByRadius;
		_dvgByRadius = NULL;
	}
}
void FieldStatistics::Reset()
{
	_maxFieldE = _maxFieldH = _maxDivergenceE = _maxDivergenceH = -DBL_MAX;
	_divergE = 0.0; _divergH = 0.0; _divergE = 0.0; _divergH = 0.0;
	_rMaxDivE = -1; _rMaxDivH = -1; _rMaxE = -1; _rMaxH = -1;
	_sumDivgE = 0.0; _sumDivgH = 0.0;
}
int FieldStatistics::AllocateList(int maxR)
{
	if(_dvgByRadius != NULL)
	{
		delete _dvgByRadius;
		_dvgByRadius = NULL;
	}
	maxRadius = maxR;
	_dvgByRadius = (DivergenceByRadius *)malloc((maxR+1) * sizeof(DivergenceByRadius));
	if(_dvgByRadius != NULL)
	{
		for(int i=0;i<=maxR;i++)
		{
			_dvgByRadius[i].r = i;
			_dvgByRadius[i].sumFieldStrengthE = _dvgByRadius[i].sumFieldStrengthH = _dvgByRadius[i].maxDivergenceE = _dvgByRadius[i].maxDivergenceH = _dvgByRadius[i].sumDivergenceE = _dvgByRadius[i].sumDivergenceH = 0;
			_dvgByRadius[i].sumEnergyCircular = _dvgByRadius[i].sumEnergyInwards = _dvgByRadius[i].sumEnergyOutwards = _dvgByRadius[i].sumEnergy = 0.0;
		}
		return ERR_OK;
	}
	else
	{
		return ERR_OUTOFMEMORY;
	}
}
DivergenceByRadius *FieldStatistics::GetList()
{
	return _dvgByRadius;
}
int FieldStatistics::WriteDivegenceToFile(FILE *filehandle)
{
	int ret = ERR_OK;
	size_t num;
	double se, sh, s;
	for(int i=0;i<=maxRadius;i++)
	{
		num = pointsAt(i);
		se = _dvgByRadius[i].sumDivergenceE/(double)num;
		sh = _dvgByRadius[i].sumDivergenceH/(double)num;
		s = sqrt(se * se + sh*sh);
		ret = writefile(filehandle, &s, sizeof(s));
		if(ret != ERR_OK)
		{
			break;
		}
	}
	return ret;
}
void FieldStatistics::ShowReport(fnProgressReport reporter)
{
	size_t num = totalPointsInSphere(maxRadius);
	reportProcess(reporter, false, "Average divergence (E, H): %g, %g", _sumDivgE / (double)num, _sumDivgH / (double)num);
	reportProcess(reporter, false, " Field(Max,r): E(%g,%d)  H(%g,%d) Dvg(Max,r): E(%g,%d)  H(%g,%d)",
		_maxFieldE, _rMaxE, 
		_maxFieldH, _rMaxH, 
		_maxDivergenceE, _rMaxDivE,
		_maxDivergenceH, _rMaxDivH
		);
	if(_dvgByRadius != NULL)
	{
		double sumRate, rateIn, rateOut, rateC;
		reportProcess(reporter,false,"r\tEnergy\t||S||\tIn\tOut\tCircle\tDiv. E\tDiv. H");
		for(int i=0;i<=maxRadius;i++)
		{
			num = pointsAt(i);
			sumRate = _dvgByRadius[i].sumEnergyCircular + _dvgByRadius[i].sumEnergyInwards + _dvgByRadius[i].sumEnergyOutwards;
			if(sumRate > 0.0)
			{
				rateOut = 100.0*(_dvgByRadius[i].sumEnergyOutwards/sumRate);
				rateIn  = 100.0*(_dvgByRadius[i].sumEnergyInwards/sumRate);
				rateC   = 100.0*(_dvgByRadius[i].sumEnergyCircular/sumRate);
			}
			else
			{
				rateIn = rateOut = rateC = 0.0;
			}
			reportProcess(reporter, false, "%d\t%g\t%g\t%g\t%g\t%g\t%g\t%g", i, 
				_dvgByRadius[i].sumEnergy,
				sumRate, rateIn, rateOut, rateC,
				_dvgByRadius[i].sumDivergenceE/(double)num, _dvgByRadius[i].sumDivergenceH/(double)num
				);
		}
	}

}
void FieldStatistics::ShowDetails(fnProgressReport reporter)
{
	reportProcess(reporter, false, " Field(Max,r):\r\n   E(%g,%d)  H(%g,%d) \r\n Dvg(Max,r):\r\n E(%g,%d)  H(%g,%d)",
		_maxFieldE, _rMaxE, 
		_maxFieldH, _rMaxH, 
		_maxDivergenceE, _rMaxDivE,
		_maxDivergenceH, _rMaxDivH
		);
	if(_dvgByRadius != NULL)
	{
		size_t num;
		puts("\r\nr  E(sum field, max divg, sum divg, average divg) H(sum field, max divg, sum divg, average divg)\r\n  Outwards,  inwards,  circular\r\n");
		for(int i=0;i<=maxRadius;i++)
		{
			num = pointsAt(i);
			reportProcess(reporter, false, " r=%d  E(%g, %g, %g, %g) H(%g, %g, %g, %g)", i, 
				_dvgByRadius[i].sumFieldStrengthE,_dvgByRadius[i].maxDivergenceE,_dvgByRadius[i].sumDivergenceE, _dvgByRadius[i].sumDivergenceE/(double)num,
				_dvgByRadius[i].sumFieldStrengthH, _dvgByRadius[i].maxDivergenceH, _dvgByRadius[i].sumDivergenceH, _dvgByRadius[i].sumDivergenceH/(double)num
				);
			reportProcess(reporter, false, "Energy:     %g,  %g,  %g",
				_dvgByRadius[i].sumEnergyOutwards, _dvgByRadius[i].sumEnergyInwards, _dvgByRadius[i].sumEnergyCircular
				);
		}
	}
}
void FieldStatistics::ShowSummary(fnProgressReport reporter)
{
	if(_dvgByRadius != NULL)
	{
		double sumE,sumH,sumDivE,sumDivH;
		size_t num = totalPointsInSphere(maxRadius);
		//puts("\r\nr  E(sum field, max divg, sum divg, average divg) H(sum field, max divg, sum divg, average divg)\r\n");
		sumE = sumH = sumDivE = sumDivH = 0.0;
		for(int i=0;i<=maxRadius;i++)
		{
			sumE += _dvgByRadius[i].sumFieldStrengthE;
			sumH += _dvgByRadius[i].sumFieldStrengthH;
			sumDivE += _dvgByRadius[i].sumDivergenceE;
			sumDivH += _dvgByRadius[i].sumDivergenceH;
		}
		reportProcess(reporter, false, "(Max, r): E val(%g, %d), dvg(%g, %d) H val(%g, %d), dvg(%g, %d)",
			_maxFieldE,_rMaxE,_maxDivergenceE,_rMaxDivE,_maxFieldH,_rMaxH,_maxDivergenceH,_rMaxDivH); 
		reportProcess(reporter, false, "(Sum val, sum dvg, avg dvg): E(%g, %g, %g) H(%g, %g, %g)",  
				sumE,sumDivE,sumDivE/(double)num,
				sumH,sumDivH,sumDivH/(double)num
			);
	}
	else
	{
		reportProcess(reporter, false, " Field(Max,r):\r\n   E(%g,%d)  H(%g,%d) \r\n Dvg(Max,r):\r\n E(%g,%d)  H(%g,%d)",
		_maxFieldE, _rMaxE, 
		_maxFieldH, _rMaxH, 
		_maxDivergenceE, _rMaxDivE,
		_maxDivergenceH, _rMaxDivH
		);
	}
}
void FieldStatistics::MakeStatistics(double fmE, double fmH, double energy, int r)
{
	double v1;
	//fmE = sqrt(eVector->x * eVector->x + eVector->y * eVector->y + eVector->z * eVector->z);
	fmE = sqrt(fmE);
	if (fmE > _maxFieldE)
	{
		_maxFieldE = fmE; _rMaxE = r;
	}
	//
	//fmH = sqrt(hVector->x * hVector->x + hVector->y * hVector->y + hVector->z * hVector->z);
	fmH = sqrt(fmH);
	if (fmH > _maxFieldH)
	{
		_maxFieldH = fmH; _rMaxH = r;
	}
	//
	v1 = abs(_divergE);
	if (v1 > _maxDivergenceE)
	{
		_maxDivergenceE = v1; _rMaxDivE = r;
	}
	if (_dvgByRadius != NULL)
	{
		if (v1 > _dvgByRadius[r].maxDivergenceE)
		{
			_dvgByRadius[r].maxDivergenceE = v1;
		}
		_dvgByRadius[r].sumFieldStrengthE += fmE;
		_dvgByRadius[r].sumDivergenceE += v1;
	}
	//
	v1 = abs(_divergH);
	if (v1 > _maxDivergenceH)
	{
		_maxDivergenceH = v1; _rMaxDivH = r;
	}
	if (_dvgByRadius != NULL)
	{
		if (v1 > _dvgByRadius[r].maxDivergenceH)
		{
			_dvgByRadius[r].maxDivergenceH = v1;
		}
		_dvgByRadius[r].sumFieldStrengthH += fmH;
		_dvgByRadius[r].sumDivergenceH += v1;
	}
	_dvgByRadius[r].sumEnergy += energy;
}
void FieldStatistics::MakeStatistics(FieldPoint3D *_fields, size_t index, int r)
{
	double fmE, fmH, v1;
	fmE = sqrt( _fields[index].E.x * _fields[index].E.x + _fields[index].E.y * _fields[index].E.y + _fields[index].E.z * _fields[index].E.z );
	if(fmE > _maxFieldE)
	{
		_maxFieldE = fmE; _rMaxE = r;
	}
	//
	fmH = sqrt( _fields[index].H.x * _fields[index].H.x + _fields[index].H.y * _fields[index].H.y + _fields[index].H.z * _fields[index].H.z );
	if(fmH > _maxFieldH)
	{
		_maxFieldH = fmH; _rMaxH = r;
	}
	//
	v1 = abs(_divergE);
	if(v1 > _maxDivergenceE)
	{
		_maxDivergenceE = v1; _rMaxDivE = r;
	}
	if(_dvgByRadius != NULL)
	{
		if(v1 > _dvgByRadius[r].maxDivergenceE)
		{
			_dvgByRadius[r].maxDivergenceE = v1;
		}
		_dvgByRadius[r].sumFieldStrengthE += fmE;
		_dvgByRadius[r].sumDivergenceE += v1;
	}
	//
	v1 = abs(_divergH);
	if(v1 > _maxDivergenceH)
	{
		_maxDivergenceH = v1; _rMaxDivH = r;
	}
	if(_dvgByRadius != NULL)
	{
		if(v1 > _dvgByRadius[r].maxDivergenceH)
		{
			_dvgByRadius[r].maxDivergenceH = v1;
		}
		_dvgByRadius[r].sumFieldStrengthH += fmH;
		_dvgByRadius[r].sumDivergenceH += v1;
	}
}
double FieldStatistics::GetAverageDivergenceE()
{
	size_t num = totalPointsInSphere(maxRadius);
	return _sumDivgE / (double)num;
}
double FieldStatistics::GetAverageDivergenceH()
{
	size_t num = totalPointsInSphere(maxRadius);
	return _sumDivgH / (double)num;
}

double FieldStatistics::GetSumDivergenceE()
{
	return _sumDivgE;
}
double FieldStatistics::GetSumDivergenceH()
{
	return _sumDivgH;
}
double FieldStatistics::GetMaxFieldE()
{
	return _maxFieldE;
}
double FieldStatistics::GetMaxFieldH()
{
	return _maxFieldH;
}
double FieldStatistics::GetMaxDivergenceE()
{
	return _maxDivergenceE;
}
double FieldStatistics::GetMaxDivergenceH()
{
	return _maxDivergenceH;
}
double FieldStatistics::GetDivergenceE()
{
	return _divergE;
}
double FieldStatistics::GetDivergenceH()
{
	return _divergH;
}
int FieldStatistics::GetRadiusMaxE()
{
	return _rMaxE;
}
int FieldStatistics::GetRadiusMaxH()
{
	return _rMaxH;
}
int FieldStatistics::GetRadiusMaxDivE()
{
	return _rMaxDivE;
}
int FieldStatistics::GetRadiusMaxDivH()
{
	return _rMaxDivH;
}

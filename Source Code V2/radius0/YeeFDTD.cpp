/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/


#include "YeeFDTD.h"
#include "FieldUpdator.h"
#include <malloc.h>
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>

#include "PopulateFields.h"
#include "../MemoryMan/memman.h"
#include "../FileUtil/fileutil.h"

YeeFDTD::YeeFDTD(void)
{
	mu0 = 4.0 * M_PI * 1.0e-7;
	eps0 = 1.0 /(mu0 * c0 * c0);
	ch = ce = 0;
}
YeeFDTD::~YeeFDTD(void)
{
	cleanup();
}

void YeeFDTD::cleanup()
{
}

void YeeFDTD::OnFinishSimulation()
{
	cleanup();
}

int YeeFDTD::PopulateFields(FieldsInitializer *fieldValues)
{
	int ret = ERR_OK;
	PopulateYeeFieldsTime0 p(fieldValues, HE, ds);
	ret = p.gothroughSphere(maxRadius, ds);
	return ret;

}

int YeeFDTD::setFieldValues(FieldsInitializer *fieldValues, double shiftX, double shiftY, double shiftZ)
{
	int ret = ERR_OK;
	PopulateYeeFieldsTime0 p(fieldValues, HE, ds);
	p.setShifted(shiftX, shiftY, shiftZ);
	ret = p.gothroughSphere(maxRadius, ds);
	return ret;
}

int YeeFDTD::onInitialized(TaskFile *taskParameters)
{
	int ret = ERR_OK;
	ch = (dt/ds)/mu0;
	ce = (dt/ds)/eps0;
	//
	updateE.setMaxRadius(seriesIndex, maxRadius, ch, ce);
	updateH.setMaxRadius(seriesIndex, maxRadius, ch, ce);
	//
	return ret;
}

int YeeFDTD::updateFieldsToMoveForward()
{
	int ret = ERR_OK;
	//advance time indicators
	_timeIndex++;
	_time += dt;
	//save existing fields to a file and allocating new memory.
	//it will copy the existing fields to new memory.
	//fields are still at a time of _time-dt2.
	ret = allocateFieldMemory(); 
	if(ret == ERR_OK)
	{
		//advance H first
		if(_recordFDTDStepTimes)
		{
			startTime = getTimeTick();
		}
		updateH.reset(HE);
		ret = updateH.gothroughSphere(maxRadius);
		if(_recordFDTDStepTimes)
		{
			endTime = getTimeTick(); timeUsed = endTime - startTime;
			_sumtimeused += timeUsed;
		}
		if(ret == ERR_OK)
		{
			if(_tfsf != NULL)
			{
				ret = _tfsf->applyTFSF(HE);
			}
		}
		//advance E next
		if(ret == ERR_OK)
		{
			if(_recordFDTDStepTimes)
			{
				startTime = getTimeTick();
			}
			updateE.reset(HE);
			ret = updateE.gothroughSphere(maxRadius);
			if(_recordFDTDStepTimes)
			{
				endTime = getTimeTick(); 
				timeUsed += (endTime - startTime);
			}
		}
		if(_recordFDTDStepTimes)
		{
			_sumtimeused += timeUsed;
			_timesteps++;
		}
	}
	return ret;
}



/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/

#include "YeeFDTDSpaceSynched.h"
#include "..\MemoryMan\memman.h"

YeeFDTDSpaceSynched::YeeFDTDSpaceSynched(void)
{
	yeeEx = NULL;
	yeeEy = NULL;
	yeeEz = NULL;
	yeeHx = NULL;
	yeeHy = NULL;
	yeeHz = NULL;
	ret = ERR_OK;
}
YeeFDTDSpaceSynched::~YeeFDTDSpaceSynched()
{
	delete yeeEx;
	delete yeeEy;
	delete yeeEz;
	delete yeeHx;
	delete yeeHy;
	delete yeeHz;
}
void YeeFDTDSpaceSynched::cleanup(void)
{
}
int YeeFDTDSpaceSynched::onInitialized(TaskFile *taskParameters)
{
	freeFieldMemory(); //initialize(...) allocated memory, we do not need it here
	yeeEx = new YeeFDTD();
	yeeEy = new YeeFDTD();
	yeeEz = new YeeFDTD();
	yeeHx = new YeeFDTD();
	yeeHy = new YeeFDTD();
	yeeHz = new YeeFDTD();
	//
	shareIndexCacheTo(yeeEx);
	shareIndexCacheTo(yeeEy);
	shareIndexCacheTo(yeeEz);
	shareIndexCacheTo(yeeHx);
	shareIndexCacheTo(yeeHy);
	shareIndexCacheTo(yeeHz);
	//
	yeeEx->SetMemoryManager(_mem);
	yeeEy->SetMemoryManager(_mem);
	yeeEz->SetMemoryManager(_mem);
	yeeHx->SetMemoryManager(_mem);
	yeeHy->SetMemoryManager(_mem);
	yeeHz->SetMemoryManager(_mem);
	//
	ds2 = ds / 2.0; //for making shift
	ret = yeeEx->initialize(NULL, _tfsf, taskParameters);
	if(ret == ERR_OK)
	{
		ret = yeeEy->initialize(NULL, _tfsf, taskParameters);
	}
	if(ret == ERR_OK)
	{
		ret = yeeEz->initialize(NULL, _tfsf, taskParameters);
	}
	if(ret == ERR_OK)
	{
		ret = yeeHx->initialize(NULL, _tfsf, taskParameters);
	}
	if(ret == ERR_OK)
	{
		ret = yeeHy->initialize(NULL, _tfsf, taskParameters);
	}
	if(ret == ERR_OK)
	{
		ret = yeeHz->initialize(NULL, _tfsf, taskParameters);
	}
	return ret;
}
int YeeFDTDSpaceSynched::formSpaceSynchedFields()
{
	ret = allocateFieldMemory(); //create a memory mapped file, memory point is HE
	if(ret == ERR_OK)
	{
		fEx = yeeEx->GetFieldMemory();
		fEy = yeeEy->GetFieldMemory();
		fEz = yeeEz->GetFieldMemory();
		fHx = yeeHx->GetFieldMemory();
		fHy = yeeHy->GetFieldMemory();
		fHz = yeeHz->GetFieldMemory();
		for(size_t index=0;index<fieldItems;index++)
		{
			HE[index].E.x = fEx[index].E.x;
			HE[index].E.y = fEy[index].E.y;
			HE[index].E.z = fEz[index].E.z;
			HE[index].H.x = fHx[index].H.x;
			HE[index].H.y = fHy[index].H.y;
			HE[index].H.z = fHz[index].H.z;
		}
		freeFieldMemory(); //close the memory mapped file
	}
	return ret;
}

/*
	populate all 6 YeeFDTD instances, make space shifts to create a space synchronized EM field
*/
int YeeFDTDSpaceSynched::PopulateFields(FieldsInitializer *fieldValues)
{
	if(ret == ERR_OK)
	{
		ret = yeeEx->setFieldValues(fieldValues, -ds2, 0.0, 0.0);
	}
	if(ret == ERR_OK)
	{
		ret = yeeEy->setFieldValues(fieldValues, 0.0, -ds2, 0.0);
	}
	if(ret == ERR_OK)
	{
		ret = yeeEz->setFieldValues(fieldValues, 0.0, 0.0, -ds2);
	}
	if(ret == ERR_OK)
	{
		ret = yeeHx->setFieldValues(fieldValues, 0.0, -ds2, -ds2);
	}
	if(ret == ERR_OK)
	{
		ret = yeeHy->setFieldValues(fieldValues, -ds2, 0.0, -ds2);
	}
	if(ret == ERR_OK)
	{
		ret = yeeHz->setFieldValues(fieldValues, -ds2, -ds2, 0.0);
	}
	if(ret == ERR_OK)
	{
		ret = formSpaceSynchedFields();
	}
	return ret;
}

void YeeFDTDSpaceSynched::OnFinishSimulation()
{
	
}

int YeeFDTDSpaceSynched::updateFieldsToMoveForward()
{
	ret = ERR_OK;
	//advance time step index
	_timeIndex++;
	//
	ret = yeeEx->moveForward();
	if(ret == ERR_OK)
	{
		ret = yeeEy->moveForward();
	}
	if(ret == ERR_OK)
	{
		ret = yeeEz->moveForward();
	}
	if(ret == ERR_OK)
	{
		ret = yeeHx->moveForward();
	}
	if(ret == ERR_OK)
	{
		ret = yeeHy->moveForward();
	}
	if(ret == ERR_OK)
	{
		ret = yeeHz->moveForward();
	}
	if(ret == ERR_OK)
	{
		if(_recordFDTDStepTimes)
		{
			_sumtimeused += (
				 yeeEx->GetCurrentFDTDStepTime()
				+yeeEy->GetCurrentFDTDStepTime()
				+yeeEz->GetCurrentFDTDStepTime()
				+yeeHx->GetCurrentFDTDStepTime()
				+yeeHy->GetCurrentFDTDStepTime()
				+yeeHz->GetCurrentFDTDStepTime())/6.0;
			_timesteps++;
		}
	}
	if(ret == ERR_OK)
	{
		ret = formSpaceSynchedFields();
	}
	return ret;
}


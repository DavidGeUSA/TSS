/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

//classes for running tasks
#include <math.h>
#include "taskClasses.h"
#include "simConsole.h"
#include "../ProcessMonitor/ProcessMonitor.h"
#include "../MemoryMan/MemoryManager.h"
#include "../MemoryMan/memman.h"

////////////////////////////////////////////////////////////////////////////////////////////
SphereIndexSpeedTest::SphereIndexSpeedTest()
{
	displayCount = 0;
	totalIndex = 0;
}
void SphereIndexSpeedTest::setTotalIndex(size_t total, int interval)
{
	totalIndex = total;
	displayInterval = interval;
}
void SphereIndexSpeedTest::handleData(int m, int n, int p)
{
	displayCount++;
	if(displayCount > displayInterval)
	{
		displayCount = 0;
		reportProcess(showProgressReport, true, "Sphere index: %d / %d", index, totalIndex);
	}
	index++;
}

////////////////////////////////////////////////////////////////////////////
PickFieldPoints::PickFieldPoints()
{
	index = 0;
	_picks = 0;
	_fields = NULL;
	_items = NULL;
	_magnitudes = NULL;
	_collectedIndexes = NULL;
	inMonotonic = true;
}
PickFieldPoints::~PickFieldPoints()
{
	if(_magnitudes != NULL)
	{
		FreeMemory(_magnitudes);
	}
	if(_collectedIndexes != NULL)
	{
		FreeMemory(_collectedIndexes);
	}
}
int PickFieldPoints::initialize(size_t picks, FieldPoint3D *fields, FieldItem3D *pickedItems)
{
	int ret = ERR_OK;
	_picks = picks;
	_fields = fields;
	_items = pickedItems;
	index = 0;
	ret = MEMMANEXIST;
	if(ret == ERR_OK)
	{
		_magnitudes = (double *)AllocateMemory(sizeof(double)*picks);
		if(_magnitudes == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}
	if(ret == ERR_OK)
	{
		_collectedIndexes = (size_t *)AllocateMemory(sizeof(size_t)*picks);
		if(_collectedIndexes == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}
	if(ret == ERR_OK)
	{
		for(size_t i=0;i<_picks;i++)
		{
			_collectedIndexes[i] = 0;
			_magnitudes[i] = 0.0;
			_items[i].E.x = 0.0;
			_items[i].E.y = 0.0;
			_items[i].E.z = 0.0;
			_items[i].H.x = 0.0;
			_items[i].H.y = 0.0;
			_items[i].H.z = 0.0;
		}
		startIndex = _picks;
		inMonotonic = true;
	}
	return ret;
}

void PickFieldPoints::handleData(double x, double y, double z)
{
	double e = _fields[index].E.x * _fields[index].E.x + _fields[index].E.y * _fields[index].E.y + _fields[index].E.z * _fields[index].E.z;
	double h = _fields[index].H.x * _fields[index].H.x + _fields[index].H.y * _fields[index].H.y + _fields[index].H.z * _fields[index].H.z;
	double eh = sqrt(e+h);
	//
	if(index == 0) //always add the first one
	{
		pos = _picks - 1;
		startIndex = pos;
	}
	else if(eh > _magnitudes[startIndex]) //collect this one
	{
		pos = _picks-1; //add to the last position if it is larger than all items collected
		//find insertion position
		for(size_t i=startIndex+1;i<_picks;i++)
		{
			if(eh <= _magnitudes[i])
			{
				//should insert before it
				pos = i - 1;
				break;
			}
		}
		if(startIndex > 0) //keep this item
		{
			startIndex--;
		}
		//shift upwards
		for(size_t k=startIndex;k<pos;k++)
		{
			_magnitudes[k] = _magnitudes[k+1];
			_collectedIndexes[k] = _collectedIndexes[k+1];
			_items[k].P.x = _items[k+1].P.x;
			_items[k].P.y = _items[k+1].P.y;
			_items[k].P.z = _items[k+1].P.z;
		}
	}
	else if(startIndex > 0) //collect it before all items already collected
	{
		startIndex --;
		pos = startIndex;
	}
	else //discard this item
	{
		index++;
		return;
	}
	//collect this item at this position
	_magnitudes[pos] = eh;
	_collectedIndexes[pos] = index;
	_items[pos].P.x = x;
	_items[pos].P.y = y;
	_items[pos].P.z = z;
	//
	index++;
}

void PickFieldPoints::onFinish()
{
	inMonotonic = true;
	for(size_t i=0;i<_picks;i++)
	{
		index = _collectedIndexes[i];
		_items[i].E.x = _fields[index].E.x;
		_items[i].E.y = _fields[index].E.y;
		_items[i].E.z = _fields[index].E.z;
		_items[i].H.x = _fields[index].H.x;
		_items[i].H.y = _fields[index].H.y;
		_items[i].H.z = _fields[index].H.z;
		//verify Monotonicity
		if(i > 0 && inMonotonic)
		{
			if(_magnitudes[i] < _magnitudes[i-1])
			{
				inMonotonic = false;
			}
		}
	}
}


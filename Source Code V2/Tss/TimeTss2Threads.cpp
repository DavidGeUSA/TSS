/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

time advancement estimation module, one thread for E-field, one thread for H-field
********************************************************************/
#include "TimeTss2Threads.h"

#include "../ProcessMonitor/ProcessMonitor.h"

#include "ThreadPool.h"

TimeTss2Threads::TimeTss2Threads()
{
	workE = NULL;
	workH = NULL;
	pool = NULL;
	retH = 0; retE = 0;
}


TimeTss2Threads::~TimeTss2Threads()
{
	cleanup();
}

void TimeTss2Threads::cleanup()
{
	TimeTssBase::cleanup();
	if (workE != NULL)
	{
		FreeMemory(workE); workE = NULL;
	}
	if (workH != NULL)
	{
		FreeMemory(workH); workH = NULL;
	}
}
int TimeTss2Threads::initFields(FieldsSetter* f0)
{
	int ret = TimeTssBase::initFields(f0);
	if (ret == ERR_OK)
	{
		if (workE != NULL)
		{
			FreeMemory(workE); workE = NULL;
		}
		workE = (Point3Dstruct *)AllocateMemory(fieldMemorySize);
		if (workE == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}

	if (ret == ERR_OK)
	{
		if (workH != NULL)
		{
			FreeMemory(workH); workH = NULL;
		}
		workH = (Point3Dstruct *)AllocateMemory(fieldMemorySize);
		if (workH == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}

	return ret;
}

void TimeTss2Threads::startThreads()
{
	if (pool == NULL)
	{
		pool = new ThreadPool(2);
	}
}
void TimeTss2Threads::removeThreads()
{
	if (pool != NULL)
	{
		delete pool;
		pool = NULL;
	}
}

inline int TimeTss2Threads::GetFirstCurls()
{
	int ret = ERR_OK;
	//
	space->SetFields(H, E);
	pool->enqueue(std::bind(&TimeTss2Threads::calCurlE, this));
	pool->enqueue(std::bind(&TimeTss2Threads::calCurlH, this));
	pool->wait();
	if (retH != ERR_OK)
	{
		ret = retH;
	}
	else if (retE != ERR_OK)
	{
		ret = retE;
	}

	return ret;
}
inline int TimeTss2Threads::GetNextCurls()
{
	int ret = ERR_OK;
	Point3Dstruct *wk = NULL;
	//
	retH = 0;
	retE = 0;
	//
	wk = curlH;
	curlH = workH;
	workH = wk;
	//
	wk = curlE;
	curlE = workE;
	workE = wk;
	pool->enqueue(std::bind(&TimeTss2Threads::calCurlE, this));
	pool->enqueue(std::bind(&TimeTss2Threads::calCurlH, this));
	pool->wait();
	if (retH != ERR_OK)
	{
		ret = retH;
	}

	if (retE != ERR_OK)
	{
		ret = retE;
	}

	return ret;
}

int TimeTss2Threads::AdvanceOneStep()
{
	int ret = ERR_OK;
	h = 0;
	//
#ifdef TIMESTEPPROFILING
	startTick = GetTimeTick();
#endif
	ret = GetFirstCurls();
#ifdef TIMESTEPPROFILING
	endTick = GetTimeTick();
	timeUsedCal1 = endTick - startTick;
	timeUsedCal1E = endTE - startTE;
	timeUsedCal1H = endTH - startTH;
#endif
	if (ret == ERR_OK)
	{
		if (operationCanceled())
		{
			ret = ERR_SIMULATION_CANCEL;
		}
	}
	if (ret != ERR_OK)
	{
		return ret;
	}
	//Appy curl-0 and curl-1
#ifdef TIMESTEPPROFILING
		startTick = GetTimeTick();
#endif
		
		retH = 0;
		retE = 0;
		pool->enqueue(std::bind(&TimeTss2Threads::apply1stCurlE, this));
		pool->enqueue(std::bind(&TimeTss2Threads::apply1stCurlH, this));
		pool->wait();
#ifdef TIMESTEPPROFILING
		endTick = GetTimeTick();
		timeUsedAppCurl01 = endTick - startTick;
		timeUsedAppCurl01E = endTE - startTE;
		timeUsedAppCurl01H = endTH - startTH;
#endif

	h++;
	while (h <= pams->kmax)
	{
		//get curl{2h}
#ifdef TIMESTEPPROFILING
		startTick = GetTimeTick();
#endif
		ret = GetNextCurls();
#ifdef TIMESTEPPROFILING
		endTick = GetTimeTick();
		timeUsedCal2 = endTick - startTick;
		timeUsedCal2E = endTE - startTE;
		timeUsedCal2H = endTH - startTH;
#endif
		if (ret == ERR_OK)
		{
			if (operationCanceled())
			{
				ret = ERR_SIMULATION_CANCEL;
			}
		}
		if (ret != ERR_OK)
		{
			return ret;
		}
		//apply curl{2h}
#ifdef TIMESTEPPROFILING
			startTick = GetTimeTick();
#endif
			retH = 0;
			retE = 0;
			pool->enqueue(std::bind(&TimeTss2Threads::apply2ndCurlE, this));
			pool->enqueue(std::bind(&TimeTss2Threads::apply2ndCurlH, this));
			pool->wait();
#ifdef TIMESTEPPROFILING
			endTick = GetTimeTick();
			timeUsedAppCurl2 = endTick - startTick;
			timeUsedAppCurl2E = endTE - startTE;
			timeUsedAppCurl2H = endTH - startTH;
#endif

#ifdef TIMESTEPPROFILING
		startTick = GetTimeTick();
#endif
		ret = GetNextCurls();
#ifdef TIMESTEPPROFILING
		endTick = GetTimeTick();
		timeUsedCal3 = endTick - startTick;
		timeUsedCal3E = endTE - startTE;
		timeUsedCal3H = endTH - startTH;
#endif
		if (ret == ERR_OK)
		{
			if (operationCanceled())
			{
				ret = ERR_SIMULATION_CANCEL;
			}
		}
		if (ret != ERR_OK)
		{
			return ret;
		}
		//apply curl{2h+1}
#ifdef TIMESTEPPROFILING
			startTick = GetTimeTick();
#endif
			retH = 0;
			retE = 0;
			pool->enqueue(std::bind(&TimeTss2Threads::apply3rdCurlE, this));
			pool->enqueue(std::bind(&TimeTss2Threads::apply3rdCurlH, this));
			pool->wait();
#ifdef TIMESTEPPROFILING
			endTick = GetTimeTick();
			timeUsedAppCurl3 = endTick - startTick;
			timeUsedAppCurl3E = endTE - startTE;
			timeUsedAppCurl3H = endTH - startTH;
#endif

		h++;
	}
	return ret;
}

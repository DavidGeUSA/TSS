/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

time advancement estimation module, single thread
********************************************************************/
#include "TimeTss.h"
#include "../ProcessMonitor/ProcessMonitor.h"

TimeTss::TimeTss()
{
	work = NULL;
}


TimeTss::~TimeTss()
{
	cleanup();
}

void TimeTss::cleanup()
{
	TimeTssBase::cleanup();
	if (work != NULL)
	{
		FreeMemory(work); work = NULL;
	}
}
int TimeTss::initFields(FieldsSetter* f0)
{
	int ret = TimeTssBase::initFields(f0);
	if (ret == ERR_OK)
	{
		if (work != NULL)
		{
			FreeMemory(work); work = NULL;
		}
		work = (Point3Dstruct *)AllocateMemory(fieldMemorySize);
		if (work == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}
	
	return ret;
}

/*
get curl(E) and curl(H)
*/
int TimeTss::GetFirstCurls()
{
	int ret = ERR_OK;
	
	{
		//set fiels 
		space->SetFields(H, E);
		//
#ifdef TIMESTEPPROFILING
		startTH = GetTimeTick();
#endif
		ret = space->CalculateNextCurlH(curlH);
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
#endif
		if (ret == ERR_OK)
		{
#ifdef TIMESTEPPROFILING
			startTE = GetTimeTick();
#endif
			ret = space->CalculateNextCurlE(curlE);
#ifdef TIMESTEPPROFILING
			endTE = GetTimeTick();
#endif
		}
	}
	return ret;
}

int TimeTss::GetNextCurls()
{
	int ret = ERR_OK;
	Point3Dstruct *wk = NULL;
	{
		//
#ifdef TIMESTEPPROFILING
		startTE = GetTimeTick();
#endif
		ret = space->CalculateNextCurlE(work);
#ifdef TIMESTEPPROFILING
		endTE = GetTimeTick();
#endif
		if (ret == ERR_OK)
		{
			wk = curlE;
			curlE = work;
			work = wk;
			//
#ifdef TIMESTEPPROFILING
			startTH = GetTimeTick();
#endif
			ret = space->CalculateNextCurlH(work);
#ifdef TIMESTEPPROFILING
			endTH = GetTimeTick();
#endif
			if (ret == ERR_OK)
			{
				wk = curlH;
				curlH = work;
				work = wk;
			}
		}
	}
	return ret;
}

int TimeTss::AdvanceOneStep()
{
	int ret = ERR_OK;
	unsigned int h = 0;
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

	{
#ifdef TIMESTEPPROFILING
		startTick = GetTimeTick();
		startTH = GetTimeTick();
		startTE = startTH;
#endif
		w = 0;
		for (i = 0; i <= pams->nx; i++)
		{
			for (j = 0; j <= pams->ny; j++)
			{
				for (k = 0; k <= pams->nz; k++)
				{
					E[w].x = GeeP[h] * E[w].x + GheP[h] * curlH[w].x;
					E[w].y = GeeP[h] * E[w].y + GheP[h] * curlH[w].y;
					E[w].z = GeeP[h] * E[w].z + GheP[h] * curlH[w].z;
					//
					H[w].x = GhhP[h] * H[w].x + GehP[h] * curlE[w].x;
					H[w].y = GhhP[h] * H[w].y + GehP[h] * curlE[w].y;
					H[w].z = GhhP[h] * H[w].z + GehP[h] * curlE[w].z;
					w++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
		endTE = endTH;
		endTick = GetTimeTick();
		timeUsedAppCurl01 = endTick - startTick;
		timeUsedAppCurl01E = endTE - startTE;
		timeUsedAppCurl01H = endTH - startTH;
#endif
	}
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

		{
#ifdef TIMESTEPPROFILING
			startTick = GetTimeTick();
			startTH = GetTimeTick();
			startTE = startTH;
#endif
			w = 0;
			for (i = 0; i <= pams->nx; i++)
			{
				for (j = 0; j <= pams->ny; j++)
				{
					for (k = 0; k <= pams->nz; k++)
					{
						E[w].x += GeeP[h] * curlE[w].x;
						E[w].y += GeeP[h] * curlE[w].y;
						E[w].z += GeeP[h] * curlE[w].z;
						//
						H[w].x += GhhP[h] * curlH[w].x;
						H[w].y += GhhP[h] * curlH[w].y;
						H[w].z += GhhP[h] * curlH[w].z;
						w++;
					}
				}
			}
#ifdef TIMESTEPPROFILING
			endTH = GetTimeTick();
			endTE = endTH;
			endTick = GetTimeTick();
			timeUsedAppCurl2 = endTick - startTick;
			timeUsedAppCurl2E = endTE - startTE;
			timeUsedAppCurl2H = endTH - startTH;
#endif
		}
		//get curl{2h+1}
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
		
		{
#ifdef TIMESTEPPROFILING
			startTick = GetTimeTick();
			startTH = GetTimeTick();
			startTE = startTH;
#endif
			w = 0;
			for (i = 0; i <= pams->nx; i++)
			{
				for (j = 0; j <= pams->ny; j++)
				{
					for (k = 0; k <= pams->nz; k++)
					{
						E[w].x += GheP[h] * curlH[w].x;
						E[w].y += GheP[h] * curlH[w].y;
						E[w].z += GheP[h] * curlH[w].z;
						//
						H[w].x += GehP[h] * curlE[w].x;
						H[w].y += GehP[h] * curlE[w].y;
						H[w].z += GehP[h] * curlE[w].z;
						w++;
					}
				}
			}
#ifdef TIMESTEPPROFILING
			endTH = GetTimeTick();
			endTE = endTH;
			endTick = GetTimeTick();
			timeUsedAppCurl3 = endTick - startTick;
			timeUsedAppCurl3E = endTE - startTE;
			timeUsedAppCurl3H = endTH - startTH;
#endif
		}
		//
		h++;
	}
	return ret;
}
void TimeTss::startThreads()
{
	
}
void TimeTss::removeThreads()
{
	
}
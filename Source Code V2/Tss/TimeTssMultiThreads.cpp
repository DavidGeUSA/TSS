/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

time advancement estimation module, use multiple threads
********************************************************************/
#include "TimeTssMultiThreads.h"

#include "ThreadPool.h"

TimeTssMultiThreads::TimeTssMultiThreads()
{
	workE = NULL;
	workH = NULL;
	pool = NULL;
	retH = 0; retE = 0;
	workCount = 0;
	curl_OneE = NULL;
	curl_OneH = NULL;
}


TimeTssMultiThreads::~TimeTssMultiThreads()
{
	cleanup();
}

void TimeTssMultiThreads::cleanup()
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
	if (curl_OneE != NULL)
	{
		for (unsigned int i = 0; i < workCount; i++)
		{
			delete curl_OneE[i];
		}
		delete curl_OneE; curl_OneE = NULL;
	}
	if (curl_OneH != NULL)
	{
		for (unsigned int i = 0; i < workCount; i++)
		{
			delete curl_OneH[i];
		}
		delete curl_OneH; curl_OneH = NULL;
	}
	removeThreads();
	workCount = 0;
}
int TimeTssMultiThreads::initFields(FieldsSetter* f0)
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
	if (ret == ERR_OK)
	{
		if (pams->threads > 1)
		{
			workCount = pams->threads / 2;
		}
		else
		{
			workCount = 1;
		}
		workCountN1 = workCount;
		divider.initializeDivider(pams->nx, pams->ny, pams->nz, workCount);
		curl_OneE = new CurlOne *[workCount];
		curl_OneH = new CurlOne *[workCount];
		for (unsigned int i = 0; i < workCount; i++)
		{
			curl_OneE[i] = new CurlOne();
			curl_OneH[i] = new CurlOne();
			curl_OneE[i]->setSpaceRange(pams->nx, pams->ny, pams->nz);
			curl_OneH[i]->setSpaceRange(pams->nx, pams->ny, pams->nz);
			curl_OneE[i]->setThreadWork(divider.StartIndex(i), divider.EndIndex(i), divider.StartIndexOneDim(i), i);
			curl_OneH[i]->setThreadWork(divider.StartIndex(i), divider.EndIndex(i), divider.StartIndexOneDim(i), i);
		}
	}
	return ret;
}

void TimeTssMultiThreads::startThreads()
{
	if (pool == NULL)
	{
		pool = new ThreadPool(pams->threads);
	}
}
void TimeTssMultiThreads::removeThreads()
{
	if (pool != NULL)
	{
		delete pool;
		pool = NULL;
	}
}

inline int TimeTssMultiThreads::GetFirstCurls()
{
	for (unsigned int th = 0; th < workCount; th++)
	{
		curl_OneE[th]->setFieldsMemory(E, curlE);
		curl_OneH[th]->setFieldsMemory(H, curlH);
	}
	for (unsigned int th = 0; th < workCount; th++)
	{
		pool->enqueue(std::bind(&CurlOne::calculateCurl, curl_OneE[th]));
		if (th == workCountN1)
		{
			curl_OneH[th]->calculateCurl(); //use the main thread
		}
		else
		{
			pool->enqueue(std::bind(&CurlOne::calculateCurl, curl_OneH[th]));
		}
	}
	pool->wait();
	return 0;
}
inline int TimeTssMultiThreads::GetNextCurls()
{
	for (unsigned int th = 0; th < workCount; th++)
	{
		curl_OneE[th]->setFieldsMemory(curlE, workE);
		curl_OneH[th]->setFieldsMemory(curlH, workH);
	}
	for (unsigned int th = 0; th < workCount; th++)
	{
		pool->enqueue(std::bind(&CurlOne::calculateCurl, curl_OneE[th]));
		if (th == workCountN1)
		{
			curl_OneH[th]->calculateCurl(); //use the main thread
		}
		else
		{
			pool->enqueue(std::bind(&CurlOne::calculateCurl, curl_OneH[th]));
		}
	}
	pool->wait();
	//
	wk = workE;
	workE = curlE;
	curlE = wk;
	//
	wk = workH;
	workH = curlH;
	curlH = wk;
	return 0;
}
int TimeTssMultiThreads::AdvanceOneStep()
{
	int ret = ERR_OK;
	h = 0;
	ret = GetFirstCurls();
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
	for (unsigned int th = 0; th < workCount; th++)
	{
		pool->enqueue(std::bind(&TimeTssMultiThreads::apply1stCurlE, this, th));
		if (th == workCountN1)
		{
			apply1stCurlH(th); //use the main thread
		}
		else
		{
			pool->enqueue(std::bind(&TimeTssMultiThreads::apply1stCurlH, this, th));
		}
	}
	pool->wait();
	if (pml.usePML())
	{
		pml.apply1stCurl(E, H, curlE, curlH);
	}
	h++;
	while (h <= pams->kmax)
	{
		//calculate curls{2h}
		ret = GetNextCurls();
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
		//apply curls{2h}
		for (unsigned int th = 0; th < workCount; th++)
		{
			pool->enqueue(std::bind(&TimeTssMultiThreads::apply2ndCurlE, this, th));
			if (th == workCountN1)
			{
				apply2ndCurlH(th); //use the main thread
			}
			else
			{
				pool->enqueue(std::bind(&TimeTssMultiThreads::apply2ndCurlH, this, th));
			}
		}
		pool->wait();
		if (pml.usePML() && h < 2)//only 4-th order PML is implemented
		{
			pml.apply2ndCurl(E, H, curlE, curlH);
		}
		//calculate curls{2h+1}
		ret = GetNextCurls();
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
		//apply curls{2h+1}
		for (unsigned int th = 0; th < workCount; th++)
		{
			pool->enqueue(std::bind(&TimeTssMultiThreads::apply3rdCurlE, this, th));
			if (th == workCountN1)
			{
				apply3rdCurlH(th); //use the main thread
			}
			else
			{
				pool->enqueue(std::bind(&TimeTssMultiThreads::apply3rdCurlH, this, th));
			}
		}
		pool->wait();
		if (pml.usePML() && h < 2) //only 4-th order PML is implemented
		{
			pml.apply3rdCurl(E, H, curlE, curlH);
		}
		h++;
	}
	//
	return ret;
}
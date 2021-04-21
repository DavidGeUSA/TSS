/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 04/03/2021
Allrights reserved by David Ge

time advancement estimation module with rotation symmetry, use multiple threads
********************************************************************/
#include "TimeTssRotateSymmetryZ.h"


#include "ThreadPool.h"

TimeTssRotateSymmetryZ::TimeTssRotateSymmetryZ()
{
	H = NULL;
	E = NULL;
	pool = NULL;
	retH = 0; retE = 0;
	workCount = 0;
	curl_OneE = NULL;
	curl_OneH = NULL;
	v1 = NULL;
	v2 = NULL;
	wk = NULL;
	curlZE = NULL;
	curlZH = NULL;
	pNextE = NULL;
	pNextH = NULL;
}


TimeTssRotateSymmetryZ::~TimeTssRotateSymmetryZ()
{
	cleanup();
}

void TimeTssRotateSymmetryZ::cleanup()
{
	TimeTssBase::cleanup();
	work0E.cleanup();
	work0H.cleanup();
	work1E.cleanup();
	work1H.cleanup();
	vE.cleanup();
	vH.cleanup();
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
	wk = NULL;
	pNextE = NULL;
	pNextH = NULL;
	curlZH = NULL;
	curlZE = NULL;
	workCount = 0;
}
void TimeTssRotateSymmetryZ::onSettingSimParams()
{
	ic = pams->nx / 2;
	cellCount = (ic + 1)*(pams->nz + 1);
}
int TimeTssRotateSymmetryZ::initFields(FieldsSetter* f0)
{
	int ret = ERR_OK;// = TimeTssBase::initFields(f0);
	puts("\r\nAllocating z-rotatione symmetry field memories...");
	//
	ret = vH.initialVirtualField(pams, _mem);
	if (ret == ERR_OK)
	{
		ret = vE.initialVirtualField(pams, _mem);
	}
	if (ret == ERR_OK)
	{
		double x = pams->xmin;
		double z;
		Point3Dstruct ef;
		Point3Dstruct hf;
		size_t w = 0;
		//
		ef.x = 0;
		ef.y = 0;
		ef.z = 0;
		hf.x = 0;
		hf.y = 0;
		hf.z = 0;
		//
		for (unsigned int i = 0; i <= ic; i++)
		{
			z = pams->zmin;
			for (unsigned int k = 0; k <= pams->nz; k++)
			{
				if (f0 == NULL)
				{

				}
				else
				{
					f0->SetFields(x, 0, z, &ef, &hf);
				}
				vE.setFieldOnPlane(w, &ef);
				vH.setFieldOnPlane(w, &hf);
				z += pams->ds;
				w++;
			}
			x += pams->ds;
		}
	}
	//allocate work memories
	if (ret == ERR_OK)
	{
		work0E.cleanup();
		ret = work0E.initialVirtualField(pams, _mem);
	}
	if (ret == ERR_OK)
	{
		work0H.cleanup();
		ret = work0H.initialVirtualField(pams,_mem);
	}
	if (ret == ERR_OK)
	{
		work1E.cleanup();
		ret = work1E.initialVirtualField(pams, _mem);
	}
	if (ret == ERR_OK)
	{
		work1H.cleanup();
		ret = work1H.initialVirtualField(pams, _mem);
	}
	if (ret == ERR_OK)
	{
		curlZE = &work0E;
		curlZH = &work0H;
		pNextE = &work1E;
		pNextH = &work1H;
	}
	//arrange threads
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
		workCountN1 = workCount - 1;
		divider.initializeDivider(pams->nx, pams->ny, pams->nz, workCount);
		curl_OneE = new CurlOneSymmetryZ *[workCount];
		curl_OneH = new CurlOneSymmetryZ *[workCount];
		for (unsigned int i = 0; i < workCount; i++)
		{
			curl_OneE[i] = new CurlOneSymmetryZ();
			curl_OneH[i] = new CurlOneSymmetryZ();
			curl_OneE[i]->setSpaceRange(pams->nx, pams->ny, pams->nz);
			curl_OneH[i]->setSpaceRange(pams->nx, pams->ny, pams->nz);
			curl_OneE[i]->setThreadWork(divider.StartIndex(i), divider.EndIndex(i), divider.StartIndexOneDim(i), i);
			curl_OneH[i]->setThreadWork(divider.StartIndex(i), divider.EndIndex(i), divider.StartIndexOneDim(i), i);
		}
	}
	return ret;
}

void TimeTssRotateSymmetryZ::startThreads()
{
	if (pool == NULL)
	{
		pool = new ThreadPool(pams->threads);
	}
}
void TimeTssRotateSymmetryZ::removeThreads()
{
	if (pool != NULL)
	{
		delete pool;
		pool = NULL;
	}
}

inline int TimeTssRotateSymmetryZ::GetFirstCurls()
{
	for (unsigned int th = 0; th < workCount; th++)
	{
		curl_OneE[th]->setFieldsMemory(&vE, curlZE);
		curl_OneH[th]->setFieldsMemory(&vH, curlZH);
	}
	for (unsigned int th = 0; th < workCount; th++)
	{
		pool->enqueue(std::bind(&CurlOneSymmetryZ::calculateCurl, curl_OneE[th]));
		if (th == workCountN1)
		{
			curl_OneH[th]->calculateCurl(); //use the main thread
		}
		else
		{
			pool->enqueue(std::bind(&CurlOneSymmetryZ::calculateCurl, curl_OneH[th]));
		}
	}
	pool->wait();
	return 0;
}
inline int TimeTssRotateSymmetryZ::GetNextCurls()
{
	for (unsigned int th = 0; th < workCount; th++)
	{
		curl_OneE[th]->setFieldsMemory(curlZE, pNextE);
		curl_OneH[th]->setFieldsMemory(curlZH, pNextH);
	}
	for (unsigned int th = 0; th < workCount; th++)
	{
		pool->enqueue(std::bind(&CurlOneSymmetryZ::calculateCurl, curl_OneE[th]));
		if (th == workCountN1)
		{
			curl_OneH[th]->calculateCurl(); //use the main thread
		}
		else
		{
			pool->enqueue(std::bind(&CurlOneSymmetryZ::calculateCurl, curl_OneH[th]));
		}
	}
	pool->wait();
	//
	wk = pNextE;
	pNextE = curlZE;
	curlZE = wk;
	//
	wk = pNextH;
	pNextH = curlZH;
	curlZH = wk;
	return 0;
}
int TimeTssRotateSymmetryZ::AdvanceOneStep()
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
		pool->enqueue(std::bind(&TimeTssRotateSymmetryZ::apply1stCurlE, this, th));
		if (th == workCountN1)
		{
			apply1stCurlH(th); //use the main thread
		}
		else
		{
			pool->enqueue(std::bind(&TimeTssRotateSymmetryZ::apply1stCurlH, this, th));
		}
	}
	pool->wait();
	if (pml.usePML())
	{
		pml.apply1stCurlZrotateSymmetry(&vE, &vH, curlZE, curlZH);
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
			pool->enqueue(std::bind(&TimeTssRotateSymmetryZ::apply2ndCurlE, this, th));
			if (th == workCountN1)
			{
				apply2ndCurlH(th); //use the main thread
			}
			else
			{
				pool->enqueue(std::bind(&TimeTssRotateSymmetryZ::apply2ndCurlH, this, th));
			}
		}
		pool->wait();
		if (pml.usePML() && h < 2)//only 4-th order PML is implemented
		{
			pml.apply2ndCurlZrotateSymmetry(&vE, &vH, curlZE, curlZH);
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
			pool->enqueue(std::bind(&TimeTssRotateSymmetryZ::apply3rdCurlE, this, th));
			if (th == workCountN1)
			{
				apply3rdCurlH(th); //use the main thread
			}
			else
			{
				pool->enqueue(std::bind(&TimeTssRotateSymmetryZ::apply3rdCurlH, this, th));
			}
		}
		pool->wait();
		if (pml.usePML() && h < 2) //only 4-th order PML is implemented
		{
			pml.apply3rdCurlZrotateSymmetry(&vE, &vH, curlZE, curlZH);
		}
		h++;
	}
	//
	return ret;
}
////////////////////////////////////////////////////////////////////////////

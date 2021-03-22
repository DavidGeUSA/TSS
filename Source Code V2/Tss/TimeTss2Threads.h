#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

time advancement estimation module, one thread for E-field, one thread for H-field
********************************************************************/
#include "TimeTssBase.h"

class ThreadPool;

/*
	use 2 threads
*/
class TimeTss2Threads : virtual public TimeTssBase
{
private:
	size_t w;
	unsigned int i, j, k;
	//threading------------------------------
	ThreadPool *pool;
	int retH;
	int retE;
	unsigned int h;
	size_t wE;
	unsigned int iE, jE, kE;
	size_t wH;
	unsigned int iH, jH, kH;
#ifdef TIMESTEPPROFILING
	size_t startTE = 0, endTE = 0, startTH = 0, endTH = 0;
#endif

protected:
	Point3Dstruct *workE;
	Point3Dstruct *workH;
	//
	virtual void cleanup();
public:
	TimeTss2Threads();
	virtual ~TimeTss2Threads();
	virtual void startThreads();
	virtual void removeThreads();
	
	virtual int initFields(FieldsSetter* f0);
	virtual int GetFirstCurls();
	virtual int GetNextCurls();
	virtual int AdvanceOneStep();

	///////////////////////////////////////////////////////////////
	void calCurlH()
	{
#ifdef TIMESTEPPROFILING
		startTH = GetTimeTick();
#endif
		retH = space->CalculateNextCurlH(curlH);
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
#endif
	}
	void calCurlE()
	{
#ifdef TIMESTEPPROFILING
		startTE = GetTimeTick();
#endif
		retE = space->CalculateNextCurlE(curlE);
#ifdef TIMESTEPPROFILING
		endTE = GetTimeTick();
#endif
	}

	void apply1stCurlE()
	{
#ifdef TIMESTEPPROFILING
		startTE = GetTimeTick();
#endif
		wE = 0;
		for (iE = 0; iE <= pams->nx; iE++)
		{
			for (jE = 0; jE <= pams->ny; jE++)
			{
				for (kE = 0; kE <= pams->nz; kE++)
				{
					E[wE].x = GeeP[h] * E[wE].x + GheP[h] * curlH[wE].x;
					E[wE].y = GeeP[h] * E[wE].y + GheP[h] * curlH[wE].y;
					E[wE].z = GeeP[h] * E[wE].z + GheP[h] * curlH[wE].z;
					wE++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTE = GetTimeTick();
#endif
	}
	void apply1stCurlH()
	{
#ifdef TIMESTEPPROFILING
		startTH = GetTimeTick();
#endif
		wH = 0;
		for (iH = 0; iH <= pams->nx; iH++)
		{
			for (jH = 0; jH <= pams->ny; jH++)
			{
				for (kH = 0; kH <= pams->nz; kH++)
				{
					H[wH].x = GhhP[h] * H[wH].x + GehP[h] * curlE[wH].x;
					H[wH].y = GhhP[h] * H[wH].y + GehP[h] * curlE[wH].y;
					H[wH].z = GhhP[h] * H[wH].z + GehP[h] * curlE[wH].z;
					wH++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
#endif
	}
	void apply2ndCurlE()
	{
#ifdef TIMESTEPPROFILING
		startTE = GetTimeTick();
#endif
		wE = 0;
		for (iE = 0; iE <= pams->nx; iE++)
		{
			for (jE = 0; jE <= pams->ny; jE++)
			{
				for (kE = 0; kE <= pams->nz; kE++)
				{
					E[wE].x += GeeP[h] * curlE[wE].x;
					E[wE].y += GeeP[h] * curlE[wE].y;
					E[wE].z += GeeP[h] * curlE[wE].z;
					//
					wE++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTE = GetTimeTick();
#endif
	}
	void apply2ndCurlH()
	{
#ifdef TIMESTEPPROFILING
		startTH = GetTimeTick();
#endif
		wH = 0;
		for (iH = 0; iH <= pams->nx; iH++)
		{
			for (jH = 0; jH <= pams->ny; jH++)
			{
				for (kH = 0; kH <= pams->nz; kH++)
				{
					//
					H[wH].x += GhhP[h] * curlH[wH].x;
					H[wH].y += GhhP[h] * curlH[wH].y;
					H[wH].z += GhhP[h] * curlH[wH].z;
					wH++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
#endif
	}
	void apply3rdCurlE()
	{
#ifdef TIMESTEPPROFILING
		startTE = GetTimeTick();
#endif
		wE = 0;
		for (iE = 0; iE <= pams->nx; iE++)
		{
			for (jE = 0; jE <= pams->ny; jE++)
			{
				for (kE = 0; kE <= pams->nz; kE++)
				{
					E[wE].x += GheP[h] * curlH[wE].x;
					E[wE].y += GheP[h] * curlH[wE].y;
					E[wE].z += GheP[h] * curlH[wE].z;
					//
					wE++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTE = GetTimeTick();
#endif
	}
	void apply3rdCurlH()
	{
#ifdef TIMESTEPPROFILING
		startTH = GetTimeTick();
#endif
		wH = 0;
		for (iH = 0; iH <= pams->nx; iH++)
		{
			for (jH = 0; jH <= pams->ny; jH++)
			{
				for (kH = 0; kH <= pams->nz; kH++)
				{
					//
					H[wH].x += GehP[h] * curlE[wH].x;
					H[wH].y += GehP[h] * curlE[wH].y;
					H[wH].z += GehP[h] * curlE[wH].z;
					wH++;
				}
			}
		}
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
#endif
	}

};


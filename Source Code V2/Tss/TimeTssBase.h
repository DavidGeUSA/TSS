#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

base class for time advancement estimation module
********************************************************************/
#include "Tss.h"
#include "FieldsSetter.h"
#include "FieldSourceTss.h"
#include "Space.h"
#include "PmlTss.h"

#include "../ProcessMonitor/workProcess.h"

//do not delete it. uncomment it to enable time tracking for measuring calculation speeds
//#define TIMESTEPPROFILING

/*
  make time advancement
*/
class TimeTssBase :virtual public MemoryManUser
{
protected:
	SimStruct *pams;
	size_t cellCount;            //(nx+1)*(ny+1)*(nz+1)
	size_t fieldMemorySize;      //(nx+1)*(ny+1)*(nz+1)*sizeof(Point3Dstruct)
	//
	unsigned int tIndex;   //=0,1,2,...
	double currentTime;    //current time = tIndex * dt 
	//
	FieldSourceTss *source; //field source provider
	Space *space;           //space derivative and curl estimator
	PmlTss pml;
	//
	fnProgressReport reporter; //report progress
	fnOperationCanceld operationCanceled; //reports if the simulation is canceled
	//
	//field memories
	Point3Dstruct *H;
	Point3Dstruct *E;
	Point3Dstruct *curlH;
	Point3Dstruct *curlE;
	//
	//time advance coefficients
	// G_e^h[0..k_max], G_h^h[0..k_max], G_e^e[0..k_max], G_h^e[0..k_max]
	double *GehP, *GhhP, *GeeP, *GheP;
	//
	virtual void cleanup();
public:
	TimeTssBase();
	virtual ~TimeTssBase();
	
	virtual void startThreads(){}
	virtual void removeThreads(){}
	void setReporter(fnProgressReport rpt, fnOperationCanceld cancelReport){ reporter = rpt; if (cancelReport) operationCanceled = cancelReport; }
	int initializeTimeModule(Space *s, SimStruct *pams0, FieldSourceTss *src);
	virtual int initFields(FieldsSetter* f0);
	size_t GetFieldMemorySize(){ return fieldMemorySize; }
	unsigned int GetemMax(){ if (source == NULL) return 0; return source->GetemMax(); }
	unsigned int GetsrcDim(){ if (source == NULL) return 0; return source->GetsrcDim(); }
	bool Canceled(){ return operationCanceled(); }
	int writeCoefficientsToFile(char *file, unsigned int maxTimeSteps, double courantFactor, unsigned int dataFileSteps);
	//
	//for unit tests
	double *Get_eh(){ return GehP; }
	double *Get_hh(){ return GhhP; }
	double *Get_ee(){ return GeeP; }
	double *Get_he(){ return GheP; }
	double *GetJmH(){ return source->GetJmH(); }
	double *GetJeH(){ return source->GetJeH(); }
	double *GetJeE(){ return source->GetJeE(); }
	double *GetJmE(){ return source->GetJmE(); }
	//
	Point3Dstruct *GetFieldH(){ return H; }
	Point3Dstruct *GetFieldE(){ return E; }
	//
	virtual int GetFirstCurls()=0;
	virtual int GetNextCurls()=0;
	Point3Dstruct *GetCurrentCurlH(){ return curlH; }
	Point3Dstruct *GetCurrentCurlE(){ return curlE; }
	//
	//for Yee-style algorithms
	virtual bool needModifyFieldsForStatistics(){ return false; }
	virtual void modifyFieldForStatistics_E(Point3Dstruct *src, Point3Dstruct *dest){}
	virtual void modifyFieldForStatistics_H(Point3Dstruct *src, Point3Dstruct *dest){}
	//
	//major functionlity: advance fields one time step
	virtual int AdvanceOneStep()=0;
	//time step management===================================
	void advanceTimeStepValue(){
		tIndex++;
		currentTime += pams->dt;
	}
	void resetTime(){ tIndex = 0; currentTime = 0.0; }
	void setStartTime(unsigned int startStep){ tIndex = startStep; currentTime = startStep*pams->dt; }
	unsigned int GetTimeIndex(){ return tIndex; }
	double GetTimeValue(){ return currentTime; }
	//========================================================
#ifdef TIMESTEPPROFILING
	//debug
	size_t startTick, endTick;
	size_t timeUsedCal1, timeUsedCal2, timeUsedCal3,timeUsedAppCurl01,timeUsedAppCurl2,timeUsedAppCurl3;
	size_t timeUsedCal1E, timeUsedCal2E, timeUsedCal3E, timeUsedAppCurl01E, timeUsedAppCurl2E, timeUsedAppCurl3E;
	size_t timeUsedCal1H, timeUsedCal2H, timeUsedCal3H, timeUsedAppCurl01H, timeUsedAppCurl2H, timeUsedAppCurl3H;
#endif
};


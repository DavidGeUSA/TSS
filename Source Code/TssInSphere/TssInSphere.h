#pragma once
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/
#include "..\EMField\EMField.h"
#include "..\EMField\FdtdMemory.h"
#include "..\ProcessMonitor\workProcess.h"
#include "DerivativeEstimator.h"
#include "CurlEstimatorAsymmetric.h"
#include "FieldStatisticsByDivergence.h"
#include "ApplyCurls.h"
#include "..\EMField\FDTD.h"

//reporter not set
#define ERR_TSS_REPORTER   201
//missing space derivative estimator
#define ERR_TSS_DERIVATIVE 202

//initialize maxRadius, maxN and ds
#define INITGEOMETRY(i_N, i_range) \
		N = i_N; \
		maxRadius = GRIDRADIUS(i_N); \
		maxN = GRIDPOINTS(N); \
		ds = SPACESTEP(i_range, i_N);

/*
	Time-Space-Synchronized FDTD algorithm
*/
class TssInSphere: public virtual FDTD //virtual FdtdMemory, public virtual RadiusIndexCacheUser
{
private:
	//
	double dtmu;  //-dt/(ds*mu0) = - 1 / (c0 * sqrt(3) * mu0)
	double dteps; // dt/(ds*eps0) = 1 / (c0 * sqrt(3) * eps0)
	//
	double ae,ah, ae0, ah0; //factors to update fields
	//
protected:
	//constants
	double c0;   //light speed
	double mu0;  //Permeability
	double eps0; //Permittivity
	//
	unsigned long _errorCode; //OS API error code
	fnProgressReport _reporter; //output
	bool _showSummaryOnly;
	//
	FieldsInitializer *eh0; //fields at time 0
	//
	//space derivative/curl estimations
	DerivativeEstimatorAsymmetric *_derivative;
	CurlEstimatorAsymmetric *_curlEstimate;
	//time advancement estimation
	ApplyCurlsEven *_applyCurlsEven;
	ApplyCurlsOdd *_applyCurlsOdd;
	//field statistics
	FieldStatisticsByDivergenceAsymmetric *_fieldStatistics;
	//work variables
	FieldPoint3D *curl0, *curl1;
	virtual int applyCurls(int k);
	//
	//simulation data
	FieldPoint3D **Curls;   //curls; Curls[0] is the curls; Curls[1] is the curls of curls; Curls[0] is the 3rd order curls; Curls[1] is the fourth order curls; and so on
	int curlCount;          //number of curls holded in Curls: Curls[0], Curls[1], ..., Curls[curCount-1]
	//
	virtual void cleanup();
	virtual int onInitialized(TaskFile *taskParameters);
	virtual void createCurlGenerators();
public:
	TssInSphere(void);
	virtual ~TssInSphere(void);
	//
	void setReporter(fnProgressReport reporter, bool showSummaryOnly);
	int verifyFieldsByDivergence(FieldPoint3D *fields);
	int verifyCurls();
	//
	virtual int updateFieldsToMoveForward();
	virtual void OnFinishSimulation();

};



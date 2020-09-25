#pragma once
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include "..\EMField\EMField.h"
#include "..\EMField\RadiusIndex.h"
#include "..\EMField\FDTD.h"
#include "..\EMField\FieldSource.h"
#include "..\EMField\BoundaryCondition.h"
#include "..\EMField\TotalFieldScatteredFieldBoundary.h"
#include "..\ProcessMonitor\workProcess.h"
#include "..\TssInSphere\TssInSphere.h"
#include "..\FileUtil\taskFile.h"
#include <stdio.h>

#define ERR_SIM_FDTD      300
#define ERR_SIM_FIELD0    301
#define ERR_SIM_BOUNDARY  302
#define ERR_SIM_MONOTONIC 303

/*
	EM Fields Simulation Class

	It uses 5 abstract classes to make EM fields simulation:
		FDTD - provide FDTD algorithm
		FieldsInitializer - provide initial value
		BoundaryCondition - provide boundary condition
		FieldSource - provide field source; it is optional
		TotalFieldScatteredFieldBoundary - provide TF/SF boundary; it is optional
	The above 5 objects are instances of subclasses provided by DLL files.
	Configurations of each of the above 5 objects are defined by derived classes.
	Configurations of each of the above 5 objects are stored in a task file.

*/
class FieldSimulation:public virtual RadiusIndexCacheUser
{
private:
	int ret;                 //error code
	//space digitization ---------------------------------------
	int N;                   //number of double space intervals in one side of an axis, starting from one space interval.
	double range;            //space range at one side of the axis
	//calculated space values ----------------------------------
	int maxRadius;           //maximum radius, it is 2N+1
	size_t datapoints;       //number of space points in the simulation cubic
	size_t memorySize;       //field memory size
	double spaceStepSize;    //space step size
	double timeStepSize;     //time step size
	//timing counts for recording simulation speed ------------
	size_t startTime;
	size_t endTime;
	size_t timeUsed;
	size_t totalTime;
	double averageStepTime;
	size_t minimumStepTime;
	//estimation orders ---------------------------------------
	unsigned timeAdvanceOrder;
	unsigned spaceDerivativeOrder;
	//
	wchar_t fieldFile[FILENAME_MAX]; //base file name for creating data files for recoding each time step of the simulation
	//
	fnProgressReport reporter; //for showing simulation progress on the screen
	//
	//plug-in objects--------------------------------------------------------------
	/*
		these objects are to be loaded from DLL files
	*/
	FDTD *fdtd;                             //FDTD algorithm
	FieldsInitializer *field0;              //initial values
	FieldSource *source;                    //field source
	BoundaryCondition *boundaryCondition;   //boundary condition
	TotalFieldScatteredFieldBoundary *tfsf; //total field/scattered field boundary
	//---------------------------------------------------------------------------------
	//
public:
	FieldSimulation();
	~FieldSimulation();
	void setReporter(fnProgressReport rep){reporter = rep;}
	int GetLastError(){return ret;}
	wchar_t *GetBaseFilename();
	//
	//set plug-in objects------------------------------------------------------------
	void setFDTD(FDTD* obj){fdtd = obj;}
	void setFieldInitializer(FieldsInitializer *obj){field0 = obj;}
	void setFieldSource(FieldSource *obj){source = obj;}
	void setBoundaryCondition(BoundaryCondition *obj){boundaryCondition = obj;}
	void setTFST(TotalFieldScatteredFieldBoundary *obj){tfsf = obj;}
	//-------------------------------------------------------------------------------
	bool EnabledFDTDtimeRecording();
	double GetAverageFDTDOneStepTime();
	double GetSumFDTDOneStepTime();
	/*
		run EM simulation and record each time step to a data file
	*/
	int simulationToFiles(TaskFile *taskConfig, const char *dataFolder);
	//
	/*
		create a report file for each data file.

		filenamebase: data file names will be formed by {filenamebase}{n}.em, where {n} = 0,1,2,...
		halfOrderDivergenceEstimate: half of the divergence estimation
	*/
	int createStatisticsFiles(TaskFile *taskConfig, const char *dataFolder);
};




#ifndef __FDTD_H__
#define __FDTD_H__

/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/

#include "EMField.h"
#include "RadiusIndex.h"
#include "FdtdMemory.h"
#include "Plugin.h"
#include "TotalFieldScatteredFieldBoundary.h"
#include "..\FileUtil\taskFile.h"
#include "..\OutputUtil\OutputUtility.h"

#define ERR_EMF_EINVAL 2001

/*
	abstract class for FDTD algorithm. An FDTD class should be implemented in a dynamic link library 
	to be plugged into a simulation system at runtime.
*/
class FDTD:public virtual FdtdMemory, public virtual RadiusIndexCacheUser, public Plugin
{
protected:
	double c0;              //light speed in vacuum
	double courant;         //Courant number
	unsigned N;             //number of double space intervals at one side of an axis, starting at one space interval
	double range;           //geometry range. the whole space range is [-range, range]
	double dt;              //time step = (ds / c0) / sqrt(3.0);
	double ds;              //space step
	//
	unsigned maxN;          //4N+3, memory row size
	unsigned maxRadius;     //2N+1, maximum radius
	//
	size_t _maximumTimeIndex;     //maximum time index for simulation
	int _maxOrderTimeAdvance;     //max half order for time advancement
	int _maxOrderSpaceDerivative; //max half order for space derivative estimations
	//
	double _time; //0, dt, 2dt, 3dt, ...
	//
	TotalFieldScatteredFieldBoundary *_tfsf; //total field/scattered field boundary
	//
	virtual int formBaseFilePath(const char *dataFolder, char *baseName);    //form full path of base file name and assigned it to _basefilename
	virtual void cleanup()=0;      //free memory
	virtual int onInitialized(TaskFile *taskParameters)=0; //called after initialize(...) returns ERR_OK
	/*
		implement FDTD in this function.
		it is called by moveForward
	*/
	virtual int updateFieldsToMoveForward()=0;
	/*
		it is called when the maximum time step is reached
	*/
	virtual void OnFinishSimulation()=0;
	//
//enable speed recording---------------
	bool _recordFDTDStepTimes;
	size_t _timesteps;
	double _sumtimeused;
	unsigned long startTime;
	unsigned long endTime;
	unsigned long timeUsed; //time used for finishing current time step
	int filehandleStepTime; //file handle for recording time for each step
//-------------------------------------
public:
	FDTD(void);
	~FDTD(void);
	//properties----------------------------------------
	double getCourantNumber(){return courant;}
	int getHalfOrderTimeAdvance(){return _maxOrderTimeAdvance;}
	int getHalfOrderSpaceDerivate(){return _maxOrderSpaceDerivative;}
	unsigned getHalfGridNumber(){return N;}
	unsigned getMaxRadius(){return maxRadius;}
	unsigned getOneDsize(){return maxN;}
	double GetSpaceStepSize(){return ds;}
	double GetTimeStepSize(){return dt;}
	double getTime(){return _time;}
	size_t getMaximumTimeIndex(){return _maximumTimeIndex;}
	//--------------------------------------------------
	/*
		prepare for starting simulations

		dataFolder - folder name for creating files to record data at each time step
		tfsf - total field/scattered field boundary object
		taskParameters - task file object. 

		at the end of this function, onInitialized(taskParameters) is called. a derived class may override
		onInitialized to provide additional initializations and read additional configurations defined by the derived class.

		basic configutation values in a task file:
		FDTD.N - integer, half number of space grid on an axis. total number is 2N+1
		FDTD.R - double, half space size on an axis. total size is 2R
		FDTD.maxTimeIndex - long integer, maximum simulation time steps
		FDTD.HalfOrderTimeAdvance - integer, half estimation order for time advancement, optional, default to 1
		FDTD.HalfOrderSpaceDerivate - integer, half estimation order for space derivative, optional, default to 1

	*/
	int initialize(const char *dataFolder, TotalFieldScatteredFieldBoundary *tfsf, TaskFile *taskParameters);
	/*
		move one time step forward
	*/
	int moveForward();
	void FinishSimulation();
	//
	/*
		set field values. 
		it is called by a simulator to initialize fields.
		if special initialization is needed then override this function
	*/
	virtual int PopulateFields(FieldsInitializer *fieldValues);
	//
	virtual bool ReachedMaximumTime(){return _timeIndex >= _maximumTimeIndex;}
	//

//enable speed recording-------------------------
	bool EnabledFDTDtimeRecording();
	double GetAverageFDTDOneStepTime();
	double GetSumFDTDOneStepTime();
	unsigned long GetCurrentFDTDStepTime();
//-----------------------------------------
};

#endif
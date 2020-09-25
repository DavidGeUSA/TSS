/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "FDTD.h"
#include "..\MemoryMan\memman.h"
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  
#include "..\FileUtil\fileutil.h"

/*
	construct an instance of FDTD algorithm module.
	do not do initializations in a constructor. 
	override onInitialized(TaskFile) to do initializations.
*/
FDTD::FDTD(void)
{
	c0 = 299792458.0; //speed of light in vacuum
	courant = 1.0 / sqrt(3.0);
	seriesIndex = NULL;
	_tfsf = NULL;
	_basefilename = NULL;
	N = 0;
	dt = 0;
	ds = 0.0;
	maxRadius = 0;
	maxN = 0;
	_maximumTimeIndex = 0;
	_maxOrderTimeAdvance = 1;
	_maxOrderSpaceDerivative = 1;
	_time = 0.0;
	_recordFDTDStepTimes = false;
	_sumtimeused = 0;
	_timesteps = 0;
	filehandleStepTime = 0;
}
FDTD::~FDTD(void)
{

}

/*
	combine data folder and base name to form a full path for data files. each data file will be formed
	by appending time step index during a simulation.

	if the base name is "DEF" then use simulation task parameters and FDTD class name to form a base name for data files
*/
int FDTD::formBaseFilePath(const char *dataFolder, char *baseName)
{
	int ret = ERR_OK;
	int err;
	char fieldFile[FILENAME_MAX];
	fieldFile[0] = 0;
	if(strcmp(baseName, "DEF") == 0)
	{
		int r1,r2;
		doubleToIntegers(range, &r1,&r2);
		err = sprintf_s(fieldFile, FILENAME_MAX, "%s\\%s_N%d_R%dR%d_T%dS%d_", dataFolder,getClassName(), N,r1,r2,2*_maxOrderTimeAdvance,2*_maxOrderSpaceDerivative);
		if(err == -1)
		{
			ret = ERR_EMF_EINVAL;
		}
	}
	else
	{
		ret = formFilePath(fieldFile, FILENAME_MAX, dataFolder, baseName);
	}
	if(ret == ERR_OK)
	{
		if(_basefilename != NULL)
		{
			free(_basefilename);
		}
		_basefilename = (wchar_t *)malloc(2 * FILENAME_MAX+2);
		if(_basefilename == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		else
		{
			ret = copyC2W(_basefilename, FILENAME_MAX, fieldFile);
		}
	}
	return ret;
}

/*
	prepare for starting simulations

	dataFolder - folder for creating files to record data at each time step
	tfsf - total field/scattered field boundary object
	taskParameters - task file object holding all task parameters. 

	at the end of this function, onInitialized(taskParameters) is called. a derived class may override
	onInitialized to provide additional initializations and read additional configurations defined by the derived class.

	common configutation values in a task file:
	FDTD.N - integer, half number of space grid on an axis. total number is 2N+1
	FDTD.R - double, half space size on an axis. total size is 2R
	FDTD.maxTimeIndex - long integer, maximum simulation time steps
	FDTD.HalfOrderTimeAdvance - integer, half estimation order for time advancement, optional, default to 1
	FDTD.HalfOrderSpaceDerivate - integer, half estimation order for space derivative, optional, default to 1
*/
int FDTD::initialize(const char *dataFolder, TotalFieldScatteredFieldBoundary *tfsf, TaskFile *taskParameters)
{
	int ret = ERR_OK;
	if(seriesIndex == NULL)
	{
		ret = ERR_RADIUS_INDEX_CACHE;
	}
	else
	{
		N = taskParameters->getUInt(TP_FDTDN, false);
		range = taskParameters->getDouble(TP_FDTDR, false);
		_maximumTimeIndex = (size_t)taskParameters->getLong(TP_MAX_TIMESTEP, false);
		_recordFDTDStepTimes = taskParameters->getBoolean(TP_REC_TIMESTEP, true);
		ret = taskParameters->getErrorCode();
		if(ret == ERR_OK)
		{
			_maxOrderTimeAdvance = taskParameters->getInt(TP_HALF_ORDER_TIME, true);
			_maxOrderSpaceDerivative = taskParameters->getInt(TP_HALF_ORDER_SPACE, true);
			if(_maxOrderTimeAdvance == 0) _maxOrderTimeAdvance = 1;
			if(_maxOrderSpaceDerivative == 0) _maxOrderSpaceDerivative = 1;
			_tfsf = tfsf;
		}
	}
	if(ret == ERR_OK)
	{
		if(N == 0)
		{
			ret = ERR_INVALID_SIZE;
		}
		else if(range <= 0.0)
		{
			ret = ERR_INVALID_SIZE;
		}
	}
	if(ret == ERR_OK)
	{
		cleanup();
		//
		if(dataFolder != NULL)
		{
			if(DirectoryExists_c(dataFolder, &ret))
			{
				char *bf = taskParameters->getString(TP_SIMBASENAME, false);
				ret = taskParameters->getErrorCode();
				if(ret == ERR_OK)
				{
					if(bf != NULL && strlen(bf) > 0)
					{
						ret = formBaseFilePath(dataFolder, bf);
					}
				}
			}
			else
			{
				ret = ERR_INVALID_DATA_FOLDER;
			}
		}
	}
	if(ret == ERR_OK)
	{
		//
		maxRadius = GRIDRADIUS(N);
		maxN = GRIDPOINTS(N);
		ds = SPACESTEP(range, N);
		//
		fieldItems = totalPointsInSphere(maxRadius);
		fieldMemorySize = sizeof(FieldPoint3D) * fieldItems;
		//
		dt = (ds / c0) * courant;
		//
		_timeIndex = 0;
		_time = 0.0;
		//
		ret = allocateFieldMemory();
	}
	if(ret == ERR_OK)
	{
		ret = onInitialized(taskParameters);
		if(_recordFDTDStepTimes)
		{
			//create file to record speed
			if(ret == ERR_OK)
			{
				char timefile[FILENAME_MAX];
				ret = copyW2C(timefile, FILENAME_MAX, _basefilename);
				if(ret == ERR_OK)
				{
					int err = sprintf_s(timefile, FILENAME_MAX, "%s.times", timefile);
					if(err == -1)
					{
						ret = ERR_MEM_EINVAL;
					}
					else
					{
						ret = openfileWrite(timefile, &filehandleStepTime);
						if(ret == ERR_OK)
						{
							_sumtimeused = 0;
							_timesteps = 0;
						}
					}
				}
			}
		}
	}
	return ret;
}

int FDTD::moveForward()
{
	int ret  = updateFieldsToMoveForward();
	if(ret == ERR_OK)
	{
		if(filehandleStepTime != 0)
		{
			//write time
			ret = writefile(filehandleStepTime, &timeUsed, sizeof(unsigned long));
		}
	}
	return ret;
}
void FDTD::FinishSimulation()
{
	OnFinishSimulation();
	if(filehandleStepTime != 0)
	{
		closefile(filehandleStepTime);
		filehandleStepTime = 0;
	}
}
/*
	this function is called by a simulation to initialize fields.
	the field initializer is loaded from a dynamic link library.
*/
int FDTD::PopulateFields(FieldsInitializer *fieldValues)
{
	FieldsFiller p(fieldValues, GetFieldMemory());
	int ret = p.gothroughSphere(maxRadius, ds);
	return ret;
}

//enable speed recording
bool FDTD::EnabledFDTDtimeRecording()
{
	return _recordFDTDStepTimes;
}
double FDTD::GetAverageFDTDOneStepTime()
{
	if(_timesteps == 0)
		return 0;
	return _sumtimeused / (double)_timesteps;
}
double FDTD::GetSumFDTDOneStepTime()
{
	return _sumtimeused;
}
unsigned long FDTD::GetCurrentFDTDStepTime()
{
	return timeUsed;
}

//-----------------------------------------

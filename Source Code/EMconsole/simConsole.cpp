/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
// simConsole.cpp : Defines the entry point for the console application.
//

#include <signal.h>
#include <Windows.h>
#include <string.h>
#include <strsafe.h>

#include "simConsole.h"
#include "tasks.h"
#include "..\EMField\EMField.h"
#include "..\OutputUtil\OutputUtility.h"
#include "..\FileUtil\fileutil.h"
#include "..\MemoryMan\memman.h"
#include "..\MemoryMan\MemoryManager.h"
#include "..\MathTools\MathTools.h"
#include "..\TssInSphere\TssInSphere.h"
#include "..\FileUtil\taskFile.h"
#include "taskdef.h"
#include "FieldSimulation.h"

/*
	memory manager is used to allocate large size memories.
	it uses file mapping to allocate memories; the memory limit is drive capacity, not RAM.
	the drive speed affects the program speed. for large domain simulations, it is a good idea to use high speed drives.
	if memory contents are not needed after executing the program then the memory is allocate in work folder specified
	by command line parameter /W
	if memory contents are to be kept after executing the program then the memory is allocated in data folder specified
	by command line parameter /D
*/
MemoryManager *_mem = NULL;

/*
	entry point of the console application. it supports following command line parameters
	/T - full path and name for a task file. a task file specifies what task to run and what task parameters to use
	/W - working folder for creating temporary files such as memory mapping files for allocating large memories
	/L - library folder for dynamic-link-library files; if it is missing then the application folder is used
	/D - folder for data files for simulation results
	/E - folder for data files for simulation results
*/
int main(int argc, char* argv[])
{
	int ret = ERR_OK; //see showError for error messages
	//command line parameters //////////////////////////////////////////////////////
	char taskFile[FILENAME_MAX];           // via /T
	wchar_t workfolderW[FILENAME_MAX];     // via /W
	char workfolder[FILENAME_MAX];         // via /W
	char libFolder[FILENAME_MAX];          // via /L
	char dataFolder[FILENAME_MAX];         // via /D
	char dataFolder2[FILENAME_MAX];        // via /E
	////////////////////////////////////////////////////////////////////////////////
	FieldSimulation *sim = NULL;           //simulator
	//plug-in modles ///////////////////////////////////////////////////////////////
	FDTD                             *FDTDplugin = NULL;
	FieldsInitializer                  *IVplugin = NULL;
	BoundaryCondition                  *BCplugin = NULL;
	FieldSource                        *FSplugin = NULL;
	TotalFieldScatteredFieldBoundary *TFSFplugin = NULL;
	////////////////////////////////////////////////////////////////////////////////
	TaskFile *taskfile = NULL; //task file object for reading task parameters
	int taskIndex = -1; //index into tasks array identifying the task to run
	puts("\r\n.\r\n  Electromagnetic fields Analysis. Version 1.1.1. 2017-02-08 - 2018-03-30.\r\n  Author: David Ge (dge893@gmail.com)\r\n.\r\n");
	//
	taskFile[0] = workfolder[0] = libFolder[0] = dataFolder[0] = dataFolder2[0] = 0;
	workfolderW[0] = 0;
	//
	//parse command line parameters
	if(argc > 1)
	{
		int i;
		int taskCount = sizeof(tasks)/sizeof(*tasks);
		for(i=1; i<argc; i++)
		{
			if(argv[i][0] == '/')
			{
				if(argv[i][1] == 'T')
				{
					strcpy_s(taskFile, &(argv[i][2]));
				}
				else if(argv[i][1] == 'W')
				{
					strcpy_s(workfolder, &(argv[i][2]));
				}
				else if(argv[i][1] == 'L')
				{
					strcpy_s(libFolder, &(argv[i][2]));
				}
				else if(argv[i][1] == 'D')
				{
					strcpy_s(dataFolder, &(argv[i][2]));
				}
				else if(argv[i][1] == 'E')
				{
					strcpy_s(dataFolder2, &(argv[i][2]));
				}
				else if(argv[i][1] == '?')
				{
					showHelp();
				}
				else
				{
					ret = ERR_CMD_COMMANDLINE;
					break;
				}
			}
			else
			{
				ret = ERR_CMD_COMMANDLINE;
				break;
			}
		}
		if(ret == ERR_OK)
		{
			if(taskFile[0] == 0)
			{
				ret = ERR_CMD_TASKFILE;
			}
			else
			{
				printf("\r\nTask file: %s\r\n", taskFile);
				taskfile = new TaskFile(taskFile);
				ret = taskfile->getErrorCode();
				if(ret == ERR_OK)
				{
					int task = taskfile->getInt(TP_SIMTASK, false);
					ret = taskfile->getErrorCode();
					if(ret == ERR_OK)
					{
						for(i=0;i<taskCount;i++)
						{
							if(tasks[i].task == task)
							{
								taskIndex = i;
								printf("\r\n   task number: %d \r\n",tasks[i].task);
								break;
							}
						}
						if(i>=taskCount) //task number specified not found in the task list
						{
							printf("\r\nUndefined task number: %d\r\n", task);
							ret = ERR_TP_INVALID_TASK;
						}
					}
				}
			}
		} //parse command line
	}
	else
	{
		ret = ERR_CMD_COMMANDLINE;
	}
	if(ret == ERR_OK)
	{
		if(workfolder[0] != 0)
		{
			ret = copyC2W(workfolderW, FILENAME_MAX, workfolder);
			if(ret == ERR_OK)
			{
				if(DirectoryExists(workfolderW))
				{
					_mem = new MemoryManager(workfolderW);
				}
				else
				{
					ret = ERR_MEM_DIR_NOTEXIST;
				}
			}
		}
		else
		{
			if(tasks[taskIndex].task != 0)
			{
				ret = ERR_CMD_WORKFOLDER;
			}
		}
	}
	if(ret == ERR_OK)
	{
		//load plug-in modules
		if(taskfile != NULL)
		{
			if(libFolder[0] == 0)
			{
				ret = GetAppFolder(libFolder, FILENAME_MAX);
			}
			if(ret == ERR_OK)
			{
				FDTDplugin = (FDTD *)loadPluginInstance(libFolder, taskfile->getString(TP_SIMFDTD_DLL, true), taskfile->getString(TP_SIMFDTD_NAME, true), &ret);
				IVplugin   = (FieldsInitializer *)loadPluginInstance(libFolder, taskfile->getString(TP_SIMIV_DLL, true), taskfile->getString(TP_SIMIV_NAME, true), &ret);
				BCplugin   = (BoundaryCondition *)loadPluginInstance(libFolder, taskfile->getString(TP_SIMBC_DLL, true), taskfile->getString(TP_SIMBC_NAME, true), &ret);
				FSplugin   = (FieldSource *)loadPluginInstance(libFolder, taskfile->getString(TP_SIMFS_DLL, true), taskfile->getString(TP_SIMFS_NAME, true), &ret);
				TFSFplugin = (TotalFieldScatteredFieldBoundary *)loadPluginInstance(libFolder, taskfile->getString(TP_SIMTFSF_DLL, true), taskfile->getString(TP_SIMTFSF_NAME, true), &ret);
				if(ret == ERR_OK)
				{
					if(FDTDplugin != NULL) FDTDplugin->SetMemoryManager(_mem);
					if(IVplugin   != NULL) IVplugin->SetMemoryManager(_mem);
					if(BCplugin   != NULL) BCplugin->SetMemoryManager(_mem);
					if(FSplugin   != NULL) FSplugin->SetMemoryManager(_mem);
					if(TFSFplugin != NULL) TFSFplugin->SetMemoryManager(_mem);
				}
			}
		}
	}
	//verify command line parameters
	if(ret == ERR_OK)
	{
		if(tasks[taskIndex].needDataFolder)
		{
			if(dataFolder[0] == 0)
			{
				ret = ERR_CMD_DATAFOLDER;
			}
		}
	}
	if(ret == ERR_OK)
	{
		if(tasks[taskIndex].needDataFolder2)
		{
			if(dataFolder2[0] == 0)
			{
				ret = ERR_CMD_DATAFOLDER2;
			}
		}
	}
	if(ret == ERR_OK)
	{
		unsigned int N;
		//execute task
		switch (tasks[taskIndex].task)
		{
		case TASK_HELP:
			showHelp();
			break;
		case TASK_TEST_INDEX_CONVERT:
			N = taskfile->getUInt(TP_FDTDN, false);
			ret = taskfile->getErrorCode();
			if(ret == ERR_OK)
			{
				ret = task1_verifyRadiusIndexConversions(N);
			}
			break;
		case TASK_TEST_RADIUS_INDEX:
			N = taskfile->getUInt(TP_FDTDN, false);
			ret = taskfile->getErrorCode();
			if(ret == ERR_OK)
			{
				ret = task2_verifySphereIndexCache(N);
			}
			break;
		case TASK_TEST_INDEX_SPEED:
			N = taskfile->getUInt(TP_FDTDN, false);
			ret = taskfile->getErrorCode();
			if(ret == ERR_OK)
			{
				ret = task3_sphereIndexSpeedTest(N);
			}
			break;
		case TASK_TEST_FDTD_INIT_FLD:
			if(IVplugin == NULL)
			{
				if(libFolder[0] == 0)
				{
					ret = ERR_CMD_LIBFOLDER;
				}
				else 
					ret = ERR_TP_IV;
			}
			else
			{
				ret = IVplugin->initialize(taskfile);
				if(ret == ERR_OK)
				{
					ret = task4_verifyFieldInitializer(IVplugin, taskfile);
				}
			}
			break;
		case TASK_TEST_FIELD_DIVER:
			if(IVplugin == NULL)
			{
				if(libFolder[0] == 0)
				{
					ret = ERR_CMD_LIBFOLDER;
				}
				else 
					ret = ERR_TP_IV;
			}
			if(ret == ERR_OK)
			{
				ret = IVplugin->initialize(taskfile);
				if(ret == ERR_OK)
				{
					ret = task5_verifyFields(IVplugin, taskfile);
				}
			}
			break;
		case TASK_FDTD_SIMULATION:
			if(IVplugin == NULL)
			{
				if(libFolder[0] == 0)
				{
					ret = ERR_CMD_LIBFOLDER;
				}
				else 
					ret = ERR_TP_IV;
			}
			else if(BCplugin == NULL)
			{
				if(libFolder[0] == 0)
				{
					ret = ERR_CMD_LIBFOLDER;
				}
				else 
					ret = ERR_TP_BC;
			}
			else if(FDTDplugin == NULL)
			{
				if(libFolder[0] == 0)
				{
					ret = ERR_CMD_LIBFOLDER;
				}
				else 
					ret = ERR_TP_FDTD;
			}
			else if(dataFolder[0] == 0)
			{
				ret = ERR_CMD_DATAFOLDER;
			}
			if(ret == ERR_OK)
			{
				sim = new FieldSimulation();
				sim->setReporter(showProgressReport);
				sim->setFDTD(FDTDplugin);
				sim->setFieldInitializer(IVplugin);
				sim->setBoundaryCondition(BCplugin);
				sim->setFieldSource(FSplugin);
				sim->setTFST(TFSFplugin);
				ret = sim->simulationToFiles(taskfile, dataFolder);
			}
			break;
		case TASK_COMPARE_DATA_FILES:
			ret = task110_compareSimData(taskfile, dataFolder, dataFolder2);
			break;
		case TASK_MAKE_REPORT_FILES:
			ret = task120_makeReportFiles(taskfile, dataFolder);
			break;
		case TASK_PICK_POINTS_FILES:
			ret = task130_pickPointsFromDataFiles(taskfile, dataFolder);
			break;
		case TASK_TWO_SUMFILES_TO_ONE:
			ret = task140_mergeSummaryFiles(taskfile, dataFolder, dataFolder2);
			break;
		case TASK_2DS_SUMFILES_TO_ONE:
			ret = task160_mergeSummaryFiles(taskfile, dataFolder, dataFolder2);
			break;
		case TASK_MAKE_POINT_TIME_FILE:
			ret = task170_makePointTimeFile(taskfile, dataFolder);
			break;
		default:
			ret = ERR_SIM_TASK_NOCODE;
			break;
		}
		//
		if(ret != ERR_OK && ret != ERR_REACHED_TIME_LIMIT)
		{
			unsigned long errOS = GetLastWinError();
			if(errOS != 0)
			{
				showWindowsError(L"Simulation", errOS);
			}
			showError("task failed. ", ret);
		}
		else
		{
			if(tasks[taskIndex].task != TASK_HELP )
			{
				puts("\r\n\r\nTask finished");
			}
		}
	}
	else
	{
		if(taskIndex > 0)
		{
			showError("Error starting the task.", ret);
		}
		else
		{
			showError("Error starting the program.", ret);
		}
		if(ret == ERR_CMD_COMMANDLINE)
		{
			showHelp();
		}
	}
	if(taskfile != NULL)
	{
		if(ret == ERR_TASK_PARAMETER_NAME)
		{
			printf("\r\n Task parameter %s is not specified in the task file.\r\n",taskfile->missingParameter());
		}
		else if(ret == ERR_TASK_INVALID_VALUE)
		{
			printf("\r\n An invalid value of task parameter %s is specified in the task file.\r\n",taskfile->invalidParameter());
		}
	}
	ret = ERR_OK;
	ClearPluginInstances(&ret);
	if(ret != ERR_OK)
	{
		showError("Error removing plugins.", ret);
	}
	if(sim != NULL)
	{
		delete sim;
	}
	puts("\r\nPress a key to finish");
	getchar();
	puts("Exiting, freeing memories...");
	//
	if(taskfile != NULL)
	{
		delete taskfile;
	}
	if(_mem != NULL)
	{
		_mem->Clear();
		delete _mem;
	}
	//
	return ret;
}

/*
	show tasks and their parameters
*/
void showHelp()
{
	int taskCount = sizeof(tasks) / sizeof(*tasks);
	puts("\r\nCommand line parameters:\r\n/T - specify a task file path and name; the file contains task number identifying a task to be executed and task parameters\r\n/W - specify a folder for creating temporary files\r\n/L - specify a folder where Dynamic-Link-Library files for plug-in modules are located\r\n/D - specify a folder for reading and writing data files\r\n/E - specify a second folder for reading and writing data files\r\n");
	puts("\r\n\r\nPress a key to see supported task numbers");
	getchar();
	puts("\r\n\r\ntasks :");
	for(int i=0;i<taskCount;i++)
	{
		printf("\r\n   task : #%d - %s\r\n",tasks[i].task,tasks[i].desc);
		if( i > 0 && (i % 25) == 0)
		{
			puts("\r\n\r\nPress a key to see more tasks");
			getchar();
		}
	}
}

/*
	display error text for an integer error code.
	msg - message to be displayed before the error text
	err - error code

	when you define new error codes, you should update this function to add new codes to the "switch ... case";
	if your new error codes duplicate with existing code then the compiler will tell you; 
	in this way you also prevent others from duplicating your error code values.
*/
void showError(const char *msg, int err)
{
	puts("\r\n"); puts(msg); puts("\r\n");
	switch (err)
	{
	case ERR_OUTOFMEMORY://    1
		printf("Out of memory (EM field). (error=%d)",err);
		break;
	case ERR_NOTINITIALIZED:// 2
		printf("FDTD not initialize. (error=%d)",err);
		break;
	case ERR_INVALID_SIZE://   3
		printf("Invalid Geometry size for FDTD. (error=%d)",err);
		break;
	case ERR_INVALID_TIME_ARRAY_SIZE:// 4
		printf("Invalid FDTD time array size. (error=%d)",err);
		break;
	case ERR_INVALID_INIT://   5
		printf("Invalid FDTD initialization. (error=%d)",err);
		break;
	case ERR_INVALID_CALL://   6
		printf("Inalid FDTD estimation algorithm. Must be second order or fourth order. (error=%d)",err);
		break;
	case ERR_INVALID_RADIUS_INDEX: // 7
		printf("Invalid one-dimension array index (too big) for a given radius. (error=%d)",err);
		break;
	case ERR_INVALID_INDEX_RADIUS: //8
		printf("Invalid three-dimension array indexes for a given radius. (error=%d)",err);
		break;
	case ERR_RADIUS_INDEX_UNUSED: //  9
		printf("There is an unused radius index for a given radius. (error=%d)",err);
		break;
	case ERR_RADIUS_INDEX_TOOMANY: //    10
		printf("Too many three-dimension array indexes for a given radius. (error=%d)",err);
		break;
	case ERR_RADIUS_INDEX_MISMATCH: //   11
		printf("Conversion mismatch. (error=%d)",err);
		break;
	case ERR_RADIUS_INDEX_TOOLITTLE: //  12
		printf("Not all points used. (error=%d)",err);
		break;
	case ERR_RADIUS_INDEX_DUPLICATE: //  13
		printf("Duplicated array indexes. (error=%d)",err);
		break;
	case ERR_RADIUS_INDEX_NOTSPHERE: //  14
		printf("array indexes not on sphere. (error=%d)",err);
		break;
	case ERR_RADIUS_INDEX_NEGATIVE: //   15
		printf("Radius index cannot be negative. (error=%d)",err);
		break;
	case ERR_SPHERE_COUNT_MISMATCH: //   16
		printf("Total number of points for a sphere mismatch a sum of points at each radius. (error=%d)",err);
		break;
	case ERR_NO_FIELD_SOURCE: //  17
		printf("Field source not specified. (error=%d)",err);
		break;
	case ERR_GEOMETRY_TOO_BIG:  //18
		printf("Field geometry size too big. (error=%d)",err);
		break;
	case ERR_RADIUS_3DINDEX_TOO_BIG:  //19
		printf("Error converting to series indexing to 3D radius indexes, converted indexes are too big. (error=%d)",err);
		break;
	case ERR_RADIUS_3DINDEXTOOSMALL:  //20
		printf("Error converting to series indexing to 3D radius indexes, converted indexes are too small. (error=%d)",err);
		break;
	case ERR_RADIUS_OUTOFSPHERE:       //21
		printf("A simulation reaches the maximum allowed radius. (error=%d)",err);
		break;
	case ERR_SIMULATION_CANCEL:       //22
		printf("A simulation is canceled . (error=%d)",err);
		break;
	case ERR_REACHED_TIME_LIMIT:     //23
		printf("A simulation has reached the maximum time. (signal=%d)",err);
		break;
	case ERR_INVALID_ESTIMATE_ORDER:  //24
		printf("Estimation order cannot be 0 or negative. (signal=%d)",err);
		break;
	case ERR_RADIUS_INDEX_NOT_END:    //25
		printf("Not all points in a sphere are used. (signal=%d)",err);
		break;
	case ERR_RADIUS_INDEX_CACHE:      //26
		printf("Indexing cache not given. You need to execute function setIndexCache(...) before calling initialize(...) and other functions. (signal=%d)",err);
		break;
	case ERR_RADIUS_HANDLE_DATA:      //27
		printf("logic error in handleData(...). (signal=%d)",err);
		break;
	case ERR_INVALID_DATA_FOLDER:     //28
		printf("data folder does not exist. (signal=%d)",err);
		break;
	//
	case ERR_CMD_COMMANDLINE:       //100
		printf("Invalid command line. Cannot interpret the command line syntax (error=%d)",err);
		break;
	case ERR_CMD_TASKFILE:          //101
		printf("Missing task file (/T parameter) (error=%d)", err);
		break;
	case ERR_CMD_LIBFOLDER:        //102
		printf("Missing dynamic-link-library folder (/L parameter) (error=%d)", err);
		break;
	case ERR_CMD_WORKFOLDER:       //103
		printf("Missing work folder (/W parameter) (error=%d)", err);
		break;
	case ERR_CMD_DATAFOLDER:       //104
		printf("Missing data folder (/D parameter) (error=%d)", err);
		break;
	case ERR_CMD_DATAFOLDER2:      //105
		printf("Missing second data folder (/D parameter) (error=%d)", err);
		break;
	case ERR_SIM_TASK_NOCODE:     //106
		printf("Task code not programmed (error=%d)", err);
		break;

	case ERR_TP_INVALID_N:        //110
		printf("Invalid value for half number of space grids. Check task parameter FDTD.N (error=%d)", err);
		break;
	case ERR_TP_INVALID_R:        //111
		printf("Invalid value for half geometry range. Check task parameter FDTD.R (error=%d)", err);
		break;
	case ERR_TP_INVALID_TASK:     //112
		printf("Invalid value for task number SIM.TASK (error=%d)", err);
		break;
	case ERR_TP_DATAFILE:         //113
		printf("data file does not exist (error=%d)", err);
		break;

	case ERR_TP_FDTD:             //120
		printf("Missing FDTD module. Check task parameters SIM.FDTD_DLL and SIM.FDTD_NAME (error=%d)", err);
		break;
	case ERR_TP_IV:               //121
		printf("Missing Initial Value module. Check task parameters SIM.IV_DLL and SIM.IV_NAME (error=%d)", err);
		break;
	case ERR_TP_BC:               //122
		printf("Missing Boundary Condition module. Check task parameters SIM.BC_DLL and SIM.BC_NAME (error=%d)", err);
		break;
	case ERR_TP_BASENAME:         //123
		printf("Missing base file name. Use task parameter SIM.BASENAME to specify it. (error=%d)", err);
		break;
	case ERR_TP_BASENAME_DEF:     //124
		printf("Cannot use 'DEF' for task parameter SIM.BASENAME. (error=%d)", err);
		break;

	case ERR_TSS_REPORTER://      201
		printf("Reporter not assigned (error=%d)", err);
		break;
	case ERR_TSS_DERIVATIVE://    202
		printf("missing space derivative estimator (error=%d)", err);
		break;

	case ERR_SIM_FDTD://     300
		printf("FDTD module not loaded (error=%d)", err);
		break;
	case ERR_SIM_FIELD0://   301
		printf("Initial Value module not loaded (error=%d)", err);
		break;
	case ERR_SIM_BOUNDARY:// 302
		printf("Boundary Condition module not loaded (error=%d)", err);
		break;
	case ERR_SIM_MONOTONIC:// 303
		printf("Sorting algoritm error: result list is not monotonic (error=%d)", err);
		break;

	case ERR_TASKFIILE_INVALID://       380
		printf("Invalid task parameter formatting. Each parameter value should be expressed as 'name=value' in one line in a task file. (error=%d)", err);
		break;
	case ERR_TASKFIILE_DUPLICATE://     381
		printf("Duplicated task parameter value in a task file (error=%d)", err);
		break;
	case ERR_TASK_PARAMETER_NAME://     382
		printf("Task parameter name not in the task file (error=%d)", err);
		break;
	case ERR_TASKFIILE_NOTEXIST://      383
		printf("Task file does not exist (error=%d)", err);
		break;
	case ERR_TASK_INVALID_VALUE://      384
		printf("Invalid task parameter value (error=%d)", err);
		break;

	case ERR_FILE_OPEN_READ_EACCES://   1001
		printf("Error open file for reading. file's sharing mode does not allow the specified operations, or the given path is a directory. (error=%d)",err);
		break;
	case ERR_FILE_OPEN_READ_EEXIST://   1002
		printf("Error open file for reading. _O_CREAT and _O_EXCL flags specified, but filename already exists. (error=%d)",err);
		break;
	case ERR_FILE_OPEN_READ_EINVAL://   1003
		printf("Error open file for reading. Invalid oflag or pmode argument. (error=%d)",err);
		break;
	case ERR_FILE_OPEN_READ_EMFILE://   1004
		printf("Error open file for reading. No more file descriptors are available (too many files are open). (error=%d)",err);
		break;
	case ERR_FILE_OPEN_READ_ENOENT://   1005
		printf("Error open file for reading. File does not exist. (error=%d)",err);
		break;
	case ERR_FILE_OPEN_READ_UNDEFI://   1006
		printf("Error open file for reading. File does not exist (UNDEFI). (error=%d)",err);
		break;
	case ERR_FILE_OPEN_WRIT_EACCES://   1011
		printf("Error open file for writing. file's sharing mode does not allow the specified operations, or the given path is a directory. (error=%d)",err);
		break;
	case ERR_FILE_OPEN_WRIT_EEXIST://   1012
		printf("Error open file for writing. _O_CREAT and _O_EXCL flags specified, but filename already exists. (error=%d)",err);
		break;
	case ERR_FILE_OPEN_WRIT_EINVAL://   1013
		printf("Error open file for writing. Invalid oflag or pmode argument. (error=%d)",err);
		break;
	case ERR_FILE_OPEN_WRIT_EMFILE://   1014
		printf("Error open file for writing. No more file descriptors are available (too many files are open). (error=%d)",err);
		break;
	case ERR_FILE_OPEN_WRIT_ENOENT://   1015
		printf("Error open file for writing. File does not exist. (error=%d)",err);
		break;
	case ERR_FILE_OPEN_WRIT_UNDEFI://   1016
		printf("Error open file for writing. File does not exist (UNDEFI). (error=%d)",err);
		break;
	case ERR_FILE_WRITE_FAIL://    1020
		printf("Error writing file. (error=%d)",err);
		break;
	case ERR_FILE_WRITE_LESS://    1021
		printf("Error writing file. Not all data written. (error=%d)",err);
		break;
	case ERR_FILE_READ_FAIL://     1030
		printf("Error reading file. (error=%d)",err);
		break;
	case ERR_FILE_READ_LESS://     1031
		printf("Error reading file. Not all data read. (error=%d)",err);
		break;
	case ERR_FILE_READ_EOF://      1032
		printf("Error reading file. Already at end of file. (error=%d)",err);
		break;
	case ERR_GET_FILESIZE://    1050
		printf("Error geting file size. Not all data read. (error=%d)",err);
		break;
	case ERR_FILE_NOTEXIST://   1051
		printf("File does not exist. (error=%d)",err);
		break;
	case ERR_FILE_NAMING://   1052
		printf("Invalid file name. (error=%d)",err);
		break;
	case ERR_FILENAME_LONG:// 1053
		printf("File name is too long. (error=%d)",err);
		break;
	case ERR_FILESIZE_MISMATCH:// 1054
		printf("File size mismatch. (error=%d)",err);
		break;

	case ERR_EMF_EINVAL: //         2001
		printf("Error formatning file name. (error=%d)",err);
		break;


	case ERR_MEM_CREATE_FILE: //    6001
		printf("Error creating/opening file. (error=%d)",err);
		break;
	case ERR_MEM_CREATE_MAP: //     6002
		printf("Error creating memory map file. (error=%d)",err);
		break;
	case ERR_MEM_CREATE_VIEW: //    6003
		printf("Error creating memory map view. (error=%d)",err);
		break;
	case ERR_MEM_ADDR_NOT_FOUND: // 6004
		printf("Error locating allocated address for memory map. (error=%d)",err);
		break;
	case ERR_MEM_EINVAL: //         6005
		printf("Error formatning file name. (error=%d)",err);
		break;
	case ERR_MEM_WRITE_FILE: //     6006
		printf("Error writing file. (error=%d)",err);
		break;
	case ERR_MEM_OPEN_FILE: //      6007
		printf("Error opening file for memory mapping. (error=%d)",err);
		break;
	case ERR_INVALID_DLLPARAM: //   6008
		printf("Error loading DLL file: invalid parameter. (error=%d)",err);
		break;
	case ERR_INVALID_DLLPATH: //    6009
		printf("Error loading DLL file: invalid path. (error=%d)",err);
		break;
	case ERR_DLL_FUNC_NOTFOUND: //  6010
		printf("Error loading DLL file: function not found. (error=%d)",err);
		break;
	case ERR_DLL_FUNC_ERROR: //     6011
		printf("Error executing DLL file. Is the class constructor listed in Export.cpp? (error=%d)",err);
		break;
	case ERR_INVALID_PARAMS: //     6012
		printf("Invalid parameters. (error=%d)",err);
		break;
	case ERR_MEM_GET_FILE_SIZE: //  6013
		printf("Error getting file size. (error=%d)",err);
		break;
	case ERR_STR_SIZE_TOO_SMALL:// 6014
		printf("String size too small. (error=%d)",err);
		break;
	case ERR_FOLDER_NOTEXIST://    6015
		printf("Folder does not exist. (error=%d)",err);
		break;
	case ERR_MEM_MAN_NULL://       6016
		printf("Memory manager does not exist. (error=%d)",err);
		break;
	case ERR_MEM_MAN_APP_PATH://   6017
		printf("Error getting app folder. (error=%d)",err);
		break;
	case ERR_MEM_OUTOFMEMORY://    6018
		printf("Out of memory. (error=%d)",err);
		break;
	case ERR_MEM_DIR_NOTEXIST://   6019
		printf("Folder does not exist. (error=%d)",err);
		break;
	case ERR_MEM_UNKNOWN: //       6030
		printf("Unknown memory management error. (error=%d)",err);
		break;
		
	case ERR_MATH_DET_0:       //8001
		printf("A matrix's determinant is 0. (error=%d)",err);
		break;
	case ERR_MATH_INV_DIM:     //8002
		printf("Matrix dimension cannot be 0 or negative. (error=%d)",err);
		break;
	case ERR_MATH_OUTOFMEMORY: //8003
		printf("Out of memory. (error=%d)",err);
		break;
	case ERR_MATH_NOT_INVERSE: //8004
		printf("It is not an inverse matrix of a given matrix. (error=%d)",err);
		break;
	case ERR_MATH_MEM_SMALL:  //8100
		printf("Memory allocated is too small. (error=%d)",err);
		break;
	default:
		printf("Unknown error. (error=%d)",err);
		break;
	}
}

/*
	show a text message on the console screen.
	message  - message to be displayed
	sameline - indicates whether the message should be displayed at the current line or at a new line
		true: the message is displayed overwriting current line; 
		false: the message is displayed at a new line
*/
void showProgressReport(char *message, bool sameline)
{
	if(sameline)
	{
		movecursorup();
	}
	puts(message);
}

/*
	display error text for an error code reported by an OS, in this case, it is Microsoft Windows
	lpszFunction - OS API function name, which reports the error
	err - OS error code
*/
void showWindowsError(LPTSTR lpszFunction, DWORD err)
{
	if(err != 0)
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );
		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 50) * sizeof(TCHAR)); 
		StringCchPrintf((LPTSTR)lpDisplayBuf, 
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("\r\n%s calling Windows API failed with error %d: %s"), 
			lpszFunction, err, lpMsgBuf); 
		_putws((const wchar_t *)lpDisplayBuf);
		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
	}
}

//file handle for a report file which is a text file. only support one report file per task run.
int reporFileHandle = 0;
/*
	create a new report file and store the file handle to reporFileHandle
*/
int CreateReportFile(const char *filename)
{
	CloseReportFile();
	return openTextfileWrite(filename, &reporFileHandle);
}

/*
	close the report file
*/
void CloseReportFile()
{
	if(reporFileHandle != 0)
	{
		closefile(reporFileHandle);
		reporFileHandle = 0;
	}
}

/*
	write text to the report file. 
	if a report file has not been created then this function falls back to showProgressReport which display the message on the screen.
	if a report file has been created then the message is written to the file. 
		if sameline is false then a a RETURN and a NEWLINE is written to the file before the message.
*/
void writeReport(char *message, bool sameline)
{
	if(reporFileHandle == 0)
	{
		showProgressReport(message, sameline);
	}
	else
	{
		if(!sameline)
		{
			writefile(reporFileHandle, "\r\n", 2);
		}
		writefile(reporFileHandle, message, (int)strlen(message));
	}
}

/*
	load a plug-in module from a dynamic-link-library file
	libFolder - folder for the dynamic-link-library file
	libName - file name for the dynamic-link-library file
	className - class name for the plug-in module
	ret - error code if the loading fails
*/
void *loadPluginInstance(char *libFolder, char *libName, char *className, int *ret)
{
	errno_t err;
	wchar_t dllFile[FILENAME_MAX];
	if(*ret == ERR_OK && libName != NULL && strlen(libName) != 0 && className != NULL && strlen(className) != 0)
	{
		*ret = copyC2W(dllFile, FILENAME_MAX, libFolder);
		if(*ret == ERR_OK)
		{
			if(libFolder[strlen(libFolder)-1] != '\\' && libFolder[strlen(libFolder)-1] != '/')
			{
				err = wcscat_s(dllFile, FILENAME_MAX, L"\\");
				if(err != 0)
				{
					*ret = ERR_STR_SIZE_TOO_SMALL;
				}
			}
		}
		if(*ret == ERR_OK)
		{
			*ret = catC2W(dllFile, FILENAME_MAX, libName);
			if(*ret == ERR_OK)
			{
				return LoadPlugin(dllFile, className, ret);
			}
		}
	}
	return NULL;
}





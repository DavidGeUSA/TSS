#pragma once
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/
#include "..\EMField\EMField.h"
#include "..\TssInSphere\TssInSphere.h"

#define NULL 0

// invalid command line syntax
#define ERR_CMD_COMMANDLINE     100
// missing task file via /T
#define ERR_CMD_TASKFILE        101
// missing library folder for DLL vis /L
#define ERR_CMD_LIBFOLDER       102
// missing work folder via /W
#define ERR_CMD_WORKFOLDER      103
// missing data folder via /D
#define ERR_CMD_DATAFOLDER      104
// missing second data folder via /E
#define ERR_CMD_DATAFOLDER2     105
// task code not implemented
#define ERR_SIM_TASK_NOCODE     106

// invalid task parameters

// invalid number of half grid N
#define ERR_TP_INVALID_N        110
// invalid range R
#define ERR_TP_INVALID_R        111
// invalid task number
#define ERR_TP_INVALID_TASK     112
// data file does not exist
#define ERR_TP_DATAFILE         113

//missing task parameters
#define ERR_TP_FDTD             120
#define ERR_TP_IV               121
#define ERR_TP_BC               122
#define ERR_TP_BASENAME         123
#define ERR_TP_BASENAME_DEF     124

typedef struct TaskInfo
{
	int task;
	bool needDataFolder;
	bool needDataFolder2;
	char *desc;
}TaskInfo;

extern MemoryManager *_mem; //memory manager

void showProgressReport(char *message, bool sameline);
void showHelp();
void showError(const char *msg, int err);
void showWindowsError(wchar_t *lpszFunction, unsigned long err);
void writeReport(char *message, bool sameline);
int CreateReportFile(const char *filename);
void CloseReportFile();
void *loadPluginInstance(char *libFolder, char *libName, char *className, int *ret);



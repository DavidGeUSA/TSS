#pragma once
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility
********************************************************************/
#include "../EMField/EMField.h"
#include "../TssInSphere/TssInSphere.h"

#define TP_SIM_PICKS   "SIM.POINTS"    //the number of field points to be picked out of a data file and included in a new data file, having largest field values
#define TP_SIM_MAXTIME "SIM.MAXTIMES"  //maximum time steps for a simulation

int task1_verifyRadiusIndexConversions(int N);
int task2_verifySphereIndexCache(int N);
int task3_sphereIndexSpeedTest(int N);
int task4_verifyFieldInitializer(FieldsInitializer *fields0, TaskFile *taskConfig);
int task5_verifyFields(FieldsInitializer *fields0, TaskFile *taskConfig);
int task110_compareSimData(TaskFile *taskConfig, const char *dataFolder1, const char *dataFolder2);
int task120_makeReportFiles(TaskFile *taskConfig, const char *dataFolder);
int task130_pickPointsFromDataFiles(TaskFile *taskConfig, const char *dataFolder);
int task140_mergeSummaryFiles(TaskFile *taskConfig, const char *dataFolder1, const char *dataFolder2);
int task160_mergeSummaryFiles(TaskFile *taskConfig, const char *dataFolder1, const char *dataFolder2);
int task170_makePointTimeFile(TaskFile *taskConfig, const char *dataFolder);
int task301_verifyCurls(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int task302_verifyTimes(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int task303_statisticsRadiusData(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int task304_statisticsInRadiusForCubicData(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int task300_TssSimulation(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter, fnOperationCanceld cancelReport);
int task310_energyDivergence(MemoryManager *_mem, const char *dataFolder, const char *baseFile, char *matrixFolder);
int task315_HeilderCurrent(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int task316_HeilderCurrentDT(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int task317_HeilderCurrentDT2(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int task330_coefficientsOnly(MemoryManager *mem, TaskFile *taskfile, char *dataFolder, fnProgressReport reporter, fnOperationCanceld cancelReport);
int task370_makePointTimeFile(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int task371_createFieldComponentLines(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int task372_makeStatisticsFile(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int task373_combineCSVFiles(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);

//
#pragma once
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include "..\EMField\EMField.h"
#include "..\TssInSphere\TssInSphere.h"

#define TP_SIM_PICKS   "SIM.POINTS"
#define TP_SIM_MAXTIME "SIM.MAXTIMES"

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

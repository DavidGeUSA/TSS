#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/17/2020
Allrights reserved by David Ge

********************************************************************/
#include "../FileUtil/taskFile.h"
#include "../MemoryMan/MemoryManager.h"
#include "../Tss/Tss.h"
#include "../Tss/Simulator.h"

#define TEST_RADIUS_FILE "TEST.RADIUSFILE"
#define TEST_CUBIC_FILE  "TEST.CUBICFILE"
//when doing comparisons, exclude boundaries
#define TEST_EXCLUDE_BOUNDARY "TEST.EXCLUDE.BOUNDARY"
//space locations for generating data files, (i,j,k),(i,j,k),...
#define POINT_GRIDNODES  "POINT.GRIDNODES"
//specify field, can be e or h
#define POINT_EH "POINT.EH"
//specify field component, can be 1 for x, 2 for y or 3 for z
#define FIELD_COMPONENT "FIELD.COMPONENT"
//specify end time steps for combining CSV output files into one file
#define SIM_ENDTIMESTEPS "SIM.ENDTIMESTEPS"

typedef enum {
	FIELD_X = 1,
	FIELD_Y = 2,
	FIELD_Z = 3,
}Field_Component;

class EHErrorSum
{
private:
	SimStruct *pams;
	unsigned int ic;
	double err;
	double maxErrE;
	double maxErrH;
	double sumErrE;
	double sumErrH;
	unsigned int countE;
	unsigned int countH;
public:
	EHErrorSum(SimStruct *p){ 
		pams = p; ic = pams->nx / 2; maxErrE = maxErrH = sumErrE = sumErrH = err = 0; countE = countH = 0; 
	}
	~EHErrorSum(){};
	void addError(double v1, double v2, FIELD_EMTYPE eh){
		err = abs(v1 - v2); 
		if (eh == Field_E)
		{
			if (err > maxErrE) maxErrE = err;
			sumErrE += err;
			countE++;
		}
		else
		{
			if (err > maxErrH) maxErrH = err;
			sumErrH += err;
			countH++;
		}
	}
	double currentError(){ return err; }
	double MaxErrorE(){ return maxErrE; }
	double MaxErrorH(){ return maxErrH; }
	double AverageErrorE(){ if (countE == 0) return 0; return sumErrE / (double)countE; }
	double AverageErrorH(){ if (countH == 0) return 0; return sumErrH / (double)countH; }
	/*
		map = 0 : negative y-axis
			  1 : positive y-axis
			  2 : positive x-axis
	*/
	void addErrorMapToAxis(Point3Dstruct *efield, Point3Dstruct *hfield, size_t w0, size_t w1, int map);
	void addErrorMapToArea(Point3Dstruct *eXaxis, Point3Dstruct *hXaxis, Point3Dstruct *eVal, Point3Dstruct *hVal, double x, double y);
};

Simulator *createSimulator(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, int *ret);
int prepareSimulation(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter, fnOperationCanceld cancelReport);
int tssSimulation(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter, fnOperationCanceld cancelReport);
int createStatisticsForRadiusDataFiles(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int createStatisticsInRadiusForCubicDataFiles(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);

//unit tests
int testSpaceModule(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int testTimeModule(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);

int testHeilderCurrent(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int testHeilderCurrentDT(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int testHeilderCurrentDT2(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);

int makePointTimeFile(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int createFieldComponentLines(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int makeStatisticsFile(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);
int combineCsvFiles(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter);

int verifyZrotateSymmetry(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter, fnOperationCanceld cancelReport);

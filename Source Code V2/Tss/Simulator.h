#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/22/2020
Allrights reserved by David Ge

Time-Space-Synchronized FDTD simulation
********************************************************************/
#include "Tss.h"
#include "Space.h"
#include "TimeTss.h"
#include "TimeTss4th.h"
#include "TimeTss2Threads.h"
#include "TimeTssMultiThreads.h"
#include "TimeYee.h"
#include "BoundaryTss.h"
#include "DivergenceStatistic.h"

//screen message buffer size
#define MESSAGELINESIZE 256

//parameters needed for a simulation
typedef struct TssSimStruct {
	MemoryManager *mem;             //memory manager using file mapping
	fnProgressReport reporter;      //for showing simulation progress on the screen
	fnOperationCanceld canceled;    //for checking if the simulation is canceled by the user
	SimStruct pams;                 //simulation parameters
	FieldSourceTss *src;            //field sources
	FieldsSetter *initFields;       //initial field values
	BoundaryTss *boundaryCondition; //boundary conditions
	DivergenceStatistic *dvgs;      //for calculating statistics on divergences
	char *mainDataFolder;           //data folder by command /D
	char *matrixFileFolder;         //subfolder under /D for read/write space estimation maxtrixes file
	char *dataFileFolder;           //sub folder under folder specified  by command line parameter /D
	char *saveToFilename;           //base file name for saving fields
	char *timeCoefficientsFile;     //if it is not NULL, save time advance coefficients to this file
	char *curlCoefficientsFile;     //if it is not NULL, save curl estimation coefficients to this file
	bool generateStatisticFile;     //whether to generate a statistics file on finishing simulation
}TssSimStruct;

/*
	run TSS simulation
*/
class Simulator:virtual public Plugin
{
private:
	Space space;                     //space derivative and curl estimation
	TimeTssBase *timeAdv;            //time advance estimation
	TssSimStruct simObj;             //simulation configuration
	unsigned int saveFileIndexCount; //time to save fields to file: saveFileIndexCount > saveToFileInterval 
	unsigned int datafileindex;      //appended to data file names; 0,1,2,...
	//
	int saveFieldsToFiles();         //save E and H to two files
	int saveFieldToFile(Point3Dstruct *field, char *filename); //save a field to a file
	int loadFieldFromFile(Point3Dstruct *field, char *filename); //load a field from a file
	int saveSimulateParametersToFile();         //save confiurations to a text file
	int formStatisticsFilename(char *filename); //form a file name for creating a field statistics file
	//
	int writeSpaceEstimationMatricesToFile();   //write space estimation matrixes to a text file
	int writeTimeAdvanceCoefficientsToFile();   //write time advance coefficiets to a text file
	//
public:
	Simulator();
	~Simulator();
	int Initialized();
	//
	void setCalculationMethod(CURLMETHOD method){ space.setCalculationMethod(method); }
	void copyField(Point3Dstruct *from, Point3Dstruct *to);
	//static utilties
	static int LoadSimulatePatamersFromFile(char *file, SimStruct *pams);
	static void GetSimulationParameters(TaskFile *taskConfig, SimStruct *pams);
	static void ResetSimStruct(TssSimStruct *tss);
	static void GetSimStruct(TaskFile *taskConfig, TssSimStruct *tss);
	static int FormSpaceMatrixFilename(char *spaceMatrixFile, const char *dataFolder, unsigned int smax, char *matrixFileFolder);
	//instance utilities
	int formDataFilename(char *filename, const char *eh, const char *ext, unsigned int fileIndex);
	int calculateStatistics(Point3Dstruct *efile, Point3Dstruct *hfile, int fileIndex, FILE *saveFieldToFilefhStatistic);
	int CreateMatrixFile(unsigned int smax0, char *file){ return space.CreateMatrixFile(smax0, file); }
	int GetTime0FilenameE(char *filename);
	int GetTime0FilenameH(char *filename);
	size_t GetFieldMemorySize(){ return space.GetFieldMemorySize(); }
	size_t GetCellCount(){ return space.GetCellCount(); }
	double GetSpaceStep(){ return simObj.pams.ds; }
	TssSimStruct *GetSimParameters(){ return &simObj; }
	int LoadStartingFields(Point3Dstruct *efile, Point3Dstruct *hfile);
	//for unit tests ===============================================================
	void ResetFields(){ space.SetFields(timeAdv->GetFieldH(), timeAdv->GetFieldE()); }
	int GetFirstCurls(){ return timeAdv->GetFirstCurls(); }
	int GetNextCurls(){ return timeAdv->GetNextCurls(); }
	Point3Dstruct *GetFieldE(){ return timeAdv->GetFieldE(); }
	Point3Dstruct *GetFieldH(){ return timeAdv->GetFieldH(); }
	Point3Dstruct *GetCurrentCurlE(){ return timeAdv->GetCurrentCurlE(); }
	Point3Dstruct *GetCurrentCurlH(){ return timeAdv->GetCurrentCurlH(); }
	Curl *GetCurlE(){ return space.GetCurlE(); }
	//derivatives
	double dx_Fx(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k){ return space.dx_Fx(F, h, i, j, k); }
	double dy_Fx(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k){ return space.dy_Fx(F, h, i, j, k); }
	double dz_Fx(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k){ return space.dz_Fx(F, h, i, j, k); }
	double dx_Fy(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k){ return space.dx_Fy(F, h, i, j, k); }
	double dy_Fy(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k){ return space.dy_Fy(F, h, i, j, k); }
	double dz_Fy(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k){ return space.dz_Fy(F, h, i, j, k); }
	double dx_Fz(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k){ return space.dx_Fz(F, h, i, j, k); }
	double dy_Fz(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k){ return space.dy_Fz(F, h, i, j, k); }
	double dz_Fz(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k){ return space.dz_Fz(F, h, i, j, k); }
	//time module
	unsigned int GetemMax(){ return timeAdv->GetemMax(); }
	unsigned int GetsrcDim(){ return timeAdv->GetsrcDim(); }
	double *Get_eh(){ return timeAdv->Get_eh(); }
	double *Get_hh(){ return timeAdv->Get_hh(); }
	double *Get_ee(){ return timeAdv->Get_ee(); }
	double *Get_he(){ return timeAdv->Get_he(); }
	double *GetJmH(){ return timeAdv->GetJmH(); }
	double *GetJeH(){ return timeAdv->GetJeH(); }
	double *GetJeE(){ return timeAdv->GetJeE(); }
	double *GetJmE(){ return timeAdv->GetJmE(); }

	//==============================================================================
	//major functionality
	virtual int initializeSimulator(char *timeClassName);
	virtual int run();
	virtual int generateStatisticFile();
};


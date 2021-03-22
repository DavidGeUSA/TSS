/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/22/2020
Allrights reserved by David Ge

********************************************************************/
#include "Tss.h"
#include "Simulator.h"
#include "DivergenceStatistic.h"
#include "../FileUtil/fileutil.h"
#include "../ProcessMonitor/ProcessMonitor.h"

#include <stdio.h>

#ifdef __linux__ 
#include <cstring>
#endif

Simulator::Simulator()
{
	timeAdv = NULL;
}


Simulator::~Simulator()
{
	if (timeAdv != NULL)
	{
		delete timeAdv;
		timeAdv = NULL;
	}
}

int Simulator::initializeSimulator(char *timeClassName)
{
	int ret = ERR_OK;
	size_t startTime = getTimeTick();
	simObj.reporter("Initialing simulator, please wait ...", false);
	//
	if (timeAdv == NULL)
	{
		if (timeClassName == NULL || strlen(timeClassName) == 0 || strcmp(timeClassName, "TimeTss") == 0)
		{
			TimeTss *p = new TimeTss();
			timeAdv = dynamic_cast<TimeTss *>(p);
		}
		else if (strcmp(timeClassName, "TimeTss4th") == 0)
		{
			TimeTss4th *p = new TimeTss4th();
			timeAdv = dynamic_cast<TimeTss4th *>(p);
		}
		else if (strcmp(timeClassName, "TimeTss2Threads") == 0)
		{
			TimeTss2Threads *p = new TimeTss2Threads();
			timeAdv = dynamic_cast<TimeTss2Threads *>(p);
		}
		else if (strcmp(timeClassName, "TimeTssMultiThreads") == 0)
		{
			TimeTssMultiThreads *p = new TimeTssMultiThreads();
			timeAdv = dynamic_cast<TimeTssMultiThreads *>(p);
		}
		else if (strcmp(timeClassName, "TimeYee") == 0)
		{
			TimeYee *p = new TimeYee();
			timeAdv = dynamic_cast<TimeYee *>(p);
		}
		else
		{
			return ERR_TIMECLASSNAME;
		}
	}
	timeAdv->SetMemoryManager(simObj.mem);
	ret = timeAdv->initializeTimeModule(&space, &(simObj.pams), simObj.src);
	if (ret == ERR_OK)
	{
		char spaceMatrixFile[FILENAME_MAX];
		ret = Simulator::FormSpaceMatrixFilename(spaceMatrixFile, simObj.mainDataFolder, simObj.pams.smax, simObj.matrixFileFolder);
		if (ret == ERR_OK)
		{
			ret = space.initializeSpace(simObj.pams.ds, simObj.pams.nx, simObj.pams.ny, simObj.pams.nz, simObj.pams.smax, spaceMatrixFile);
		}
	}
	if (ret == ERR_OK)
	{
		simObj.dvgs->initialize(&space, &(simObj.pams), simObj.src, true);
	}
	if (ret == ERR_OK)
	{
		if (simObj.boundaryCondition != NULL)
		{
			ret = simObj.boundaryCondition->setSpace(&space);
		}
	}
	if (ret == ERR_OK)
	{
		ret = timeAdv->initFields(simObj.initFields);
	}
	if (ret == ERR_OK)
	{
		space.SetFields(timeAdv->GetFieldH(), timeAdv->GetFieldE());
	}
	if (ret == ERR_OK)
	{
		ret = writeSpaceEstimationMatricesToFile();
	}
	if (ret == ERR_OK)
	{
		ret = writeTimeAdvanceCoefficientsToFile();
	}
	saveFileIndexCount = 0;
	datafileindex = 0;
	size_t endTime = getTimeTick();
	if (ret == ERR_OK)
	{
		ret = Initialized();
	}
	if (ret == ERR_OK)
	{
		char msg[MESSAGELINESIZE];
#ifdef __linux__
		snprintf(msg, MESSAGELINESIZE, "Initialization finished. Time used:%Ilu", endTime - startTime);
#elif _WIN32
		sprintf_s(msg, MESSAGELINESIZE, "Initialization finished. Time used:%Iu", endTime - startTime);
#else
#endif
		simObj.reporter(msg, true);
	}
	else if (ret == ERR_OUTOFMEMORY)
	{
		double dm = (((double)(timeAdv->GetFieldMemorySize()) / 1024.0) / 1024.0) / 1024.0;
		char msg[MESSAGELINESIZE];
#ifdef __linux__
		snprintf(msg, MESSAGELINESIZE, "Initialization failed. Memory needed for one field:%Ilu Bytes (%g GB)", timeAdv->GetFieldMemorySize(), dm);
#elif _WIN32
		sprintf_s(msg, MESSAGELINESIZE, "Initialization failed. Memory needed for one field:%Iu Bytes (%g GB)", timeAdv->GetFieldMemorySize(), dm);
#else
#endif
		
		simObj.reporter(msg, false);
	}
	else
	{
		simObj.reporter("Initialization failed", false);
	}
	return ret;
}

void Simulator::copyField(Point3Dstruct *from, Point3Dstruct *to)
{
	size_t w = 0;
	for (unsigned int i = 0; i <= simObj.pams.nx; i++)
	{
		for (unsigned int j = 0; j <= simObj.pams.ny; j++)
		{
			for (unsigned int k = 0; k <= simObj.pams.nz; k++)
			{
				to[w].x = from[w].x;
				to[w].y = from[w].y;
				to[w].z = from[w].z;
				w++;
			}
		}
	}
}

int Simulator::writeTimeAdvanceCoefficientsToFile()
{
	int ret = ERR_OK;
	if (simObj.timeCoefficientsFile != NULL)
	{
		char fn[FILENAME_MAX];
		char pn[FILENAME_MAX];
		if (simObj.dataFileFolder != NULL)
		{
			ret = formFilePath(pn, FILENAME_MAX, simObj.mainDataFolder, simObj.dataFileFolder);
			if (ret == ERR_OK)
			{
				if (!directoryExists(pn))
				{
					if (!createdirectory(pn))
					{
						ret = ERR_INVALID_DATA_FOLDER;
					}
				}
				if (ret == ERR_OK)
				{
					ret = formFilePath(fn, FILENAME_MAX, pn, simObj.timeCoefficientsFile);
				}
			}
		}
		else
		{
			ret = formFilePath(fn, FILENAME_MAX, simObj.mainDataFolder, simObj.timeCoefficientsFile);
		}
		if (ret == ERR_OK)
		{
			ret = deleteFile(fn);
			if (ret == ERR_OK)
			{
				ret = timeAdv->writeCoefficientsToFile(fn,simObj.pams.maxTimeSteps, simObj.pams.courantFactor, simObj.pams.saveInterval);
			}
		}
	}
	return ret;
}

int Simulator::writeSpaceEstimationMatricesToFile()
{
	int ret = ERR_OK;
	if (simObj.curlCoefficientsFile != NULL)
	{
		char fn[FILENAME_MAX];
		char pn[FILENAME_MAX];
		if (simObj.dataFileFolder != NULL)
		{
			ret = formFilePath(pn, FILENAME_MAX, simObj.mainDataFolder, simObj.dataFileFolder);
			if (ret == ERR_OK)
			{
				if (!directoryExists(pn))
				{
					if (!createdirectory(pn))
					{
						ret = ERR_INVALID_DATA_FOLDER;
					}
				}
				if (ret == ERR_OK)
				{
					ret = formFilePath(fn, FILENAME_MAX, pn, simObj.curlCoefficientsFile);
				}
			}
		}
		else
		{
			ret = formFilePath(fn, FILENAME_MAX, simObj.mainDataFolder, simObj.curlCoefficientsFile);
		}
		if (ret == ERR_OK)
		{
			ret = space.writeMatrixesToFile(fn);
		}
	}
	return ret;
}

int Simulator::saveFieldToFile(Point3Dstruct *field, char *filename)
{
	int ret = ERR_OK;
#ifdef __linux__ 
	size_t cc = space.GetCellCount();
	FILE *fp = fopen(filename, "wb");
	if (fp == NULL)
	{
		ret = ERR_FILE_OPEN_WRIT_EACCES;
	}
	else
	{
		size_t sizew = fwrite(field, sizeof(Point3Dstruct), cc, fp);
		fclose(fp);
		if (sizew != cc)
		{
			ret = ERR_FILE_WRITE_LESS;
		}
	}
#elif _WIN32
	MemoryManager *_mem = simObj.mem;
	size_t cc = space.GetCellCount();
	if (ret == ERR_OK)
	{
		Point3Dstruct *f = (Point3Dstruct *)CreateFileIntoMemory(filename, timeAdv->GetFieldMemorySize(), &ret);
		if (ret == ERR_OK)
		{
			for (size_t w = 0; w < cc; w++)
			{
				f[w].x = field[w].x;
				f[w].y = field[w].y;
				f[w].z = field[w].z;
			}
			FreeMemory(f);
		}
	}
#else
#endif
	return ret;
}
int Simulator::loadFieldFromFile(Point3Dstruct *field, char *filename)
{
	int ret = ERR_OK;
	MemoryManager *_mem = simObj.mem;
	//wchar_t fnW[FILENAME_MAX];
	size_t cc = space.GetCellCount();
	size_t fSize = 0;
	//ret = copyC2W(fnW, FILENAME_MAX, filename);
	if (ret == ERR_OK)
	{
		Point3Dstruct *f = (Point3Dstruct *)ReadFileIntoMemory(filename, &fSize, &ret);
		if (ret == ERR_OK)
		{
			if (timeAdv->GetFieldMemorySize() != fSize)
			{
				ret = ERR_FILESIZE_MISMATCH;
			}
			else
			{
				for (size_t w = 0; w < cc; w++)
				{
					field[w].x = f[w].x;
					field[w].y = f[w].y;
					field[w].z = f[w].z;
				}
			}
			FreeMemory(f);
		}
	}
	return ret;
}

/*
	load SimStruct from a file created by saveSimulateParametersToFile
*/
int Simulator::LoadSimulatePatamersFromFile(char *file, SimStruct *pams)
{
	int ret = ERR_OK;
	FILE *fh = 0;
	ret = openfileRead(file, &fh);
	if (ret == ERR_OK)
	{
		ret = readfile(fh, pams, sizeof(SimStruct));
		closefile(fh);
	}
	return ret;
}

void Simulator::ResetSimStruct(TssSimStruct *tss)
{
	tss->boundaryCondition = NULL;
	tss->curlCoefficientsFile = NULL;
	tss->initFields = NULL;
	tss->matrixFileFolder = NULL;
	tss->dataFileFolder = NULL;
	tss->mainDataFolder = NULL;
	tss->mem = NULL;
	tss->pams.ds = 0;
	tss->pams.dt = 0;
	tss->pams.eps = 0;
	tss->pams.kmax = 0;
	tss->pams.maxTimeSteps = 0;
	tss->pams.startTimeStep = 0;
	tss->pams.mu = 0;
	tss->pams.nx = 0;
	tss->pams.ny = 0;
	tss->pams.nz = 0;
	tss->pams.rho = 0;
	tss->pams.saveInterval = 0;
	tss->pams.sie = 0;
	tss->pams.sim = 0;
	tss->pams.smax = 0;
	tss->pams.xmin = 0;
	tss->pams.ymin = 0;
	tss->pams.zmin = 0;
	tss->reporter = NULL;
	tss->saveToFilename = NULL;
	tss->src = NULL;
	tss->timeCoefficientsFile = NULL;
}

/*
	read FDTD parameters from task file
	parameter validations are not performed.
*/
void Simulator::GetSimulationParameters(TaskFile *taskConfig, SimStruct *pams)
{
	pams->courantFactor = taskConfig->getDouble(TP_COURFACTOR, true);
	pams->threads = taskConfig->getUInt(TP_SIM_USETHREADS, true);
	pams->rho = taskConfig->getDouble(TP_TSS_RHO, true);
	if (taskConfig->getBoolean(TP_TSS_VAC, true))
	{
		//vacuum physics constants ==============
		double c0 = 299792458.0; //speed of light
		pams->mu = 4.0 * M_PI * 1.0e-7;
		pams->eps = 1.0 / ((pams->mu * c0) * c0);
		//=================================
	}
	else
	{
		pams->eps = taskConfig->getDouble(TP_TSS_EPS, true);
		pams->mu = taskConfig->getDouble(TP_TSS_MU, true);
	}
	pams->sie = taskConfig->getDouble(TP_TSS_SIE, true);
	pams->sim = taskConfig->getDouble(TP_TSS_SIM, true);
	pams->ds = taskConfig->getDouble(TP_TSS_DS, true);
	pams->nx = taskConfig->getUInt(TP_TSS_NX, true);
	pams->ny = taskConfig->getUInt(TP_TSS_NY, true);
	pams->nz = taskConfig->getUInt(TP_TSS_NZ, true);
	pams->xmin = taskConfig->getDouble(TP_TSS_XMIN, true);
	pams->ymin = taskConfig->getDouble(TP_TSS_YMIN, true);
	pams->zmin = taskConfig->getDouble(TP_TSS_ZMIN, true);
	pams->smax = taskConfig->getUInt(TP_TSS_SMAX, true);
	pams->kmax = taskConfig->getUInt(TP_TSS_KMAX, true);
	pams->maxTimeSteps = taskConfig->getUInt(TP_TSS_MAXTIME, true);
	pams->startTimeStep = taskConfig->getUInt(TP_TSS_STARTTIME, true);
	pams->saveInterval = taskConfig->getUInt(TP_TSS_RECINV, true);
	pams->pml.Ln = taskConfig->getUInt(TP_PML_THICK, true);
	pams->pml.alphaMax = taskConfig->getDouble(TP_PML_ALPHA, true);
	pams->pml.betaMax = taskConfig->getDouble(TP_PML_BETA, true);
	pams->pml.Pxl = taskConfig->getBoolean(TP_PML_X_LOW, true);
	pams->pml.Pxh = taskConfig->getBoolean(TP_PML_X_HIGH, true);
	pams->pml.Pyl = taskConfig->getBoolean(TP_PML_Y_LOW, true);
	pams->pml.Pyh = taskConfig->getBoolean(TP_PML_Y_HIGH, true);
	pams->pml.Pzl = taskConfig->getBoolean(TP_PML_Z_LOW, true);
	pams->pml.Pzh = taskConfig->getBoolean(TP_PML_Z_HIGH, true);
	pams->pml.power = taskConfig->getDouble(TP_PML_POWER, true);
	pams->pml.disable = taskConfig->getBoolean(TP_PML_DISABLE, true);
}

/*
	read simulation configurations from task file
	configuration validation not performed
*/
void Simulator::GetSimStruct(TaskFile *taskConfig, TssSimStruct *tss)
{
	Simulator::ResetSimStruct(tss);
	Simulator::GetSimulationParameters(taskConfig, &(tss->pams));
	tss->matrixFileFolder = taskConfig->getString(TP_TSS_MATRIXFILEDIR, true);
	tss->saveToFilename = taskConfig->getString(TP_TSS_RECFILE, true);
	tss->dataFileFolder = taskConfig->getString(TP_TSS_DATAFOLDER, true);
	tss->timeCoefficientsFile = taskConfig->getString(TP_TSS_TIMEFILE, true);
	tss->curlCoefficientsFile = taskConfig->getString(TP_TSS_SPACEFILE, true);
	tss->generateStatisticFile = taskConfig->getBoolean(TP_TSS_STATISTICS, true);
}

int Simulator::FormSpaceMatrixFilename(char *spaceMatrixFile, const char *dataFolder, unsigned int smax, char *matrixFileFolder)
{
	int ret = ERR_OK;
	char matrixName[FILENAME_MAX];
	ret = sprintf_1(matrixName, FILENAME_MAX, "spacematrix_smax%u.dat", smax) <=0?ERR_STR_PRF:ERR_OK;
	if (ret == ERR_OK)
	{
		if (matrixFileFolder != NULL)
		{
			char fs[FILENAME_MAX];
			ret = formFilePath(fs, FILENAME_MAX, matrixFileFolder, matrixName);
			if (ret == ERR_OK)
			{
				ret = formFilePath(spaceMatrixFile, FILENAME_MAX, dataFolder, fs);
			}
		}
		else
		{
			ret = formFilePath(spaceMatrixFile, FILENAME_MAX, dataFolder, matrixName);
		}
	}
	return ret;
}
/*
	save SimStruct to a file
*/
int Simulator::saveSimulateParametersToFile()
{
	int ret = ERR_OK;
	if (simObj.saveToFilename != NULL)
	{
		char filename[FILENAME_MAX];
		ret = sprintf_1(filename, FILENAME_MAX, "%s_TSS_smax%u_kmax%u_parameters.dat", simObj.saveToFilename, simObj.pams.smax, simObj.pams.kmax) <=0?ERR_STR_PRF:ERR_OK;
		
		if (ret == ERR_OK)
		{
			FILE *fh = 0;
			ret = openfileWrite(filename, &fh);
			if (ret == ERR_OK)
			{
				ret = writefile(fh, &(simObj.pams), (unsigned int)sizeof(SimStruct));
				closefile(fh);
			}
		}
	}
	return ret;
}

int Simulator::formStatisticsFilename(char *filename)
{
	int ret = ERR_OK;
	char df[FILENAME_MAX];
	ret = formFilePath(df, FILENAME_MAX, simObj.mainDataFolder, simObj.dataFileFolder);
	if (ret == ERR_OK)
	{
		char fn[FILENAME_MAX];
		ret = formFilePath(fn, FILENAME_MAX, df, simObj.saveToFilename);
		if (ret == ERR_OK)
		{
			ret = sprintf_1(filename, FILENAME_MAX, "%s_TSS_smax%u_kmax%u_statistic%u.txt", fn, simObj.pams.smax, simObj.pams.kmax, simObj.pams.startTimeStep) <=0?ERR_STR_PRF:ERR_OK;
		}
	}
	return ret;
}

int Simulator::formDataFilename(char *filename, const char *eh, const char *ext, unsigned int fileIndex)
{
	int ret = ERR_OK;
	char df[FILENAME_MAX];
	if (simObj.saveToFilename == NULL) return ERR_FILENAME_MISSING;
	ret = formFilePath(df, FILENAME_MAX, simObj.mainDataFolder, simObj.dataFileFolder);
	if (ret == ERR_OK)
	{
		char fn[FILENAME_MAX];
		ret = formFilePath(fn, FILENAME_MAX, df, simObj.saveToFilename);
		if (ret == ERR_OK)
		{
			ret = sprintf_1(filename, FILENAME_MAX, "%s_TSS_smax%u_kmax%u_%s_%u.%s", fn, simObj.pams.smax, simObj.pams.kmax, eh, fileIndex, ext) <=0?ERR_STR_PRF:ERR_OK;
		}
	}
	return ret;
}

/*
	for testing
*/
int Simulator::GetTime0FilenameE(char *filename)
{
	int ret = ERR_OK;
	ret = formDataFilename(filename, "e", "dat", 0);
	return ret;
}

/*
	for testing
*/
int Simulator::GetTime0FilenameH(char *filename)
{
	int ret = ERR_OK;
	ret = formDataFilename(filename, "h", "dat", 0);
	return ret;
}

int Simulator::saveFieldsToFiles()
{
	int ret = ERR_OK;
	if (simObj.saveToFilename != NULL)
	{
		char filename[FILENAME_MAX];
		ret = formDataFilename(filename, "e", "dat", datafileindex);
		if (ret == ERR_OK)
		{
			ret = saveFieldToFile(timeAdv->GetFieldE(), filename);
		}
		if (ret == ERR_OK)
		{
			ret = formDataFilename(filename, "h", "dat", datafileindex);
			if (ret == ERR_OK)
			{
				ret = saveFieldToFile(timeAdv->GetFieldH(), filename);
			}
		}
	}
	datafileindex++;
	return ret;
}

/*
	for startTimeStep > 0, load fields from data files
*/
int Simulator::LoadStartingFields(Point3Dstruct *efile, Point3Dstruct *hfile)
{
	int ret = ERR_OK;
	//calculate file index
	int m = simObj.pams.startTimeStep % simObj.pams.saveInterval;
	if (m != 0)
	{
		return ERR_START_INTERVAL;
	}
	unsigned int fileIndex = simObj.pams.startTimeStep / simObj.pams.saveInterval;
	char filename[FILENAME_MAX];
	ret = formDataFilename(filename, "e", "dat", fileIndex);
	if (ret == ERR_OK)
	{
		ret = loadFieldFromFile(timeAdv->GetFieldE(), filename);
	}
	if (ret == ERR_OK)
	{
		ret = formDataFilename(filename, "h", "dat", fileIndex);
		if (ret == ERR_OK)
		{
			ret = loadFieldFromFile(timeAdv->GetFieldH(), filename);
		}
	}
	datafileindex = fileIndex + 1; //next file index
	return ret;
}

int Simulator::Initialized()
{
	if (simObj.src != NULL)
	{
		if (!simObj.src->Initialized())
		{
			return ERR_SRC_NOT_INIT;
		}
	}
	if (simObj.boundaryCondition != NULL)
	{
		if (!simObj.boundaryCondition->Initialized())
		{
			return ERR_BC_NOT_INIT;
		}
	}
	return ERR_OK;
}

/*
	run simulation
*/
int Simulator::run()
{
	int ret = Initialized();
	if (ret != ERR_OK) return ret;
	//ret = saveSimulateParametersToFile();
	//if (ret != ERR_OK) return ret;
	//
	size_t startTimeTotal, endTimeTotal;
	size_t startTime, endTime;
	if (!setProcessHighPtiority())
	{
		simObj.reporter("Cannot set process priority", false);
	}
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	//
	printf("Field source:%s, boundary conditions:%s\r\n", simObj.src == NULL ? "null" : simObj.src->getClassName(), simObj.boundaryCondition == NULL ? "null" : simObj.boundaryCondition->getClassName());
	//
	simObj.reporter("Preparing to start simulation, applying source and boundary...", false);
	startTime = getTimeTick();
	//
	saveFileIndexCount = 0;
	if (simObj.pams.startTimeStep == 0)
	{
		timeAdv->resetTime();
		if (ret == ERR_OK)
		{
			//apply source first
			if (simObj.src != NULL)
			{
				ret = simObj.src->applySources(timeAdv->GetTimeValue(), timeAdv->GetTimeIndex(), timeAdv->GetFieldE(), timeAdv->GetFieldH());
			}
			//apply boundary conditions next
			if (ret == ERR_OK)
			{
				if (simObj.boundaryCondition != NULL)
				{
					ret = simObj.boundaryCondition->apply(timeAdv->GetFieldE(), timeAdv->GetFieldH());
				}
			}
			if (ret == ERR_OK)
			{
				//data files at time 0, with source and boundary conditions applied
				ret = saveFieldsToFiles();
			}
		}
	}
	else
	{
		//load fields from files
		ret = LoadStartingFields(timeAdv->GetFieldE(), timeAdv->GetFieldH());
		if (ret == ERR_OK)
		{
			timeAdv->setStartTime(simObj.pams.startTimeStep);
		}
	}
	if (ret == ERR_OK)
	{
		timeAdv->setReporter(simObj.reporter, simObj.canceled);
		endTime = getTimeTick();
#ifdef __linux__
		printf("Prepared fields. Time used:%Ilu\r\n", endTime - startTime);
#elif _WIN32
		printf("Prepared fields. Time used:%Iu\r\n", endTime - startTime);
#else
#endif
		//reportProcess(simObj.reporter, false, "Prepared fields. Time used:%u", endTime - startTime);
		simObj.reporter("Start TSS simulation. ", false);
		timeAdv->startThreads();
		startTimeTotal = getTimeTick();
		while (timeAdv->GetTimeIndex() < simObj.pams.maxTimeSteps)
		{
			startTime = getTimeTick();
			//reportProcess(simObj->reporter, false, "Time index %d ...", timeAdv->GetTimeIndex());
			//do FDTD one step
			ret = timeAdv->AdvanceOneStep();
			if (ret != ERR_OK)
			{
				break;
			}
			timeAdv->advanceTimeStepValue();
			endTime = getTimeTick();
#ifdef TIMESTEPPROFILING
			//reportProcess(simObj->reporter, true, "Time index %d finished. Time used: %d                  ", timeAdv->GetTimeIndex(), endTime - startTime);
			printf("%d:%d, c1:%d(%d,%d),a1:%d(%d,%d),c2:%d(%d,%d),a2:%d(%d,%d),c3:%d(%d,%d),a3:%d(%d,%d)\r\n", timeAdv->GetTimeIndex(), endTime - startTime
				,timeAdv->timeUsedCal1
				, timeAdv->timeUsedCal1E
				, timeAdv->timeUsedCal1H
				,timeAdv->timeUsedAppCurl01
				, timeAdv->timeUsedAppCurl01E
				, timeAdv->timeUsedAppCurl01H
				,timeAdv->timeUsedCal2
				, timeAdv->timeUsedCal2E
				, timeAdv->timeUsedCal2H
				,timeAdv->timeUsedAppCurl2
				, timeAdv->timeUsedAppCurl2E
				, timeAdv->timeUsedAppCurl2H
				,timeAdv->timeUsedCal3
				, timeAdv->timeUsedCal3E
				, timeAdv->timeUsedCal3H
				,timeAdv->timeUsedAppCurl3
				, timeAdv->timeUsedAppCurl3E
				, timeAdv->timeUsedAppCurl3H
				);
#else
#ifdef __linux__ 
			printf("%u:%Ilu ",timeAdv->GetTimeIndex(), endTime - startTime);
#elif _WIN32
			printf("%Iu:%Iu ", timeAdv->GetTimeIndex(), endTime - startTime);
#endif
#endif
			//apply source first
			if (simObj.src != NULL)
			{
				ret = simObj.src->applySources(timeAdv->GetTimeValue(), timeAdv->GetTimeIndex(), timeAdv->GetFieldE(), timeAdv->GetFieldH());
			}
			//apply boundary conditions next
			if (ret == ERR_OK)
			{
				if (simObj.boundaryCondition != NULL)
				{
					ret = simObj.boundaryCondition->apply(timeAdv->GetFieldE(), timeAdv->GetFieldH());
				}
			}
			if (ret != ERR_OK)
			{
				break;
			}
			saveFileIndexCount++;
			if (saveFileIndexCount >= simObj.pams.saveInterval)
			{
				saveFileIndexCount = 0;
				ret = saveFieldsToFiles();
				if (ret != ERR_OK)
				{
					break;
				}
			}
			if (timeAdv->Canceled())
			{
				break;
			}
		}
		endTimeTotal = getTimeTick();
		timeAdv->removeThreads();
		//finished simulation
		if (simObj.generateStatisticFile)
		{
			puts("\r\nGenerating statistics file... ");
			ret = generateStatisticFile();
			if (ret == ERR_OK)
			{
				puts("\r\nGenerating statistics file...Done ");
			}
			else
			{
				reportProcess(simObj.reporter, false, "Generating statistics file...failed, err code %d ", ret);
			}
		}
		//show total time
		
#ifdef __linux__ 
		printf("\r\nTotal simulation time: %Ilu\r\n", endTimeTotal - startTimeTotal);
#elif _WIN32
		printf("\r\nTotal simulation time: %Iu\r\n", endTimeTotal - startTimeTotal);
#endif

	}
	return ret;
}

/*
	field statistics on energy transfer and divergences
*/
int Simulator::generateStatisticFile()
{
	int ret = ERR_OK;
	if (simObj.saveToFilename != NULL)
	{
		char filename[FILENAME_MAX];
		ret = formStatisticsFilename(filename);
		if (ret == ERR_OK)
		{
			FILE *fhStatistic = 0;
			ret = openTextfileWrite(filename, &fhStatistic);
			if (ret == ERR_OK)
			{
				char msg[MESSAGELINESIZE];
				Point3Dstruct *efile = NULL, *hfile = NULL;
				size_t esize = 0, hsize = 0;
				MemoryManager *_mem = simObj.mem;
				//
				sprintf_1(msg, MESSAGELINESIZE, "Time\tEnergy\tMax Divg E\tAvg Divg E\tMax Divg H\tAvg Divg H");
				ret = writefile(fhStatistic, msg, (unsigned int)strnlen_0(msg, MESSAGELINESIZE));
				if (ret == ERR_OK)
				{
					//loop through all data files
					int fileIndex = 0;
					if (simObj.pams.startTimeStep > 0)
					{
						if (simObj.pams.saveInterval > 1)
							fileIndex = simObj.pams.startTimeStep / simObj.pams.saveInterval;
						else
							fileIndex = simObj.pams.startTimeStep;
					}
					while (ret == ERR_OK)
					{
						ret = formDataFilename(filename, "e","dat", fileIndex);
						if (fileexists(filename))
						{
							//open E data file
							if (ret == ERR_OK)
							{
								efile = (Point3Dstruct *)ReadFileIntoMemory(filename, &esize, &ret);
								if (ret == ERR_OK)
								{
									if (esize != space.GetFieldMemorySize())
									{
										ret = ERR_INVALID_SIZE;
									}
									else
									{
										if (timeAdv->needModifyFieldsForStatistics())
										{
											Point3Dstruct *dest = (Point3Dstruct *)AllocateMemory(esize);
											if (dest == NULL)
											{
												ret = MEMRETLAST;
												if (ret == ERR_OK)
												{
													ret = ERR_OUTOFMEMORY;
												}
											}
											else
											{
												timeAdv->modifyFieldForStatistics_E(efile, dest);
												FreeMemory(efile);
												efile = dest;
											}
										}
									}
								}
							}
						}
						else
						{
							break;
						}
						if (ret == ERR_OK)
						{
							ret = formDataFilename(filename, "h","dat", fileIndex);
							if (fileexists(filename))
							{
								//open H data file
								if (ret == ERR_OK)
								{
									hfile = (Point3Dstruct *)ReadFileIntoMemory(filename, &hsize, &ret);
									if (ret == ERR_OK)
									{
										if (hsize != space.GetFieldMemorySize())
										{
											ret = ERR_INVALID_SIZE;
										}
										else
										{
											if (timeAdv->needModifyFieldsForStatistics())
											{
												Point3Dstruct *dest = (Point3Dstruct *)AllocateMemory(hsize);
												if (dest == NULL)
												{
													ret = MEMRETLAST;
													if (ret == ERR_OK)
													{
														ret = ERR_OUTOFMEMORY;
													}
												}
												else
												{
													timeAdv->modifyFieldForStatistics_H(hfile, dest);
													FreeMemory(hfile);
													hfile = dest;
												}
											}
										}
									}
								}
							}
							else
							{
								break;
							}
						}
						if (ret == ERR_OK)
						{
							if (efile != NULL && hfile != NULL)
							{
								ret = calculateStatistics(efile, hfile, fileIndex, fhStatistic);
							}
						}
						//
						if (efile != NULL) FreeMemory(efile);
						if (hfile != NULL) FreeMemory(hfile);
						efile = hfile = NULL;
						//
						printf(" %d", fileIndex);
						//
						fileIndex++;
					}
					if (efile != NULL) FreeMemory(efile);
					if (hfile != NULL) FreeMemory(hfile);
				}
				closefile(fhStatistic);
			}
		}
	}
	return ret;
}

int Simulator::calculateStatistics(Point3Dstruct *efile, Point3Dstruct *hfile, int fileIndex, FILE *fhStatistic)
{
	int ret = ERR_OK;
	char msg[MESSAGELINESIZE];
	FieldStatisticStruct fs;
	size_t pointCount = 0;	
	//
	space.SetFields(hfile, efile);
	//
	/*
	Field energy = (eps * ||E||^2 + mu * ||H||^2) / 2
	*/
	fs.averageDivergenceE = fs.averageDivergenceH = 0.0;
	fs.energySum = 0.0;
	fs.maxDivergenceE = fs.maxDivergenceH = 0.0;
	fs.magSumE = fs.magSumH = 0.0;
	if (simObj.pams.saveInterval <= 1)
		fs.timeIndex = fileIndex;
	else
		fs.timeIndex = simObj.pams.saveInterval * fileIndex;
	//
	pointCount = simObj.dvgs->calculate(efile, hfile, &fs);
	//
	fs.averageDivergenceE = fs.averageDivergenceE / (double)pointCount;
	fs.averageDivergenceH = fs.averageDivergenceH / (double)pointCount;
	//
	sprintf_1(msg, MESSAGELINESIZE, "\n%u\t%g\t%g\t%g\t%g\t%g",
		fs.timeIndex, fs.energySum,
		fs.maxDivergenceE, fs.averageDivergenceE,
		fs.maxDivergenceH, fs.averageDivergenceH);
	ret = writefile(fhStatistic, msg, (unsigned int)strnlen_0(msg, MESSAGELINESIZE));
	return ret;
}

//
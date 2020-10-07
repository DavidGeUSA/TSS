/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include "..\FileUtil\fileutil.h"
#include "..\EMField\EMField.h"
#include "..\EMField\RadiusIndex.h"
#include "..\TssInSphere\TssInSphere.h"
#include "..\MemoryMan\memman.h"
#include "..\ProcessMonitor\ProcessMonitor.h"
#include "FieldSimulation.h"
#include "simConsole.h"
#include "..\TssInSphere\FieldAnalysor.h"

#include <limits.h>
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  
#include <stdio.h>
#include <float.h>
#include <signal.h>
#include <string.h>

//let user use Ctrl-C to stop simulation
bool cancel_simulation_flag;
extern "C" void signals_handler(int)
{
	puts("\r\n\r\nStopping simulation, please wait ... \r\n");
	cancel_simulation_flag = true;
}

/*
	construct a simulation object
*/
FieldSimulation::FieldSimulation()
{
	fdtd = NULL;
	field0 = NULL;
	source = NULL;
	boundaryCondition = NULL;
	tfsf = NULL;
	seriesIndex = NULL;
}

FieldSimulation::~FieldSimulation()
{
	if(seriesIndex != NULL)
	{
		delete seriesIndex;
	}
}
wchar_t *FieldSimulation::GetBaseFilename()
{
	return fieldFile;
}
bool FieldSimulation::EnabledFDTDtimeRecording()
{
	if(fdtd != NULL)
	{
		return fdtd->EnabledFDTDtimeRecording();
	}
	return false;
}
double FieldSimulation::GetAverageFDTDOneStepTime()
{
	if(fdtd != NULL)
	{
		return fdtd->GetAverageFDTDOneStepTime();
	}
	return 0.0;
}
double FieldSimulation::GetSumFDTDOneStepTime()
{
	if(fdtd != NULL)
	{
		return fdtd->GetSumFDTDOneStepTime();
	}
	return 0.0;
}
/*
	run EM simulation and record each time step to a data file
*/
int FieldSimulation::simulationToFiles(TaskFile *taskConfig, const char *dataFolder)
{
	int ret = ERR_OK;
	//time statistics---------------
	size_t timeStepCount;
	timeUsed = 0;
	totalTime = 0;
	averageStepTime = 0.0;
	timeStepCount = 0;
	//-------------------------------
	if(fdtd == NULL)
	{
		ret = ERR_SIM_FDTD;
	}
	else if(field0 == NULL)
	{
		ret = ERR_SIM_FIELD0;
	}
	else if(boundaryCondition == NULL)
	{
		ret = ERR_SIM_BOUNDARY;
	}
	if(ret == ERR_OK)
	{
		N = taskConfig->getInt(TP_FDTDN, false);
		range = taskConfig->getDouble(TP_FDTDR, false);
		ret = taskConfig->getErrorCode();
		if(ret == ERR_OK)
		{
			if(N <= 0)
			{
				ret = ERR_TP_INVALID_N;
			}
			else if(range <= 0.0)
			{
				ret = ERR_TP_INVALID_R;
			}
		}
		if(ret == ERR_OK)
		{
			maxRadius = GRIDRADIUS(N);
			minimumStepTime = UINT_MAX;
			averageStepTime = 0.0;
			timeStepCount = 0;
			datapoints = totalPointsInSphere(maxRadius);
			//
			puts("\r\nStarting FDTD simulation. Press Ctrl-C to stop. \r\n Initialize and prepare fields at time 0 ...\r\n");
			startTime = GetTimeTick();
			//
			cancel_simulation_flag = false;
			signal(SIGINT, &signals_handler);
			//
			//create a index converter for quick accessing fields
			seriesIndex = new RadiusIndexToSeriesIndex();
			ret = seriesIndex->initialize(maxRadius);
			//
			if(ret == ERR_OK)
			{
				boundaryCondition->setIndexCache(seriesIndex);
				fdtd->setIndexCache(seriesIndex);
				if(tfsf != NULL)
				{
					tfsf->setIndexCache(seriesIndex);
				}
				//initialize plug-in objects
				ret = field0->initialize(taskConfig);
			}
			if(ret == ERR_OK)
			{
				if(ret == ERR_OK)
				{
					ret = fdtd->initialize(dataFolder, tfsf, taskConfig);
					if(ret == ERR_OK)
					{
						if(source != NULL)
						{
							ret = source->initialize(fdtd->getCourantNumber(), maxRadius, taskConfig);
						}
					}
				}
			}
			if(ret == ERR_OK)
			{
				timeAdvanceOrder = 2 * fdtd->getHalfOrderTimeAdvance();
				spaceDerivativeOrder = 2 * fdtd->getHalfOrderSpaceDerivate();
				memorySize = fdtd->GetMemorySize();
				spaceStepSize = fdtd->GetSpaceStepSize();
				timeStepSize = fdtd->GetTimeStepSize();
				//set initial field values
				ret = fdtd->PopulateFields(field0);
				if(ret == ERR_OK)
				{
					ret = boundaryCondition->initialize(fdtd->getCourantNumber(), maxRadius, taskConfig);
					if(ret == ERR_OK)
					{
						if(tfsf != NULL)
						{
							ret = tfsf->initialize(fdtd->getCourantNumber(), maxRadius, taskConfig);
						}
					}
				}
				else
				{
					RememberOSerror();
				}
			}
		}
	}
	if(ret == ERR_OK)
	{
		FieldAnalysor *fa = NULL;
		int fhSummary = 0;
		double avgE = 0.0;
		double avgH = 0.0;
		//
		endTime = GetTimeTick(); timeUsed = endTime - startTime;
		printf("\r\nInitilization finished in %d ms. Start simulation.\r\n", timeUsed);
		printf("Space step = %g, time step = %g. Time advance estimation order: %d; curl estimation order:%d.\r\n Number of field points:%d. \r\n",
					spaceStepSize, timeStepSize, timeAdvanceOrder, spaceDerivativeOrder, datapoints);
		//
		if(EnabledFDTDtimeRecording())
		{
			fa = new FieldAnalysor(seriesIndex);
		}
		//time advance loop
		while(ret == ERR_OK)
		{
			reportProcess(reporter, false, "Current time index %d. Moving forward, please wait ...", fdtd->GetTimeStepIndex());
			startTime = GetTimeTick();
			//move forward
			ret = fdtd->moveForward();
			if(ret == ERR_OK)
			{
				if(source != NULL)
				{
					//apply field source
					source->reset(fdtd->GetFieldMemory(), fdtd->GetTimeStepIndex(), fdtd->getTime());
					ret = source->gothroughSphere(maxRadius);
				}
				if(ret == ERR_OK)
				{
					//apply boundary condition
					boundaryCondition->setFields(fdtd->GetFieldMemory());
					ret = boundaryCondition->gothroughSphere(maxRadius);
				}
			}
			if(ret == ERR_OK)
			{
				endTime = GetTimeTick(); timeUsed = endTime - startTime;
				totalTime += timeUsed;
				if(timeUsed < minimumStepTime) minimumStepTime = timeUsed;
				timeStepCount++;
				averageStepTime = (((double)timeStepCount-1.0)/(double)timeStepCount) * averageStepTime + (double)timeUsed / (double)timeStepCount;
				reportProcess(reporter, true, "Reached time index: %d, time for this step: %d ms. average time:%g", fdtd->GetTimeStepIndex(), timeUsed, averageStepTime);
				if(fa != NULL)
				{
					int radiusToCount = maxRadius - (int)fdtd->GetTimeStepIndex() - 2; //the maximum radius to calculate the divergence, excluding the boudary
					if (radiusToCount <= 0)
					{
						radiusToCount = maxRadius;//the boudary error has reached the center, no more excluding the boundary
					}
					ret = fa->setFields(fdtd->GetFieldMemory(), radiusToCount, fdtd->GetSpaceStepSize(), fdtd->getHalfOrderSpaceDerivate());
					if(ret == ERR_OK)
					{
						ret = fa->execute();
						if(ret == ERR_OK)
						{
							avgE += fa->getAverageDivergenceE();
							avgH += fa->getAverageDivergenceH();
						}
					}
				}
				if(ret == ERR_OK)
				{
					if(cancel_simulation_flag) //user terminates simulation
					{
						ret = ERR_SIMULATION_CANCEL;
					}
					else
					{
						if(fdtd->ReachedMaximumTime())
						{
							fdtd->FinishSimulation();
							ret = ERR_REACHED_TIME_LIMIT;
						}
					}
				}
			}
		}
		if (ret == ERR_OK)
		{
			if (EnabledFDTDtimeRecording())
			{
				//create a summary and append it to a file.
				char sumFile[FILENAME_MAX];
				ret = fdtd->GetTemporaryFolder(sumFile, FILENAME_MAX);
				if (ret == ERR_OK)
				{
					ret = formFilePath(sumFile, FILENAME_MAX, sumFile, "EMsummary");
					if (ret == ERR_OK)
					{
						int err = sprintf_s(sumFile, FILENAME_MAX, "%s%d.txt", sumFile, fdtd->getMaximumTimeIndex());
						if (err <= 0)
						{
							ret = ERR_MEM_EINVAL;
						}
					}
				}
				if (ret == ERR_OK)
				{
					ret = openTextfileAppend(sumFile, &fhSummary);
					if (fhSummary != 0)
					{
						//append report
						//{average divergence magnitude},{time used},{FDTD class name}, order ({time order}, {space order}), N={N}
						int err;
						char msg[500];
						avgE = avgE / (double)fdtd->GetTimeStepIndex();
						avgH = avgH / (double)fdtd->GetTimeStepIndex();
						avgE = sqrt(avgE*avgE + avgH*avgH);
						err = sprintf_s(msg, 500, "%g,%g,%s(%d,%d)N%d\r\n", avgE, fdtd->GetSumFDTDOneStepTime(), fdtd->getClassName(), timeAdvanceOrder, spaceDerivativeOrder, N);
						if (err <= 0)
						{
							ret = ERR_MEM_EINVAL;
						}
						else
						{
							ret = writefile(fhSummary, msg, (unsigned int)strlen(msg));
						}
						closefile(fhSummary);
						printf("\r\nAverage FDTD step time:%g, total FDTD time:%g\r\n", GetAverageFDTDOneStepTime(), GetSumFDTDOneStepTime());
					}
				}
			}
		}
		if(fa != NULL)
		{
			free(fa);
		}
		printf("\r\nTotal time used:%d\r\n", totalTime);
	}
	return ret;
}

/*
	create a report file for each data file.

	filenamebase: data file names will be formed by {filenamebase}{n}.em, where {n} = 0,1,2,...
	halfOrderDivergenceEstimate: half of the divergence estimation
*/
int FieldSimulation::createStatisticsFiles(TaskFile *taskConfig, const char *dataFolder)
{
	int ret = ERR_OK;
	int k;
	int err;
	char filenamebase[FILENAME_MAX];
	int halfOrder;
	wchar_t fieldFileBase[FILENAME_MAX];
	wchar_t fieldFile[FILENAME_MAX];
	char reportFile[FILENAME_MAX];
	FieldPoint3D *fields = NULL;
	size_t fsize;
	unsigned int simulationRadius;
	double ds;
	int divergReportFileHandle = 0;
	char *basefile = taskConfig->getString(TP_SIMBASENAME, false);
	range = taskConfig->getDouble(TP_FDTDR, false);
	halfOrder = taskConfig->getInt(TP_HALF_ORDER_SPACE, true);
	ret = taskConfig->getErrorCode();
	if(ret == ERR_OK)
	{
		if(range <= 0.0)
		{
			ret = ERR_TP_INVALID_R;
		}
		else if(strlen(basefile)==0)
		{
			ret = ERR_TP_BASENAME;
		}
		else if(strcmp(basefile,"DEF")==0)
		{
			ret = ERR_TP_BASENAME_DEF;
		}
		else if(strlen(dataFolder) == 0)
		{
			ret = ERR_CMD_DATAFOLDER;
		}
		else
		{
			ret = formFilePath(filenamebase, FILENAME_MAX, dataFolder, basefile);
			if(ret == ERR_OK)
			{
				ret = copyC2W(fieldFileBase, FILENAME_MAX, filenamebase);
			}
		}
	}
	if(ret == ERR_OK)
	{
		//create a binary report file handle
		char rptFile[FILENAME_MAX];
		err = sprintf_s(rptFile, FILENAME_MAX, "%sradiusError.dat", filenamebase);
		if(err <= 0)
		{
			ret = ERR_MEM_EINVAL;
		}
		else
		{
			ret = openfileWrite(rptFile,&divergReportFileHandle);
		}
	}
	if(ret == ERR_OK)
	{
		FieldAnalysor fa(NULL);
		if(halfOrder < 1) halfOrder = 1;
		k = 0;
		puts("Creating report files. File number: ");
		while(ret == ERR_OK)
		{
			//load fields from file
			ret = formDataFileNameW(fieldFile, FILENAME_MAX, fieldFileBase, k);
			if(ret != ERR_OK)
			{
				ret = ERR_MEM_EINVAL;
				break;
			}
			if(FileExists(fieldFile))
			{
				err = sprintf_s(reportFile, FILENAME_MAX, "%s%d.em.txt", filenamebase, k);
				if(err == -1)
				{
					ret = ERR_MEM_EINVAL;
					break;
				}
				ret = CreateReportFile(reportFile); //create a text file handle for the report file
				if(ret != ERR_OK)
				{
					break;
				}
			}
			else
			{
				break;
			}
			printf("%d ",k);
			//calculate simulation region
			fields = (FieldPoint3D *) ReadFileIntoMemory(fieldFile, &fsize, &ret);
			if( ret == ERR_OK)
			{
				if(fields == NULL)
				{
					ret = ERR_OUTOFMEMORY;
				}
				else
				{
					ret = MemorySizeToRadius(fsize, &simulationRadius);
					if(ret == ERR_OK)
					{
						if(k == 0) //the first file, create indexing cache
						{
							if( simulationRadius > 1 && ((simulationRadius-1) % 2) == 0)
							{
								ret = writefile(divergReportFileHandle, &simulationRadius, sizeof(simulationRadius));
								if(ret == ERR_OK)
								{
									maxRadius = simulationRadius;
									N = (maxRadius - 1) / 2;
									ds = SPACESTEP(range, N);
									//create indexing cache
									seriesIndex = new RadiusIndexToSeriesIndex();
									seriesIndex->initialize(maxRadius);
									fa.setIndexCache(seriesIndex);
								}
							}
							else
							{
								ret = ERR_INVALID_SIZE;
							}
						}
						else if(simulationRadius != maxRadius)
						{
							ret = ERR_FILESIZE_MISMATCH;
						}
						if(ret == ERR_OK)
						{
							ret = fa.setFields(fields, simulationRadius, ds, halfOrder);
						}
						if(ret == ERR_OK)
						{
							ret = fa.execute();
							if(ret == ERR_OK)
							{
								ret = fa.WriteDivegenceToFile(divergReportFileHandle);
								if(ret == ERR_OK)
								{
									fa.ShowReport(writeReport); //write to report file
								}
							}
						}
						fa.cleanup();
					}
					FreeMemory(fields);
				}
			}
			CloseReportFile();
			k++;
		}
		closefile(divergReportFileHandle);
	}
	return ret;
}

//



/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "simConsole.h"
#include "tasks.h"
#include "../FileUtil/fileutil.h"
#include "../EMField/RadiusIndex.h"
#include "../ProcessMonitor/ProcessMonitor.h"
#include "../MemoryMan/memman.h"
#include "../MemoryMan/MemoryManager.h"
#include "../TssInSphere/TssInSphere.h"
#include "taskClasses.h"
#include "FieldSimulation.h"
#include "../FieldDataComparer/FieldDivergenceComparer.h"
#include "../FieldDataComparer/FieldDataComparer.h"
#include "../Tss/Simulator.h"
#include "../TssTest/PluginInstances.h"

#include "../TssTest/TssTest.h"

#include <malloc.h>
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  

#include <stdio.h>
#include <string.h>

/*
	verify that IndexToIndexes and IndexesToIndex work as index conversion and inverse conversion
	for each series index, IndexToIndexes convert it to m,n,p, 
	it needs to verify that m,n,p is a unique combination for each series index
	it uses IndexesToIndex to convert m,n,p back to a series index and compare it with the original one

	note that IndexToIndexes and IndexesToIndex work by radius. 
	the series index is from 0 to the maximum number of points at the raius minus 1
*/
int task1_verifyRadiusIndexConversions(int N)
{
	int ret = ERR_OK;
	size_t idx = 0;
	size_t indexCount = 0;
	size_t startIndex = 0;
	int displayCount = 0;
	unsigned maxN;
	size_t maxN2;
	bool *used = NULL;
	int maxRadius = GRIDRADIUS(N);
	size_t pointsInSphere = 0;
	RadiusIndexToSeriesIndex idxCache;
	if(N < 0)
	{
		return ERR_TP_INVALID_N;
	}
	puts("\r\nLoading index cache...");
	ret = idxCache.initialize(maxRadius);
	if(ret != ERR_OK)
	{
		return ret;
	}
	maxN = GRIDPOINTS(N); //4N+3
	maxN2 = maxN * maxN;
	indexCount = totalPointsInSphere(maxRadius);
	used = (bool *)AllocateMemory(indexCount * sizeof(bool));
	if(used == NULL)
	{
		return ERR_OUTOFMEMORY;
	}
	for(idx = 0; idx < indexCount; idx++)
	{
		used[idx]=false;
	}
	for(int r = 0; r <= maxRadius; r++)
	{
		size_t ai;
		int m,n,p;
		size_t a;
		//check counts
		size_t pointsTotal = totalPointsInSphere(r);
		size_t pointsAtRadius = pointsAt(r);
		for(idx = 0; idx < indexCount; idx++)
		{
			used[idx]=false;
		}
		pointsInSphere += pointsAtRadius;
		if(pointsInSphere != pointsTotal)
		{
			ret = ERR_SPHERE_COUNT_MISMATCH;
#ifdef __linux__ 
			printf("\r\n  sphere points calculation error for r=%d, calculated:%Ilu, sum=%Ilu", r,pointsTotal,pointsInSphere);
#elif _WIN32
			printf("\r\n  sphere points calculation error for r=%d, calculated:%Iu, sum=%Iu", r,pointsTotal,pointsInSphere);
#else
#endif
			break;
		}
		//check conversion-invert-conversion
		for(a = 0; a < pointsAtRadius; a++)
		{
			//series indexing to 3D radius indexing, a is unique, (m,n,p) is expected to be unique
			ret = IndexToIndexes(r, a, &m, &n, &p);
			if(ret != ERR_OK)
			{
				break;
			}
			if(m < -r || m > r || n < -r || n > r || p < -r || p > r)
			{
				ret = ERR_RADIUS_3DINDEX_TOO_BIG;
				break;
			}
			if(m < r && m > -r && n < r && n > -r && p < r && p > -r)
			{
				ret = ERR_RADIUS_3DINDEXTOOSMALL;
				break;
			}
			idx = ROWMAJORINDEX(m,n,p);
			if(idx >= indexCount)
			{
				ret = ERR_RADIUS_3DINDEX_TOO_BIG;
				break;
			}
			if(used[idx])
			{
				ret = ERR_RADIUS_INDEX_DUPLICATE;
				break;
			}
			used[idx] = true;
			//check inverse conversion
			ai = IndexesToIndex(r, m, n, p, &ret);
			if(ret != ERR_OK)
			{
				break;
			}
			if(ai != a)
			{
				ret = ERR_RADIUS_INDEX_MISMATCH;
				break;
			}
			//check index cache
			ai = idxCache.Index(m,n,p);
			if(ai != a + startIndex)
			{
				ret = ERR_RADIUS_INDEX_MISMATCH;
				break;
			}
			displayCount++;
			if(displayCount > 1000)
			{
				displayCount = 0;
				reportProcess(showProgressReport, true, "verified %d / %d", a, pointsAtRadius);
			}
		} //for a=0...,pointsAtRadius-1
		
		if(ret != ERR_OK)
		{
#ifdef __linux__ 
			printf("\r\n\r\n FAILED!. radius:%d Series index:%Ilu, m=%d, n=%d, p=%d",r, a, m,n,p);
#elif _WIN32
			printf("\r\n\r\n FAILED!. radius:%d Series index:%Iu, m=%d, n=%d, p=%d",r, a, m,n,p);
#else
#endif
			break;
		}
		else
		{
			startIndex += pointsAtRadius;
			reportProcess(showProgressReport,true, " OK: r=%d, points at radius=%d, points in sphere=%d\r\n ",r,pointsAtRadius,pointsInSphere);
		}
	} //for r=0...rm
	puts("\r\n\r\nFreeing memory...");
	FreeMemory(used);
	return ret;
}

/*
	RadiusIndexToSeriesIndex converts (m,n,p) to a series index 
	via caching series indexes in a 3D array to provide fast access
	unlike IndexToIndexes and IndexesToIndex, RadiusIndexToSeriesIndex does not use radius,
	it uses series index of entire sphere, not at a radius
	this function verify that for each unique combination of m,n,p, RadiusIndexToSeriesIndex gives unique index in correct range
*/
int task2_verifySphereIndexCache(int N)
{
	int ret = ERR_OK;
	int maxR = GRIDRADIUS(N);
	RadiusIndexToSeriesIndex idxCache;
	puts("\r\nLoading index cache...");
	ret = idxCache.initialize(maxR);
	if(ret == ERR_OK)
	{
		size_t count = 0;
		size_t idx;
		size_t monitorCount = 0;
		size_t points = totalPointsInSphere(maxR);
		bool *used = (bool *)AllocateMemory(points * sizeof(bool));
		if(used == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		else
		{
			int mnp;
			int M[8], N[8], P[8];
			for(idx=0;idx<points;idx++)
			{
				used[idx]=false;
			}
			puts("\r\nIndex cache loaded. verifying...\r\n");
			for(int m=0;m<=maxR;m++)
			{
				for(int n=0;n<=maxR;n++)
				{
					for(int p=0;p<=maxR;p++)
					{
						if(m == 0 && n == 0 && p == 0)
						{
							idx = idxCache.Index(m,n,p);
							if(idx != 0)
							{
								ret = ERR_RADIUS_INDEX_MISMATCH;
								break;
							}
							count++;
							monitorCount++;
						}
						else
						{
							mnp = 1;
							M[0] =  m; N[0] =  n; P[0] =  p;
							if(m > 0)
							{
								M[mnp] = -m; N[mnp] =  n; P[mnp] =  p;
								mnp ++;
							}
							if(n > 0)
							{
								M[mnp] = m; N[mnp] = -n; P[mnp] =  p;
								mnp ++;
							}
							if(p > 0)
							{
								M[mnp] = m; N[mnp] = n; P[mnp] = -p;
								mnp ++;
							}
							if(n > 0 && p > 0)
							{
								M[mnp] = m; N[mnp] = -n; P[mnp] = -p;
								mnp ++;
							}
							if(m > 0 && p > 0)
							{
								M[mnp] = -m; N[mnp] = n; P[mnp] = -p;
								mnp ++;
							}
							if(m > 0 && n > 0)
							{
								M[mnp] = -m; N[mnp] = -n; P[mnp] = p;
								mnp ++;
							}
							if(m > 0 && n > 0 && p > 0)
							{
								M[mnp] = -m; N[mnp] = -n; P[mnp] = -p;
								mnp ++;
							}
							//
							for(int i=0;i<mnp;i++)
							{
								idx = idxCache.Index(M[i],N[i],P[i]);
								if(idx >= points)
								{
									ret = ERR_RADIUS_3DINDEX_TOO_BIG;
									break;
								}
								else
								{
									//check uniqueness of the index
									if(used[idx])
									{
										ret = ERR_RADIUS_INDEX_DUPLICATE;
										break;
									}
									else
									{
										used[idx] = true;
									}
								}
								count++;
								monitorCount++;
							}
						}
						if(ret != ERR_OK)
						{
							break;
						}
					}
					if(ret != ERR_OK)
					{
						break;
					}
					if( monitorCount > 1000)
					{
						reportProcess(showProgressReport, true, "Verified: %d / %d", count, points);
						monitorCount = 0;
					}
				}
				if(ret != ERR_OK)
				{
					break;
				}
			}
			if(ret == ERR_OK)
			{
				reportProcess(showProgressReport, true, "Verified: %d / %d", count, points);
			}
			FreeMemory(used);
		}
	}
	return ret;
}

/*
	compare going through sphere indexing against going through 3D array indexing
	results: 
		larger displayInterval -> less time spent on processing data, 3D array indexing performs much better
		smaller displayInterval -> more time spent on processing data, 3D array indexing performs a little better 
	conclusion: 3D array indexing is much faster. since the iteration number is huge, the major fact is the time each iteration needs to process data.
	  if data processing takes time then the gain of 3D array indexing is not much.
	  if data processing takes no time then the total time is small, the gain of 3D array is also not important, i.e. 0.3 seconds vs 0.6 seconds
*/
int task3_sphereIndexSpeedTest(int N)
{
	int ret = ERR_OK;
	int displayInterval = 100000;
	unsigned long startTick, endTick, tickCountSphere, tickCount3Darray; //one tick is one milliseconds 
	SphereIndexSpeedTest sphereTest;
	int maxRadius = GRIDRADIUS(N);
	size_t totalPoints = totalPointsInSphere(maxRadius);
	puts("\r\ncompare speeds\r\n");
	sphereTest.setTotalIndex(totalPoints, displayInterval);
	//go through 3D space points by class GoThroughSphereByIndexes and count the time used -- method 1
	startTick = getTimeTick();
	ret = sphereTest.gothroughSphere(maxRadius);
	endTick = getTimeTick();
	//finished going through 3D points by class GoThroughSphereByIndexes
	tickCountSphere = endTick - startTick;
	reportProcess(showProgressReport, true, "Went through sphere indexes %d in %d ticks\r\n", sphereTest.getCurrentIndex(), tickCountSphere);
	if(ret == ERR_OK)
	{
		if(totalPoints != sphereTest.getCurrentIndex())
		{
			ret = ERR_RADIUS_INDEX_NOT_END;
		}
		else
		{
			int displayCount = 0;
			size_t idx = 0;
			int maxN = GRIDPOINTS(N);
			size_t maxN3 = maxN * maxN * maxN;
			if(maxN3 != totalPoints)
			{
				ret = ERR_SPHERE_COUNT_MISMATCH;
			}
			else
			{
				//go through 3D points by row-major indexing and count the time used -- method 2
				startTick = getTimeTick();
				for(int i = 0; i < maxN; i++)
				{
					for(int j = 0; j < maxN; j++)
					{
						for(int k = 0; k < maxN; k++)
						{
							//this part of handling must be exactly the same as function handleData(...) of SphereIndexSpeedTest
							//so that the times for both methods are the same
							displayCount ++;
							if(displayCount > displayInterval)
							{
								displayCount = 0;
								reportProcess(showProgressReport, true, "Sphere index: %d / %d", idx, maxN3);
							}
							idx++;
						}
					}
				}
				endTick = getTimeTick();
				//finished going through row-major indexing
				tickCount3Darray = endTick - startTick;
				reportProcess(showProgressReport, true, "Went through 3D array indexes %d in %d ticks\r\n", maxN3, tickCount3Darray);
#ifdef __linux__ 
				printf("\r\n  Time difference (3D array) - (sphere) = %Ilu, diff percent:%g%%\r\n", tickCount3Darray - tickCountSphere,100.0*((double)tickCount3Darray - (double)tickCountSphere)/(double)tickCountSphere);
#elif _WIN32
				printf("\r\n  Time difference (3D array) - (sphere) = %Iu, diff percent:%g%%\r\n", tickCount3Darray - tickCountSphere,100.0*((double)tickCount3Darray - (double)tickCountSphere)/(double)tickCountSphere);
#else
#endif
			}
		}
	}
	return ret;
}

/*
	Verify that fields initialized in a FDTD class are the same as that provided by the same FieldsInitializer instance.
	it goes through all space points, radius by radius, getting fields from FieldsInitializer for each space point,
	compare the fields with data from FDTD instance via a sphere index of (m,n,p)
	it reports the maximum difference and the average difference between the them 
	it reports the maximum field strength and average field strength to see that the fields are not small comparing to differences
*/
int task4_verifyFieldInitializer(FieldsInitializer *fields0, TaskFile *taskConfig)
{
	int ret = ERR_OK;
	int N = taskConfig->getInt(TP_FDTDN, false);
	double range = taskConfig->getDouble(TP_FDTDR, false);
	int maxRadius = GRIDRADIUS(N);
	RadiusIndexToSeriesIndex idxCache;
	puts("\r\nInitializing...");
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
		else
		{
			//create a 3D array to hold (m,n,p) to 1D memory index mapping
			ret = idxCache.initialize(maxRadius);
			if(ret == ERR_OK)
			{
				//initialize field provider
				ret = fields0->initialize(taskConfig);
			}
		}
	}
	if(ret == ERR_OK)
	{
		TssInSphere tss;
		double ds = SPACESTEP(range, N);
		tss.setIndexCache(&idxCache);
		ret = tss.initialize(NULL, NULL, taskConfig);
		if(ret == ERR_OK)
		{
			//this is the function we want to test
			//it is a function defined in base abstract class FDTD
			ret = tss.PopulateFields(fields0);
		}
		if(ret == ERR_OK)
		{
			double maxField0 = 0.0, sumField0 = 0.0, sumerror = 0.0, maxErr = 0.0; 
			size_t displayCount = 0, count = 0;
			FieldPoint3D f;
			size_t idx; //1D memory index
			size_t startIndex = 0; //1D memory index for the first point of each memory block
			FieldPoint3D *HE = tss.GetFieldMemory(); //EM field already initialized
			size_t numPoints = totalPointsInSphere(maxRadius);
			puts("\r\nInitialized. \r\n");
			for(int r = 0; r <= maxRadius; r++) //go through memory blocks for each radius
			{
				double v;
				double x,y,z;
				int m,n,p;
				size_t a;
				size_t pointsAtRadius = pointsAt(r); //item count in the memory block
				//
				for(a = 0; a < pointsAtRadius; a++) //go through memory in the block
				{
					//series indexing (1D memory indexing) to 3D radius indexing, a is unique, (m,n,p) is expected to be unique
					ret = IndexToIndexes(r, a, &m, &n, &p);
					if(ret != ERR_OK)
					{
						break;
					}
					idx = a + startIndex; //this is the index into 1D memory
					//space point:
					x = (double)m * ds;
					y = (double)n * ds;
					z = (double)p * ds;
					//get field for the space point
					fields0->getField(x,y,z,&f);
					//max field0 -- we cannot do a good verification if field strength is too small 
					v = abs(f.E.x);
					sumField0 += v;
					if(v > maxField0) maxField0 = v;
					v = abs(f.E.y);
					sumField0 += v;
					if(v > maxField0) maxField0 = v;
					v = abs(f.E.z);
					sumField0 += v;
					if(v > maxField0) maxField0 = v;
					v = abs(f.H.x);
					sumField0 += v;
					if(v > maxField0) maxField0 = v;
					v = abs(f.H.y);
					sumField0 += v;
					if(v > maxField0) maxField0 = v;
					v = abs(f.H.z);
					sumField0 += v;
					if(v > maxField0) maxField0 = v;
					//max error -- compare field strength from populated field and from the initializer
					v = abs(f.E.x - HE[idx].E.x);
					sumerror += v;
					if(v > maxErr) maxErr = v;
					v = abs(f.E.y - HE[idx].E.y);
					sumerror += v;
					if(v > maxErr) maxErr = v;
					v = abs(f.E.z - HE[idx].E.z);
					sumerror += v;
					if(v > maxErr) maxErr = v;
					v = abs(f.H.x - HE[idx].H.x);
					sumerror += v;
					if(v > maxErr) maxErr = v;
					v = abs(f.H.y - HE[idx].H.y);
					sumerror += v;
					if(v > maxErr) maxErr = v;
					v = abs(f.H.z - HE[idx].H.z);
					sumerror += v;
					if(v > maxErr) maxErr = v;
					//
					displayCount ++;
					count ++;
					if(displayCount > 10000)
					{
						displayCount=0;
						reportProcess(showProgressReport, true, "finished %d / %d",count, numPoints);
					}
				}
				if(ret != ERR_OK)
				{
					break;
				}
				startIndex += pointsAtRadius;
			}
			if(ret == ERR_OK)
			{
				double avgErr = sumerror / (double) (6.0 * (double)numPoints);
				double avgFld = sumField0 / (double) (6.0 * (double)numPoints);
				reportProcess(showProgressReport, true, "finished %d / %d",count, numPoints);
				reportProcess(showProgressReport, false, "max field: %g, average field:%g max err: %g, average err:%g", maxField0, avgFld, maxErr, avgErr);
			}
		}
	}
	return ret;
}

/*
	Verify that EM fields provided by a Initial Value module are source-free: the divergences are 0
	it reports the maximum divergence and the average divergence of the fields 
	it reports the maximum field strength and average field strength to see that the fields are not small comparing to divergences
*/
int task5_verifyFields(FieldsInitializer *fields0, TaskFile *taskConfig)
{
	int ret = ERR_OK;
	int N = taskConfig->getInt(TP_FDTDN, false);
	double range = taskConfig->getDouble(TP_FDTDR, false);
	int maxRadius = GRIDRADIUS(N);
	RadiusIndexToSeriesIndex idxCache;
	puts("\r\nInitializing...");
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
		else
		{
			ret = idxCache.initialize(maxRadius);
		}
	}
	if(ret == ERR_OK)
	{
		TssInSphere tss;
		tss.setIndexCache(&idxCache);
		tss.setReporter(showProgressReport, false);
		ret = tss.initialize(NULL, NULL, taskConfig);
		if(ret == ERR_OK)
		{
			ret = tss.PopulateFields(fields0);
			if(ret == ERR_OK)
			{
				ret = tss.verifyFieldsByDivergence(NULL);
			}
		}
	}
	return ret;
}


/*
	compare {datafilename1}{n}.em and {datafilename2}{n}.em from r=0,1,...,internalRadius, n=0,1,2,...
	These values can be calculated from data in files for each r:
		e1 = (average divergence E of file 1)
		h1 = (average divergence H of file 1)
		e2 = (average divergence E of file 2)
		h2 = (average divergence H of file 2)
	calculate these values
		de(r) = (e1 - e2)
		dh(r) = (h1 - h2)
	record these values in the report file named by {datafilename1}_compare.txt:
		n, min(de(r)), max(de(r)), min(dh(r)), max(dh(r)), average(de(r)), average(dh(r))	

	taskConfig  - task file
	dataFolder1 - folder for data files 1
	dataFolder2 - folder for data files 2

*/
int task110_compareSimData(TaskFile *taskConfig, const char *dataFolder1, const char *dataFolder2)
{
	int ret = ERR_OK;
	char *basefile1 = taskConfig->getString(TP_SIMFILE1, false);
	char *basefile2 = taskConfig->getString(TP_SIMFILE2, false);
	unsigned int N = taskConfig->getUInt(TP_FDTDN, false);
	double range = taskConfig->getDouble(TP_FDTDR, false);
	/*
	edgeSize        - number of grids near the boundary to be excluded from comparison. this is to exclude boundary errors of data.
						basically every time step generates simulation error at one more radius away from the boundary. 
						if 30 time steps are simulated then edgeSize should be at least 30
	*/
	int edgeSize = taskConfig->getInt(TP_SIMTHICKNESS, false);
	//
	ret = taskConfig->getErrorCode();
	if(ret == ERR_OK)
	{
		char fnameC1[FILENAME_MAX];
		char fnameC2[FILENAME_MAX];
		//wchar_t fname1[FILENAME_MAX];
		//wchar_t fname2[FILENAME_MAX];
		int maxRadius = GRIDRADIUS(N);
		//these ds should be the same
		double ds = SPACESTEP(range, N);;
		int n;
		unsigned int simulationRadius;
		char file1[FILENAME_MAX];
		RadiusIndexToSeriesIndex idxCache;
		FieldPoint3D *fields1;
		size_t size1;
		char file2[FILENAME_MAX];
		FieldPoint3D *fields2;
		size_t size2;
		//
		int internalRadius = maxRadius - edgeSize;
		//
		if(internalRadius <= 0)
		{
			ret = ERR_RADIUS_INDEX_NEGATIVE;
		}
		else
		{
			ret = idxCache.initialize(maxRadius);
		}
		if(ret == ERR_OK)
		{
			char reportfile[FILENAME_MAX];
			ret = formFilePath(fnameC1, FILENAME_MAX, dataFolder1, basefile1);
			if(ret == ERR_OK)
			{
				ret = formFilePath(fnameC2, FILENAME_MAX, dataFolder2, basefile2);
			}
			/*if(ret == ERR_OK)
			{
				ret = copyC2W(fname1, FILENAME_MAX, fnameC1);
			}
			if(ret == ERR_OK)
			{
				ret = copyC2W(fname2, FILENAME_MAX, fnameC2);
			}
			ret = copyW2C(reportfile, FILENAME_MAX, fname2);*/
			if(ret == ERR_OK)
			{
#ifdef __linux__
				ret = snprintf(reportfile, FILENAME_MAX, "%s_compare.txt", fnameC2);
#elif _WIN32
				ret = sprintf_s(reportfile, FILENAME_MAX, "%s_compare.txt", fnameC2);
#else
#endif
				
			}
			if(ret == ERR_OK)
			{
				printf("\r\nBase file name 1: %s", fnameC1);
				printf("\r\nBase file name 2: %s\r\n", fnameC2);
				printf("\r\nGenerating report file %s, ...\r\n\r\n", reportfile);
				ret = CreateReportFile(reportfile);
				if(ret == ERR_OK)
				{
					reportProcess(writeReport, false,"\tMinE_A - MinE_B\tMinH_A - MinH_B\tMaxE_A - MaxE_B\tMaxH_A - MaxH_B\tDiff E\tDiff H");
					n = 0;
					while(ret == ERR_OK)
					{
						ret = formDataFileName(file1, FILENAME_MAX, fnameC1, n);
						if(ret == ERR_OK)
						{
							if(fileexists(file1))
							{
								ret = formDataFileName(file2, FILENAME_MAX, fnameC2, n);
								if(ret == ERR_OK)
								{
									if(!fileexists(file2))
									{
										break;
									}
								}
							}
							else
							{
								break;
							}
						}
						if(ret == ERR_OK)
						{
							reportProcess(showProgressReport, true, "Processing file %d...", n);
							fields1 = (FieldPoint3D *)ReadFileIntoMemory(file1, &size1, &ret);
							if(ret == ERR_OK)
							{
								if(fields1 == NULL)
								{
									ret = ERR_OUTOFMEMORY;
								}
								else
								{
									fields2 = (FieldPoint3D *)ReadFileIntoMemory(file2, &size2, &ret);
									if(ret == ERR_OK)
									{
										if(fields2 == NULL)
										{
											ret = ERR_OUTOFMEMORY;
										}
										else
										{
											if(size1 != size2)
											{
												ret = ERR_FILESIZE_MISMATCH;
											}
											else
											{
												ret = MemorySizeToRadius(size1, &simulationRadius);
												if(ret == ERR_OK)
												{
													if(simulationRadius < (unsigned int)internalRadius)
													{
														ret = ERR_RADIUS_INDEX_TOOLITTLE;
													}
													else
													{
														FieldDivergenceComparer fc(&idxCache);
														ret = fc.setFields(fields1, fields2, maxRadius, ds, internalRadius);
														if(ret == ERR_OK)
														{
															ret = fc.compareDivergenceByRadius();
															if(ret == ERR_OK)
															{
																reportProcess(writeReport, false,"%d\t%g - %g\t%g - %g\t%g - %g\t%g - %g\t%g\t%g",n
																	,fc.GetMinimumE_A()
																	,fc.GetMinimumE_B()
																	,fc.GetMinimumH_A()
																	,fc.GetMinimumH_B()
																	,fc.GetMaximumE_A()
																	,fc.GetMaximumE_B()
																	,fc.GetMaximumH_A()
																	,fc.GetMaximumH_B()
																	,fc.GetAverageDiffE()
																	,fc.GetAverageDiffH()
																	);
															}
														}
													}
												}
											}
											FreeMemory(fields2);
										}
									}
									FreeMemory(fields1);
								}
							}
						}
						n++;
					}
					//
					CloseReportFile();
					if(ret == ERR_OK)
					{
						if(n == 0)
						{
							ret = ERR_TP_DATAFILE;
						}
					}
				}
			}
		}
	}
	//
	return ret;
}


/*
	for each data file, generate a report file showing average divergences at each radius.
	also generate one report file summarizing divergences at each radius and at each time step.

	taskConfig  - task file
	dataFolder - folder for data files

*/
int task120_makeReportFiles(TaskFile *taskConfig, const char *dataFolder)
{
	FieldSimulation fs;
	return fs.createStatisticsFiles(taskConfig, dataFolder);
}

/*
	pick field points with the largest strengths from each data file and generate a new data file. 
	the new file contains items of 9 doubles: 3 for space location, 6 for EM field. 
	It requires command line parameters "/W" and "/D". 
	It requires following task parameters: "FDTD.R", "SIM.BASENAME", 
	"SIM.POINTS" for the number of field points to pick, 
	and "SIM.MAXTIMES" for maximum number of data files to process, use 0 to process all data files.
*/
int task130_pickPointsFromDataFiles(TaskFile *taskConfig, const char *dataFolder)
{
	int ret = ERR_OK;
	char *basefile = taskConfig->getString(TP_SIMBASENAME, false);
	double range = taskConfig->getDouble(TP_FDTDR, false);
	unsigned int picks = taskConfig->getUInt(TP_SIM_PICKS, false);
	size_t maxTimeSteps = (size_t)taskConfig->getLong(TP_SIM_MAXTIME, false);
	ret = taskConfig->getErrorCode();
	if(ret == ERR_OK)
	{
		if(picks == 0)
		{
			ret = ERR_TASK_INVALID_VALUE;
			taskConfig->setNameOfInvalidValue(TP_SIM_PICKS);
		}
		else if(range <= 0.0)
		{
			ret = ERR_TASK_INVALID_VALUE;
			taskConfig->setNameOfInvalidValue(TP_FDTDR);
		}
		else if(strlen(basefile) == 0)
		{
			ret = ERR_TASK_INVALID_VALUE;
			taskConfig->setNameOfInvalidValue(TP_SIMBASENAME);
		}
		else
		{
			if(strcmp(basefile, "DEF") == 0)
			{
				ret = ERR_TASK_INVALID_VALUE;
				taskConfig->setNameOfInvalidValue(TP_SIMBASENAME);
			}
		}
	}
	if(ret == ERR_OK)
	{
		char fbase[FILENAME_MAX];
		ret = formFilePath(fbase, FILENAME_MAX, dataFolder, basefile);
		if(ret == ERR_OK)
		{
			char fCollectbase[FILENAME_MAX];
#ifdef __linux__
			ret = snprintf(fCollectbase, FILENAME_MAX, "%s%u_", fbase, picks);
#elif _WIN32
			ret = sprintf_s(fCollectbase, FILENAME_MAX, "%s%u_", fbase, picks);
#else
#endif
				
			if(ret == ERR_OK)
			{
				PickFieldPoints collector;
				FieldPoint3D *fields;
				FieldItem3D *items;
				unsigned int maxRadius = 0;
				unsigned int N = 0;
				double ds = 0;
				char dataFile[FILENAME_MAX];
				char dataFileCollected[FILENAME_MAX];
				size_t size;
				size_t sizeItems = sizeof(FieldItem3D)*picks;
				size_t k = 0;
				puts("Creating data collection files. File number: ");
				collector.SetMemoryManager(_mem);
				while(ret == ERR_OK)
				{
					ret = formDataFileName(dataFile, FILENAME_MAX, fbase, k);
					if(ret == ERR_OK)
					{
						ret = formFieldFileName(dataFileCollected, FILENAME_MAX, fCollectbase, k);
						if(ret == ERR_OK)
						{
							if(fileexists(dataFile))
							{
#ifdef __linux__ 
								printf("%Ilu ",k);
#elif _WIN32
								printf("%Iu ",k);
#else
#endif
								fields = (FieldPoint3D *)ReadFileIntoMemory(dataFile, &size, &ret);
								if(ret == ERR_OK)
								{
									if(k == 0)
									{
										ret = MemorySizeToRadius(size, &maxRadius);
										if(ret == ERR_OK)
										{
											if( ((maxRadius-1) % 2) == 0)
											{
												N = (maxRadius - 1) / 2;
												ds = SPACESTEP(range, N);
												if(picks >= totalPointsInSphere(maxRadius))
												{
													ret = ERR_TASK_INVALID_VALUE;
													taskConfig->setNameOfInvalidValue(TP_SIM_PICKS);
												}
											}
											else
											{
												ret = ERR_RADIUS_INDEX_MISMATCH;
											}
										}
									}
									items = (FieldItem3D *)CreateFileIntoMemory(dataFileCollected, sizeItems, &ret);
									if(ret == ERR_OK)
									{
										ret = collector.initialize(picks, fields, items);
										if(ret == ERR_OK)
										{
											ret = collector.gothroughSphere(maxRadius, ds);
										}
										FreeMemory(items);
									}
									FreeMemory(fields);
									if(ret == ERR_OK)
									{
										if(!collector.IsInMonotonic())
										{
											ret = ERR_SIM_MONOTONIC;
										}
										else
										{
											k++;
											if(maxTimeSteps > 0)
											{
												if(k >= maxTimeSteps)
												{
													ret = ERR_REACHED_TIME_LIMIT;
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
					}
				}
			}
		}
	}
	return ret;
}

/*
	merge two summary files into one file. a summary file is generated by task 120. 
	It requires command line parameters "/W", "/D" and "/E". 
	Use task parameters "SIM.FILE1" and "SIM.FILE2" to specify the names of the summary files;
	"/D" specifies folder for "SIM.FILE1" and "/E" specifies folder for "SIM.FILE2". 
	Use task parameter "SIM.THICKNESS" to specify boundary thickness to be excluded from the merge.

	taskConfig  - task file
	dataFolder1 - folder for summary file 1
	dataFolder2 - folder for summary file 2

	summary file format:
	sizeof(unsigned int): {maxRadius}
	{maxRadius+1} * sizeof(double): each double is an average divergence magnitude on a radius; this row is for time 0
	{maxRadius+1} * sizeof(double): each double is an average divergence magnitude on a radius; this row is for time 1
	...

	merged file format:
	sizeof(unsigned int): {maxRadius-thickness}
	2*{maxRadius-thickness+1} * sizeof(double): each double is an average divergence magnitude on a radius from file1; and then from file2; for time 0
	2*{maxRadius-thickness+1} * sizeof(double): each double is an average divergence magnitude on a radius from file1; and then from file2; for time 1
	...


*/
int task140_mergeSummaryFiles(TaskFile *taskConfig, const char *dataFolder1, const char *dataFolder2)
{
	int ret = ERR_OK;
	char *file1 = taskConfig->getString(TP_SIMFILE1, false);
	char *file2 = taskConfig->getString(TP_SIMFILE2, false);
	/*
	edgeSize        - number of grids near the boundary to be excluded from comparison. this is to exclude boundary errors of data.
						basically every time step generates simulation error at one more radius away from the boundary. 
						if 30 time steps are simulated then edgeSize should be at least 30
	*/
	unsigned int edgeSize = taskConfig->getUInt(TP_SIMTHICKNESS, true);
	ret = taskConfig->getErrorCode();
	if(ret == ERR_OK)
	{
		char fnameC1[FILENAME_MAX];
		char fnameC2[FILENAME_MAX];
		char fnmerge[FILENAME_MAX];
		double *row;
		ret = formFilePath(fnameC1, FILENAME_MAX, dataFolder1, file1);
		if(ret == ERR_OK)
		{
			ret = formFilePath(fnameC2, FILENAME_MAX, dataFolder2, file2);
		}
		if(ret == ERR_OK)
		{
#ifdef __linux__
			ret = snprintf(fnmerge, FILENAME_MAX, "%s.diff", fnameC2);
#elif _WIN32
			ret = sprintf_s(fnmerge, FILENAME_MAX, "%s.diff", fnameC2);
#else
#endif
			
			if(ret == ERR_OK)
			{
				FILE *fh1, *fh2, *fhm;
				ret = openfileRead(fnameC1, &fh1);
				if(ret == ERR_OK)
				{
					ret = openfileRead(fnameC2, &fh2);
					if(ret == ERR_OK)
					{
						unsigned int maxRadius;
						unsigned int u;
						ret = readfile(fh1, &maxRadius, sizeof(unsigned int));
						if(ret == ERR_OK)
						{
							ret = readfile(fh2, &u, sizeof(unsigned int));
							if(ret == ERR_OK)
							{
								if(maxRadius != u)
								{
									ret = ERR_FILESIZE_MISMATCH;
								}
								else
								{
									if(maxRadius <= edgeSize)
									{
										ret = ERR_INVALID_SIZE;
									}
									else
									{
										size_t sz = (maxRadius + 1) * sizeof(double);
										row = (double *)malloc(sz);
										if(row == NULL)
										{
											ret = ERR_OUTOFMEMORY;
										}
										else
										{
											u = maxRadius - edgeSize;
											ret = openfileWrite(fnmerge, &fhm);
											if(ret == ERR_OK)
											{
												unsigned int szu = (u + 1) * sizeof(double);
												ret = writefile(fhm, &u, sizeof(unsigned int));
												while(ret == ERR_OK)
												{
													ret = readfile(fh1, row, (unsigned int)sz);
													if(ret == ERR_OK)
													{
														ret = writefile(fhm, row, szu);
														if(ret == ERR_OK)
														{
															ret = readfile(fh2, row, (unsigned int)sz);
															if(ret == ERR_OK)
															{
																ret = writefile(fhm, row, szu);
															}
														}
													}
												}
												if(ret == ERR_FILE_READ_EOF)
												{
													ret = ERR_OK;
												}
												closefile(fhm);
											}
											free(row);
										}
									}
								}
							}
						}
						closefile(fh2);
					}
					closefile(fh1);
				}
			}
		}
	}
	return ret;
}

/*
	merge two summary files into one file. a summary file is generated by task 120. 
	It requires command line parameters "/W", "/D" and "/E". 
	Use task parameters "SIM.FILE1" and "SIM.FILE2" to specify the names of the summary files;
	"/D" specifies folder for "SIM.FILE1" and "/E" specifies folder for "SIM.FILE2". 

	it assumes the two simulations use following space digitizations:
		ds1 = ds2 / 2
		maxRadius1 + 1 = 2 * maxRadius2

	for summary file format, see descriptions for Task 140.

	merged file is formed by:
	sizeof(unsigned int): {maxRadius2-1}
	{maxRadius2} * sizeof(double): the first {maxRadius2} doubles are from row 1 columns 0, 2, 4, ..., 2*{maxRadius2-1} of file 1;
								   the next  {maxRadius2} doubles are from row 1 columns 0, 1, 2, ...,   {maxRadius2-1} of file 2;
								   this is for time 0
	{maxRadius2} * sizeof(double): this is for time 1
	{maxRadius2} * sizeof(double): this is for time 2
	...

	taskConfig  - task file
	dataFolder1 - folder for summary file 1
	dataFolder2 - folder for summary file 2

*/
int task160_mergeSummaryFiles(TaskFile *taskConfig, const char *dataFolder1, const char *dataFolder2)
{
	int ret = ERR_OK;
	char *file1 = taskConfig->getString(TP_SIMFILE1, false);
	char *file2 = taskConfig->getString(TP_SIMFILE2, false);
	ret = taskConfig->getErrorCode();
	if(ret == ERR_OK)
	{
		char fnameC1[FILENAME_MAX];
		char fnameC2[FILENAME_MAX];
		char fnmerge[FILENAME_MAX];
		double *row;
		ret = formFilePath(fnameC1, FILENAME_MAX, dataFolder1, file1);
		if(ret == ERR_OK)
		{
			ret = formFilePath(fnameC2, FILENAME_MAX, dataFolder2, file2);
		}
		if(ret == ERR_OK)
		{
#ifdef __linux__
			ret = snprintf(fnmerge, FILENAME_MAX, "%s.diff", fnameC2);
#elif _WIN32
			ret = sprintf_s(fnmerge, FILENAME_MAX, "%s.diff", fnameC2);
#else
#endif
			
			if(ret == ERR_OK)
			{
				FILE *fh1, *fh2, *fhm;
				ret = openfileRead(fnameC1, &fh1);
				if(ret == ERR_OK)
				{
					ret = openfileRead(fnameC2, &fh2);
					if(ret == ERR_OK)
					{
						unsigned int maxRadius;
						unsigned int u;
						ret = readfile(fh1, &maxRadius, sizeof(unsigned int));
						if(ret == ERR_OK)
						{
							ret = readfile(fh2, &u, sizeof(unsigned int));
							if(ret == ERR_OK)
							{
								if(maxRadius != (2 * u - 1))
								{
									ret = ERR_FILESIZE_MISMATCH;
								}
								else
								{
									size_t sz = (maxRadius + 1) * sizeof(double); //row size of file 1
									row = (double *)malloc(sz);
									if(row == NULL)
									{
										ret = ERR_OUTOFMEMORY;
									}
									else
									{
										unsigned int sz2 = (u + 1) * sizeof(double); //row size of file 2
										unsigned int szu = sz2 - sizeof(double); //row size of merged file
										u--; //maxRadius of merged file
										ret = openfileWrite(fnmerge, &fhm);
										if(ret == ERR_OK)
										{
											//u = maxRadius2-1
											ret = writefile(fhm, &u, sizeof(unsigned int));
											while(ret == ERR_OK)
											{
												ret = readfile(fh1, row, (unsigned int)sz);
												if(ret == ERR_OK)
												{
													//move even columns to front
													for(unsigned int r = 1; r <= u; r++)
													{
														*(row + r) = *(row + 2*r);
													}
													//write first half row
													ret = writefile(fhm, row, szu);
													if(ret == ERR_OK)
													{
														ret = readfile(fh2, row, (unsigned int)sz2);
														if(ret == ERR_OK)
														{
															//write second half row
															ret = writefile(fhm, row, szu);
														}
													}
												}
											}
											if(ret == ERR_FILE_READ_EOF)
											{
												ret = ERR_OK;
											}
											closefile(fhm);
										}
										free(row);
									}
								}
							}
						}
						closefile(fh2);
					}
					closefile(fh1);
				}
			}
		}
	}
	return ret;
}
/*
	from a set of simulation data files (*.em files generated by task 100), generate one file (actually 2 files, one binary and one text), 
	the file contains an array of 6 values of Ex,Ey,Ez,Hx,Hy,Hz at one given space location, 
	the first set of the 6 field values is for time=0, the next set for t=dt, and 2dt, 3dt, ... 
	It requires command line parameters /W, /D. 
	Use task parameters SIM.FILE1 to specify a base file name for the data source files, 
	i.e. for SIM.FILE1=YeeFDTD_N8_R0R2_T2S2, the source files are YeeFDTD_N8_R0R2_T2S2_*.em under the folder specified by /D, 
	  the generated file is named as YeeFDTD_N8_R0R2_T2S2_n_m_p.dat for a binary file and as YeeFDTD_N8_R0R2_T2S2_n_m_p.txt for a text file. 
	Use task parameters POINT.m, POINT.n, and POINT.p to specify space location m, n, and p. 
*/
int task170_makePointTimeFile(TaskFile *taskConfig, const char *dataFolder)
{
	int ret = ERR_OK;
	//simulation data file base name
	char *filename = taskConfig->getString(TP_SIMFILE1, false);
	//space location
	int m = taskConfig->getInt(TP_POINTM, false);
	int n = taskConfig->getInt(TP_POINTN, false);
	int p = taskConfig->getInt(TP_POINTP, false);
	ret = taskConfig->getErrorCode();
	if (ret == ERR_OK)
	{
		char fnameBase[FILENAME_MAX];
		//unsigned int r = max(abs(m), max(abs(n), abs(p)));
		ret = formFilePath(fnameBase, FILENAME_MAX, dataFolder, filename);
		if (ret == ERR_OK)
		{
			//one dimensional array index for this space location
			size_t index = 0;
			char outFileBin[FILENAME_MAX]; //binary output file name
			char outFileTxt[FILENAME_MAX]; //text output file name
			
#ifdef __linux__
			ret = snprintf(outFileBin, FILENAME_MAX, "%s_%d_%d_%d.dat", fnameBase,m,n,p)<=0?ERR_STR_PRF:ERR_OK;
#elif _WIN32
			ret = sprintf_s(outFileBin, FILENAME_MAX, "%s_%d_%d_%d.dat", fnameBase, m, n, p) <=0?ERR_STR_PRF:ERR_OK;
#else
#endif
			if (ret == ERR_OK)
			{
#ifdef __linux__
				ret = snprintf(outFileTxt, FILENAME_MAX, "%s_%d_%d_%d.txt", fnameBase, m, n, p)<=0?ERR_STR_PRF:ERR_OK;
#elif _WIN32
				ret = sprintf_s(outFileTxt, FILENAME_MAX, "%s_%d_%d_%d.txt", fnameBase, m, n, p)<=0?ERR_STR_PRF:ERR_OK;
#else
#endif
				
			}
			if (ret == ERR_OK)
			{
				FILE *fhBin, *fhTxt;
				ret = openfileWrite(outFileBin, &fhBin);
				if (ret == ERR_OK)
				{
					ret = openTextfileWrite(outFileTxt, &fhTxt);
					if (ret == ERR_OK)
					{
						char sourceFile[FILENAME_MAX];
						//wchar_t sourceFileW[FILENAME_MAX];
						size_t sizeMem = 0;
						unsigned int rMax = 0;
						int t = 0;
						puts("Starting...\r\n");
						while (ret == ERR_OK)
						{
							reportProcess(showProgressReport, true, "processing time %d ", t);
							//form data file path
							
#ifdef __linux__
							ret = snprintf(sourceFile, FILENAME_MAX, "%s_%d.em", fnameBase, t)<=0?ERR_STR_PRF:ERR_OK;
#elif _WIN32
							ret = sprintf_s(sourceFile, FILENAME_MAX, "%s_%d.em", fnameBase, t) <=0?ERR_STR_PRF:ERR_OK;
#else
#endif
							
							if (ret != ERR_OK)
							{
								break;
							}
							//ret = copyC2W(sourceFileW, FILENAME_MAX, sourceFile);
							if (ret == ERR_OK)
							{
								if (fileexists(sourceFile))
								{
									size_t size1;
									//read fields from simulation data file
									FieldPoint3D* fields = (FieldPoint3D *)ReadFileIntoMemory(sourceFile, &size1, &ret);
									if (ret == ERR_OK)
									{
										if (fields == NULL)
										{
											ret = ERR_OUTOFMEMORY;
										}
										else
										{
											if (sizeMem == 0)
											{
												//verify that the space location is not outside of the domain
												sizeMem = size1;
												ret = MemorySizeToRadius(size1, &rMax);
												if (ret == ERR_OK)
												{
													size_t itemCount = totalPointsInSphere(rMax);
													RadiusIndexToSeriesIndex R;
													ret = R.initialize(rMax);
													size_t idx = R.Index(m, n, p);
													index = idx;
													if (itemCount < index)
													{
														//invalid space location
														ret = ERR_INVALID_RADIUS_INDEX;
													}
												}
											}
											else
											{
												if (sizeMem != size1)
												{
													ret = ERR_FILESIZE_MISMATCH;
												}
											}
											if (ret == ERR_OK)
											{
												//write to binary file
												ret = writefile(fhBin, fields+index, sizeof(FieldPoint3D));
												if (ret == ERR_OK)
												{
													//form text data
													char buff[1024];
													ret = sprintf_1(buff, 1024, "%g\t%g\t%g\t%g\t%g\t%g\n", 
														fields[index].E.x,
														fields[index].E.y,
														fields[index].E.z,
														fields[index].H.x,
														fields[index].H.y,
														fields[index].H.z) <=0?ERR_STR_PRF:ERR_OK;

													if (ret == ERR_OK)
													{
														//write to text file
														ret = writefile(fhTxt, buff, (unsigned int)strnlen_0(buff,1024));
													}
												}
											}
											FreeMemory(fields);
										}
									}
								}
								else
								{
									break;
								}
							}
							t++;
						}
						closefile(fhTxt);
					}
					closefile(fhBin);
				}
			}
		}
	}
	return ret;
}

int task301_verifyCurls(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret = ERR_OK;
	ret = testSpaceModule(mem, taskConfig, dataFolder, reporter);
	return ret;
}
int task302_verifyTimes(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret = ERR_OK;
	ret = testTimeModule(mem, taskConfig, dataFolder, reporter);
	return ret;
}
/*
	run EM simulation in rectangle space indexing
*/
int task300_TssSimulation(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter, fnOperationCanceld cancelReport)
{
	SetupCancelHandler();
	puts("\r\nStarting electromagnetic field simulation. Press Ctrl-C to stop. \r\n Initialize and prepare fields at time 0 ...\r\n");
	return tssSimulation(mem, taskConfig, dataFolder, reporter, cancelReport);
}

int task310_energyDivergence(MemoryManager *_mem, const char *dataFolder, const char *baseFile, char *matrixFolder)
{
	int ret = ERR_OK;
	char filenameBase[FILENAME_MAX];
	char fnameStatistic[FILENAME_MAX];
	char msg[MESSAGELINESIZE];
	unsigned int fileIndex = 0;
	FieldStatisticStruct fs;
	FILE *fhStatistic=0;
	SimStruct pams;
	ret = formFilePath(filenameBase, FILENAME_MAX, dataFolder, baseFile);
	if (ret == ERR_OK)
	{
		ret = sprintf_1(fnameStatistic, FILENAME_MAX, "%sparameters.dat", filenameBase) <=0?ERR_STR_PRF:ERR_OK;
		if (ret == ERR_OK)
		{
			ret = Simulator::LoadSimulatePatamersFromFile(fnameStatistic, &pams);
		}
	}
	if (ret == ERR_OK)
	{
		ret = sprintf_1(fnameStatistic, FILENAME_MAX, "%sstatistic.txt", filenameBase) <=0?ERR_STR_PRF:ERR_OK;
		
		if (ret == ERR_OK)
		{
			ret = openTextfileWrite(fnameStatistic, &fhStatistic);
			if (ret == ERR_OK)
			{
				ret = sprintf_1(msg, MESSAGELINESIZE, "Time\tEnergy\tMax Divg E\tAvg Divg E\tMax Divg H\tAvg Divg H") <=0?ERR_STR_PRF:ERR_OK;
				if (ret == ERR_OK)
				{
					ret = writefile(fhStatistic, msg, (unsigned int)strnlen_0(msg, MESSAGELINESIZE));
				}
			}
		}
	}
	if (ret == ERR_OK)
	{
		char matrixFile[FILENAME_MAX];
		Space space;
		Point3Dstruct *efile = NULL, *hfile = NULL;
		size_t esize = 0, hsize = 0;
		size_t w;
		//
		fs.averageDivergenceE = fs.averageDivergenceH = 0.0;
		fs.energySum = 0.0;
		fs.maxDivergenceE = fs.maxDivergenceH = 0.0;
		fs.timeIndex = 0;
		//
		ret = Simulator::FormSpaceMatrixFilename(matrixFile, dataFolder, pams.smax, matrixFolder);
		if (ret == ERR_OK)
		{
			ret = space.initializeSpace(pams.ds, pams.nx, pams.ny, pams.nz, pams.smax, matrixFile);
		}
		//
		while (ret == ERR_OK)
		{
			ret = sprintf_1(fnameStatistic, FILENAME_MAX, "%se_%u.dat", filenameBase, fileIndex) <=0?ERR_STR_PRF:ERR_OK;
			if (ret != ERR_OK)
			{
				break;
			}
			if (fileexists(fnameStatistic))
			{
				//open E data file
				efile = (Point3Dstruct *)ReadFileIntoMemory(fnameStatistic, &esize, &ret);
			}
			else
			{
				break;
			}
			if (ret == ERR_OK)
			{
				ret = sprintf_1(fnameStatistic, FILENAME_MAX, "%sh_%u.dat", filenameBase, fileIndex) <=0?ERR_STR_PRF:ERR_OK;
				if (ret != ERR_OK)
				{
					break;
				}
				if (fileexists(fnameStatistic))
				{
					//open H data file
					hfile = (Point3Dstruct *)ReadFileIntoMemory(fnameStatistic, &hsize, &ret);
				}
				else
				{
					break;
				}
			}
			if (efile != NULL && hfile != NULL)
			{
				size_t pointCount = 0;
				//exclude boundaries
				unsigned int s0 = pams.smax + 2;
				unsigned int endX = pams.nx - 2*pams.smax;
				unsigned int endY = pams.ny - 2*pams.smax;
				unsigned int endZ = pams.nz - 2*pams.smax;
				double divg = 0.0;
				double emag = 0.0;
				double hmag = 0.0;
				//
				space.SetFields(hfile,efile);
				//
				/*
				Field energy = (eps * ||E||^2 + mu * ||H||^2) / 2
				*/
				fs.averageDivergenceE = fs.averageDivergenceH = 0.0;
				fs.energySum = 0.0;
				fs.maxDivergenceE = fs.maxDivergenceH = 0.0;
				fs.timeIndex = pams.saveInterval * fileIndex;
				pointCount = 0;
				for (unsigned int i = s0; i < endX; i++)
				{
					for (unsigned int j = s0; j < endY; j++)
					{
						for (unsigned int k = s0; k < endZ; k++)
						{
							w = space.Idx(i,j,k);
							emag = efile[w].x * efile[w].x + efile[w].y * efile[w].y + efile[w].z * efile[w].z;
							hmag = hfile[w].x * hfile[w].x + hfile[w].y * hfile[w].y + hfile[w].z * hfile[w].z;
							divg = abs( space.dx_Fx(efile, 1, i, j, k) + space.dy_Fy(efile, 1, i, j, k) + space.dz_Fz(efile, 1, i, j, k));
							divg = divg / pams.ds;
							if (emag > 0.00001)
							{
								divg = divg / emag;
							}
							if (divg > fs.maxDivergenceE)
							{
								fs.maxDivergenceE = divg;
							}
							fs.averageDivergenceE += divg;
							//
							divg = abs(space.dx_Fx(hfile, 1, i, j, k) + space.dy_Fy(hfile, 1, i, j, k) + space.dz_Fz(hfile, 1, i, j, k));
							divg = divg / pams.ds;
							if (hmag > 0.00001)
							{
								divg = divg / hmag;
							}
							if (divg > fs.maxDivergenceH)
							{
								fs.maxDivergenceH = divg;
							}
							fs.averageDivergenceH += divg;
							fs.energySum += pams.eps*(emag) / 2.0 
								+ pams.mu*(hmag) / 2.0;
							pointCount++;
						}
					}
				}
				//
				fs.averageDivergenceE = fs.averageDivergenceE / (double)pointCount;
				fs.averageDivergenceH = fs.averageDivergenceH / (double)pointCount;
				//
				ret = sprintf_1(msg, MESSAGELINESIZE, "\n%u\t%g\t%g\t%g\t%g\t%g",
					fs.timeIndex, fs.energySum, 
					fs.maxDivergenceE, fs.averageDivergenceE,
					fs.maxDivergenceH, fs.averageDivergenceH) <=0?ERR_STR_PRF:ERR_OK;
				if (ret == ERR_OK)
					ret = writefile(fhStatistic, msg, (unsigned int)strnlen_0(msg, MESSAGELINESIZE));
			}
			//
			if (efile != NULL) FreeMemory(efile);
			if (hfile != NULL) FreeMemory(hfile);
			efile = hfile = NULL;
			//
			fileIndex++;
		}
		if (efile != NULL) FreeMemory(efile);
		if (hfile != NULL) FreeMemory(hfile);
	}
	if (fhStatistic != 0)
	{
		closefile(fhStatistic);
	}
	return ret;
}

//
int task303_statisticsRadiusData(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	return createStatisticsForRadiusDataFiles(mem, taskConfig, dataFolder, reporter);
}
//
int task304_statisticsInRadiusForCubicData(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	return createStatisticsInRadiusForCubicDataFiles(mem, taskConfig, dataFolder, reporter);
}

int task315_HeilderCurrent(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	return testHeilderCurrent(taskConfig, dataFolder, reporter);
}

int task316_HeilderCurrentDT(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	return testHeilderCurrentDT(taskConfig, dataFolder, reporter);
}

int task317_HeilderCurrentDT2(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	return testHeilderCurrentDT2(taskConfig, dataFolder, reporter);
}

int task330_coefficientsOnly(MemoryManager *mem, TaskFile *taskfile, char *dataFolder, fnProgressReport reporter, fnOperationCanceld cancelReport)
{
	return prepareSimulation(mem, taskfile, dataFolder, reporter, cancelReport);
}

int task370_makePointTimeFile(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	return makePointTimeFile(mem, taskConfig, dataFolder, reporter);
}

int task371_createFieldComponentLines(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	return createFieldComponentLines(mem, taskConfig, dataFolder, reporter);
}

int task372_makeStatisticsFile(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	return makeStatisticsFile(mem, taskConfig, dataFolder, reporter);
}




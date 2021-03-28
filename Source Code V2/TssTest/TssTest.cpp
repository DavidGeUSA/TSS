/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/17/2020
Allrights reserved by David Ge

********************************************************************/
//

#include <stdio.h>
#include "../EMField/EMField.h"
#include "../EMField/RadiusIndex.h"
#include "../FieldProvider0/IV.h"
#include "RadiusCubic.h"
#include "TssTest.h"
#include "../Tss/Space.h"
#include "../Tss/TimeTss.h"
#include "../Tss/Simulator.h"
#include "../FileUtil/fileutil.h"
#include "../Tss/GaussianCurl.h"
#include "CurlXYZ.h"
#include "CurlXYZ2.h"
#include "GaussianTest.h"
#include "PluginInstances.h"
#include "FieldsTester.h"
#include "RadiusStatistics.h"
#include "../EMField/FieldSource.h"
#include "../SRC0/SRC0.h"
#include "HeidlerCurrent.h"
#include "../Tss/Curl.h"
#include "../Tss/CurlOne.h"
#include "../Tss/CurlTwo.h"
#include "../MemoryMan/memman.h"

#include <boost/multiprecision/cpp_dec_float.hpp>

#define Idx(i,j,k)  ((k) + nz1 * ((j) + ny1 * (i)));

unsigned int nx1=0, ny1=0, nz1=0;

Simulator *createSimulator(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, int *ret)
{
	Simulator *pSim = NULL;
	char *strName;
	//
	*ret = ERR_OK;
	TssSimStruct *tss;
	strName = taskConfig->getString(TP_TSS_SIMULATE_CLASS, true);
	*ret = taskConfig->getErrorCode();
	if (*ret == ERR_OK)
	{
		if (strName == NULL)
			pSim = CreateSimulatorInstance("Simulator");
		else
			pSim = CreateSimulatorInstance(strName);
		if (pSim == NULL)
		{
			*ret = ERR_SIMULATOR_NAME;
		}
	}
	if (*ret == ERR_OK)
	{
		tss = pSim->GetSimParameters();
		//read configurations from the task file
		Simulator::GetSimStruct(taskConfig, tss);
		*ret = taskConfig->getErrorCode();
	}
	if (*ret == ERR_OK)
	{
		tss->mem = mem;
		tss->mainDataFolder = dataFolder;
		tss->pams.spaceFactor = taskConfig->getDouble(TP_SPACE_FACTOR, true);
		if (tss->pams.spaceFactor <= 1)
		{
			tss->pams.spaceFactor = 1;
		}
		else
		{
			tss->pams.eps *= tss->pams.spaceFactor;
			tss->pams.mu *= tss->pams.spaceFactor;
			tss->pams.rho *= tss->pams.spaceFactor;
			tss->pams.sie *= tss->pams.spaceFactor;
			tss->pams.sim *= tss->pams.spaceFactor;
		}
		//parameter validations
		if (tss->mem == NULL)
		{
			*ret = ERR_MEM_UNKNOWN;
		}
		else if (tss->pams.eps <= 0.0 || tss->pams.mu <= 0.0)
		{
			*ret = ERR_INVALID_EPS_MU;
		}
		else if (tss->pams.ds <= 0.0)
		{
			*ret = ERR_INVALID_SPACE_STEP;
		}
		else if (tss->pams.nx == 0 || tss->pams.ny == 0 || tss->pams.nz == 0)
		{
			*ret = ERR_INVALID_DOMAIN;
		}
		else if (tss->pams.smax == 0)
		{
			//*ret = ERR_INVALID_ESTIMATE_ORDER;
			//use default because Yee algorithm does not specify estimation order
			tss->pams.smax = 1;
		}
		else if (tss->pams.maxTimeSteps <= tss->pams.startTimeStep)
		{
			*ret = ERR_MAXTIME_START;
		}

		if (*ret == ERR_OK)
		{
			if (tss->pams.courantFactor != 0)
			{
				if (tss->pams.courantFactor <= 0 || tss->pams.courantFactor > 1)
				{
					*ret = ERR_INVALID_COUR_F;
				}
			}
			else
			{
				tss->pams.courantFactor = 1.0;
			}
		}
		if (*ret == ERR_OK)
		{
			double courant = 1.0 / sqrt(3.0);
			double dt = taskConfig->getDouble(TP_TSS_DT, true);
			if (dt != 0)
			{
				tss->pams.dt = dt;
				tss->pams.courantFactor = dt / ((tss->pams.ds * sqrt(tss->pams.eps*tss->pams.mu))*courant);
			}
			else
			{
				courant = courant * tss->pams.courantFactor;
				tss->pams.dt = (tss->pams.ds * sqrt(tss->pams.eps*tss->pams.mu)) * courant;
			}
			//parameters are valid. create plugins
			//else
			strName = taskConfig->getString(TP_TSS_SOURCE_CLASS, true);
			if (strName != NULL)
			{
				tss->src = CreateSourceInstance(strName);
				if (tss->src != NULL)
				{
					*ret = tss->src->initialize(taskConfig);
					if (*ret == ERR_OK)
					{
						*ret = tss->src->initialize(&(tss->pams));
					}
				}
				else
				{
					*ret = ERR_SOURCE_NAME;
				}
			}
			if (*ret == ERR_OK)
			{
				strName = taskConfig->getString(TP_TSS_BOUNDARY_CLASS, true);
				if (strName != NULL)
				{
					tss->boundaryCondition = CreateBoundaryInstance(strName);
					if (tss->boundaryCondition != NULL)
					{
						*ret = tss->boundaryCondition->initialize(taskConfig);
					}
					else
					{
						*ret = ERR_BOUNDARY_NAME;
					}
				}
				if (*ret == ERR_OK)
				{
					strName = taskConfig->getString(TP_TSS_FIELDSET_CLASS, true);
					if (strName != NULL)
					{
						tss->initFields = CreateFieldsSetterInstance(strName);
						if (tss->initFields != NULL)
						{
							*ret = tss->initFields->initializeByConfig(taskConfig);
						}
						else
						{
							*ret = ERR_FIELD0_NAME;
						}
					}
				}
				if (*ret == ERR_OK)
				{
					strName = taskConfig->getString(TP_TSS_STATISTIC_CLASS, true);
					if (strName != NULL)
					{
						tss->dvgs = CreateDivgStatistics(strName);
						if (tss->dvgs != NULL)
						{
							*ret = tss->dvgs->initializeByConfig(taskConfig);
						}
						else
						{
							*ret = ERR_STATISTIC_NAME;
						}
					}
					else
					{
						tss->dvgs = new DivergenceStatistic();
						*ret = tss->dvgs->initializeByConfig(taskConfig);
					}
				}
			}
		}
	}
	return pSim;
}

/*
	comparing data files in radius indexing to data files in cubic indexing.
	data files in two indexings should be identical. 
*/
int compareDataFilesOfCubicAndRadius(MemoryManager *mem, int fileIndex,
	TaskFile *taskConfig, Simulator *tssSim, 
	fnProgressReport reporter,
	Point3Dstruct *curl, Point3Dstruct * curlPrev)
{
	int ret = ERR_OK;
	MemoryManager *_mem = mem;
	TssSimStruct *simObj = tssSim->GetSimParameters();
	char msg[MESSAGELINESIZE];
	size_t sizeRadius;
	double R = taskConfig->getDouble(TP_FDTDR, false);// 1.0;
	unsigned int N = taskConfig->getUInt(TP_FDTDN, false);//10;
	unsigned int radius = 2 * N + 1;
	unsigned int NX = 4 * N + 2;
	RadiusCubic rc;
	FieldSource *fs = NULL;
	FieldsInitializer *fiv = NULL;
	FieldPoint3D *fRadius = NULL;
	Point3Dstruct *fCubic = NULL;
	char *rdF = taskConfig->getString(TEST_RADIUS_FILE, false);
	char *cbF = taskConfig->getString(TEST_CUBIC_FILE, false);
	char rdFilename[FILENAME_MAX];
	char cbFilename[FILENAME_MAX];
	//example: "C:\_data\RadiusRickerDEBUG\TssFDTD_N10_R1R0_T4S4_%u.em"
	ret = sprintf_1(rdFilename, FILENAME_MAX, rdF, fileIndex) <=0?ERR_STR_PRF:ERR_OK;
	if (ret == ERR_OK)
	{
		//example: "C:\\_data\\CubicRicker4DEBUG\\Ricker4_TSS_smax2_kmax1_e_%u.dat"
		ret = sprintf_1(cbFilename, FILENAME_MAX, cbF, fileIndex) <=0?ERR_STR_PRF:ERR_OK;
	}
	if (ret != ERR_OK)
	{
		return ret;
	}
	if (simObj->pams.nx != NX)
	{
		return ERR_INVALID_SIZE;
	}
	if (abs(simObj->pams.nx*simObj->pams.ds - 2.0*R) > 0.000000001)
	{
		return ERR_INVALID_SIZE;
	}
	if (tssSim->GetCellCount() != (NX + 1)*(NX + 1)*(NX + 1))
	{
		return ERR_INVALID_SIZE;
	}
	if (!fileexists(rdFilename))
	{
		return ERR_FILE_NAMING;
	}
	if (!fileexists(cbFilename))
	{
		return ERR_FILE_NAMING;
	}
	//===================================================================

	//read radius indexing file
	fRadius = (FieldPoint3D *)ReadFileIntoMemory(rdFilename, &sizeRadius, &ret);
	if (ret != ERR_OK)
	{
		FreeMemory(curlPrev); curlPrev = NULL;
		FreeMemory(curl); curl = NULL;
		return ret;
	}
	if (sizeRadius != 2 * tssSim->GetFieldMemorySize())
	{
		FreeMemory(curlPrev); curlPrev = NULL;
		FreeMemory(curl); curl = NULL;
		ret = ERR_INVALID_SIZE;
		return ret;
	}
	//read cubic indexing file
	fCubic = (Point3Dstruct *)ReadFileIntoMemory(cbFilename, &sizeRadius, &ret);
	if (ret != ERR_OK)
	{
		FreeMemory(curlPrev); curlPrev = NULL;
		FreeMemory(curl); curl = NULL;
		FreeMemory(fRadius);
		return ret;
	}
	if (sizeRadius != tssSim->GetFieldMemorySize())
	{
		ret = ERR_INVALID_SIZE;
		FreeMemory(curlPrev); curlPrev = NULL;
		FreeMemory(curl); curl = NULL;
		FreeMemory(fRadius);
		FreeMemory(fCubic);
		return ret;
	}
	if (fileIndex == 0)
	{
		//field source used by radius indexing, example:"FieldSourceEz"
		fs = (FieldSource *)CreateRadiusSourceInstance(taskConfig->getString(TP_SIMFS_NAME, true));
		if (fs == NULL)
		{
			FreeMemory(fCubic);
			FreeMemory(fRadius);
			FreeMemory(curlPrev); curlPrev = NULL;
			FreeMemory(curl); curl = NULL;
			ret = ERR_TASK_PARAMETER_NAME;
			return ret;
		}
		fs->SetMemoryManager(_mem);
		ret = fs->initialize(simObj->pams.courantFactor / sqrt(3.0), simObj->pams.dt, simObj->pams.ds, radius, taskConfig);
		if (ret != ERR_OK)
		{
			FreeMemory(fCubic);
			FreeMemory(fRadius);
			FreeMemory(curlPrev); curlPrev = NULL;
			FreeMemory(curl); curl = NULL;
			return ret;
		}
		//initializer used by radius indexing, example:"ZeroFields"
		fiv = CreateIVInstance(taskConfig->getString(TP_SIMIV_NAME, false));
		if (fiv == NULL)
		{
			FreeMemory(fCubic);
			FreeMemory(fRadius);
			FreeMemory(curlPrev); curlPrev = NULL;
			FreeMemory(curl); curl = NULL;
			ret = ERR_TASK_PARAMETER_NAME;
			return ret;
		}
		ret = fiv->initialize(taskConfig);
		if (ret != ERR_OK)
		{
			return ret;
		}
	}
	//convert radius indexing file fRadius into cunic indexing files curl and curlPrev
	rc.SetFields(curl, curlPrev, fRadius, fiv, fs, simObj->pams.ds);
	ret = rc.gothroughSphere(radius);
	if (ret != ERR_OK)
	{
		FreeMemory(curlPrev); curlPrev = NULL;
		FreeMemory(curl); curl = NULL;
		FreeMemory(fCubic);
		FreeMemory(fRadius);
		return ret;
	}
	if (fileIndex == 0)
	{
		sprintf_1(msg, MESSAGELINESIZE, "Maximum error comparing saved radius indexing file with its initialier and source (auto loop): %g ", rc.MaxError());
		reporter(msg, false);
		//comparing converted/unconverted radius indexing file with field initializer
		Point3Dstruct rd,rdH;
		double x, y, z, err = 0, maxErr = 0, x2, y2, z2, errCN = 0, maxErrCN = 0;
		int m, n, p;
		int i, j, k;
		size_t radiusStart, indexAtRadius, indexCubic;
		rdH.x = rdH.y = rdH.z = 0;
		radiusStart = 0;
		for (unsigned int r = 0; r <= radius; r++)
		{
			size_t pCount = pointsAt(r);
			for (size_t a = 0; a < pCount; a++)
			{
				ret = IndexToIndexes(r, a, &m, &n, &p);
				if (ret != ERR_OK) break;
				indexAtRadius = radiusStart + a; //index into radius memory
				//in cubic indexing =====================================
				i = m + radius;
				j = n + radius;
				k = p + radius;
				x = simObj->pams.xmin + ((double)i)*simObj->pams.ds;
				y = simObj->pams.ymin + ((double)j)*simObj->pams.ds;
				z = simObj->pams.zmin + ((double)k)*simObj->pams.ds;
				//comparing unconverted radius indexing file with the initializer
				//get data from the initializer
				rd.x = fiv->funcE0x(x, y, z);
				rd.y = fiv->funcE0y(x, y, z);
				rd.z = fiv->funcE0z(x, y, z);
				//apply field source
				fs->applySourceAtPoint(&rd, &rdH, 0, m, n, p);
				if (r < radius) //exclude boundary conditions
				{
					err = abs(rd.x - fRadius[indexAtRadius].E.x);
					if (err > maxErr) maxErr = err;
					err = abs(rd.y - fRadius[indexAtRadius].E.y);
					if (err > maxErr) maxErr = err;
					err = abs(rd.z - fRadius[indexAtRadius].E.z);
					if (err > maxErr) maxErr = err;
				}
				//in radius indexing==================================
				x2 = ((double)m)*simObj->pams.ds;
				y2 = ((double)n)*simObj->pams.ds;
				z2 = ((double)p)*simObj->pams.ds;
				//get data from the initializer
				rd.x = fiv->funcE0x(x2, y2, z2);
				rd.y = fiv->funcE0y(x2, y2, z2);
				rd.z = fiv->funcE0z(x2, y2, z2);
				//apply the field source
				fs->applySourceAtPoint(&rd, &rdH, 0, m, n, p);
				if (r < radius) //exclude boundary conditions
				{
					//comparing unconverted radius indexing file with the initializer
					err = abs(rd.x - fRadius[indexAtRadius].E.x);
					if (err > maxErr) maxErr = err;
					err = abs(rd.y - fRadius[indexAtRadius].E.y);
					if (err > maxErr) maxErr = err;
					err = abs(rd.z - fRadius[indexAtRadius].E.z);
					if (err > maxErr) maxErr = err;
				}
				//comparing converted with uncnverted files, including the boundary conditions and source
				//fCubic is read from a cubic indexing file
				//fRadius is read from a radius indexing file
				indexCubic = Idx(i, j, k);
				errCN = abs(fCubic[indexCubic].x - fRadius[indexAtRadius].E.x);
				if (errCN > maxErrCN)
					maxErrCN = errCN;
				errCN = abs(fCubic[indexCubic].y - fRadius[indexAtRadius].E.y);
				if (errCN > maxErrCN)
					maxErrCN = errCN;
				errCN = abs(fCubic[indexCubic].z - fRadius[indexAtRadius].E.z);
				if (errCN > maxErrCN)
					maxErrCN = errCN;
				//
				if (maxErr > 1.0)
				{
					err = err;
				}
			}
			radiusStart += pCount;
		}
		FreeMemory(fRadius); fRadius = NULL;
		FreeMemory(fCubic); fCubic = NULL;
		if (ret != ERR_OK) return ret;
		//
		sprintf_1(msg, MESSAGELINESIZE, "Maximum error comparing radius indexing file with initializer and source (manual loop): %g ", maxErr);
		reporter(msg, false);
		//
		sprintf_1(msg, MESSAGELINESIZE, "Maximum error comparing cubic indexing file with radius indexing file: %g ", maxErrCN);
		reporter(msg, false);
		//
		sprintf_1(msg, MESSAGELINESIZE, "comparing cubic indexing initializer with converted radius indexing file, including boundary and source: ...");
		reporter(msg, false);
	}
	else
	{
		//copy cubic indexing file contents to fieldE for comparing
		Point3Dstruct *fieldE = tssSim->GetFieldE();
		for (size_t w = 0; w < tssSim->GetCellCount(); w++)
		{
			fieldE[w].x = fCubic[w].x;
			fieldE[w].y = fCubic[w].y;
			fieldE[w].z = fCubic[w].z;
		}
	}
	if (fCubic != NULL)
	{
		FreeMemory(fCubic); fCubic = NULL;
	}
	if (fRadius != NULL)
	{
		FreeMemory(fRadius); fRadius = NULL;
	}
	return ret;
}
// 
/*
	test space module, called by task 301
	test case specified by SIM.TESTCASE
	Test cases:
	-1000: generate space estimation matrixes and write to a file
	-1999: execute following test cases and write results to a text file: {0, -1, -2, -3, -4, -5, -6, -100, 1, 2, 3, 101, 102, 103}
	-130: compare a field initializer for cubic indexing with a field initializer for radius indexing
	-200 to -400: compare radius index simulation data files with cubic index simulation data files; time index = test case + 200
	the following test cases are for cubic indexing
	-110: compare data file at time 0 with field values from the field initializer
	-120: from data file at time 0, calculate dFx/dx, dFy/dy and dFz/dz and compare them with the values from the field initializer
	-100: compare calculations of dFx/dx, dFy/dy and dFz/dz with the values provided by the field initializer
	-6: calculate d^2Fx/dx^2, d^2Fy/dy^2 and d^2Fz/dz^2 and compare them with values from the field initializer
	-26: compare the calculations of d^2Fx/dx^2, d^2Fy/dy^2 and d^2Fz/dz^2 using optimized and not-optimized coding
	-5: calculate d^2Fx/dy^2, d^2Fy/dz^2 and d^2Fz/dx^2 and compare them with values from the field initializer
	-25: compare the calculations of d^2Fx/dy^2, d^2Fy/dz^2 and d^2Fz/dx^2 using optimized and not-optimized coding
	-4: calculate d^2Fx/dz^2, d^2Fy/dx^2 and d^2Fz/dy^2 and compare them with values from the field initializer
	-24: compare the calculations of d^2Fx/dz^2, d^2Fy/dx^2 and d^2Fz/dy^2 using optimized and not-optimized coding
	-3: calculate dFx/dz, dFy/dx and dFz/dy and compare them with values from the field initializer
	-23: compare the calculations of dFx/dz, dFy/dx and dFz/dy using optimized and not-optimized coding
	-2: calculate dFx/dy, dFy/dz and dFz/dx and compare them with values from the field initializer
	-22: compare the calculations of dFx/dy, dFy/dz and dFz/dx using optimized and not-optimized coding
	-1: calculate dFx/dx, dFy/dy and dFz/dz and compare them with values from the field initializer
	-21: compare the calculations of dFx/dx, dFy/dy and dFz/dz using optimized and not-optimized coding
	0: compare field values from the field initializer and from the simulator initialized by the same field initializer
	1: comapre the first curl calculated by the simulator using first order code and by the field inializer
	101: comapre the first curl calculated by the simulator using higher order code and by the field inializer
	201: comapre the first curl calculated by the simulator with optimized code and by the field inializer
	301: comapre the first curl calculated directly by optimized code and not-optimized code
	401: comapre the first curl calculated by the simulator and by optimized code
	2: comapre the second curl calculated by the simulator and by the field inializer
	102: comapre the second curl calculated by the simulator using higher order code and by the field inializer
	302: comapre the second curl calculated by optimized code and not-optimized code
	402: comapre the second curl calculated by the simulator and by optimized code
	502: comapre the second curl calculated by the optimized code and by the field inializer 
	3: comapre the third curl calculated by the simulator and by the field inializer
	103: comapre the third curl calculated by the simulator using higher order code and by the field inializer
	4: comapre the 4th curl calculated by the simulator and by the field inializer
	5: comapre the 5th curl calculated by the simulator and by the field inializer
	6: comapre the 6th curl calculated by the simulator and by the field inializer
	7: comapre the 7th curl calculated by the simulator and by the field inializer
	*/
int testSpaceModule(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret;
	MemoryManager *_mem = mem;
	size_t startTime, endTime, timeUsed;
	char msg[MESSAGELINESIZE];
	Simulator *tssSim = createSimulator(mem, taskConfig, dataFolder, &ret);
	if (ret != ERR_OK)
	{
		return ret;
	}
	TssSimStruct *simObj = tssSim->GetSimParameters();
	simObj->reporter = reporter;
	if (simObj->src == NULL)
	{
		return ERR_SOURCE_NAME;
	}
	if (simObj->initFields == NULL)
	{
		return ERR_FIELD0_NAME;
	}
	//
	bool bExcludeBoundary = taskConfig->getBoolean(TEST_EXCLUDE_BOUNDARY, true); //exclude errors at boundaries
	int crNum = taskConfig->getInt(TP_TEST_CASE, false); //test case
	if (taskConfig->getErrorCode() != ERR_OK)
	{
		return ERR_TASK_PARAMETER_NAME;
	}
	sprintf_1(msg, MESSAGELINESIZE, "Test case : %d", crNum);
	reporter(msg, false);
	//
	FieldsSetter *fsetter = simObj->initFields;
	FieldsTester *fieldsValue = NULL; //a tester provides more functions not needed for simulations
	fieldsValue = dynamic_cast<FieldsTester *>(fsetter);
	if (fieldsValue == NULL)
	{
		return ERR_SETTER_NOTTEST;
	}

	startTime = getTimeTick();
	reporter("Initializing simulator ...", false);
	ret = tssSim->initializeSimulator(taskConfig->getString(TP_TSS_TIME_CLASS, true));
	if (ret == ERR_OK)
	{
		endTime = getTimeTick();
		timeUsed = endTime - startTime;
#ifdef __linux__
		snprintf(msg, MESSAGELINESIZE, "Initializing simulator...done. time used:%Ilu", timeUsed);
#elif _WIN32
		sprintf_s(msg, MESSAGELINESIZE, "Initializing simulator...done. time used:%Iu", timeUsed);
#else
#endif
		
		reporter(msg, true);
		//
		//initialization is done, ready to execute testing cases
		if (crNum == -1000)
		{
			//write space matrix to file
			char spaceMatrixFile[FILENAME_MAX];
			ret = Simulator::FormSpaceMatrixFilename(spaceMatrixFile, dataFolder, simObj->pams.smax, simObj->matrixFileFolder);
			if (ret == ERR_OK)
			{
				ret = tssSim->CreateMatrixFile(simObj->pams.smax, spaceMatrixFile);
			}
			return ret;
		}
		//==============================================
		char reportFile[FILENAME_MAX];
		FILE *reportHandle = 0;
		int *cases = NULL;
		int caseIndex = 0;
		int cases0[]{crNum, -2000};
		int cases1[]{0, -1, -2, -3, -4, -5, -6, -100, 1, 2, 3, 101, 201, 301, 401, 102, 103, -2000};
		nx1 = simObj->pams.nx + 1;
		ny1 = simObj->pams.ny + 1;
		nz1 = simObj->pams.nz + 1;
		if (crNum == -1999)
		{
			//execute multiple test cases
			char dPath[FILENAME_MAX];
			cases = cases1;
			ret = formFilePath(dPath, FILENAME_MAX, simObj->mainDataFolder, simObj->dataFileFolder);
			if (ret == ERR_OK)
			{
#ifdef __linux__
				snprintf(msg, MESSAGELINESIZE, "%s_smax%dkmax%d_%Ilu.txt", simObj->initFields->getClassName(), simObj->pams.smax, simObj->pams.kmax, getTimeTick());
#elif _WIN32
				sprintf_s(msg, MESSAGELINESIZE, "%s_smax%dkmax%d_%Iu.txt", simObj->initFields->getClassName(), simObj->pams.smax, simObj->pams.kmax, getTimeTick());
#else
#endif
				
				ret = formFilePath(reportFile, FILENAME_MAX, dPath, msg);
				if (ret == ERR_OK)
				{
					ret = openTextfileWrite(reportFile, &reportHandle);
					if (ret == ERR_OK)
					{
						sprintf_1(msg, MESSAGELINESIZE, "smax=%u kmax=%u, dt=%g, exclude boundary:%d, initial fields:%s\n", simObj->pams.smax, simObj->pams.kmax, simObj->pams.ds, bExcludeBoundary, simObj->initFields->getClassName());
						ret = writefile(reportHandle, msg, (unsigned int)strnlen_0(msg, MESSAGELINESIZE));
					}
				}
			}
		}
		else
			cases = cases0;
		//
		if (ret != ERR_OK)
		{
			return ret;
		}
		//execute test cases one by one
		while (cases[caseIndex] != -2000)
		{
			size_t w = 0, w2 = 0;
			FieldsInitializer *fiv = NULL;
			Point3Dstruct *fieldE = NULL;
			Point3Dstruct *fieldH = NULL;
			Point3Dstruct *curl = NULL;
			Point3Dstruct *curlPrev = NULL;
			Point3Dstruct cr;
			double err=0, errSum=0, errMax=0, mg=0, mgSum=0;
			double cx=0, cy=0, cz=0;
			double dsN = simObj->pams.ds;
			double x = simObj->pams.xmin, y = 0, z = 0;
			CURLMETHOD curlMethod = ByFirstOrder;
			CurlOne crE;
			CurlTwo crE2;
			//
			crE.setSpaceRange(simObj->pams.nx, simObj->pams.ny, simObj->pams.nz);
			crE2.setSpaceRange(simObj->pams.nx, simObj->pams.ny, simObj->pams.nz);
			//
			crNum = cases[caseIndex];
			//
			ret = tssSim->initializeSimulator(taskConfig->getString(TP_TSS_TIME_CLASS, true));
			if (ret != ERR_OK)
			{
				break;
			}
			fieldE = tssSim->GetFieldE();
			//
			sprintf_1(msg, MESSAGELINESIZE, "starting test case %d ...", crNum);
			reporter(msg, false);
			//
			startTime = getTimeTick();
			//
			dsN = 1.0;
			if (crNum <= 0)
			{
				//curl = tssSim->GetFieldH();
				if (crNum >=-400 && crNum <= -200) 
				{
					//these test cases compare cubic index data with radius index data
					//they should match if both simulations are correct
					int fileIndex = -crNum - 200; //file index for the data files to be compared
					if (fileIndex == 0)
					{
						//generate cubic indexing fields with source and boundary conditions =================
						fieldE = tssSim->GetFieldE();
						fieldH = tssSim->GetFieldH();
						//apply source first
						if (simObj->src != NULL)
						{
							ret = simObj->src->applySources(0.0, 0, fieldE, fieldH);
							if (ret != ERR_OK)
								return ret;
						}
						//apply boundary conditions next
						if (simObj->boundaryCondition != NULL)
						{
							ret = simObj->boundaryCondition->apply(fieldE, fieldH);
							if (ret != ERR_OK)
								return ret;
						}
					}
					//use curl and curlPrev as memory for reading radius indexing file
					curl = (Point3Dstruct *)AllocateMemory(tssSim->GetFieldMemorySize());
					if (curl == NULL)
					{
						ret = ERR_OUTOFMEMORY;
						return ret;
					}
					curlPrev = (Point3Dstruct *)AllocateMemory(tssSim->GetFieldMemorySize());
					if (curlPrev == NULL)
					{
						FreeMemory(curl); curl = NULL;
						ret = ERR_OUTOFMEMORY;
						return ret;
					}
					ret = compareDataFilesOfCubicAndRadius(mem, fileIndex, taskConfig,tssSim,reporter,curl,curlPrev);
					if (ret != ERR_OK)
					{
						FreeMemory(curl); curl = NULL;
						FreeMemory(curlPrev); curlPrev = NULL;
						return ret;
					}
					//now we will compare fieldE with curl; 
					//fieldE holds field E from initializer, source and boundary
					//curl holds field E converted from the radius file
				}
				else if (crNum == -130)
				{
					fiv = CreateIVInstance("GaussianFields");
					if (fiv == NULL)
					{
						ret = ERR_INVALID_PARAM; break;
					}
					fiv->initialize(taskConfig);
				}
				else if (crNum == -110 || crNum == -120)
				{
					char fn[FILENAME_MAX];
					//wchar_t fw[FILENAME_MAX];
					size_t esize;
					ret = tssSim->GetTime0FilenameE(fn);
					if (ret == ERR_OK)
					{
						//ret = copyC2W(fw, FILENAME_MAX, fn);
						//if (ret == ERR_OK)
						//{
							curlPrev = (Point3Dstruct *)ReadFileIntoMemory(fn, &esize, &ret);
							if (esize != (simObj->pams.nx + 1)*(simObj->pams.ny + 1)*(simObj->pams.nz + 1)*sizeof(Point3Dstruct))
							{
								ret = ERR_INVALID_SIZE;
							}
						//}
					}
				}
				else
				{
					if (crNum == -1 || crNum == -2 || crNum == -3)
					{
						dsN = simObj->pams.ds;
					}
					else if (crNum == -4 || crNum == -5 || crNum == -6)
					{
						dsN = simObj->pams.ds * simObj->pams.ds;
					}
					//if (crNum == 0 || crNum == -21 || crNum == -22 || crNum == -23 || crNum == -24 || crNum == -25 || crNum == -26)
					//{
						curl = (Point3Dstruct *)AllocateMemory(tssSim->GetFieldMemorySize());
						if (curl == NULL)
						{
							ret = ERR_OUTOFMEMORY;
							break;
						}
					//}
				}
			}
			else if (crNum > 0) //calculate curl
			{
				if (crNum < 300)
				{
					if (crNum > 200)
					{
						crNum = crNum - 200;
						curlMethod = ByFastMethod;
						tssSim->setCalculationMethod(curlMethod);
					}
					else if (crNum > 100)
					{
						crNum = crNum - 100;
						curlMethod = ByHigherOrder;
						tssSim->setCalculationMethod(curlMethod);
					}
				}
				tssSim->ResetFields();
				dsN = 1.0;
				if (crNum < 8)
				{
					for (int h = 1; h <= crNum; h++)
					{
						if (h == 1)
						{
							ret = tssSim->GetFirstCurls();
						}
						else
						{
							ret = tssSim->GetNextCurls();
						}
						if (ret != ERR_OK)
						{
							return ret;
						}
						curl = tssSim->GetCurrentCurlE();
						dsN = dsN * simObj->pams.ds;
					}
				}
				else if (crNum == 301 || crNum == 302 || crNum == 502)
				{
					dsN = simObj->pams.ds;
					if (crNum == 302 || crNum == 502)
					{
						dsN = simObj->pams.ds * simObj->pams.ds;
					}
					curl = (Point3Dstruct *)AllocateMemory(tssSim->GetFieldMemorySize());
					if (curl == NULL)
					{
						ret = ERR_OUTOFMEMORY;
						break;
					}
					if (crNum == 502)
					{
						tssSim->ResetFields();
						tssSim->setCalculationMethod(ByFastMethod);
						ret = tssSim->GetFirstCurls();
						if (ret != ERR_OK)
						{
							FreeMemory(curl); curl = NULL;
							break;
						}
						ret = tssSim->GetNextCurls();
						if (ret != ERR_OK)
						{
							FreeMemory(curl); curl = NULL;
							break;
						}
						tssSim->copyField(tssSim->GetCurrentCurlE(), curl);
					}
				}
				else if (crNum == 401 || crNum == 402)
				{
					curl = (Point3Dstruct *)AllocateMemory(tssSim->GetFieldMemorySize());
					if (curl == NULL)
					{
						ret = ERR_OUTOFMEMORY;
						break;
					}
					curlPrev = (Point3Dstruct *)AllocateMemory(tssSim->GetFieldMemorySize());
					if (curlPrev == NULL)
					{
						FreeMemory(curl); curl = NULL;
						ret = ERR_OUTOFMEMORY;
						break;
					}
					tssSim->ResetFields();
					tssSim->setCalculationMethod(ByFirstOrder);
					ret = tssSim->GetFirstCurls();
					if (ret != ERR_OK)
					{
						FreeMemory(curl); curl = NULL;
						FreeMemory(curlPrev); curlPrev = NULL;
						break;
					}
					if (crNum == 402)
					{
						ret = tssSim->GetNextCurls();
						if (ret != ERR_OK)
						{
							FreeMemory(curl); curl = NULL;
							FreeMemory(curlPrev); curlPrev = NULL;
							break;
						}
					}
					tssSim->copyField(tssSim->GetCurrentCurlE(), curl);
					//
					tssSim->ResetFields();
					tssSim->setCalculationMethod(ByFastMethod);
					ret = tssSim->GetFirstCurls();
					if (ret != ERR_OK)
					{
						FreeMemory(curl); curl = NULL;
						FreeMemory(curlPrev); curlPrev = NULL;
						break;
					}
					if (crNum == 402)
					{
						ret = tssSim->GetNextCurls();
						if (ret != ERR_OK)
						{
							FreeMemory(curl); curl = NULL;
							FreeMemory(curlPrev); curlPrev = NULL;
							break;
						}
					}
					tssSim->copyField(tssSim->GetCurrentCurlE(), curlPrev);
				}
			}
			if (ret == ERR_OK)
			{
				errSum = 0.0;
				errMax = 0.0;
				mgSum = 0.0;
				x = simObj->pams.xmin;
				w2 = 0;
				for (unsigned int i = 0; i <= simObj->pams.nx; i++)
				{
					y = simObj->pams.ymin;
					for (unsigned int j = 0; j <= simObj->pams.ny; j++)
					{
						z = simObj->pams.zmin;
						for (unsigned int k = 0; k <= simObj->pams.nz; k++)
						{
							w = Idx(i,j,k);
							if (w != w2)
							{
								w = w;
								throw;
							}
							if (crNum == -130)
							{
								//fieldsValue: used by cubic indexing
								//fiv: used by radius indexing
								fieldsValue->SetFields(x, y, z, &(curl[w]), &(cr));
								cr.x = fiv->funcE0x(x, y, z);
								cr.y = fiv->funcE0y(x, y, z);
								cr.z = fiv->funcE0z(x, y, z);
							}
							else if (crNum <= -200 && crNum >= -400)
							{
								//curl is from saved data file in radiux indexing at the same time step, converted to cubic indexing
								//fieldE is from the cubic indexing simulator
								cr.x = fieldE[w].x;
								cr.y = fieldE[w].y;
								cr.z = fieldE[w].z;
							}
							else if (crNum == -120)
							{
								//curlPrev is from saved data file at time=0
								curl[w].x = tssSim->dx_Fx(curlPrev, 1, i, j, k);
								curl[w].y = tssSim->dy_Fy(curlPrev, 1, i, j, k);
								curl[w].z = tssSim->dz_Fz(curlPrev, 1, i, j, k);
								//
								cr.x = fieldsValue->fnDxEx(x, y, z);
								cr.y = fieldsValue->fnDyEy(x, y, z);
								cr.z = fieldsValue->fnDzEz(x, y, z);
								err = abs(curl[w].x + curl[w].y + curl[w].z) + abs(cr.x + cr.y + cr.z);
							}
							else if (crNum == -110)
							{
								fieldsValue->SetFields(x, y, z, &(curl[w]), &(cr));
								//curlPrev is from saved data file at time=0
								cr.x = curlPrev[w].x;
								cr.y = curlPrev[w].y;
								cr.z = curlPrev[w].z;
							}
							else if (crNum == -100)
							{
								curl[w].x = tssSim->dx_Fx(fieldE, 1, i, j, k);
								curl[w].y = tssSim->dy_Fy(fieldE, 1, i, j, k);
								curl[w].z = tssSim->dz_Fz(fieldE, 1, i, j, k);
								cr.x = fieldsValue->fnDxEx(x, y, z);
								cr.y = fieldsValue->fnDyEy(x, y, z);
								cr.z = fieldsValue->fnDzEz(x, y, z);
								err = abs(curl[w].x + curl[w].y + curl[w].z) + abs(cr.x + cr.y + cr.z);
							}
							else if (crNum == -7)
							{
								curl[w].x = tssSim->dx_Fx(fieldE, 3, i, j, k);
								curl[w].y = tssSim->dy_Fy(fieldE, 3, i, j, k);
								curl[w].z = tssSim->dz_Fz(fieldE, 3, i, j, k);
								cr.x = 0;
								cr.y = 0;
								cr.z = 0;
							}
							else if (crNum == -6 || crNum == -26)
							{
								if (crNum == -6)
								{
									curl[w].x = tssSim->dx_Fx(fieldE, 2, i, j, k) / dsN;
									curl[w].y = tssSim->dy_Fy(fieldE, 2, i, j, k) / dsN;
									curl[w].z = tssSim->dz_Fz(fieldE, 2, i, j, k) / dsN;
									cr.x = fieldsValue->fnD2xEx(x, y, z);
									cr.y = fieldsValue->fnD2yEy(x, y, z);
									cr.z = fieldsValue->fnD2zEz(x, y, z);
								}
								else
								{
									curl[w].x = tssSim->dx_Fx(fieldE, 2, i, j, k);
									curl[w].y = tssSim->dy_Fy(fieldE, 2, i, j, k);
									curl[w].z = tssSim->dz_Fz(fieldE, 2, i, j, k);
									////
									cr.x = crE2.Dx2Vx(fieldE, i, j, k) / 12;
									cr.y = crE2.Dy2Vy(fieldE, i, j, k) / 12;
									cr.z = crE2.Dz2Vz(fieldE, i, j, k) / 12;
								}
							}
							else if (crNum == -5 || crNum == -25)
							{
								if (crNum == -5)
								{
									curl[w].x = tssSim->dy_Fx(fieldE, 2, i, j, k) / dsN;
									curl[w].y = tssSim->dz_Fy(fieldE, 2, i, j, k) / dsN;
									curl[w].z = tssSim->dx_Fz(fieldE, 2, i, j, k) / dsN;
									cr.x = fieldsValue->fnD2yEx(x, y, z);
									cr.y = fieldsValue->fnD2zEy(x, y, z);
									cr.z = fieldsValue->fnD2xEz(x, y, z);
								}
								else
								{
									curl[w].x = tssSim->dy_Fx(fieldE, 2, i, j, k);
									curl[w].y = tssSim->dz_Fy(fieldE, 2, i, j, k);
									curl[w].z = tssSim->dx_Fz(fieldE, 2, i, j, k);
									////
									cr.x = crE2.Dy2Vx(fieldE, i, j, k) / 12;
									cr.y = crE2.Dz2Vy(fieldE, i, j, k) / 12;
									cr.z = crE2.Dx2Vz(fieldE, i, j, k) / 12;
								}
							}
							else if (crNum == -4 || crNum == -24) //2nd order
							{
								if (crNum == -4)
								{
									curl[w].x = tssSim->dz_Fx(fieldE, 2, i, j, k) / dsN;
									curl[w].y = tssSim->dx_Fy(fieldE, 2, i, j, k) / dsN;
									curl[w].z = tssSim->dy_Fz(fieldE, 2, i, j, k) / dsN;
									cr.x = fieldsValue->fnD2zEx(x, y, z);
									cr.y = fieldsValue->fnD2xEy(x, y, z);
									cr.z = fieldsValue->fnD2yEz(x, y, z);
								}
								else
								{
									curl[w].x = tssSim->dz_Fx(fieldE, 2, i, j, k);
									curl[w].y = tssSim->dx_Fy(fieldE, 2, i, j, k);
									curl[w].z = tssSim->dy_Fz(fieldE, 2, i, j, k);
									////
									cr.x = crE2.Dz2Vx(fieldE, i, j, k) / 12;
									cr.y = crE2.Dx2Vy(fieldE, i, j, k) / 12;
									cr.z = crE2.Dy2Vz(fieldE, i, j, k) / 12;
								}
							}
							else if (crNum == -3 || crNum == -23)
							{
								if (crNum == -3)
								{
									curl[w].x = tssSim->dz_Fx(fieldE, 1, i, j, k) / dsN;
									curl[w].y = tssSim->dx_Fy(fieldE, 1, i, j, k) / dsN;
									curl[w].z = tssSim->dy_Fz(fieldE, 1, i, j, k) / dsN;
									cr.x = fieldsValue->fnDzEx(x, y, z);
									cr.y = fieldsValue->fnDxEy(x, y, z);
									cr.z = fieldsValue->fnDyEz(x, y, z);
								}
								else
								{
									curl[w].x = tssSim->dz_Fx(fieldE, 1, i, j, k);
									curl[w].y = tssSim->dx_Fy(fieldE, 1, i, j, k);
									curl[w].z = tssSim->dy_Fz(fieldE, 1, i, j, k);
									//
									cr.x = crE.DzVx(fieldE, i, j, k) / 12;
									cr.y = crE.DxVy(fieldE, i, j, k) / 12;
									cr.z = crE.DyVz(fieldE, i, j, k) / 12;
								}
							}
							else if (crNum == -2 || crNum == -22)
							{
								if (crNum == -2)
								{
									curl[w].x = tssSim->dy_Fx(fieldE, 1, i, j, k) / dsN;
									curl[w].y = tssSim->dz_Fy(fieldE, 1, i, j, k) / dsN;
									curl[w].z = tssSim->dx_Fz(fieldE, 1, i, j, k) / dsN; 
									cr.x = fieldsValue->fnDyEx(x, y, z);
									cr.y = fieldsValue->fnDzEy(x, y, z);
									cr.z = fieldsValue->fnDxEz(x, y, z);
								}
								else
								{
									curl[w].x = tssSim->dy_Fx(fieldE, 1, i, j, k);
									curl[w].y = tssSim->dz_Fy(fieldE, 1, i, j, k);
									curl[w].z = tssSim->dx_Fz(fieldE, 1, i, j, k);
									//
									cr.x = crE.DyVx(fieldE, i, j, k)/12;
									cr.y = crE.DzVy(fieldE, i, j, k)/12;
									cr.z = crE.DxVz(fieldE, i, j, k)/12;
								}
							}
							else if (crNum == -1 || crNum == -21)
							{
								if (crNum == -1)
								{
									curl[w].x = tssSim->dx_Fx(fieldE, 1, i, j, k) / dsN;
									curl[w].y = tssSim->dy_Fy(fieldE, 1, i, j, k) / dsN;
									curl[w].z = tssSim->dz_Fz(fieldE, 1, i, j, k) / dsN;
									cr.x = fieldsValue->fnDxEx(x, y, z);
									cr.y = fieldsValue->fnDyEy(x, y, z);
									cr.z = fieldsValue->fnDzEz(x, y, z);
								}
								else
								{
									curl[w].x = tssSim->dx_Fx(fieldE, 1, i, j, k);
									curl[w].y = tssSim->dy_Fy(fieldE, 1, i, j, k);
									curl[w].z = tssSim->dz_Fz(fieldE, 1, i, j, k);
									//
									cr.x = crE.DxVx(fieldE, i, j, k) / 12;
									cr.y = crE.DyVy(fieldE, i, j, k) / 12;
									cr.z = crE.DzVz(fieldE, i, j, k) / 12;
								}
							}
							else if (crNum == 0)
							{
								fieldsValue->SetFields(x, y, z, &(curl[w]), &(cr));
								cr.x = fieldE[w].x;
								cr.y = fieldE[w].y;
								cr.z = fieldE[w].z;
							}
							else if (crNum == 1 || crNum == 301 || crNum == 401)
							{
								if (crNum == 1)
								{
									fieldsValue->curl1(x, y, z, &cr);
								}
								else if (crNum == 301)
								{
									cr.x = tssSim->dy_Fz(fieldE, 1, i, j, k) - tssSim->dz_Fy(fieldE, 1, i, j, k);
									cr.y = tssSim->dz_Fx(fieldE, 1, i, j, k) - tssSim->dx_Fz(fieldE, 1, i, j, k);
									cr.z = tssSim->dx_Fy(fieldE, 1, i, j, k) - tssSim->dy_Fx(fieldE, 1, i, j, k);
									curl[w].x = (crE.DyVz(fieldE, i, j, k) - crE.DzVy(fieldE, i, j, k)) / 12;
									curl[w].y = (crE.DzVx(fieldE, i, j, k) - crE.DxVz(fieldE, i, j, k)) / 12;
									curl[w].z = (crE.DxVy(fieldE, i, j, k) - crE.DyVx(fieldE, i, j, k)) / 12;
								}
								else //401
								{
									cr.x = curlPrev[w].x;
									cr.y = curlPrev[w].y;
									cr.z = curlPrev[w].z;
								}
							}
							else if (crNum == 2 || crNum == 302 || crNum == 402 || crNum == 502)
							{
								if (crNum == 2 || crNum == 502)
								{
									fieldsValue->curl2(x, y, z, &cr);
								}
								else if (crNum == 302)
								{
									cr.x = tssSim->dx_Fx(fieldE, 2, i, j, k) + tssSim->dy_Fx(fieldE, 2, i, j, k) + tssSim->dz_Fx(fieldE, 2, i, j, k);
									cr.y = tssSim->dx_Fy(fieldE, 2, i, j, k) + tssSim->dy_Fy(fieldE, 2, i, j, k) + tssSim->dz_Fy(fieldE, 2, i, j, k);
									cr.z = tssSim->dx_Fz(fieldE, 2, i, j, k) + tssSim->dy_Fz(fieldE, 2, i, j, k) + tssSim->dz_Fz(fieldE, 2, i, j, k);
									curl[w].x = (crE2.Dx2Vx(fieldE, i, j, k) + crE2.Dy2Vx(fieldE, i, j, k) + crE2.Dz2Vx(fieldE, i, j, k)) / 12;
									curl[w].y = (crE2.Dx2Vy(fieldE, i, j, k) + crE2.Dy2Vy(fieldE, i, j, k) + crE2.Dz2Vy(fieldE, i, j, k)) / 12;
									curl[w].z = (crE2.Dx2Vz(fieldE, i, j, k) + crE2.Dy2Vz(fieldE, i, j, k) + crE2.Dz2Vz(fieldE, i, j, k)) / 12;
								}
								else //402
								{
									cr.x = curlPrev[w].x;
									cr.y = curlPrev[w].y;
									cr.z = curlPrev[w].z;
								}
							}
							else if (crNum == 3)
							{
								fieldsValue->curl3(x, y, z, &cr);
							}
							else if (crNum == 4)
							{
								fieldsValue->curl4(x, y, z, &cr);
							}
							else if (crNum == 5)
							{
								fieldsValue->curl5(x, y, z, &cr);
							}
							else if (crNum == 6)
							{
								fieldsValue->curl6(x, y, z, &cr);
							}
							else if (crNum == 7)
							{
								fieldsValue->curl7(x, y, z, &cr);
							}
							else
							{
								err = 100.0;
							}
							if (crNum != -100 && crNum != -120)
							{
								err = 0.0;
								if (!bExcludeBoundary || (i > 0 && j > 0 && k > 0 && i < simObj->pams.nx && j < simObj->pams.ny && k < simObj->pams.nz))
								{
									cx = curl[w].x; cy = curl[w].y; cz = curl[w].z;
									if ((crNum > 0 && crNum < 8) || crNum == 502)
									{
										cx = cx / dsN; cy = cy / dsN; cz = cz / dsN;
									}
									err += abs(cr.x - cx);
									err += abs(cr.y - cy);
									err += abs(cr.z - cz);
									if (err > 0.2)
									{
										cx = cx;
									}
								}
							}
							//cr holds exact values; curl[w] holds estimated values
							mg = sqrt(cr.x*cr.x + cr.y*cr.y + cr.z*cr.z);
							if (mg > 0.00001)
							{
								err = err / mg;
							}
							if (err > errMax)
							{
								errMax = err;
								if (errMax > 0.2)
								{
									err = err;
								}
							}
							errSum += err;
							mgSum += mg;
							z += simObj->pams.ds;
							w2++;
						}
						y += simObj->pams.ds;
					}
					x += simObj->pams.ds;
				}
				errSum = errSum / (double)w2;
				mgSum = mgSum / (double)w2;
				//
				endTime = getTimeTick();
				timeUsed = endTime - startTime;
#ifdef __linux__
				snprintf(msg, MESSAGELINESIZE, "test case %d done. time used:%Ilu",crNum, timeUsed);
#elif _WIN32
				sprintf_s(msg, MESSAGELINESIZE, "test case %d done. time used:%Iu", crNum, timeUsed);
#else
#endif
				
				reporter(msg, false);
				//
				sprintf_1(msg, MESSAGELINESIZE, "max error:%g, average error:%g, space order:%u, average mag:%g", errMax, errSum, 2 * simObj->pams.smax, mgSum);
				reporter(msg, false);
				//
				if (crNum == -110 || crNum == -120 || (crNum <= -200 && crNum >= -400))
				{
					if (curlPrev != NULL)
					{
						FreeMemory(curlPrev); curlPrev = NULL;
					}
					if (curl != NULL)
					{
						FreeMemory(curl); curl = NULL;
					}
				}
			}
			if (crNum == -110 || crNum == -120 || crNum == -200)
			{
				if (curlPrev != NULL)
				{
					FreeMemory(curlPrev); curlPrev = NULL;
				}
			}
			if (crNum == 301 || crNum == 401 || crNum == 302 || crNum == 402 || crNum == 502)
			{
				if (curl != NULL)
				{
					FreeMemory(curl); curl = NULL;
				}
				if (curlPrev != NULL)
				{
					FreeMemory(curlPrev); curlPrev = NULL;
				}
			}
			//if (crNum == 0 || crNum == -21 || crNum == -22 || crNum == -23 || crNum == -24 || crNum == -25 || crNum == -26)
			//{
				if (curl != NULL)
				{
					FreeMemory(curl); curl = NULL;
				}
			//}
			if (ret != ERR_OK) break;
			//write test results
			if (reportHandle != 0)
			{
				if (crNum == 0)
					sprintf_1(msg, MESSAGELINESIZE, "\nTest case 0. Field initialization");
				else if (crNum == 1)
				{
					if (curlMethod == ByHigherOrder)
					{
						sprintf_1(msg, MESSAGELINESIZE, "\nTest case 101. Curl estimation (by higher order");
					}
					else if (curlMethod == ByFastMethod)
					{
						sprintf_1(msg, MESSAGELINESIZE, "\nTest case 201. Curl estimation (by fast method)");
					}
					else
						sprintf_1(msg, MESSAGELINESIZE, "\nTest case 1. Curl estimation");
				}
				else if (crNum == 301)
				{
					sprintf_1(msg, MESSAGELINESIZE, "\nTest case 301. comapre the first curl calculated directly by optimized code and not-optimized code");
				}
				else if (crNum == 401)
				{
					sprintf_1(msg, MESSAGELINESIZE, "\nTest case 401. comapre the first curl calculated by the simulator and by optimized code");
				}
				else if (crNum == 2)
				{
					if (curlMethod == ByHigherOrder)
					{
						sprintf_1(msg, MESSAGELINESIZE, "\nTest case 102. Curl(Curl) estimation (by higher order)");
					}
					else if (curlMethod == ByFastMethod)
					{
						sprintf_1(msg, MESSAGELINESIZE, "\nTest case 202. Curl(Curl) estimation (by fast method)");
					}
					else
						sprintf_1(msg, MESSAGELINESIZE, "\nTest case 2. Curl(Curl) estimation");
				}
				else if (crNum == 3)
				{
					if (curlMethod == ByHigherOrder)
					{
						sprintf_1(msg, MESSAGELINESIZE, "\nTest case 103. Curl(Curl(Curl)) estimation (by higher order)");
					}
					else if (curlMethod == ByFastMethod)
					{
						sprintf_1(msg, MESSAGELINESIZE, "\nTest case 203. Curl(Curl(Curl)) estimation (by fast method)");
					}
					else
						sprintf_1(msg, MESSAGELINESIZE, "\nTest case 3. Curl(Curl(Curl)) estimation");
				}
				else if (crNum == -1)
					sprintf_1(msg, MESSAGELINESIZE, "\nTest case -1. estimations of dEx/dx, dEy/dy, dEz/dz");
				else if (crNum == -2)
					sprintf_1(msg, MESSAGELINESIZE, "\nTest case -2. estimations of dEx/dy, dEy/dz, dEz/dx");
				else if (crNum == -3)
					sprintf_1(msg, MESSAGELINESIZE, "\nTest case -3. estimations of dEx/dz, dEy/dx, dEz/dy");
				else if (crNum == -4)
					sprintf_1(msg, MESSAGELINESIZE, "\nTest case -4. estimations of d2Ex/dz, d2Ey/dx, d2Ez/dy");
				else if (crNum == -5)
					sprintf_1(msg, MESSAGELINESIZE, "\nTest case -5. estimations of d2Ex/dy, d2Ey/dz, d2Ez/dx");
				else if (crNum == -6)
					sprintf_1(msg, MESSAGELINESIZE, "\nTest case -6. estimations of d2Ex/dx, d2Ey/dy, d2Ez/dz");
				else if (crNum == -100)
					sprintf_1(msg, MESSAGELINESIZE, "\nTest case -100. estimations of divergences");
				ret = writefile(reportHandle, msg, (unsigned int)strnlen_0(msg, MESSAGELINESIZE));
				if (ret == ERR_OK)
				{
					sprintf_1(msg, MESSAGELINESIZE, "\nmax error:%g, average error:%g, average mag:%g\n", errMax, errSum, mgSum);
					ret = writefile(reportHandle, msg, (unsigned int)strnlen_0(msg, MESSAGELINESIZE));
				}
			}
			if (ret != ERR_OK)
			{
				break;
			}
			caseIndex++;
		}//cases
		if (reportHandle != 0)
		{
			closefile(reportHandle);
		}
	}
	return ret;
}

/*
	
*/
double getRelativeError(double v1, double v2)
{
	double err = abs(v1 - v2);
	if (err > 0)
	{
		err = err / sqrt(v1*v1 + v2*v2);
	}
	return err;
}

int testTimeModule(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	using namespace boost::multiprecision;
	int ret = ERR_OK;
	size_t startTime, endTime, timeUsed;
	char msg[MESSAGELINESIZE];
	Simulator *tssSim = createSimulator(mem, taskConfig, dataFolder, &ret);
	if (ret != ERR_OK)
	{
		return ret;
	}
	TssSimStruct *simObj = tssSim->GetSimParameters();
	simObj->reporter = reporter;
	if (simObj->src == NULL)
	{
		return ERR_SOURCE_NAME;
	}
	if (simObj->initFields == NULL)
	{
		return ERR_FIELD0_NAME;
	}
	startTime = getTimeTick();
	reporter("Initializing simulator ...", false);
	ret = tssSim->initializeSimulator(taskConfig->getString(TP_TSS_TIME_CLASS, true));
	if (ret == ERR_OK)
	{
		endTime = getTimeTick();
		timeUsed = endTime - startTime;
#ifdef __linux__
		snprintf(msg, MESSAGELINESIZE, "Initializing simulator...done. time used:%Ilu", timeUsed);
#elif _WIN32
		sprintf_s(msg, MESSAGELINESIZE, "Initializing simulator...done. time used:%Iu", timeUsed);
#else
#endif
		
		reporter(msg, true);
	}
	else
	{
		return ret;
	}
	//
	//cpp_dec_float_100 c0 = 299792458.0; //speed of light
	cpp_dec_float_100 mu0 = simObj->pams.mu;//4.0 * M_PI * 1.0e-7;
	cpp_dec_float_100 mu2 = mu0 * mu0;
	cpp_dec_float_100 mu3 = mu2 * mu0;
	//cpp_dec_float_100 mu4 = mu3 * mu0;
	cpp_dec_float_100 eps0 = simObj->pams.eps;//1.0 / (mu0 * c0 * c0);
	cpp_dec_float_100 eps2 = eps0 * eps0;
	cpp_dec_float_100 eps3 = eps2 * eps0;
	//cpp_dec_float_100 eps4 = eps3 * eps0;
	cpp_dec_float_100 eu = eps0 * mu0;
	//cpp_dec_float_100 eu2 = eu * eu;
	
	//cpp_dec_float_100 pl = simObj->pams.rho; //0.1;  // ρ
	cpp_dec_float_100 de = simObj->pams.sie; //0.2;  // σ
	cpp_dec_float_100 dm = simObj->pams.sim; //0.3;  // σ_m
	cpp_dec_float_100 dm2 = dm * dm;
	cpp_dec_float_100 dm3 = dm2 * dm;
	//cpp_dec_float_100 dm4 = dm3 * dm;
	//
	cpp_dec_float_100 de2 = de * de;
	cpp_dec_float_100 de3 = de2 * de;
	//cpp_dec_float_100 de4 = de3 * de;
	//cpp_dec_float_100 courant = 1.0 / sqrt(3.0);
	cpp_dec_float_100 ds = simObj->pams.ds;
	cpp_dec_float_100 ds2 = ds * ds;
	cpp_dec_float_100 ds3 = ds2 * ds;
	//cpp_dec_float_100 ds4 = ds3 * ds;
	cpp_dec_float_100 dt = simObj->pams.dt;//(ds / c0) * courant;
	cpp_dec_float_100 dt2 = dt*dt;
	cpp_dec_float_100 dt3 = dt2*dt;
	//cpp_dec_float_100 dt4 = dt3*dt;
	//
	unsigned int kmax = simObj->pams.kmax;
	//
	double err = 0, errMax = 0;
	cpp_dec_float_100 dts = dt / ds;
	//cpp_dec_float_100 pp = 1.0 / (eps0*mu0);
	//
	//cpp_dec_float_100 q_0u1 = -1.0 / mu0;
	//cpp_dec_float_100 q_1u2 = de * pp + dm / (mu0*mu0);
	//cpp_dec_float_100 q_0u2 = -pp;
	//
	if (ret == 0)
	{
		unsigned int srcDim = tssSim->GetsrcDim();
		size_t srcDim2 = srcDim * srcDim;
		//
		double *G_ehP = tssSim->Get_eh();
		double *G_hhP = tssSim->Get_hh();
		double *G_eeP = tssSim->Get_ee();
		double *G_heP = tssSim->Get_he();
		//
		//H = C.F.dJm + C.G.dJe + ...
		//E = C.U.dJe + C.W.dJm + ...
		double *f = tssSim->GetJmH();
		double *g = tssSim->GetJeH();
		double *u = tssSim->GetJeE();
		double *w = tssSim->GetJmE();
		//
		cpp_dec_float_100 *G_ehP0 = (cpp_dec_float_100*)malloc((kmax + 1)*sizeof(cpp_dec_float_100));
		cpp_dec_float_100 *G_hhP0 = (cpp_dec_float_100*)malloc((kmax + 1)*sizeof(cpp_dec_float_100));
		cpp_dec_float_100 *G_eeP0 = (cpp_dec_float_100*)malloc((kmax + 1)*sizeof(cpp_dec_float_100));
		cpp_dec_float_100 *G_heP0 = (cpp_dec_float_100*)malloc((kmax + 1)*sizeof(cpp_dec_float_100));
		//
		cpp_dec_float_100 *f0 = (cpp_dec_float_100*)malloc(srcDim2*sizeof(cpp_dec_float_100));
		cpp_dec_float_100 *g0 = (cpp_dec_float_100*)malloc(srcDim2*sizeof(cpp_dec_float_100));
		cpp_dec_float_100 *w0 = (cpp_dec_float_100*)malloc(srcDim2*sizeof(cpp_dec_float_100));
		cpp_dec_float_100 *u0 = (cpp_dec_float_100*)malloc(srcDim2*sizeof(cpp_dec_float_100));
		//
		if (G_ehP0 != NULL && G_hhP0 != NULL && G_eeP0 != NULL && G_heP0 != NULL && f0 != NULL && g0 != NULL && w0 != NULL && u0 != NULL)
		{
			for (unsigned int i = 0; i < srcDim2; i++)
			{
				f0[i] = g0[i] = w0[i] = u0[i] = 0.0;
			}
			//
			for (unsigned int k = 0; k <= kmax; k++)
			{
				G_ehP0[k] = G_hhP0[k] = G_eeP0[k] = G_heP0[k] = 0.0;
			}
			errMax = 0;
			if (kmax == 0)
			{
				/*
				G_h^h (0)=1-∆_t  σ_m/μ,
				G_e^e (0)=1-∆_t  σ/ε,
				G_h^e (0)=∆_t/ε  1/∆_s,
				G_e^h (0)=-∆_t/μ  1/∆_s
				F=[-∆_t/μ],G=[0],U=[-∆_t/ε],W[0]
				*/
				G_hhP0[0] = 1.0 - dt * dm / mu0;
				G_eeP0[0] = 1.0 - dt * de / eps0;
				G_heP0[0] = dts / eps0;
				G_ehP0[0] = -dts / mu0;
				f0[Sr_ih(0, 0)] = -dt / mu0;  g0[0] = 0.0;
				u0[Sr_ih(0, 0)] = -dt / eps0; w0[0] = 0.0;
				//
			}
			else if (kmax == 1)
			{
				/*
				G_e^e [0]=1-∆_t  σ/ε+1/2 ∆_t^2  σ^2/ε^2 -1/6 ∆_t^3  σ^3/ε^3
				G_e^e [1]=-  1/2 ∆_t^2  1/εμ+1/6 ∆_t^3 (σ_m/(εμ^2 )+2 σ/(με^2 )  )
				G_h^e [0]=∆_t/ε-1/2 ∆_t^2 (σ_m/εμ+σ/ε^2   )+1/6 ∆_t^3 ((σ_m^2)/(εμ^2 )+(σ_m σ)/(με^2 )+σ^2/ε^3 )
				G_h^e [1]=-1/6 ∆_t^3  1/(ε^2 μ)
				G_e^e [0]=1-∆_t  σ/ε+1/2 ∆_t^2  σ^2/ε^2 -1/6 ∆_t^3  σ^3/ε^3
				G_e^e [1]=-  1/2 ∆_t^2  1/εμ+1/6 ∆_t^3 (σ_m/(εμ^2 )+2 σ/(με^2 )  )
				G_h^e [0]=∆_t/ε-1/2 ∆_t^2 (σ_m/εμ+σ/ε^2   )+1/6 ∆_t^3 ((σ_m^2)/(εμ^2 )+(σ_m σ)/(με^2 )+σ^2/ε^3 )
				G_h^e [1]=-1/6 ∆_t^3  1/(ε^2 μ)
				*/
				G_eeP0[0] = 1.0 - dt *  de / eps0 + 0.5* dt2 * de2 / eps2 - (1.0 / 6.0)*  dt3 * de3 / eps3;
				G_eeP0[1] = -0.5*dt2 /eu + (1.0/6.0)*dt3*(dm/(eps0*mu2) + 2.0*de/(mu0*eps2));
				G_eeP0[1] = G_eeP0[1] / ds2;
				G_heP0[0] = dt / eps0 - 0.5 * dt2 * (dm / eu + de / eps2) + (1.0 / 6.0)*dt3*(dm2/(eps0*mu2)+dm*de/(mu0*eps2)+de2/eps3);
				G_heP0[0] = G_heP0[0] / ds;
				G_heP0[1] = -(1.0 / 6.0) * dt3 / (eps2*mu0);
				G_heP0[1] = G_heP0[1] / ds3;
				//
				/*
				G_h^h [0]=1-∆_t  σ_m/μ+1/2 ∆_t^2  (σ_m^2)/μ^2 -1/6 ∆_t^3  (σ_m^3)/μ^3
				G_h^h [1]=-1/2 ∆_t^2  1/με  +1/6 ∆_t^3 (σ/(με^2 )+2 σ_m/(μ^2 ε))
				G_e^h [0]=-∆_t/μ+1/2 ∆_t^2 (σ/με+σ_m/μ^2 )-1/6 ∆_t^3 (σ^2/(με^2 )+(σσ_m)/(μ^2 ε)+(σ_m^2)/μ^3 )
				G_e^h [1]=1/6 ∆_t^3  1/(μ^2 ε)
				*/
				G_hhP0[0] = 1.0 - dt * dm / mu0 + 0.5*dt2*dm2 / mu2 - (1.0 / 6.0)*dt3*dm3 / mu3;
				G_hhP0[1] = -0.5*dt2 / eu + (1.0 / 6.0)*dt3*(de / (mu0*eps2) + 2.0*dm / (mu2*eps0));
				G_hhP0[1] = G_hhP0[1] / ds2;
				G_ehP0[0] = -dt / mu0 + 0.5*dt2*(de / eu + dm / mu2) - (1.0 / 6.0)*dt3*(de2 / (mu0*eps2) + de*dm / (mu2*eps0) + dm2 / mu3);
				G_ehP0[0] = G_ehP0[0] / ds;
				G_ehP0[1] = (1.0 / 6.0)*dt3 / (mu2*eps0);
				G_ehP0[1] = G_ehP0[1] / ds3;
				////////////////////////////////////////////////////////////////////
				/*
				F[0,0]=-∆_t/μ+1/2 ∆_t^2  σ_m/μ^2 -1/6 ∆_t^3  (σ_m^2)/μ^3 
				F[0,1]=-1/2 ∆_t^2  1/μ+1/6 ∆_t^3  σ_m/μ^2 
				F[0,2]=-1/6 ∆_t^3  1/μ
				F[2,0]=1/6 ∆_t^3  1/(μ^2 ε)
				G[1,0]=1/2 ∆_t^2  1/με-1/6 ∆_t^3 (σ/(ε^2 μ)+σ_m/(μ^2 ε))
				G[1,1]=1/6 ∆_t^3  1/με
				*/
				f0[Sr_ih(0, 0)] = -dt / mu0 + 0.5*dt2*dm / mu2 - (1.0 / 6.0)*dt3*dm2 / mu3;
				f0[Sr_ih(1, 0)] = -0.5 * dt2 / mu0 + (1.0 / 6.0) * dt3 * dm / mu2;
				f0[Sr_ih(2, 0)] = -(1.0 / 6.0) * dt3 / mu0;
				f0[Sr_ih(0, 2)] = (1.0 / 6.0) * dt3 / (mu2 * eps0);
				g0[Sr_ih(0, 1)] = 0.5 * dt2 / eu - (1.0 / 6.0) * dt3 *(de / (eps2*mu0) + dm / (mu2*eps0));
				g0[Sr_ih(1, 1)] = (1.0 / 6.0) * dt3 / eu;
				//
				/*
				U[0,0]=-∆_t/ε+1/2 ∆_t^2  σ/ε^2 -1/6 ∆_t^3  σ^2/ε^3 
				U[0,1]=-1/2 ∆_t^2  1/ε+1/6 ∆_t^3  σ/ε^2 
				U[0,2]=-1/6 ∆_t^3  1/ε
				U[2,0]=1/6 ∆_t^3  1/(ε^2 μ)
				W[1,0]=-1/2 ∆_t^2  1/εμ+1/6 ∆_t^3 (σ_m/(εμ^2 )+σ/(με^2 )  )
				W[1,1]=-1/6 ∆_t^3  1/εμ
				*/
				u0[Sr_ih(0, 0)] = -dt / eps0 + 0.5*dt2*de / eps2 - (1.0 / 6.0)*dt3*de2 / eps3;
				u0[Sr_ih(1, 0)] = -0.5 * dt2 / eps0 + (1.0 / 6.0) * dt3 * de / eps2;
				u0[Sr_ih(2, 0)] = -(1.0 / 6.0) * dt3 / eps0;
				u0[Sr_ih(0, 2)] = (1.0 / 6.0) * dt3 / (eps2 * mu0);
				w0[Sr_ih(0, 1)] = -0.5 * dt2 / eu + (1.0 / 6.0) * dt3 *(dm / (eps0*mu2) + de / (mu0*eps2));
				w0[Sr_ih(1, 1)] = -(1.0 / 6.0) * dt3 / eu;
			}
			//check calculations
			for (unsigned int i = 0; i < srcDim2; i++)
			{
				err = getRelativeError(f[i], f0[i].convert_to<double>());
				if (err > errMax)
				{
					errMax = err;
				}
				err = getRelativeError(g[i], g0[i].convert_to<double>());
				if (err > errMax)
				{
					errMax = err;
				}
				err = getRelativeError(w[i], w0[i].convert_to<double>());
				if (err > errMax)
				{
					errMax = err;
				}
				err = getRelativeError(u[i], u0[i].convert_to<double>());
				if (err > errMax)
				{
					errMax = err;
				}
			}
			for (unsigned int k = 0; k <= kmax; k++)
			{
				err = getRelativeError(G_ehP0[k].convert_to<double>(), G_ehP[k]);
				if (err > errMax)
				{
					errMax = err;
				}
				err = getRelativeError(G_hhP0[k].convert_to<double>(), G_hhP[k]);
				if (err > errMax)
				{
					errMax = err;
				}
				err = getRelativeError(G_eeP0[k].convert_to<double>(), G_eeP[k]);
				if (err > errMax)
				{
					errMax = err;
				}
				err = getRelativeError(G_heP0[k].convert_to<double>(), G_heP[k]);
				if (err > errMax)
				{
					errMax = err;
				}
			}
			char msg[MESSAGELINESIZE];
			sprintf_1(msg, MESSAGELINESIZE, "kmax=%u. Max error:%g", simObj->pams.kmax, errMax);
			reporter(msg, false);
		}
		else
		{
			ret = ERR_OUTOFMEMORY;
		}
		if (G_ehP0 != NULL) { free(G_ehP0); }
		if (G_hhP0 != NULL) { free(G_hhP0); }
		if (G_eeP0 != NULL) { free(G_eeP0); }
		if (G_heP0 != NULL) { free(G_heP0); }
		//
		if (f0 != NULL) { free(f0); }
		if (g0 != NULL) { free(g0); }
		if (w0 != NULL) { free(w0); }
		if (u0 != NULL) { free(u0); }

		
	}
	return ret;
}
Simulator *prepareSimulator(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter, fnOperationCanceld cancelReport, int *pRet)
{
	*pRet = ERR_OK;
	TssSimStruct *simObj;
	Simulator *tssSim = createSimulator(mem, taskConfig, dataFolder, pRet);
	if (*pRet == ERR_OK)
	{
		simObj = tssSim->GetSimParameters();
		if (simObj->src == NULL)
		{
			*pRet = ERR_SOURCE_NAME;
		}
		else if (simObj->initFields == NULL)
		{
			*pRet = ERR_FIELD0_NAME;
		}

		if (*pRet == ERR_OK)
		{
			simObj->reporter = reporter;
			simObj->canceled = cancelReport;
			//parameter validations
			if (simObj->mem == NULL)
			{
				*pRet = ERR_MEM_UNKNOWN;
				if (reporter != NULL)
				{
					reporter("missing memory manager", false);
				}
			}
		}
		if (*pRet == ERR_OK)
		{
			if (simObj->dataFileFolder == NULL)
			{
				*pRet = ERR_INVALID_PARAMS;
			}
			else
			{
				char dataPath[FILENAME_MAX];
				*pRet = formFilePath(dataPath, FILENAME_MAX, dataFolder, simObj->dataFileFolder);
				if (*pRet == ERR_OK)
				{
					if (!directoryExists(dataPath))
					{
						if (!createdirectory(dataPath))
						{
							*pRet = ERR_INVALID_DATA_FOLDER;
						}
					}
				}
				if (*pRet == ERR_OK)
				{
					char taskfilecopy[FILENAME_MAX];
					*pRet = formFilePath(taskfilecopy, FILENAME_MAX, dataPath, "taskfile.task");
					if (*pRet == ERR_OK)
					{
						*pRet = taskConfig->saveToFile(taskfilecopy);
					}
				}
			}
			if (*pRet != ERR_OK)
			{
				if (reporter != NULL)
				{
					reporter(" missing or invalid data folder name", false);
				}
			}
		}
		//parameters OK
		if (*pRet == ERR_OK)
		{
			char *strName = taskConfig->getString(TP_TSS_BOUNDARY_CLASS, true);
			if (strName != NULL && simObj->boundaryCondition == NULL)
			{
				*pRet = ERR_BOUNDARY_NAME;
			}
			if (*pRet == ERR_OK)
			{
				//initialize simulation
				*pRet = tssSim->initializeSimulator(taskConfig->getString(TP_TSS_TIME_CLASS, true));
			}
		}
	}
	return tssSim;
}
int prepareSimulation(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter, fnOperationCanceld cancelReport)
{
	int ret = ERR_OK;
	prepareSimulator(mem, taskConfig, dataFolder, reporter, cancelReport, &ret);
	return ret;
}
/*
run EM simulation in rectangle space indexing
*/
int tssSimulation(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter, fnOperationCanceld cancelReport)
{
	int ret = ERR_OK;
	Simulator *tssSim = prepareSimulator(mem, taskConfig, dataFolder, reporter, cancelReport, &ret);
	if (ret == ERR_OK)
	{
		ret = tssSim->run();
	}
	return ret;
}

/*
	create statistics file for radius indexing data files, for comparison purpose
*/
int createStatisticsForRadiusDataFiles(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret = ERR_OK;
	MemoryManager *_mem = mem;
	FILE *fhStatistic = 0;
	char msg[MESSAGELINESIZE];
	//wchar_t baseFilenameW[FILENAME_MAX];
	Simulator *tssSim = createSimulator(mem, taskConfig, dataFolder, &ret);
	if (ret != ERR_OK)
	{
		return ret;
	}
	ret = tssSim->initializeSimulator(taskConfig->getString(TP_TSS_TIME_CLASS, true));
	if (ret != ERR_OK)
	{
		return ret;
	}
	char *baseName = taskConfig->getString(TP_SIMBASENAME, false);
	if (baseName == NULL)
	{
		return ERR_FILENAME_MISSING;
	}
	TssSimStruct *simObj = tssSim->GetSimParameters();
	simObj->reporter = reporter;
	ret = formFilePath(msg, MESSAGELINESIZE, simObj->mainDataFolder, simObj->dataFileFolder);
	if (ret == ERR_OK)
	{
		char baseFilename[FILENAME_MAX];
		ret = formFilePath(baseFilename, FILENAME_MAX, msg, baseName);
		if (ret == ERR_OK)
		{
			//ret = copyC2W(baseFilenameW, FILENAME_MAX, baseFilename);
			//if (ret == ERR_OK)
			//{
			char filename[FILENAME_MAX];
			ret = sprintf_1(filename, FILENAME_MAX, "%sstatistics.txt", baseFilename) <= 0 ? ERR_STR_PRF : ERR_OK;
			if (ret == ERR_OK)
			{
				ret = openTextfileWrite(filename, &fhStatistic);
				if (ret == ERR_OK)
				{
					sprintf_1(msg, MESSAGELINESIZE, "Time\tEnergy\tMax Divg E\tAvg Divg E\tMax Divg H\tAvg Divg H");
					ret = writefile(fhStatistic, msg, (unsigned int)strnlen_0(msg, MESSAGELINESIZE));
				}
			}
			//}
		}
		if (ret != ERR_OK)
		{
			return ret;
		}
		int fileIndex = 0;
		while (ret == ERR_OK)
		{
			char dataFile[FILENAME_MAX];
			ret = sprintf_1(dataFile, FILENAME_MAX, "%s%d.em", baseFilename, fileIndex) <= 0 ? ERR_STR_PRF : ERR_OK;
			if (ret != ERR_OK)
			{
				break;
			}
			if (!fileexists(dataFile))
			{
				break;
			}
			RadiusCubic rc;
			unsigned int radius = 0;
			size_t fileSize;
			FieldPoint3D *fRadius = (FieldPoint3D *)ReadFileIntoMemory(dataFile, &fileSize, &ret);
			if (ret != ERR_OK) break;
			ret = MemorySizeToRadius(fileSize, &radius);
			if (ret != ERR_OK)
			{
				FreeMemory(fRadius); fRadius = NULL;
				break;
			}
			if (simObj->pams.nx != 2 * radius)
			{
				FreeMemory(fRadius); fRadius = NULL;
				ret = ERR_INVALID_SIZE;
				break;
			}
			Point3Dstruct *efile = (Point3Dstruct *)AllocateMemory(tssSim->GetFieldMemorySize());
			if (efile == NULL)
			{
				FreeMemory(fRadius); fRadius = NULL;
				ret = ERR_OUTOFMEMORY;
				break;
			}
			Point3Dstruct *hfile = (Point3Dstruct *)AllocateMemory(tssSim->GetFieldMemorySize());
			if (hfile == NULL)
			{
				FreeMemory(fRadius); fRadius = NULL;
				FreeMemory(efile); efile = NULL;
				ret = ERR_OUTOFMEMORY;
				break;
			}
			//
			rc.SetFields(efile, hfile, fRadius, NULL, NULL, simObj->pams.ds);
			ret = rc.gothroughSphere(radius);
			if (ret != ERR_OK)
			{
				FreeMemory(fRadius); fRadius = NULL;
				FreeMemory(efile); efile = NULL;
				FreeMemory(hfile); hfile = NULL;
				break;
			}
			ret = tssSim->calculateStatistics(efile, hfile, fileIndex, fhStatistic);
			//
			FreeMemory(fRadius); fRadius = NULL;
			FreeMemory(efile); efile = NULL;
			FreeMemory(hfile); hfile = NULL;
			//
			fileIndex++;
		}
		if (fhStatistic != 0)
		{
			closefile(fhStatistic);
		}
	}
	return ret;
}
//
/*
	make radius statistics for cubic-indexing data files
	make one statistics report file for every pair of E and H data file
*/
int createStatisticsInRadiusForCubicDataFiles(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret = ERR_OK;
	MemoryManager *_mem = mem;
	FILE *fhStatistic = 0;
	int fileIndex = 0;
	TssSimStruct *simObj;
	Simulator *tssSim = createSimulator(mem, taskConfig, dataFolder, &ret);
	if (ret != ERR_OK)
	{
		return ret;
	}
	simObj = tssSim->GetSimParameters();
	simObj->reporter = reporter;
	ret = tssSim->initializeSimulator(taskConfig->getString(TP_TSS_TIME_CLASS, true));
	if (ret != ERR_OK)
	{
		return ret;
	}
	while (ret == ERR_OK)
	{
		size_t fSize;
		Point3Dstruct *efile=NULL, *hfile=NULL;
		char filename[FILENAME_MAX];
		//wchar_t fW[FILENAME_MAX];
		//
		ret = tssSim->formDataFilename(filename, "e","dat", fileIndex);
		if (ret == ERR_OK)
		{
			if (!fileexists(filename)) break;
			//ret = copyC2W(fW,FILENAME_MAX, filename);
			//if (ret == ERR_OK)
			//{
				efile = (Point3Dstruct*)ReadFileIntoMemory(filename, &fSize, &ret);
				if (ret == ERR_OK)
				{
					if (fSize != tssSim->GetFieldMemorySize())
					{
						ret = ERR_INVALID_SIZE;
						if (efile != NULL)
						{
							FreeMemory(efile); efile = NULL;
						}
					}
				}
			//}
		}
		if (ret != ERR_OK) break;
		//
		ret = tssSim->formDataFilename(filename, "h","dat", fileIndex);
		if (ret == ERR_OK)
		{
			if (!fileexists(filename)) break;
			//ret = copyC2W(fW, FILENAME_MAX, filename);
			//if (ret == ERR_OK)
			//{
				hfile = (Point3Dstruct*)ReadFileIntoMemory(filename, &fSize, &ret);
				if (ret == ERR_OK)
				{
					if (fSize != tssSim->GetFieldMemorySize())
					{
						ret = ERR_INVALID_SIZE;
						if (efile != NULL)
						{
							FreeMemory(efile); efile = NULL;
						}
						if (hfile != NULL)
						{
							FreeMemory(hfile); hfile = NULL;
						}
					}
				}
			//}
		}
		if (ret != ERR_OK) break;
		//
		fhStatistic = 0;
		ret = tssSim->formDataFilename(filename, "st", "txt", fileIndex);
		if (ret != ERR_OK) break;
		ret = openTextfileWrite(filename, &fhStatistic);
		if (ret != ERR_OK) break;
		//
		unsigned int radius = 0;
		ret = MemorySizeToRadius(2 * fSize, &radius);
		if (ret == ERR_OK)
		{
			if (simObj->pams.nx != 2 * radius)
			{
				ret = ERR_INVALID_SIZE;
			}
		}
		if (ret != ERR_OK)
		{
			if (efile != NULL)
			{
				FreeMemory(efile); efile = NULL;
			}
			if (hfile != NULL)
			{
				FreeMemory(hfile); hfile = NULL;
			}
			break;
		}
		//
		//Size3Dstruct *ic = tssSim->GetIndexCache();
		RadiusStatistics rs;
		ret = rs.initialize(efile, hfile, radius, tssSim);
		if (ret == ERR_OK)
		{
			ret = rs.gothroughSphere(radius);
		}
		if (ret == ERR_OK)
		{
			rs.writeStatisticsToFile(fhStatistic);
		}
		//
		if (efile != NULL)
		{
			FreeMemory(efile); efile = NULL;
		}
		if (hfile != NULL)
		{
			FreeMemory(hfile); hfile = NULL;
		}
		if (fhStatistic != 0)
		{
			closefile(fhStatistic);
			fhStatistic = 0;
		}
		//
		printf(" %d", fileIndex);
		//
		fileIndex++;
	}
	if (fhStatistic != 0)
	{
		closefile(fhStatistic);
		fhStatistic = 0;
	}
	return ret;
}
//
int testHeilderCurrent(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret = ERR_OK;
	char msg[MESSAGELINESIZE];
	char *fileFolder = taskConfig->getString(TP_TSS_DATAFOLDER, false);
	double ds = taskConfig->getDouble(TP_TSS_DS, false);
	double dt = taskConfig->getDouble(TP_TSS_DT, false);
	unsigned int nz = taskConfig->getUInt(TP_TSS_NZ, false);
	unsigned int maxTime = taskConfig->getUInt(TP_TSS_MAXTIME, false);
	double factor = taskConfig->getDouble(TP_SPACE_FACTOR, false);
	ret = taskConfig->getErrorCode();
	if (ret == ERR_OK)
	{
		//form file name
		FILE *fhandle = 0;
		char filename[FILENAME_MAX];
		ret = formFilePath(msg, MESSAGELINESIZE, dataFolder, fileFolder);
		if (ret == ERR_OK)
		{
			if (!directoryExists(msg))
			{
				if (!createdirectory(msg))
				{
					ret = ERR_INVALID_DATA_FOLDER;
				}
			}
			char fn[FILENAME_MAX];
			char sDs[FILENAME_MAX];
			char sDt[FILENAME_MAX];
			ret = doubleToIntegersString(ds, sDs, FILENAME_MAX);
			if (ret == ERR_OK)
			{
				ret = doubleToIntegersString(dt, sDt, FILENAME_MAX);
			}
			if (ret == ERR_OK)
			{
#ifdef __linux__
				ret = snprintf(fn, FILENAME_MAX, "nz%umt%uds%s_dt%s_f%d_heilder_%Ilu.dat", nz, maxTime, sDs, sDt, (int)factor, getTimeTick()) <=0?ERR_STR_PRF:ERR_OK;
#elif _WIN32
				ret = sprintf_s(fn, FILENAME_MAX, "nz%umt%uds%s_dt%s_f%d_heilder_%Iu.dat", nz, maxTime, sDs, sDt, (int)factor, getTimeTick()) <=0?ERR_STR_PRF:ERR_OK;
#else
#endif
				

				
			}
			if (ret == ERR_OK)
			{
				ret = formFilePath(filename, FILENAME_MAX, msg, fn);
			}
		}
		if (ret == ERR_OK)
		{
			ret = openfileWrite(filename, &fhandle);
		}
		if (ret == ERR_OK)
		{
			HeidlerCurrent hc;
			size_t sz = (nz + 1)*(maxTime + 1)*sizeof(double);
			double *data = (double *)malloc(sz);
			if (data == NULL)
			{
				ret = ERR_OUTOFMEMORY;
			}
			else
			{
				ret = hc.initialize(taskConfig);
			}
			if (ret == ERR_OK)
			{
				double z, t;
				size_t w;
				//z: 0 ...ds*k...ds*nz
				//t: 0 ...dt*h...dt*maxTime
				z = 0.0; t = 0.0; w = 0;
				for (unsigned int k = 0; k <= nz; k++)
				{
					t = 0.0;
					for (unsigned int h = 0; h <= maxTime; h++)
					{
						hc.setTime(z, t);
						data[w] = hc.I();
						t += dt;
						w++;
					}
					z += ds;
				}
				ret = writefile(fhandle, data, (unsigned int)sz);
				free(data);
			}
			closefile(fhandle);
		}
	}
	return ret;
}

int testHeilderCurrentDT(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret = ERR_OK;
	Space s;
	Array3Dstruct *A = NULL;
	char msg[MESSAGELINESIZE];
	char *fileFolder = taskConfig->getString(TP_TSS_DATAFOLDER, false);
	double ds = taskConfig->getDouble(TP_TSS_DS, false);
	double dt = taskConfig->getDouble(TP_TSS_DT, false);
	unsigned int nz = taskConfig->getUInt(TP_TSS_NZ, false);
	unsigned int maxTime = taskConfig->getUInt(TP_TSS_MAXTIME, false);
	unsigned int smax = taskConfig->getUInt(TP_TSS_SMAX, false);
	char *matrixFolder = taskConfig->getString(TP_TSS_MATRIXFILEDIR, false);
	double factor = taskConfig->getDouble(TP_SPACE_FACTOR, false);
	unsigned int M = 2 * smax;
	ret = taskConfig->getErrorCode();
	if (ret == ERR_OK)
	{
		char spaceMatrixFile[FILENAME_MAX];
		ret = Simulator::FormSpaceMatrixFilename(spaceMatrixFile, dataFolder, smax, matrixFolder);
		if (ret == ERR_OK)
		{
			if (fileexists(spaceMatrixFile))
			{
				A = Space::LoadEstimationMatrixesFromFile(spaceMatrixFile, M, &ret);
			}
			else
			{
				ret = s.initializeSpace(0.1, 1, 1, 1, smax, spaceMatrixFile);
				if (ret == ERR_OK)
				{
					A = s.GetMaxtrixes();
				}
			}
		}
	}
	if (ret == ERR_OK)
	{
		//form file name
		FILE *fhandle = 0;
		char filename[FILENAME_MAX];
		ret = formFilePath(msg, MESSAGELINESIZE, dataFolder, fileFolder);
		if (ret == ERR_OK)
		{
			if (!directoryExists(msg))
			{
				if (!createdirectory(msg))
				{
					ret = ERR_INVALID_DATA_FOLDER;
				}
			}
			char fn[FILENAME_MAX];
			char sDs[FILENAME_MAX];
			char sDt[FILENAME_MAX];
			ret = doubleToIntegersString(ds, sDs, FILENAME_MAX);
			if (ret == ERR_OK)
			{
				ret = doubleToIntegersString(dt, sDt, FILENAME_MAX);
			}

			if (ret == ERR_OK)
			{
#ifdef __linux__
				ret = snprintf(fn, FILENAME_MAX, "DT_nz%umt%uds%s_dt%s_f%d_heilder_%Ilu.dat", nz, maxTime, sDs, sDt, (int)factor, getTimeTick()) <=0?ERR_STR_PRF:ERR_OK;
#elif _WIN32
				ret = sprintf_s(fn, FILENAME_MAX, "DT_nz%umt%uds%s_dt%s_f%d_heilder_%Iu.dat", nz, maxTime, sDs, sDt, (int)factor, getTimeTick()) <=0?ERR_STR_PRF:ERR_OK;
#else
#endif

				
			}
			if (ret==ERR_OK)
			{
				ret = formFilePath(filename, FILENAME_MAX, msg, fn);
			}
		}
		if (ret == ERR_OK)
		{
			ret = openfileWrite(filename, &fhandle);
		}
		if (ret == ERR_OK)
		{
			HeidlerCurrent hc;
			size_t sz = (nz + 1)*(maxTime + 1)*sizeof(double);
			double *data = (double *)malloc(sz);
			if (data == NULL)
			{
				ret = ERR_OUTOFMEMORY;
			}
			else
			{
				ret = hc.initialize(taskConfig);
			}
			if (ret == ERR_OK)
			{
				double z, t;
				size_t w;
				double err = 0, maxErr = 0, errSum = 0, maxErrRel=0, errSumRel=0;
				double dtEst = dt / 1000.0, t0, dI, v0;
				//z: 0 ...ds*k...ds*nz
				//t: 0 ...dt*h...dt*maxTime
				z = 0.0; t = 0.0; w = 0;
				for (unsigned int k = 0; k <= nz; k++)
				{
					t = 0.0; err = 0; maxErr = 0; errSum = 0; maxErrRel = 0; errSumRel = 0;
					for (unsigned int h = 0; h <= maxTime; h++)
					{
						hc.setTime(z, t);
						data[w] = hc.dI_dt();
						//use derivative estimation to verify the formula
						if (h > 0)
						{
							dI = 0.0; t0 = t;
							for (unsigned int m = 0; m < smax; m++)
							{
								t0 += dtEst;
								hc.setTime(z, t0);
								v0 = hc.I();
								dI += A->a[0].r[0].c[m] * (v0 - data[w]);
							}
							t0 = t;
							for (unsigned int m = 0; m < smax; m++)
							{
								t0 -= dtEst;
								hc.setTime(z, t0);
								v0 = hc.I();
								dI += A->a[0].r[0].c[m+smax] * (v0 - data[w]);
							}
							dI = dI / dtEst;
							//
							err = abs(dI - data[w]);
							if (err > maxErr)
							{
								maxErr = err;
							}
							errSum += err;
							//
							if (data[w] != 0)
							{
								err = err / abs(data[w]);
								if (err > maxErrRel)
								{
									maxErrRel = err;
								}
								errSumRel += err;
							}
						}
						//
						t += dt;
						w++;
					}
					sprintf_1(msg, MESSAGELINESIZE, "z=%g, max error = %g, aver. error = %g, max relative error:%g, aver. relative error:%g", z, maxErr, errSum / (double)maxTime, maxErrRel, errSumRel/(double)maxTime);
					reporter(msg, false);
					z += ds;
				}
				ret = writefile(fhandle, data, (unsigned int)sz);
				free(data);
			}
			closefile(fhandle);
		}
	}
	tss_free3Darray(A, M + 1, M);
	return ret;
}


int testHeilderCurrentDT2(TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret = ERR_OK;
	Space s;
	Array3Dstruct *A = NULL;
	char msg[MESSAGELINESIZE];
	char *fileFolder = taskConfig->getString(TP_TSS_DATAFOLDER, false);
	double ds = taskConfig->getDouble(TP_TSS_DS, false);
	double dt = taskConfig->getDouble(TP_TSS_DT, false);
	unsigned int nz = taskConfig->getUInt(TP_TSS_NZ, false);
	unsigned int maxTime = taskConfig->getUInt(TP_TSS_MAXTIME, false);
	unsigned int smax = taskConfig->getUInt(TP_TSS_SMAX, false);
	char *matrixFolder = taskConfig->getString(TP_TSS_MATRIXFILEDIR, false);
	double factor = taskConfig->getDouble(TP_SPACE_FACTOR, false);
	unsigned int M = 2 * smax;
	ret = taskConfig->getErrorCode();
	if (ret == ERR_OK)
	{
		char spaceMatrixFile[FILENAME_MAX];
		ret = Simulator::FormSpaceMatrixFilename(spaceMatrixFile, dataFolder, smax, matrixFolder);
		if (ret == ERR_OK)
		{
			if (fileexists(spaceMatrixFile))
			{
				A = Space::LoadEstimationMatrixesFromFile(spaceMatrixFile, M, &ret);
			}
			else
			{
				ret = s.initializeSpace(0.1, 1, 1, 1, smax, spaceMatrixFile);
				if (ret == ERR_OK)
				{
					A = s.GetMaxtrixes();
				}
			}
		}
	}
	if (ret == ERR_OK)
	{
		//form file name
		FILE *fhandle = 0;
		char filename[FILENAME_MAX];
		ret = formFilePath(msg, MESSAGELINESIZE, dataFolder, fileFolder);
		if (ret == ERR_OK)
		{
			if (!directoryExists(msg))
			{
				if (!createdirectory(msg))
				{
					ret = ERR_INVALID_DATA_FOLDER;
				}
			}
			char fn[FILENAME_MAX];
			
			char sDs[FILENAME_MAX];
			char sDt[FILENAME_MAX];
			ret = doubleToIntegersString(ds, sDs, FILENAME_MAX);
			if (ret == ERR_OK)
			{
				ret = doubleToIntegersString(dt, sDt, FILENAME_MAX);
			}
			if (ret == ERR_OK)
			{
#ifdef __linux__
				ret = snprintf(fn, FILENAME_MAX, "DT2_nz%umt%uds%s_dt%s_f%d_heilder_%Ilu.dat", nz, maxTime, sDs, sDt, (int)factor, getTimeTick()) <=0?ERR_STR_PRF:ERR_OK;
#elif _WIN32
				ret = sprintf_s(fn, FILENAME_MAX, "DT2_nz%umt%uds%s_dt%s_f%d_heilder_%Iu.dat", nz, maxTime, sDs, sDt, (int)factor, getTimeTick()) <=0?ERR_STR_PRF:ERR_OK;
#else
#endif

				
			}
			if (ret == ERR_OK)
			{
				ret = formFilePath(filename, FILENAME_MAX, msg, fn);
			}
		}
		if (ret == ERR_OK)
		{
			ret = openfileWrite(filename, &fhandle);
		}
		if (ret == ERR_OK)
		{
			HeidlerCurrent hc;
			size_t sz = (nz + 1)*(maxTime + 1)*sizeof(double);
			double *data = (double *)malloc(sz);
			if (data == NULL)
			{
				ret = ERR_OUTOFMEMORY;
			}
			else
			{
				ret = hc.initialize(taskConfig);
			}
			if (ret == ERR_OK)
			{
				double z, t;
				size_t w;
				double err = 0, maxErr = 0, errSum = 0, maxErrRel = 0, errSumRel = 0;
				double dtEst = dt / 10000.0, t0, dI, v0;
				//z: 0 ...ds*k...ds*nz
				//t: 0 ...dt*h...dt*maxTime
				z = 0.0; t = 0.0; w = 0;
				for (unsigned int k = 0; k <= nz; k++)
				{
					t = 0.0; err = 0; maxErr = 0; errSum = 0; maxErrRel = 0; errSumRel = 0;
					for (unsigned int h = 0; h <= maxTime; h++)
					{
						hc.setTime(z, t);
						data[w] = hc.d2I_dt2();
						//use derivative estimation to verify the formula
						if (h > 0)
						{
							dI = 0.0; t0 = t;
							for (unsigned int m = 0; m < smax; m++)
							{
								t0 += dtEst;
								hc.setTime(z, t0);
								v0 = hc.dI_dt();
								dI += A->a[0].r[0].c[m] * (v0 - data[w]);
							}
							t0 = t;
							for (unsigned int m = 0; m < smax; m++)
							{
								t0 -= dtEst;
								hc.setTime(z, t0);
								v0 = hc.dI_dt();
								dI += A->a[0].r[0].c[m + smax] * (v0 - data[w]);
							}
							dI = dI / dtEst;
							//
							err = abs(dI - data[w]);
							if (err > maxErr)
							{
								maxErr = err;
							}
							errSum += err;
							//
							if (data[w] != 0)
							{
								err = err / abs(data[w]);
								if (err > maxErrRel)
								{
									maxErrRel = err;
								}
								errSumRel += err;
							}
						}
						//
						t += dt;
						w++;
					}
					sprintf_1(msg, MESSAGELINESIZE, "z=%g, max err:%g, aver. err:%g, max relative err:%g, aver. relative err:%g", z, maxErr, errSum / (double)maxTime, maxErrRel, errSumRel / (double)maxTime);
					reporter(msg, false);
					z += ds;
				}
				ret = writefile(fhandle, data, (unsigned int)sz);
				free(data);
			}
			closefile(fhandle);
		}
	}
	tss_free3Darray(A, M + 1, M);
	return ret;
}

/*
	task 370
	generate a text file (or files) from all the data files. 
	the text file contains E or H fields at the given space point over time.
*/
int makePointTimeFile(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret = ERR_OK;
	unsigned int fileIndex = 0;
	size_t w;
	size_t tIndex;
	unsigned int i,   j,  k;
	char *eh;
	char filename[FILENAME_MAX];
	char textFilename[FILENAME_MAX];
	FILE *fhText = 0;
	MemoryManager *_mem = mem;
	TssSimStruct *simObj;
	Simulator *tssSim = createSimulator(mem, taskConfig, dataFolder, &ret);
	if (ret != ERR_OK)
	{
		return ret;
	}
	size_t size = 0;
	GridNode3Dstruct *ps = taskConfig->getGridNodes(POINT_GRIDNODES, false, &size);
	if (ps == NULL)
		return ERR_TASK_INVALID_VALUE;
	eh = taskConfig->getString(POINT_EH, false);
	ret = taskConfig->getErrorCode();
	if (ret != ERR_OK)
	{
		return ret;
	}
	simObj = tssSim->GetSimParameters();
	simObj->reporter = reporter;
	for (size_t h = 0; h < size; h++)
	{
		i = ps[h].i;
		j = ps[h].j;
		k = ps[h].k;
		w = k + (simObj->pams.nz + 1) * (j + (simObj->pams.ny + 1)*i);
		fileIndex = 0;
		fhText = 0;
		sprintf_1(filename, FILENAME_MAX, "%u_%u_%u", i, j, k);
		ret = tssSim->formDataFilename(textFilename, filename, "txt", fileIndex);
		if (ret != ERR_OK)
		{
			return ret;
		}
		ret = openTextfileWrite(textFilename, &fhText);
		if (ret != ERR_OK)
		{
			return ret;
		}
		while (fileIndex <= simObj->pams.maxTimeSteps)
		{
			printf("%d ", fileIndex);
			if (simObj->pams.saveInterval == 0)
			{
				tIndex = fileIndex;
			}
			else
			{
				tIndex = fileIndex * simObj->pams.saveInterval;
			}
			ret = tssSim->formDataFilename(filename, eh, "dat", fileIndex);
			if (ret != ERR_OK)
			{
				break;
			}
			if (fileexists(filename))
			{
				size_t fsize = 0;
				Point3Dstruct *field = (Point3Dstruct *)ReadFileIntoMemory(filename, &fsize, &ret);
				if (ret == ERR_OK)
				{
#ifdef __linux__
					snprintf(filename, FILENAME_MAX, "%Ilu\t%g\t%g\t%g\n", tIndex, field[w].x, field[w].y, field[w].z);
#elif _WIN32
					sprintf_s(filename, FILENAME_MAX, "%Iu\t%g\t%g\t%g\n", tIndex, field[w].x, field[w].y, field[w].z);
#else
#endif
					
					ret = writefile(fhText, filename, (unsigned int)strnlen_0(filename, FILENAME_MAX));
					if (ret != ERR_OK)
					{
						break;
					}
				}
				if (field != NULL)
				{
					FreeMemory(field);
				}
			}
			else
			{
				break;
			}
			fileIndex++;
		}
		if (fhText != 0)
		{
			closefile(fhText);
		}
		puts("\r\n");
	}
	return ret;
}

/*
	task 371
	generate a binary data file from all the data files. the data file contains one field component at each time.
	the generated data file is like a 2D array, each row is one space location, each column is one time,
	each value in the data file represents the magnitude of the field component at the space location and at the time.
*/
int createFieldComponentLines(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret = ERR_OK;
	unsigned int fileIndex = 0;
	size_t w;
	//size_t tIndex;
	unsigned int i, j, k;
	char *eh;
	char filename[FILENAME_MAX];
	char dataFilename[FILENAME_MAX];
	FILE *fhData = 0;
	MemoryManager *_mem = mem;
	TssSimStruct *simObj;
	Simulator *tssSim = createSimulator(mem, taskConfig, dataFolder, &ret);
	if (ret != ERR_OK)
	{
		return ret;
	}
	size_t size = 0;
	GridNode3Dstruct *ps = taskConfig->getGridNodes(POINT_GRIDNODES, false, &size); //space locations
	if (ps == NULL)
		return ERR_TASK_INVALID_VALUE;
	eh = taskConfig->getString(POINT_EH, false);
	int fc = taskConfig->getInt(FIELD_COMPONENT, false);
	ret = taskConfig->getErrorCode();
	if (ret != ERR_OK)
	{
		return ret;
	}
	if (fc < 1 || fc>3)
	{
		ret = ERR_INVALID_PARAM;
		return ret;
	}
	Field_Component fComponent = (Field_Component)fc;
	simObj = tssSim->GetSimParameters();
	simObj->reporter = reporter;
	//tssSim->setParametersWithoutInitialize(&simObj);
	//
	unsigned int saveInterval = simObj->pams.saveInterval;
	if (saveInterval == 0) saveInterval = 1;
	fhData = 0;
#ifdef __linux__
	sprintf_1(filename, FILENAME_MAX, "ts%u_lines%Ilu_%Ilu", simObj->pams.maxTimeSteps / saveInterval + 1, size, getTimeTick());
#elif _WIN32
	sprintf_1(filename, FILENAME_MAX, "ts%u_lines%Iu_%Iu", simObj->pams.maxTimeSteps / saveInterval + 1, size, getTimeTick());
#else
#endif
	
	ret = tssSim->formDataFilename(dataFilename, filename, "dat", fileIndex);
	if (ret != ERR_OK)
	{
		return ret;
	}
	ret = openfileWrite(dataFilename, &fhData);
	if (ret != ERR_OK)
	{
		return ret;
	}
	//each h represents one line; file contents are line by line
	for (size_t h = 0; h < size; h++)
	{
		i = ps[h].i;
		j = ps[h].j;
		k = ps[h].k;
		w = k + (simObj->pams.nz + 1) * (j + (simObj->pams.ny + 1)*i);
		printf("(%u,%u,%u)\r\n", i, j, k);
		fileIndex = 0;
		while (fileIndex < simObj->pams.maxTimeSteps)
		{
			printf("%u ", fileIndex);
			//if (simObj->pams.saveInterval == 0)
			//{
			//	//tIndex = fileIndex;
			//}
			//else
			//{
			//	//tIndex = fileIndex * simObj->pams.saveInterval;
			//}
			ret = tssSim->formDataFilename(filename, eh, "dat", fileIndex);
			if (ret != ERR_OK)
			{
				break;
			}
			if (fileexists(filename))
			{
				//wchar_t fn[FILENAME_MAX];
				size_t fsize = 0;
				//ret = copyC2W(fn, FILENAME_MAX, filename);
				//if (ret == ERR_OK)
				//{
					Point3Dstruct *field = (Point3Dstruct *)ReadFileIntoMemory(filename, &fsize, &ret);
					if (ret == ERR_OK)
					{
						switch (fComponent)
						{
						case FIELD_X:
							ret = writefile(fhData, &(field[w].x), sizeof(double));
							break;
						case FIELD_Y:
							ret = writefile(fhData, &(field[w].y), sizeof(double));
							break;
						case FIELD_Z:
							ret = writefile(fhData, &(field[w].z), sizeof(double));
							break;
						}
						if (ret != ERR_OK)
						{
							break;
						}
					}
					if (field != NULL)
					{
						FreeMemory(field);
					}
				//}
			}
			else
			{
				break;
			}
			fileIndex++;
		}
		puts("\r\n");
	}
	if (fhData != 0)
	{
		closefile(fhData);
	}
	return ret;
}

int makeStatisticsFile(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret = ERR_OK;
	TssSimStruct *simObj;
	Simulator *tssSim = createSimulator(mem, taskConfig, dataFolder, &ret);
	if (ret != ERR_OK)
	{
		return ret;
	}
	simObj = tssSim->GetSimParameters();
	simObj->reporter = reporter;
	ret = tssSim->initializeSimulator(taskConfig->getString(TP_TSS_TIME_CLASS, true));
	if (ret == ERR_OK)
	{
		ret = tssSim->generateStatisticFile();
	}
	return ret;
}

int combineCsvFiles(MemoryManager *mem, TaskFile *taskConfig, char *dataFolder, fnProgressReport reporter)
{
	int ret = ERR_OK;
	TssSimStruct *simObj;
	Simulator *tssSim = createSimulator(mem, taskConfig, dataFolder, &ret);
	if (ret != ERR_OK)
	{
		return ret;
	}
	simObj = tssSim->GetSimParameters();
	simObj->reporter = reporter;
	ret = tssSim->initializeSimulator(taskConfig->getString(TP_TSS_TIME_CLASS, true));
	if (ret == ERR_OK)
	{
		size_t count;
		unsigned *endtimeSteps = taskConfig->getUIntArray(SIM_ENDTIMESTEPS, false, &count);
		ret = taskConfig->getErrorCode();
		if (ret == ERR_OK)
		{
			ret = tssSim->combineCsvFiles(endtimeSteps, count);
		}
	}
	return ret;
}

#pragma once
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility
	2021-03-27		David Ge	Add a task to combine output CSV files for different time steps into one file
********************************************************************/

#include "simConsole.h"

//task numbers
#define TASK_HELP                 0
//radius indexing tasks
#define TASK_TEST_INDEX_CONVERT   1
#define TASK_TEST_RADIUS_INDEX    2
#define TASK_TEST_INDEX_SPEED     3
#define TASK_TEST_FDTD_INIT_FLD   4
#define TASK_TEST_FIELD_DIVER     5
#define TASK_FDTD_SIMULATION      100
#define TASK_COMPARE_DATA_FILES   110
#define TASK_MAKE_REPORT_FILES    120
#define TASK_PICK_POINTS_FILES    130
#define TASK_TWO_SUMFILES_TO_ONE  140
#define TASK_2DS_SUMFILES_TO_ONE  160
#define TASK_MAKE_POINT_TIME_FILE 170
//cubic indexing tasks
#define TASK_TSS_SIMULATION       300
#define TASK_VERIFY_CURLS         301
#define TASK_VERIFY_TIMES         302
#define TASK_RADIUS_DATA_STATIS   303
#define TASK_CUBIC_DATA_STATIS    304
#define TASK_FIELDSTATISTIC       310
#define TASK_HEILDER_CURRENT      315
#define TASK_HEILDER_CURRENT_DT   316
#define TASK_HEILDER_CURRENT_DT2  317
#define TASK_COEFFICIENTS_ONLY    330
#define TASK_POINT_TIME_FILE      370
#define TASK_POINT_TIME_BIN_FILES 371
#define TASK_MAKE_STATISTICS_FILE 372
#define TASK_COMBINE_CSV_FILES    373

#define TASK_VERIFY_3D_Z_ROTATE   390

/*
	task definitions.
	each element:
	task#, need data folder, need second data folder, description
*/
TaskInfo tasks[] = {
	  {TASK_HELP,                false, false, "Show help"}
	 ,{TASK_TEST_INDEX_CONVERT,  false, false, "verify that functions IndexToIndexes and IndexesToIndex work correctly; it requires a command line parameter \"/W\"; it requires a task parameter \"FDTD.N\""}
	 ,{TASK_TEST_RADIUS_INDEX,   false, false, "verify that function RadiusIndexToSeriesIndex works correctly; it requires a command line parameter \"/W\"; it requires a task parameter \"FDTD.N\""}
	 ,{TASK_TEST_INDEX_SPEED,    false, false, "speed comparison between using function RadiusIndexToSeriesIndex and using a row-major 3D array looping; it requires a command line parameter \"/W\"; it requires a task parameter \"FDTD.N\""}
	 ,{TASK_TEST_FDTD_INIT_FLD,  false, false, "verify that the abstract class FDTD uses a field Initial Value module correctly. It requires command line parameters \"/W\" and \"/L\". It requires following task parameters: \"FDTD.N\", \"FDTD.R\", \"SIM.IV_DLL\" and \"SIM.IV_NAME\""}
	 ,{TASK_TEST_FIELD_DIVER,    false, false, "verify an Initial Value module by divergences. It requires command line parameter \"/W\"; and \"/L\" is optional. It requires following task parameters: \"FDTD.N\", \"FDTD.R\", \"SIM.IV_DLL\", \"SIM.IV_NAME\" and \"FDTD.HALF_ORDER_SPACE\""}
	 ,{TASK_FDTD_SIMULATION,     true,  false, "execute an EM field simulation. It requires command line parameters \"/W\" and \"/D\", and \"/L\" is optional. It requires following task parameters: \"FDTD.N\", \"FDTD.R\" , \"SIM.FDTD_DLL\", \"SIM.FDTD_NAME\", \"SIM.BC_DLL\", \"SIM.BC_NAME\", \"SIM.IV_DLL\" and \"SIM.IV_NAME\". Following task parameters are optional: \"SIM.TFSF_DLL\", \"SIM.TFSF_NAME\", \"SIM.FS_DLL\", and \"SIM.FS_NAME\". It also requires a task parameter \"SIM.BASENAME\" for specifying base file name, which does not include file name extension. Suppose \"SIM.BASENAME\" is specified as\r\nSIM.BASENAME=simA\r\nand command line uses \"/Dc:\\simulation\\data\" then for each simulation time step, the electromagnetic field is saved in a file \"c:\\simulation\\data\\simA{n}.em\", where {n} is time step index which can be 0, 1, 2, .... Use \"FDTD.HALF_ORDER_SPACE\" and \"FDTD.HALF_ORDER_TIME\" to specify estimation orders for space curls and time advancement, respectively."}
	 ,{TASK_COMPARE_DATA_FILES,  true,  true,  "compare two simulations and generate a report file for each time step. It requires command line parameters \"/W\", \"/D\" and \"/E\". Use task parameters \"FDTD.N\" and \"FDTD.R\" to specify space geometry.  Use task parameters \"SIM.FILE1\" and \"SIM.FILE2\" to specify the base file names used by the two simulations; \"/D\" specifies folder for \"SIM.FILE1\" and \"/E\" specifies folder for \"SIM.FILE2\". Use task parameter \"SIM.THICKNESS\" to specify boundary thickness to be excluded from the comparison."}
	 ,{TASK_MAKE_REPORT_FILES,   true,  false, "create a report file for each data file, It requires command line parameters \"/W\" and \"/D\". It also requires a task parameter \"SIM.BASENAME\" for specifying base file name, which does not include file name extension. It searches data files by {path by /D}\\{base file name}{n}.em, where {n}=0,1,2,...; it uses an optional task parameter, \"FDTD.HALF_ORDER_SPACE\", to specify half estimation order for divergence estimations; default value is 1. "}
	 ,{TASK_PICK_POINTS_FILES,   true,  false, "pick field points with the largest strengths from each data file and generate a new data file. the new file contains items of 9 doubles: 3 for space location, 6 for EM field. It requires command line parameters \"/W\" and \"/D\". It requires following task parameters: \"FDTD.R\", \"SIM.BASENAME\", \"SIM.POINTS\" for the number of field points to pick, and \"SIM.MAXTIMES\" for maximum number of data files to process, use 0 to process all data files."}
	 ,{TASK_TWO_SUMFILES_TO_ONE, true,  true,  "merge two summary files into one file. a summary is generated by task 120. It requires command line parameters \"/W\", \"/D\" and \"/E\". Use task parameters \"SIM.FILE1\" and \"SIM.FILE2\" to specify the names of the summary files; \"/D\" specifies folder for \"SIM.FILE1\" and \"/E\" specifies folder for \"SIM.FILE2\". Use an optional task parameter \"SIM.THICKNESS\" to specify boundary thickness to be excluded from the merge; if it is missing then 0 is assumed."}
	 ,{TASK_2DS_SUMFILES_TO_ONE, true,  true,  "merge two summary files into one file. a summary is generated by task 120. It requires command line parameters \"/W\", \"/D\" and \"/E\". Use task parameters \"SIM.FILE1\" and \"SIM.FILE2\" to specify the names of the summary files; \"/D\" specifies folder for \"SIM.FILE1\" and \"/E\" specifies folder for \"SIM.FILE2\". the space steps of the two simulations are ds1 and ds2, and ds1 = ds2/2, and maxRadius1 + 1 = 2 * maxRadius2 "}
	 ,{TASK_MAKE_POINT_TIME_FILE,true,  false, "from a set of simulation data files (*.em files), generate one file, the file contains an array of 6 values of Ex,Ey,Ez,Hx,Hy,Hz at one given space location, the first set of the 6 field values is for time=0, the next set for t=dt, 2dt, 3dt, ... It requires command line parameters \"/W\", \"/D\". Use task parameters \"SIM.FILE1\" to specify base file name for data source files, i.e. for SIM.FILE1=YeeFDTD_N8_R0R2_T2S2, the source files are YeeFDTD_N8_R0R2_T2S2_*.em under folder specified by \"/D\", the generated file is named as YeeFDTD_N8_R0R2_T2S2_n_m_p.dat for a binary file, or as YeeFDTD_N8_R0R2_T2S2_n_m_p.txt for a text file. Use task parameters \"POINT.m\", \"POINT.n\", and \"POINT.p\" to specify space location m, n, and p. " }
	//cubic indexing 
	 ,{TASK_VERIFY_CURLS,        false, false, "verify curls. It requires command line parameters \"/W\" and \"/D\". " }
	 ,{TASK_VERIFY_TIMES,        true,  false, "verify time parameters. It requires command line parameters \"/W\" and \"/D\". " }
	 ,{TASK_TSS_SIMULATION,      true,  false, "execute an EM field simulation. use a task file to specofy all required parameters. It requires command line parameters \"/W\" and \"/D\". " }
	 ,{TASK_FIELDSTATISTIC,      true,  false, "make EM field statistics from all data files. use a task file to specofy a base file name for data files. It requires command line parameters \"/W\" and \"/D\", use \"/D\" to specifiy where the data files are stored. " }
	 ,{TASK_RADIUS_DATA_STATIS,  true,  false, "make EM field statistics from all data files in radius indexing. use a task file to specofy a base file name for data files. It requires command line parameters \"/W\" and \"/D\". " }
	 ,{TASK_CUBIC_DATA_STATIS,   true,  false, "generate one EM field radius statistics for each data file in cubic indexing. use a task file to specofy a base file name for data files. It requires command line parameters \"/W\" and \"/D\". " }
	 ,{TASK_HEILDER_CURRENT,     true,  false, "calculate Heilder current. use a task file to specofy z-space step size and time step size for collecting current values. It requires command line parameters \"/W\" and \"/D\". " }
	 ,{TASK_HEILDER_CURRENT_DT,  true,  false, "calculate Heilder current derivatives with respect to time. use a task file to specofy z-space step size and time step size for collecting current values. It requires command line parameters \"/W\" and \"/D\". " }
	 ,{TASK_HEILDER_CURRENT_DT2, true,  false, "calculate Heilder current 2nd derivatives with respect to time. use a task file to specofy z-space step size and time step size for collecting current values. It requires command line parameters \"/W\" and \"/D\". " }
	 ,{TASK_POINT_TIME_FILE,     true,  false, "from a set of simulation data files (*e_*.dat files or *h_*.dat files), generate one or more text files, each file for one space location, the file contains an array of 3 values of (Ex,Ey,Ez) or (Hx,Hy,Hz) at one given space location, the first set of the 3 field values is for time=0, the next sets for t=dt, 2dt, 3dt, ... It requires command line parameters \"/W\", \"/D\". Use the same task file doing the simulation for this task. Use task parameters \"POINT.GRIDNODES=(i,j,k),(i,j,k)...\", \"POINT.EH=e|h\" to specify space locations i, j, and k, and for E-field or for H-field. " }
	 ,{TASK_POINT_TIME_BIN_FILES,true,  false, "from a set of simulation data files (*e_*.dat files or *h_*.dat files), generate one bin file, the file contains a 2D array of values, each row for one space location, each column for one time, each value is a field value of Ex,Ey,Ez,Hx,Hy, or Hz. It requires command line parameters \"/W\", \"/D\". Use the same task file doing the simulation for this task. Use task parameters \"POINT.GRIDNODES=(i,j,k),(i,j,k)...\", \"POINT.EH=e|h\" to specify space locations i, j, and k, and for E-field or for H-field, use \"FIELD.COMPONENT\" to specify field component, 1 for x, 2 for y and 3 for z. " }
	 ,{TASK_MAKE_STATISTICS_FILE,true,  false, "from a set of simulation data files (*e_*.dat files and *h_*.dat files), generate one text file, the file contains field energy and divergences summarized at each time. It requires command line parameters \"/W\", \"/D\". Use the same task file doing the simulation for this task. " }
	 ,{TASK_COEFFICIENTS_ONLY,   true,  false, "generate coefficients without starting simulation. use a task file to specofy all required parameters. It requires command line parameters \"/W\" and \"/D\"," }
	 ,{TASK_COMBINE_CSV_FILES,   true,  false, "combine the CSV output files for different time steps into one file. use the same task file for the simulations generating the CSV output files. use \"SIM.ENDTIMESTEPS=t1,t2,...\" to specify the the end-time-step for each CSV file. It requires command line parameters \"/W\" and \"/D\"," }
	 ,{TASK_VERIFY_3D_Z_ROTATE,  true,  false, "verify fields data files are z-rotation symmetry. use the same task file and command line parameters for the simulations generating the 3D fields files. " }
};

//
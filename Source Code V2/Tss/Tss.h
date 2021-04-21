#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

types for Time-Space-Synchronized FDTD algorithm
********************************************************************/
#include <cmath>
#include "../EMField/EMField.h"

#define ERR_OK             0
#define ERR_INVALID_PARAM  10001
#define ERR_INVERSEMATRIX  10002
#define ERR_SIMULATOR_NAME 10003
#define ERR_SOURCE_NAME    10004
#define ERR_FIELD0_NAME    10005
#define ERR_BOUNDARY_NAME  10006
#define ERR_SETTER_NOTTEST 10007
#define ERR_INVALID_COUR_F 10008
#define ERR_NOT_IMPLEMENT  10009
#define ERR_CURL_FAILED    10010
#define ERR_SRC_NOT_INIT   10011
#define ERR_BC_NOT_INIT    10012
#define ERR_TIMECLASSNAME  10013
#define ERR_STATISTIC_NAME 10014
#define ERR_MAXTIME_START  10015
#define ERR_START_INTERVAL 10016

#define ERR_SOURCE_NOT_SUPPORT   10017
#define ERR_BOUNDARY_NOT_SUPPORT 10018

#define ERR_PML_LN_BIG     10101
#define ERR_PML_POWER_0    10102
#define ERR_PML_MAGNITUDE  10103

#define ERR_CYLINTRICAL_Y_N_X   10200
#define ERR_CYLINTRICAL_NX_ODD  10201
#define ERR_CYLINTRICAL_XMIN_0  10202
#define ERR_CYLINTRICAL_CENTER  10203

//configuration names to be used in a task file
#define TP_TSS_SIMULATE_CLASS  "TSS.SIMULATE.CLASS" //class name of field simulator
#define TP_TSS_SOURCE_CLASS    "TSS.SOURCE.CLASS" //class name of field sources
#define TP_TSS_BOUNDARY_CLASS  "TSS.BOUNDARY.CLASS" //class name of boundary conditions
#define TP_TSS_FIELDSET_CLASS  "TSS.FIELDSETTER.CLASS" //class name of fields setter
#define TP_TSS_TIME_CLASS      "TSS.TIME.CLASS" //class name of time advancement class
#define TP_TSS_STATISTIC_CLASS "TSS.STATISTICS.CLASS" //class name of statistics class

#define TP_TSS_SCRNINV   "TSS.SCREEN_INTERVAL" //time steps before showing step info on the screen
#define TP_TSS_RECINV    "TSS.REC_INTERVAL"    //time steps before saving fields to files
#define TP_TSS_RECFILE   "TSS.REC_BASEFILE"    //forming field files: <BASEFILE>_<time step>.e and <BASEFILE>_<time step>.h
#define TP_TSS_TIMEFILE  "TSS.TIMEMATRIXFILE"  //file name for saving time advance matrix
#define TP_TSS_SPACEFILE "TSS.SPACEMATRIXFILE" //file name for saving space estimation matrix
#define TP_TSS_RECINV_SPACE    "TSS.REC_INTERVAL_SPACE"    //space steps before saving fields to files

#define TP_TSS_MAXTIME   "SIM.MAXTIMES"  //Maximum simulation time steps, counting from 0
#define TP_TSS_STARTTIME "SIM.STARTTIME" //the time step number to start with

//estimation settings
#define TP_TSS_KMAX   "TSS.KMAX"
#define TP_TSS_SMAX   "TSS.SMAX"
#define TP_TSS_DS     "TSS.DS"
#define TP_TSS_DT     "TSS.DT"
#define TP_COURFACTOR "TSS.COURANT_FACTOR"  //factor for courant number between 0 and 1

//materials
#define TP_TSS_RHO     "TSS.RHO"
#define TP_TSS_VAC     "TSS.IS_VACUUM"  //if it is true then TSS.EPS and TSS.MU are ignored, eps0 and mu0 are used
#define TP_TSS_EPS     "TSS.EPS"
#define TP_TSS_MU      "TSS.MU"
#define TP_TSS_SIE     "TSS.SIGMA_E"
#define TP_TSS_SIM     "TSS.SIGMA_M"

//threads 
#define TP_SIM_USETHREADS "SIM.USETHREADS"

//it is 1/ks; i.e. if ks=1/1000 then put 1000 here
#define TP_SPACE_FACTOR "TSS.SPACE_FACTOR"

//computing domain
#define TP_TSS_NX      "TSS.NX"
#define TP_TSS_NY      "TSS.NY"
#define TP_TSS_NZ      "TSS.NZ"
#define TP_TSS_XMIN    "TSS.XMIN"
#define TP_TSS_YMIN    "TSS.YMIN"
#define TP_TSS_ZMIN    "TSS.ZMIN"

//PML settings
#define TP_PML_DISABLE "PML.DISABLE"
#define TP_PML_THICK   "PML.THICK"
#define TP_PML_ALPHA   "PML.ALPHA"
#define TP_PML_BETA    "PML.BETA"
#define TP_PML_X_LOW   "PML.X.LOW"
#define TP_PML_X_HIGH  "PML.X.HIGH"
#define TP_PML_Y_LOW   "PML.Y.LOW"
#define TP_PML_Y_HIGH  "PML.Y.HIGH"
#define TP_PML_Z_LOW   "PML.Z.LOW"
#define TP_PML_Z_HIGH  "PML.Z.HIGH"
#define TP_PML_POWER   "PML.POWER"

//folder for reading/writing the space derivative estimation matrix
//for smax=1 or smax=2, calculating the matrix takes no time, no need of loading from file
//for smax=3 and above, it taskes a long time to calculate the matrix. it is better to calculate once and load it from file later.
#define TP_TSS_MATRIXFILEDIR "TSS.MATRIXFILEFOLDER"

//subfolder for generating data files
#define TP_TSS_DATAFOLDER "TSS.DATAFOLDER"
//true: generate statistics file after simulation
#define TP_TSS_STATISTICS "TSS.MAKESTATISTICS"
//true: divide divergence by field strength
#define TP_RELATIVE_DIVERGENCE "RELATIVE.DIVERGENCE"
//divide divergence by field strength only if the field strength is greater than the threshold
#define TP_REL_DIVG_THRESHOLD "RELATIVE.DIVERGENCE.THRESHOLD" 

//test case to be executed
#define TP_TEST_CASE "SIM.TESTCASE"

//It specifies an array delimited by “:”. Each array item is in a format of “I,j,k,Ex|Ey|Ez|Hx|Hy|Hz”, i.e. “20,11,23,Ez:42:13:21,Hy”, this example will generate 2 CSV files, each line of the CSV file contains two values, time in milliseconds and the specified field value.
#define SIM_OUTPUT_CSV "SIM.OUTPUT.CSV"


typedef enum
{
	Field_E = 1,
	Field_H = 2,
}FIELD_EMTYPE;

typedef enum
{
	Field_type_3D               = 1,
	Field_type_z_rotateSymmetry = 2,
}FIELD_TYPE;

/*
parameters needed for implementing the Perfect Match Layer
*/
typedef struct PmlStruct{
	unsigned int Ln; //PML thickness
	double alphaMax; //maximum alpha
	double betaMax;  //maximum beta
	bool Pxl, Pxh;   //apply PML at low and high ends of x-axis
	bool Pyl, Pyh;   //apply PML at low and high ends of y-axis
	bool Pzl, Pzh;   //apply PML at low and high ends of z-axis
	double power;    //power to the depth to thikness ratio
	bool disable;    //disable PML
}PmlStruct;

/*
parameters needed for running a simulation
*/
typedef struct SimStruct{
	double rho; // ρ
	double eps; // ε
	double mu;  // μ
	double sie; // σ
	double sim; // σ_m
	double ds;  //space step size
	double dt;  //time advance step size
	double courantFactor; //factor for courant number, between 0 and 1
	double spaceFactor;   //factor for space, multiply it to rho, eps, mu, sie and sim; defaut is 1
	double xmin; double ymin; double zmin;             //computation domain. nx*ds>=x>=xmin, ny*ds>=y>=ymin, nz*ds>=z>=zmin
	unsigned int nx; unsigned int ny; unsigned int nz; //computation domain. 0...nx; 0...ny; 0...nz
	unsigned int smax;          //>=1. half estimatin order. Estimation order = 2*smax
	unsigned int kmax;          //>=0. estimation order = 2*(kmax+1)
	unsigned int maxTimeSteps;  //maximum time steps
	unsigned int startTimeStep; //the time step number to start with, if it is greater than 0 then the field data files for the time step are used as the initial fields
	unsigned int saveInterval;  //time steps before saving fields to files
	unsigned int showInterval;  //time steps before showing step info on the screen
	unsigned int threads;       //threads to be used
	PmlStruct pml;              //PML parameters
	bool relativeDivergence;    //true: divide divergence by field strength
	double relDivergThreshold;  //divide divergence by field strength only if the field strength is greater than the threshold
}SimStruct;

//space derivative estimation matrixes
//dimensions: M+1 X M X M, M=2 - 8  these arays are not large, usually < 4K memory
typedef struct Array1Dstruct { double *c; }Array1Dstruct;
typedef struct Array2Dstruct { Array1Dstruct *r; }Array2Dstruct;
typedef struct Array3Dstruct { Array2Dstruct *a; }Array3Dstruct;
//
int tss_allocate2Darray(Array2Dstruct *a, unsigned int dr, unsigned int dc);
void tss_free2Darray(Array2Dstruct *a, unsigned int dr);
void copy2Darray(double *W, Array2Dstruct *A, unsigned int M);
//
Array3Dstruct *tss_allocate3Darray(unsigned int da, unsigned int dr, unsigned int dc);
void tss_free3Darray(Array3Dstruct *a, unsigned int da, unsigned int dr);


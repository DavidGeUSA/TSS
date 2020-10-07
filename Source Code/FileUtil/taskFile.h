#ifndef __TASKFILE_H__
#define __TASKFILE_H__

/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

/*
	a task file contains lines of strings
	each line is formed by "name"="value"
	"name" is a string. All names in a task file are unique.
	"value" is a value of a primary type, i.e. double, integer, unsigned integer, long integer, Boolean, and string.
	a line can be a comment if it starts with "//"

	each module, i.e. simulation console, boundary condition, TFSF, FDTD, field source, and initial condition, defines names and expected values

	a task file defines a task by the values assigned to names

	this module provides functions, get???(name, optional), for searching and parsing values in a task file,
	where "name" identifies a task parameter.

	if a given "name" does not exist in a task file then a function get???(name, optional) returns a default value;
	the default value for a string is NULL; the default value for a number is 0; the default value for a Boolean is false;
	function getErrorCode returns ERR_OK if "optonal" is true; 
	function getErrorCode returns ERR_TASK_PARAMETER_NAME if "optonal" is false, and subsequent calling of get???(name, optional) will do nothing.

	the designed usage of this class is that when get???(name, false) is called (that is, optional is false) then getErrorCode should be called to
	check whether the name exists; if the name does not exist then the program should bail out and return the error code. But you do not
	have to call getErrorCode after each call of get???(name, optional); you may make calls of get???(name, optional) for all the task parameters
	and then make one call of getErrorCode to do checking.

	if you want to know whether a "name" exists but also want to treat the task parameter as optional then use false for the "optional" when 
	calling get???(name, optional), and call getErrorCode to examine whether the "name" exist, then call resetErrorCode before calling get???(name, optional)
	for other task parameters.

	Future modifications of this class should keep the above code logic so that the modifications do not break existing code using this class.

*/

#include <iostream>
#include <fstream>
using namespace std;

#define ERR_OUTOFMEMORY             1
#define ERR_TASKFIILE_INVALID       380
#define ERR_TASKFIILE_DUPLICATE     381
#define ERR_TASK_PARAMETER_NAME     382
#define ERR_TASKFIILE_NOTEXIST      383
//invalid task parameter value
#define ERR_TASK_INVALID_VALUE      384 

/*
	task parameter name and value
*/
typedef struct NameValue
{
	char *name;
	char *value;
}NameValue;

/*
	convert a double number to two integers. It is for forming a file name.
	One integer is by removing digits after the decimal point
	One integer is using digits after the decimal point
*/
void doubleToIntegers(double d, int *i1, int *i2);

/*
	get task parameters from a task file
*/
class TaskFile
{
private:
	int ret;           //error code
	NameValue *_lines; //lines in the task file
	int lineArraySize; //items in _lines
	int count;         //count of (name, value) pairs read from the file
	char *unfoundName; //the name passed to getIndexByName but not found in _lines
	char *nameBadVal;  //the name with an invalid value
	//
	void allocateLines();
	int getIndexByName(const char *name);
public:
	TaskFile(const char *taskfile);
	~TaskFile();
	int getErrorCode(){return ret;}
	void resetErrorCode(void);
	char *missingParameter(){return unfoundName;}
	char *invalidParameter(){return nameBadVal;}
	void setNameOfInvalidValue(const char *name);
	//
	double getDouble(const char *name, bool optional);
	int getInt(const char *name, bool optional);
	unsigned int getUInt(const char *name, bool optional);
	long int getLong(const char *name, bool optional);
	char *getString(const char *name, bool optional);
	bool getBoolean(const char *name, bool optional);
};

//common task parameter names

//task parameters used by the simulation console
#define TP_SIMTASK      "SIM.TASK"
#define TP_SIMFDTD_DLL  "SIM.FDTD_DLL"
#define TP_SIMFDTD_NAME "SIM.FDTD_NAME"
#define TP_SIMIV_DLL    "SIM.IV_DLL"
#define TP_SIMIV_NAME   "SIM.IV_NAME"
#define TP_SIMBC_DLL    "SIM.BC_DLL"
#define TP_SIMBC_NAME   "SIM.BC_NAME"
#define TP_SIMFS_DLL    "SIM.FS_DLL"
#define TP_SIMFS_NAME   "SIM.FS_NAME"
#define TP_SIMTFSF_DLL  "SIM.TFSF_DLL"
#define TP_SIMTFSF_NAME "SIM.TFSF_NAME"
#define TP_SIMBASENAME  "SIM.BASENAME"

//task parameters used by a FDTD module
#define TP_FDTDN            "FDTD.N"
#define TP_FDTDR            "FDTD.R"
#define TP_MAX_TIMESTEP     "FDTD.MAXTIMESTEP"
//enable recording speeds
#define TP_REC_TIMESTEP     "FDTD.RECTIMESTEP"
//task parameters used by a FDTD module which may use different estimation orders
#define TP_HALF_ORDER_SPACE "FDTD.HALF_ORDER_SPACE"
#define TP_HALF_ORDER_TIME  "FDTD.HALF_ORDER_TIME"

//task parameters needed by some tasks
#define TP_SIMFILE1     "SIM.FILE1"
#define TP_SIMFILE2     "SIM.FILE2"
#define TP_SIMTHICKNESS "SIM.THICKNESS"
#define TP_POINTM       "POINT.m"
#define TP_POINTN       "POINT.n"
#define TP_POINTP       "POINT.p"

#endif
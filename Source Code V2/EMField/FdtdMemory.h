#pragma once
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include "EMField.h"
#include "../MemoryMan/MemoryManager.h"

/*
	for allocating simulation memory
*/
class FdtdMemory:public virtual MemoryManUser
{
//private:
//	unsigned long error_OS; //error code from operating system
protected:
	char *_basefilename; //file path and file name base for forming data file names
	size_t fieldMemorySize; //size of memory for HE
	size_t fieldItems;      //number of points in HE
	FieldPoint3D *HE;       //field data
	size_t _timeIndex;      //0,1,2,...,_maximumTimeIndex
	//
	/*
		allocate HE using file mapping so that large memory can be allocated for large domain. 
		if _basefilename != NULL then mapped file is a newly formed file name;
		if _basefilename == NULL then a temporary file name is used
	*/
	int allocateFieldMemory();
	//
	/*
		free memory HE. 
		close mapped file.
		if mapped file is a temporary file then delete it
	*/
	void freeFieldMemory();
public:
	FdtdMemory(void);
	virtual ~FdtdMemory();
	//properties-----------------------------
	//unsigned long GetOSError();
	size_t GetMemorySize();
	size_t GetMemoryItemCount();
	size_t GetTimeStepIndex();
	FieldPoint3D *GetFieldMemory();
	//---------------------------------------
};

//this macro should only be used inside FdtdMemory or derived classes
//it returns a pointer to FieldPoint3D at index m,n,p
#define FIELDS(m,n,p) (&(HE[SINDEX((m),(n),(p))]))


/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include <string.h>
#include <stdio.h>

#include "FdtdMemory.h"
#include "..\MemoryMan\memman.h"

FdtdMemory::FdtdMemory(void)
{
	HE = NULL;
	_basefilename = NULL;
	_timeIndex = 0;
}
FdtdMemory::~FdtdMemory()
{
	freeFieldMemory();
	if(_basefilename != NULL)
	{
		free(_basefilename);
		_basefilename = NULL;
	}
}

/*
	memory HE is mapped to a file,
	freeing the memory closes the file.
*/
void FdtdMemory::freeFieldMemory()
{
	if(HE != NULL)
	{
		FreeMemory(HE);
		HE = NULL;
	}
}
FieldPoint3D *FdtdMemory::GetFieldMemory()
{
	return HE;
}
size_t FdtdMemory::GetMemorySize()
{
	return fieldMemorySize;
}
size_t FdtdMemory::GetMemoryItemCount()
{
	return fieldItems;
}
size_t FdtdMemory::GetTimeStepIndex()
{
	return _timeIndex;
}

/*
		allocate HE using file mapping. 
		if _basefilename != NULL then mapped file uses a newly formed data file name;
		if _basefilename == NULL then a temporary file name is used
*/
int FdtdMemory::allocateFieldMemory()
{
	int ret = MEMMANEXIST;
	if(ret == ERR_OK)
	{
		if(_basefilename == NULL) //field data will not be saved to files
		{
			if(HE == NULL)
			{
				//allocate memory by mapping to a temporary file
				HE = (FieldPoint3D *)AllocateMemory(fieldMemorySize);
				if(HE == NULL)
				{
					ret = ERR_OUTOFMEMORY;
				}
			}
		}
		else
		{
			FieldPoint3D *p;
			//int err;
			wchar_t filename[FILENAME_MAX];
			ret = formDataFileNameW(filename, FILENAME_MAX, _basefilename, _timeIndex);
			//form a new field data file name
			/*wcscpy_s(filename, _basefilename);
			err = swprintf_s(filename, FILENAME_MAX, L"%s%d.em", filename, _timeIndex);
			if(err == -1)
			{
				ret = ERR_MEM_EINVAL;
			}
			else*/
			if(ret == ERR_OK)
			{
				//allocate memory and map the memory to a file using the newly formed file name
				p = (FieldPoint3D *)CreateFileIntoMemory(filename, fieldMemorySize, &ret);
				if(ret == ERR_OK)
				{
					if(p == NULL)
					{
						ret = ERR_OUTOFMEMORY;
					}
					else
					{
						if(HE !=NULL)
						{
							//copy current field data to the new memory
							for(size_t i=0;i<fieldItems;i++)
							{
								p[i] = HE[i];
							}
							//free current memory, close the mapped file
							FreeMemory(HE);
						}
						//new memory becomes current memory
						HE = p;
					}
				}
				else
				{
					RememberOSerror();
				}
			}
		}
	}
	return ret;
}


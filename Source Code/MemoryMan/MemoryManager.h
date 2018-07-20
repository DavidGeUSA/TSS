#pragma once
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/

#include <Windows.h>
#include <FileAPI.h>
#include <stdlib.h>
#include <stdio.h>

#include "memman.h"

#define NAMESIZE 100

typedef void* (*fnCreatePluginInstance)(char *name, double *params);
typedef void (*fnRemovePluginInstances)();

class MemoryItem
{
private:
	TCHAR filename[FILENAME_MAX];
	HANDLE filehandle;
	HANDLE maphandle;
	//
	bool readOnly;
	bool keepFileOnFree;
	//
	DWORD lastError;
public:
	MemoryItem(LPCWSTR diskfile);
	virtual ~MemoryItem(void);
	void Free();
	DWORD LastError();
	//
	LPVOID address;
	//
	void KeepFile();
	virtual int AllocateMemoryItem(size_t size);
	int ReadFileIntoMemoryItem(size_t *size);
	bool IsReadOnly();
};

class MemoryManager
{
private:
	MemoryItem **items;
	int count;
	TCHAR fileDir[FILENAME_MAX];
	//
	DWORD lastError;
	//
	void *mapFileIntoMemory(wchar_t *filepath, size_t *size, int *nRet, bool createFile);
public:
	MemoryManager(LPCWSTR folder);
	~MemoryManager();
	void SetFolder(LPCWSTR folder);
	int GetTemporaryFolder(char *folder, int size);
	void Clear();
	DWORD LastError();
	//
	void *ReadFileIntoMemoryItem(wchar_t *filepath, size_t *size, int *nRet);
	void *CreateFileIntoMemoryItem(wchar_t *filepath, size_t size, int *nRet);
	LPVOID Allocate(size_t size);
	int Free(LPVOID address);
};

/*
	base class to let all plugs share the same copy of memory manager
*/
class MemoryManUser
{
protected:
	MemoryManager *_mem;
public:
	MemoryManUser(void){_mem = NULL;}
	void SetMemoryManager(MemoryManager *m);
	int GetTemporaryFolder(char *folder, int size);
};

//test whether the memory manager is available
#define MEMMANEXIST (_mem==NULL)?ERR_MEM_MAN_NULL:ERR_OK

//allocate memory using a temporary file for mapping. the file will be deleted on freeing the memory
#define AllocateMemory(size) _mem->Allocate(size)

//free memory. close the memory mapping file. delete the file if it is a tempory file. 
#define FreeMemory(e) _mem->Free((e))

//map a file to a read-only block of memory, fname: wchar_t*, sizeRet: size_t*, errRet: int*
#define ReadFileIntoMemory(fname, sizeRet, errRet) _mem->ReadFileIntoMemoryItem((fname), (sizeRet), (errRet))

//create a new file and map it to a writable memory, fname: wchar_t*, sizeRet: size_t, errRet: int*
#define CreateFileIntoMemory(fname, size, errRet) _mem->CreateFileIntoMemoryItem((fname), (size), (errRet))
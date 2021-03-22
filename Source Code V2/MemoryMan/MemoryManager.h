#pragma once
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility

********************************************************************/
#ifdef __linux__ 
#include <cstdio>
#include <cstddef>
#elif _WIN32
#include <Windows.h>
#else
#endif

#include <stdio.h>

typedef void* (*fnCreatePluginInstance)(char *name, double *params);
typedef void (*fnRemovePluginInstances)();

class MemoryItem
{
private:
	char filename[FILENAME_MAX];
#ifdef __linux__ 
	size_t memSize;
	int fd;
#elif _WIN32
	HANDLE filehandle;
	HANDLE maphandle;
#else
#endif
	//
	bool readOnly;
	bool keepFileOnFree;
	//
	unsigned long lastError;
public:
	MemoryItem(const char *diskfile);
	virtual ~MemoryItem(void);
	void Free();
	unsigned long LastError();
	//
	void *address;
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
	char fileDir[FILENAME_MAX];
	//
	bool useRAM; //allocate memory in RAM instead of in a file
	//
	int retLast; //last allocation error code
	unsigned long lastError;
	//
	void *mapFileIntoMemory(char *filepath, size_t *size, int *nRet, bool createFile);
public:
	MemoryManager(const char *folder);
	~MemoryManager();
	void SetFolder(const char *folder);
	void SetUseRam(bool use_ram){ useRAM = use_ram; }
	int GetTemporaryFolder(char *folder, int size);
	void Clear();
	unsigned long LastError();
	int LastRetCode(){ return retLast; }
	//
	void *ReadFileIntoMemoryItem(char *filepath, size_t *size, int *nRet);
	void *CreateFileIntoMemoryItem(char *filepath, size_t size, int *nRet);
	void *Allocate(size_t size);
	int Free(void *address);
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
	virtual ~MemoryManUser(){}
	void SetMemoryManager(MemoryManager *m);
	int GetTemporaryFolder(char *folder, int size);
};

//test whether the memory manager is available
#define MEMMANEXIST (_mem==NULL)?ERR_MEM_MAN_NULL:ERR_OK

//allocate memory by file mapping. if a temporary file is used then the file will be deleted on freeing the memory
#define AllocateMemory(size) _mem->Allocate(size)

//if the memory allocation returns NULL, use it to get return code
#define MEMRETLAST _mem->LastRetCode()

//free memory. close the memory mapping file. delete the file if it is a tempory file. 
#define FreeMemory(e) _mem->Free((e))

//map a file to a read-only block of memory, fname: char*, sizeRet: size_t*, errRet: int*
#define ReadFileIntoMemory(fname, sizeRet, errRet) _mem->ReadFileIntoMemoryItem((fname), (sizeRet), (errRet))

//create a new file and map it to a writable memory, fname: char*, sizeRet: size_t, errRet: int*
#define CreateFileIntoMemory(fname, size, errRet) _mem->CreateFileIntoMemoryItem((fname), (size), (errRet))
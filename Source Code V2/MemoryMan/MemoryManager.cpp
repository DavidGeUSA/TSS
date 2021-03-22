/*
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 04/01/2018
	Allrights reserved

	This module provides memory management functions.
	It also provides an interface to other functions specific to operating systems

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility

*/
#ifdef __linux__ 
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#elif _WIN32

#include <Shlwapi.h>
#else
#endif

#include <stdlib.h>
#include <stdio.h>

#include "../FileUtil/fileutil.h"
#include "../ProcessMonitor/workProcess.h"
#include "memman.h"
#include "MemoryManager.h"

unsigned long osErrorCode = 0;

void RememberOSerror()
{
	if(osErrorCode == 0)
	{
#ifdef _WIN32
		osErrorCode = GetLastError();
#endif
	}
}
void ClearOSerror()
{
	osErrorCode = 0;
}
unsigned long GetLastWinError()
{
	if(osErrorCode == 0)
	{
#ifdef _WIN32
		osErrorCode = GetLastError();
#endif
	}
	return osErrorCode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryItem::MemoryItem(const char *diskfile)
{
	if (diskfile == NULL)
	{
		filename[0] = 0;
	}
	else
	{
		int i;
		for (i = 0; i < FILENAME_MAX; i++)
		{
			if (diskfile[i] == 0)
			{
				filename[i] = 0;
				break;
			}
			filename[i] = diskfile[i];
		}
	}
#ifdef __linux__ 
	memSize = 0;
	fd = -1;
#elif _WIN32
	filehandle = NULL;
	maphandle = NULL;
#else
#endif
	address = NULL;
	lastError = 0;
	keepFileOnFree = true;
}
void MemoryItem::KeepFile()
{
	keepFileOnFree = true;
}
void MemoryItem::Free()
{
	if(address != NULL)
	{
#ifdef __linux__ 
		munmap(address, memSize);
#elif _WIN32
		UnmapViewOfFile(address);
#else
#endif
		address = NULL;
	}
#ifdef __linux__ 
	if (fd != -1)
	{
		close(fd);
		fd = -1;
	}
#endif
#ifdef _WIN32
	if(maphandle != NULL && maphandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(maphandle);
		maphandle = NULL;
	}
	if(filehandle != NULL && filehandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(filehandle);
		filehandle = NULL;
	}
#endif
	if(!keepFileOnFree)
	{
		if (filename[0] != 0)
		{
			if (fileexists(filename))
			{
				deleteFile(filename);
			}
		}
	}
}

MemoryItem::~MemoryItem(void)
{
	Free();
}
unsigned long MemoryItem::LastError()
{
	return lastError;
}
bool MemoryItem::IsReadOnly()
{
	return readOnly;
}

int MemoryItem::ReadFileIntoMemoryItem(size_t *size)
{
	int ret = ERR_OK;
	//
	keepFileOnFree = true;
	readOnly = true;
#ifdef __linux__ 
	void *addr;
	int fd;
	struct stat sb;
	off_t offset, pa_offset;
	size_t length;
	//

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ret = ERR_FILE_OPEN;
	}
	else if (fstat(fd, &sb) == -1)           /* To obtain file size */
	{
		ret = ERR_GET_FILESIZE;
	}
	if(ret == ERR_OK)
	{
		offset = 0;
		pa_offset = 0;
		pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
		/* offset for mmap() must be page aligned */

		length = sb.st_size - offset;
		addr = mmap(NULL, length + offset - pa_offset, PROT_READ,
				MAP_PRIVATE, fd, pa_offset);
		if (addr == MAP_FAILED)
			ret = ERR_FILE_MAP;
	}
	if(ret == ERR_OK)
	{
		*size = length;
		memSize = length;
		address = addr;
	}
	
#elif _WIN32
	wchar_t filenameW[FILENAME_MAX];
	copyC2W(filenameW, FILENAME_MAX, filename);
	filehandle = CreateFile(filenameW,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(filehandle == INVALID_HANDLE_VALUE)
	{
		lastError = GetLastError();
		ret = ERR_MEM_OPEN_FILE;
	}
	else
	{
		LARGE_INTEGER fs;
		if(!GetFileSizeEx(filehandle, &fs))
		{
			lastError = GetLastError();
			CloseHandle(filehandle);
			filehandle = NULL;
			ret = ERR_MEM_GET_FILE_SIZE;
		}
		else
		{
			*size = (size_t)(fs.QuadPart);
			maphandle = CreateFileMapping(filehandle, NULL, PAGE_READONLY, 0, 0, NULL);
			if(maphandle == NULL)
			{
				lastError = GetLastError();
				CloseHandle(filehandle);
				filehandle = NULL;
				ret = ERR_MEM_CREATE_MAP;
			}
			else
			{
				address = MapViewOfFileEx(maphandle,FILE_MAP_READ, 0, 0, 0, NULL);
				if(address == NULL)
				{
					lastError = GetLastError();
					CloseHandle(maphandle);
					maphandle = NULL;
					CloseHandle(filehandle);
					filehandle = NULL;
					ret = ERR_MEM_CREATE_VIEW;
				}
			}
		}
	}
#else
#endif
	return ret;
}
int MemoryItem::AllocateMemoryItem(size_t size)
{
	int ret = ERR_OK;
	keepFileOnFree = false;
	readOnly = false;
#ifdef __linux__ 
	void *addr;
	struct stat sb;
	off_t offset=0, pa_offset=0;
	size_t length=size;
	//
	if (filename[0] != 0)
	{
		FILE *fp=fopen(filename, "w");
		ftruncate(fileno(fp), size);
		fclose(fp);
		fd = open(filename, O_RDWR|O_CREAT);
		if (fd == -1)
		{
			ret = ERR_FILE_OPEN;
		}
		else if (fstat(fd, &sb) == -1)           /* To obtain file size */
		{
			close(fd); fd = -1;
			ret = ERR_GET_FILESIZE;
		}
		else
		{
			if((size_t)(sb.st_size) != size)
			{
				close(fd); fd = -1;
				ret = ERR_GET_FILESIZE;
			}
			else
			{
				offset = 0;
				pa_offset = 0;
				/* offset for mmap() must be page aligned */
				//pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

				length = sb.st_size - offset;
			}
		}
	}
	else
	{
		fd = -1;
	}
	if (ret == ERR_OK)
	{
		if(fd == -1)
			addr = mmap(NULL, length + offset - pa_offset, PROT_WRITE | PROT_READ | PROT_EXEC,
			MAP_PRIVATE|MAP_ANONYMOUS, fd, pa_offset);
		else
			addr = mmap(NULL, length + offset - pa_offset, PROT_WRITE | PROT_READ | PROT_EXEC,
				MAP_PRIVATE, fd, pa_offset);
		if (addr == MAP_FAILED)
		{
			close(fd); fd = -1;
			ret = ERR_FILE_MAP;
		}
	}
	if (ret == ERR_OK)
	{
		memSize = length;
		address = addr;
	}

#elif _WIN32
	if (filename[0] != 0)
	{
		wchar_t filenameW[FILENAME_MAX];
		ret = copyC2W(filenameW, FILENAME_MAX, filename);
		if (ret == ERR_OK)
		{
			filehandle = CreateFile(filenameW, GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (filehandle == INVALID_HANDLE_VALUE)
			{
				lastError = GetLastError();
				ret = ERR_MEM_CREATE_FILE;
			}
			else
			{
				DWORD l = LODWORD(size);
				DWORD h = HIDWORD(size);
				LONG hl = (LONG)h;
				DWORD r = SetFilePointer(filehandle, l, &hl, FILE_BEGIN);
				if (r == INVALID_SET_FILE_POINTER)
				{
					lastError = GetLastError();
					CloseHandle(filehandle);
					filehandle = NULL;
					ret = ERR_MEM_CREATE_FILE;
				}
				else
				{
					if (!SetEndOfFile(filehandle))
					{
						lastError = GetLastError();
						CloseHandle(filehandle);
						filehandle = NULL;
						ret = ERR_MEM_CREATE_FILE;
					}
				}
			}
		}
	}
	else
	{
		filehandle = INVALID_HANDLE_VALUE;
	}
	if (ret == ERR_OK)
	{
		if (filehandle == INVALID_HANDLE_VALUE)
			maphandle = CreateFileMapping(filehandle, NULL, PAGE_EXECUTE_READWRITE, 0, (DWORD)size, NULL);
		else
			maphandle = CreateFileMapping(filehandle, NULL, PAGE_EXECUTE_READWRITE, 0, 0, NULL);
		if (maphandle == NULL)
		{
			lastError = GetLastError();
			CloseHandle(filehandle);
			filehandle = NULL;
			ret = ERR_MEM_CREATE_MAP;
		}
		else
		{
			if (filehandle == INVALID_HANDLE_VALUE)
				address = MapViewOfFileEx(maphandle, FILE_MAP_ALL_ACCESS, 0, 0, size, NULL);
			else
				address = MapViewOfFileEx(maphandle, FILE_MAP_ALL_ACCESS, 0, 0, 0, NULL);
			if (address == NULL)
			{
				lastError = GetLastError();
				CloseHandle(maphandle);
				maphandle = NULL;
				if (filehandle != INVALID_HANDLE_VALUE)
					CloseHandle(filehandle);
				filehandle = NULL;
				ret = ERR_MEM_CREATE_VIEW;
			}
		}
	}
#else
#endif
	return ret;
}
/////////////////MemoryManager///////////////////////////////////////////////
MemoryManager::MemoryManager(const char *folder)
{
	count = 0;
	items = NULL;
	if (folder == NULL)
	{
		fileDir[0] = 0;
		useRAM = true;
	}
	else
	{
		SetFolder(folder);
#ifdef __linux__
		useRAM = true;
#endif
		useRAM = false;
	}
}
MemoryManager::~MemoryManager()
{
	Clear();
}
void MemoryManager::Clear()
{
	int i;
	for(i=0;i<count;i++)
	{
		if(items[i] != NULL)
		{
			items[i]->Free();
			items[i] = NULL;
		}
	}
	free(items);
	items = NULL;
	count = 0;
}
void MemoryManager::SetFolder(const char *folder)
{
	int i;
	for(i=0;i<FILENAME_MAX;i++)
	{
		if(folder[i] == 0)
		{
			fileDir[i] = 0;
			break;
		}
		fileDir[i] = folder[i];
	}
}
unsigned long MemoryManager::LastError()
{
	return lastError;
}
void *MemoryManager::mapFileIntoMemory(char *filepath, size_t *size, int *nRet, bool createFile)
{
	int i;
	int ret;
	bool full = true;
	MemoryItem *p = NULL;
	p = new MemoryItem(filepath);
	if(createFile)
	{
		ret = p->AllocateMemoryItem(*size);
		p->KeepFile();
	}
	else
	{
		ret = p->ReadFileIntoMemoryItem(size);
	}
	if(ret != ERR_OK)
	{
		lastError = p->LastError();
		*nRet = ret;
	}
	else
	{
		for(i=0;i<count;i++)
		{
			if(items[i] == NULL)
			{
				items[i] = p;
				full = false;
				break;
			}
		}
		if(full)
		{
			MemoryItem **pp = (MemoryItem **) malloc((count+1)*sizeof(MemoryItem *));
			for(i=0;i<count;i++)
			{
				pp[i] = items[i];
			}
			pp[count] = p;
			if(items != NULL)
			{
				free(items);
			}
			items = pp;
			count ++;
		}
		return p->address;
	}
	return NULL;
}
void *MemoryManager::ReadFileIntoMemoryItem(char *filepath, size_t *size, int *nRet)
{
	return mapFileIntoMemory(filepath, size, nRet, false);
}
void *MemoryManager::CreateFileIntoMemoryItem(char *filepath, size_t size, int *nRet)
{
	return mapFileIntoMemory(filepath, &size, nRet, true);
}
void *MemoryManager::Allocate(size_t size)
{
	retLast = ERR_OK;
	int i;
	bool full = true;
	MemoryItem *p = NULL;
	char  buffer[FILENAME_MAX];
	if (useRAM)
	{
		buffer[0] = 0;
	}
	else
	{
		//find an unused file name
		i = 0;
		while (true)
		{
			retLast = sprintf_1(buffer, FILENAME_MAX, "%s/mp%d.dat", fileDir, i) <=0?ERR_STR_PRF:ERR_OK;
			if (retLast != ERR_OK)
			{
				lastError = ERR_MEM_EINVAL;
				break;
			}
			if (fileexists(buffer))
			{
				i++;
			}
			else
			{
				break;
			}
		}
	}
	if(retLast == ERR_OK)
	{
		p = new MemoryItem(buffer);
		retLast = p->AllocateMemoryItem(size);
		if(retLast != ERR_OK)
		{
			lastError = p->LastError();
		}
		else
		{
			for(i=0;i<count;i++)
			{
				if(items[i] == NULL)
				{
					items[i] = p;
					full = false;
					break;
				}
			}
			if(full)
			{
				MemoryItem **pp = (MemoryItem **) malloc((count+1)*sizeof(MemoryItem *));
				for(i=0;i<count;i++)
				{
					pp[i] = items[i];
				}
				pp[count] = p;
				if(items != NULL)
				{
					free(items);
				}
				items = pp;
				count ++;
			}
			return p->address;
		}
	}
	return NULL;
}
int MemoryManager::Free(void *address)
{
	int i;
	for(i=0;i<count;i++)
	{
		if(items[i] != NULL)
		{
			if(items[i]->address == address)
			{
				items[i]->Free();
				items[i] = NULL;
				return ERR_OK;
			}
		}
	}
	return ERR_MEM_ADDR_NOT_FOUND;
}

int MemoryManager::GetTemporaryFolder(char *folder, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (i == FILENAME_MAX)
		{
			return ERR_STR_SIZE_TOO_SMALL;
		}
		folder[i] = fileDir[i];
		if (fileDir[i] == 0)
		{
			break;
		}
	}
	return ERR_OK;
}
void MemoryManUser::SetMemoryManager(MemoryManager *m)
{
	_mem = m;
}

int MemoryManUser::GetTemporaryFolder(char *folder, int size)
{
	return _mem->GetTemporaryFolder(folder, size);
}

//
/*
	Author: Bob Limnor (bob@limnor.com)
	Last modified: 04/01/2018
	Allrights reserved

	This module provides memory management functions.
	It also provides an interface to other functions specific to operating systems
*/

#include <Windows.h>
#include <FileAPI.h>
#include <stdlib.h>
#include <stdio.h>

#include "..\ProcessMonitor\workProcess.h"
#include "memman.h"
#include "MemoryManager.h"
#include "Shlwapi.h"

unsigned long osErrorCode = 0;

unsigned long GetTimeTick()
{
	return GetTickCount();
}
void RememberOSerror()
{
	if(osErrorCode == 0)
	{
		osErrorCode = GetLastError();
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
		osErrorCode = GetLastError();
	}
	return osErrorCode;
}
bool DirectoryExists(const wchar_t* szPath)
{
  DWORD dwAttrib = GetFileAttributes(szPath);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
bool DirectoryExists_c(const char *szPath, int *ret)
{
	wchar_t wpath[FILENAME_MAX];
	*ret = copyC2W(wpath, FILENAME_MAX, szPath);
	if(*ret == ERR_OK)
	{
		return DirectoryExists(wpath);
	}
	return false;
}
__int64 WinFileSize(const wchar_t* name)
{
    HANDLE hFile = CreateFile(name, GENERIC_READ, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile==INVALID_HANDLE_VALUE)
        return -1; // error condition, could call GetLastError to find out more

    LARGE_INTEGER size;
    if (!GetFileSizeEx(hFile, &size))
    {
        CloseHandle(hFile);
        return -1; // error condition, could call GetLastError to find out more
    }

    CloseHandle(hFile);
    return size.QuadPart;
}
__int64 FileSize(const char* name)
{
	int i;
	wchar_t wname[FILENAME_MAX];
    WIN32_FILE_ATTRIBUTE_DATA fad;
	for(i=0;i<FILENAME_MAX;i++)
	{
		if(name[i] == 0)
		{
			wname[i] = 0;
			break;
		}
		wname[i] = name[i];
	}
    if (!GetFileAttributesEx(wname, GetFileExInfoStandard, &fad))
        return -1; // error condition, could call GetLastError to find out more
    LARGE_INTEGER size;
    size.HighPart = fad.nFileSizeHigh;
    size.LowPart = fad.nFileSizeLow;
    return size.QuadPart;
}
__int64 FileSizeW(wchar_t* wname)
{
    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (!GetFileAttributesEx(wname, GetFileExInfoStandard, &fad))
        return -1; // error condition, could call GetLastError to find out more
    LARGE_INTEGER size;
    size.HighPart = fad.nFileSizeHigh;
    size.LowPart = fad.nFileSizeLow;
    return size.QuadPart;
}
double *parseDecimals(char *name_argv, int *argc, int *ret)
{
	int i,j,k,f;
	double *params = NULL;
	char argv[NAMESIZE];
	//
	*ret = ERR_OK;
	*argc = 0;
	if(name_argv != NULL)
	{
		i = j = 0;
		f = -1;
		argv[0] = 0;
		while(true)
		{
			if(name_argv[i] == ';' || name_argv[i] == 0)
			{
				double a;
				double *dd;
				argv[j] = 0;
				if(j == 0)
					a = 0;
				else
					a = atof(argv);
				*argc = *argc + 1;
				f++;
				dd = (double*)malloc((f+1)*sizeof(double));
				if(f > 0)
				{
					for(k=0;k<f;k++)
					{
						dd[k] = params[k];
					}
					free(params);
				}
				params = dd;
				params[f] = a;
				argv[0] = 0;
				j = 0;
				if(name_argv[i] == 0) break;
			}
			else
			{
				argv[j] = name_argv[i];
				argv[j+1] = 0;
				j++;
				if(j >= NAMESIZE)
				{
					*ret = ERR_INVALID_DLLPARAM;
					if(params != NULL)
					{
						free(params);
					}
					return NULL;
				}
			}
			i++;
		}
	}
	return params;
}
HINSTANCE *libs = NULL;
int libCount = 0;
void *LoadPlugin(wchar_t *libname, char *name_argv, int *ret)
{
	HINSTANCE hinstLib;
	void * func = NULL;
	int i,j,k;
	int f, namelength;
	char name[NAMESIZE];
	double *params = NULL;
	char argv[NAMESIZE];
	int argc = 0;
	//
	*ret = ERR_OK;
	//
	i = 0; j = 0; namelength = 0;
	f = -1;
	while(true)
	{
		if(name_argv[i] == ';' || name_argv[i] == 0)
		{
			double *dd;
			if(f == -1)
			{
				name[j] = 0;
				namelength = j;
			}
			else
			{
				argv[j] = 0;
				params[f] = atof(argv);
				argc++;
			}
			if(name_argv[i] == 0) break;
			f++;
			dd = (double*)malloc((f+1)*sizeof(double));
			if(f > 0)
			{
				for(k=0;k<f;k++)
				{
					dd[k] = params[k];
				}
				free(params);
			}
			params = dd;
			argv[0] = 0;
			j = 0;
		}
		else
		{
			if(f == -1)
			{
				name[j] = name_argv[i];
				name[j+1] = 0;
			}
			else
			{
				argv[j] = name_argv[i];
				argv[j+1] = 0;
			}
			j++;
			if(j >= NAMESIZE)
			{
				*ret = ERR_INVALID_DLLPARAM;
				if(params != NULL)
				{
					free(params);
				}
				return NULL;
			}
		}
		i++;
	}
	if(namelength == 0)
	{
		*ret = ERR_INVALID_DLLPARAM;
		return NULL;
	}
	hinstLib = LoadLibrary(libname);
	if (hinstLib == NULL)
	{
		*ret = ERR_INVALID_DLLPATH;
	}
	else
	{
		fnCreatePluginInstance proc;
		proc = (fnCreatePluginInstance) GetProcAddress(hinstLib, (LPCSTR)"CreatePluginInstance");
		if(proc == NULL)
		{
			*ret = ERR_DLL_FUNC_NOTFOUND;
		}
		else
		{
			func = proc(name, params);
			if(func == NULL)
			{
				FreeLibrary(hinstLib); 
				*ret = ERR_DLL_FUNC_ERROR;
			}
			else
			{
				if(libs == NULL)
				{
					libs = (HINSTANCE *)malloc(sizeof(HINSTANCE));
					libs[0] = hinstLib;
					libCount = 1;
				}
				else
				{
					HINSTANCE *s = (HINSTANCE *)malloc((libCount+1)*sizeof(HINSTANCE));
					for(i=0;i<libCount;i++)
					{
						s[i] = libs[i];
					}
					s[libCount] = hinstLib;
					free(libs);
					libs = s;
					libCount++;
				}
			}
		}
	}
	if(params != NULL)
	{
		free(params);
	}
	return func;
}
void ClearPluginInstances(int *ret)
{
	if(libCount > 0 && libs != NULL)
	{
		int i;
		for(i=0;i<libCount;i++)
		{
			if(libs[i] != NULL)
			{
				fnRemovePluginInstances proc;
				{
					proc = (fnRemovePluginInstances) GetProcAddress(libs[i], (LPCSTR)"RemovePluginInstances");
					if(proc == NULL)
					{
						*ret = ERR_DLL_FUNC_NOTFOUND;
					}
					else
					{
						proc();
					}
				}
				FreeLibrary(libs[i]);
				libs[i] = NULL;
			}
		}
		free(libs);
	}
	libCount = 0;
	libs = NULL;
}
int copyC2W(wchar_t *dest, int size, const char *src)
{
	int i=0;
	size_t s = strlen(src);
	if(s > size)
	{
		return ERR_STR_SIZE_TOO_SMALL;
	}
	for(i=0;i<size;i++)
	{
		if(src[i] == 0)
		{
			dest[i] = 0;
			break;
		}
		else
			dest[i] = src[i];
	}
	if(i >= size)
	{
		return ERR_STR_SIZE_TOO_SMALL;
	}
	else
	{
		return ERR_OK;
	}
}
int catC2W(wchar_t *dest, int size, const char *src)
{
	size_t i0 = wcslen(dest);
	size_t s = strlen(src);
	size_t i=0;
	if(size < i0 + s)
	{
		return ERR_STR_SIZE_TOO_SMALL;
	}
	size -= (int)i0;
	for(i=0;i<size;i++)
	{
		if(src[i] == 0)
		{
			dest[i+i0] = 0;
			break;
		}
		else
			dest[i+i0] = src[i];
	}
	if(i >= size)
	{
		return ERR_STR_SIZE_TOO_SMALL;
	}
	else
	{
		return ERR_OK;
	}
}
int copyW2C(char *dest, int size, const wchar_t *src)
{
	int i;
	for(i=0;i<size;i++)
	{
		if(src[i] == 0)
		{
			dest[i] = 0;
			break;
		}
		else
			dest[i] = (char) src[i];
	}
	if(i >= size)
	{
		return ERR_STR_SIZE_TOO_SMALL;
	}
	else
	{
		return ERR_OK;
	}
}

int WriteToBinaryFile(wchar_t *filename, void *data, size_t itemSize, size_t itemCount, unsigned long *err, fnProgressReport progressReport)
{
	int ret = ERR_OK;
	HANDLE filehandle;
	*err = 0;
	filehandle = CreateFile(filename,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(filehandle == INVALID_HANDLE_VALUE)
	{
		*err = GetLastError();
		ret = ERR_MEM_CREATE_FILE;
	}
	else
	{
		size_t size = itemSize * itemCount;
		DWORD l = LODWORD(size);
		DWORD h = HIDWORD(size);
		LONG hl = (LONG)h;
		DWORD r = SetFilePointer(filehandle, l, &hl, FILE_BEGIN);
		if( r == INVALID_SET_FILE_POINTER)
		{
			*err = GetLastError();
			CloseHandle(filehandle);
			filehandle = NULL;
			ret = ERR_MEM_CREATE_FILE;
		}
		else 
		{
			if(!SetEndOfFile(filehandle))
			{
				*err = GetLastError();
				CloseHandle(filehandle);
				filehandle = NULL;
				ret = ERR_MEM_CREATE_FILE;
			}
			else
			{
				DWORD hw;
				size_t p;
				size_t i;
				BYTE *pdata = (BYTE *)data;
				hw = 0;
				hl = 0;
				r = SetFilePointer(filehandle, 0, &hl, FILE_BEGIN);
				if( r == INVALID_SET_FILE_POINTER)
				{

					*err = GetLastError();
					CloseHandle(filehandle);
					filehandle = NULL;
					ret = ERR_MEM_CREATE_FILE;
				}
				else 
				{ 
					char msg[SHORTMESSAGESIZE];
					size_t pp = 0;
					double pr;
					size_t skip = itemCount / 1000;
					for(i=0,p=0;i<itemCount;i++,p+=itemSize)
					{
						if(!WriteFile(filehandle, &(pdata[p]), (DWORD)itemSize, &hw, NULL ))
						{
							*err = GetLastError();
							ret = ERR_MEM_WRITE_FILE;
							break;
						}
						if(progressReport != NULL)
						{
							pp++;
							if(pp > skip)
							{
								pr = ((double)i/(double)itemCount) * 100.0;
								sprintf_s(msg, SHORTMESSAGESIZE, "written %#.2f %%",pr);
								progressReport(msg, true);
								pp = 0;
							}
						}
					}
					CloseHandle(filehandle);
					if(progressReport != NULL)
					{
						sprintf_s(msg, SHORTMESSAGESIZE, "written 100 %%");
						progressReport(msg, true);
					}
				}
			}
		}
	}
	return ret;
}

bool FileExists(const wchar_t *fileName)
{
	DWORD fileAttr;
	fileAttr = GetFileAttributes(fileName);
	if (0xFFFFFFFF == fileAttr)
		return false;
	return true;
}

int GetAppFolder(char *appfolder, unsigned long size)
{
	int ret = ERR_OK;
	LPTSTR folder = (LPTSTR)malloc(2*size);
	if(folder == NULL)
	{
		ret = ERR_MEM_OUTOFMEMORY;
	}
	else
	{
		DWORD n = GetModuleFileName(NULL, folder, size);
		if(n == 0)
		{
			osErrorCode = GetLastError();
			ret = ERR_MEM_MAN_APP_PATH;
		}
		else if(n == size)
		{
			ret = ERR_STR_SIZE_TOO_SMALL;
		}
		else
		{
			for(int i=n-1;i>0;i--)
			{
				if(folder[i] == '\\' || folder[i] == '/')
				{
					folder[i] = 0;
					break;
				}
			}
			ret = copyW2C(appfolder, size, folder);
		}
		free(folder);
	}
	return ret;
}

int formDataFileName(char *filename, size_t size, const char *basename, size_t timeIndex)
{
	int ret = ERR_OK;
	int err = sprintf_s(filename, size, "%s%d.em", basename, timeIndex);
	if(err <= 0)
	{
		ret = ERR_MEM_EINVAL;
	}
	return ret;
}

int formDataFileNameW(wchar_t *filename, size_t size, const wchar_t *basename, size_t timeIndex)
{
	int ret = ERR_OK;
	int err = swprintf_s(filename, size, L"%s%d.em", basename, timeIndex);
	if(err == -1)
	{
		ret = ERR_MEM_EINVAL;
	}
	return ret;
}
int formFieldFileNameW(wchar_t *filename, size_t size, const wchar_t *basename, size_t timeIndex)
{
	int ret = ERR_OK;
	int err = swprintf_s(filename, size, L"%s%d.field", basename, timeIndex);
	if(err == -1)
	{
		ret = ERR_MEM_EINVAL;
	}
	return ret;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileWriter::FileWriter()
{
	filehandle = NULL;
}
int FileWriter::OpenBinaryFileWrite(wchar_t *filename, unsigned long *err)
{
	int ret = ERR_OK;
	*err = 0;
	filehandle = CreateFile(filename,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(filehandle == INVALID_HANDLE_VALUE)
	{
		*err = GetLastError();
		ret = ERR_MEM_CREATE_FILE;
	}
	return ret;
}
void FileWriter::CloseBineryFile()
{
	if(filehandle != NULL)
	CloseHandle(filehandle);
}
int FileWriter::WriteDataToBineryFile(void *data, size_t itemSize, size_t itemCount)
{
	int ret = ERR_OK;
	DWORD hw;
	size_t p;
	size_t i;
	BYTE *pdata = (BYTE *)data;
	for(i=0,p=0;i<itemCount;i++,p+=itemSize)
	{
		if(!WriteFile(filehandle, &(pdata[p]), (DWORD)itemSize, &hw, NULL ))
		{
			ret = ERR_MEM_WRITE_FILE;
			break;
		}
	}
	return ret;
}

MemoryItem::MemoryItem(LPCWSTR diskfile)
{
	int i;
	for(i=0;i<FILENAME_MAX;i++)
	{
		if(diskfile[i] == 0)
		{
			filename[i] = 0;
			break;
		}
		filename[i] = diskfile[i];
	}
	filehandle = NULL;
	maphandle = NULL;
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
		UnmapViewOfFile(address);
		address = NULL;
	}
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
	if(!keepFileOnFree)
	{
		if(FileExists(filename))
		{
			DeleteFile(filename);
		}
	}
}
MemoryItem::~MemoryItem(void)
{
	Free();
}
DWORD MemoryItem::LastError()
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
	keepFileOnFree = true;
	readOnly = true;
	filehandle = CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
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
	return ret;
}
int MemoryItem::AllocateMemoryItem(size_t size)
{
	int ret = ERR_OK;
	keepFileOnFree = false;
	readOnly = false;
	filehandle = CreateFile(filename,GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(filehandle == INVALID_HANDLE_VALUE)
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
		if( r == INVALID_SET_FILE_POINTER)
		{
			lastError = GetLastError();
			CloseHandle(filehandle);
			filehandle = NULL;
			ret = ERR_MEM_CREATE_FILE;
		}
		else 
		{
			if(!SetEndOfFile(filehandle))
			{
				lastError = GetLastError();
				CloseHandle(filehandle);
				filehandle = NULL;
				ret = ERR_MEM_CREATE_FILE;
			}
			else
			{
				maphandle = CreateFileMapping(filehandle, NULL, PAGE_EXECUTE_READWRITE, 0, 0, NULL);
				if(maphandle == NULL)
				{
					lastError = GetLastError();
					CloseHandle(filehandle);
					filehandle = NULL;
					ret = ERR_MEM_CREATE_MAP;
				}
				else
				{
					address = MapViewOfFileEx(maphandle,FILE_MAP_ALL_ACCESS, 0, 0, 0, NULL);
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
	}
	return ret;
}

MemoryManager::MemoryManager(LPCWSTR folder)
{
	count = 0;
	items = NULL;
	if(folder == NULL)
		fileDir[0] = 0;
	else
	{
		SetFolder(folder);
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
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
void MemoryManager::SetFolder(LPCWSTR folder)
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
DWORD MemoryManager::LastError()
{
	return lastError;
}
void *MemoryManager::mapFileIntoMemory(wchar_t *filepath, size_t *size, int *nRet, bool createFile)
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
void *MemoryManager::ReadFileIntoMemoryItem(wchar_t *filepath, size_t *size, int *nRet)
{
	return mapFileIntoMemory(filepath, size, nRet, false);
}
void *MemoryManager::CreateFileIntoMemoryItem(wchar_t *filepath, size_t size, int *nRet)
{
	return mapFileIntoMemory(filepath, &size, nRet, true);
}
LPVOID MemoryManager::Allocate(size_t size)
{
	int ret = ERR_OK;
	int i;
	int si;
	bool full = true;
	MemoryItem *p = NULL;
	TCHAR  buffer[FILENAME_MAX];
	//find an unused file name
	i = 0;
	while(true)
	{
		si = swprintf_s(buffer, FILENAME_MAX, L"%s\\mp%d.dat",fileDir, i);
		if(si < 0)
		{
			lastError = ERR_MEM_EINVAL;
			ret = ERR_MEM_EINVAL;
			break;
		}
		if(FileExists(buffer))
		{
			i++;
		}
		else
		{
			break;
		}
	}
	if(ret == ERR_OK)
	{
		p = new MemoryItem(buffer);
		ret = p->AllocateMemoryItem(size);
		if(ret != ERR_OK)
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
int MemoryManager::Free(LPVOID address)
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
	return copyW2C(folder, size, fileDir);
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
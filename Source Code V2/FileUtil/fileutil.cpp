/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

Modifications
Date            Author      Description
---------------------------------------------
2021-03-06      David Ge    Linux compatibility
********************************************************************/

#include "fileutil.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h> 
#include <stdio.h>
#include <string.h>
#include <cstdio>
#include <cstdarg>
#include <malloc.h>
#include <stdlib.h>
#include <wchar.h>

#ifdef __linux__ 

#include <unistd.h>
#include <time.h>
#include <sys/resource.h>

unsigned long getTimeTick() {
	//taken from https://stackoverflow.com/questions/2958291/equivalent-to-gettickcount-on-linux
	struct timespec ts;
	unsigned long theTick = 0U;
	clock_gettime(CLOCK_REALTIME, &ts);
	theTick = ts.tv_nsec / 1000000;
	theTick += ts.tv_sec * 1000;
	return theTick;
}

bool setProcessHighPtiority()
{
	int ret;
	int which = PRIO_PROCESS;
	id_t pid;
	int priority = -20;
	//
	pid = getpid();
	ret = setpriority(which, pid, priority);
	return (ret == 0);
}

bool directoryExists(const char *dir)
{
	struct stat st;
	return (stat(dir, &st) == 0 && S_ISDIR(st.st_mode));
}
bool createdirectory(const char *dir)
{
	int status = mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	return (status == 0);
}
bool fileexists(const char *fileName)
{
	struct stat st;   
	return (stat (fileName, &st) == 0); 
}
bool FileExists(const wchar_t *fileName)
{
	char fileA[FILENAME_MAX];
	if(copyW2C(fileA,FILENAME_MAX,fileName) == ERR_OK)
	{
		return fileexists(fileA);
	}
	return false;
}
long long int fileSize(const char* fileName)
{
	struct stat st;
	int rc = stat(fileName, &st);
	return rc == 0 ? st.st_size : -1;
}

int GetCurrentFolder(char *appfolder, unsigned long size)
{
	char *buff = getcwd(appfolder, size);
	if (buff == NULL)
	{
		return ERR_DIR_WORK;
	}
	return ERR_OK;
}
//
FILE *fopen_0(const char *file, const char *mode)
{
	return fopen(file, mode);
}
int strcpy_0(char *dest, size_t size, const char *src)
{
	char *c = strcpy(dest, src);
	if(c == NULL)
	{
		return ERR_STR_CPY;
	}
	return ERR_OK;
}
int strcat_0(char *dest, size_t size, const char *src)
{
	char *c = strcat(dest, src);
	if (c == NULL)
	{
		return ERR_STR_CAT;
	}
	return ERR_OK;
}
int strcmpi_0(const char * str1, const char * str2)
{
	return strcasecmp(str1,str2);
}
//int sprintf_0(char *buff, size_t size, const char *format, ...)
//{
//	va_list a_list;
//
//	va_start(a_list, format);
//	int wi = snprintf(buff, format, a_list);
//	if (wi <= 0)
//	{
//		return ERR_STR_PRF;
//	}
//	return ERR_OK;
//}
size_t strnlen_0(char *buff, size_t size)
{
	if(buff == NULL)
	{
		return 0;
	}
	for(size_t i=0;i<size;i++)
	{
		if(buff[i] == 0)
		{
			return i;
		}
	}
	return size;
}
#elif _WIN32
#include <Windows.h>
#include <Shlwapi.h>
#include <direct.h>
unsigned long getTimeTick()
{
	return GetTickCount();
}
bool setProcessHighPtiority()
{
	if (SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS))
		return true;
	return false;
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
	if (*ret == ERR_OK)
	{
		return DirectoryExists(wpath);
	}
	return false;
}

bool directoryExists(const char *dir)
{
	int ret = 0;
	bool b = DirectoryExists_c(dir, &ret);
	if (ret == ERR_OK)
	{
		return b;
	}
	return false;
}
bool createdirectory(const char *dir)
{
	if (CreateDirectoryA(dir, NULL))
		return true;
	return false;
}
bool fileexists(const char *fileName)
{
	wchar_t fW[FILENAME_MAX];
	int ret = copyC2W(fW, FILENAME_MAX, fileName);
	if (ret == ERR_OK)
	{
		return FileExists(fW);
	}
	return false;
}
bool FileExists(const wchar_t *fileName)
{
	DWORD fileAttr;
	fileAttr = GetFileAttributes(fileName);
	if (0xFFFFFFFF == fileAttr)
		return false;
	return true;
}
long long int fileSize(const char* name)
{
	int i;
	wchar_t wname[FILENAME_MAX];
	WIN32_FILE_ATTRIBUTE_DATA fad;
	for (i = 0; i<FILENAME_MAX; i++)
	{
		if (name[i] == 0)
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
/*
int copyfile(const char *filename, char *tofile)
{
	int ret = ERR_OK;
	wchar_t filenameW[FILENAME_MAX];
	wchar_t toFileW[FILENAME_MAX];
	copyC2W(filenameW, FILENAME_MAX, filename);
	copyC2W(toFileW, FILENAME_MAX, tofile);
	if (!CopyFileW(filenameW, toFileW, true))
	{
		ret = ERR_COPYFILE;
	}
	return ret;
}
*/
int GetCurrentFolder(char *appfolder, unsigned long size)
{
	char *buff = _getcwd(appfolder, size);
	if (buff == NULL)
	{
		return ERR_DIR_WORK;
	}
	return ERR_OK;
}

FILE *fopen_0(const char *file, const char *mode)
{
	FILE *h = 0;
	errno_t err = fopen_s(&h, file, mode);
	if (err == 0)
	{
		return h;
	}
	return NULL;
}
int strcpy_0(char *dest, size_t size, const char *src)
{
	errno_t err = strcpy_s(dest, size, src);
	if (err == -1)
	{
		return ERR_STR_CPY;
	}
	return ERR_OK;
}
int strcat_0(char *dest, size_t size, const char *src)
{
	errno_t err = strcat_s(dest, size, src);
	if (err == -1)
	{
		return ERR_STR_CAT;
	}
	return ERR_OK;
}
int strcmpi_0(const char * str1, const char * str2)
{
	return _strcmpi(str1, str2);
}
//int sprintf_0(char *buff, size_t size, const char *format, ...)
//{
//	va_list a_list;
//
//	va_start ( a_list, format ); 
//	int wi = sprintf_s(buff, size, format, a_list);
//	if (wi <= 0)
//	{
//		return ERR_STR_PRF;
//	}
//	return ERR_OK;
//}
size_t strnlen_0(char *buff, size_t size)
{
	return strnlen_s(buff, size);
}
#else

#endif

////////////////////////////////////////////////////////////////////////////////////////////////
/*
	merge file path and file name to form a fule file path and name
*/
int formFilePath(char *filepath, size_t size, const char *path, const char *name)
{
	int ret = ERR_OK;
	//errno_t err;
	if(name != NULL && strlen(name) > 0)
	{
		size_t n = strlen(path);
		if(path != NULL && n > 0)
		{
			ret = strcpy_0(filepath, size, path);
			if(ret != ERR_OK)
			{
				ret = ERR_FILENAME_LONG;
			}
			else
			{
				if(path[n-1] != '\\' && path[n-1] != '/')
				{
					ret = strcat_0(filepath, size, "/");
					if(ret != ERR_OK)
					{
						ret = ERR_FILENAME_LONG;
					}
				}
				if(ret == ERR_OK)
				{
					ret = strcat_0(filepath, size, name);
					if(ret != ERR_OK)
					{
						ret = ERR_FILENAME_LONG;
					}
				}
			}
		}
		else
		{
			ret = strcpy_0(filepath, size, name);
			if(ret != ERR_OK)
			{
				ret = ERR_FILENAME_LONG;
			}
		}
	}
	else
	{
		ret = ERR_FILENAME_MISSING;
	}
	return ret;
}

int getFilename(const char *filepath, char * filename, size_t targetSize)
{
	int ret = ERR_OK;
	size_t i;
	size_t sl = strlen(filepath);
	for(i = sl-1;i>=0;i--)
	{
		if(filepath[i] == '\\' || filepath[i] == ':' || filepath[i] == '/')
		{
			break;
		}
	}
	i++;
	if(i >= sl)
	{
		ret = ERR_FILE_NAMING;
	}
	else
	{
		size_t k = 0;
		while(i<sl)
		{
			filename[k] = filepath[i];
			k++;
			i++;
			if(k >= targetSize)
			{
				ret = ERR_FILENAME_LONG;
				break;
			}
		}
		if(ret == ERR_OK)
		{
			filename[k] = 0;
		}
	}
	return ret;
}
int openfileRead(const char* file, FILE** handle)
{
	*handle = fopen_0(file, "rb");
	if (*handle == NULL)
	{
		return ERR_FILE_OPEN_READ_EINVAL;
	}
	return 0;
}
int openfileWrite(const char *file, FILE **handle)
{
	*handle = fopen_0(file, "wb");
	if (*handle == NULL)
	{
		return ERR_FILE_OPEN_WRIT_EINVAL;
	}
	return ERR_OK;
}

int writefile(FILE *handle, const void* data, unsigned int size)
{
	size_t w = fwrite(data, 1, (size_t)size, handle);
	if (w != (size_t)size)
		return ERR_FILE_WRITE_LESS;
	return ERR_OK;
}
int readfile(FILE *handle, void *data, unsigned int size)
{
	size_t r = fread(data, 1, (size_t)size, handle);
	if (r < (size_t)size)
	{
		if (r == 0)
		{
			return ERR_FILE_READ_EOF;
		}
		return ERR_FILE_READ_LESS;
	}
	return ERR_OK;
}
void closefile(FILE *handle)
{
	fclose(handle);
}

int WriteToFile(char *filename, void* data, size_t itemSize, size_t count, int *errCode)
{
	int ret = ERR_OK;
	FILE *stream;
	stream = fopen_0(filename, "wb");
	if (stream == NULL)
	{
		*errCode = 1;
		ret = ERR_FILE_OPEN;
	}
	else
	{
		size_t written = fwrite(data, itemSize, count, stream);
		fclose( stream );
		*errCode = 0;
		if(written < count)
		{
			ret = ERR_FILE_WRITE_LESS;
		}
	}
	return ret;
}
int openTextfileWrite(const char *file, FILE **handle)
{
	*handle = fopen_0(file, "w");
	if (*handle == NULL)
	{
		return ERR_FILE_OPEN_WRIT_EINVAL;
	}
	return ERR_OK;
}
int openTextfileAppend(const char *file, FILE **handle)
{
	*handle = fopen_0(file, "a");
	if (*handle == NULL)
	{
		return ERR_FILE_OPEN_WRIT_EINVAL;
	}
	return ERR_OK;
}
//
int openTextfileRead(const char *file, FILE **handle)
{
	*handle = fopen_0(file, "r");
	if (*handle == NULL)
	{
		return ERR_FILE_OPEN_READ_EINVAL;
	}
	return ERR_OK;
}

int deleteFile(const char * file)
{
	if (fileexists(file))
	{
		int ret = remove(file);
		if (ret != 0)
		{
			return ERR_FILE_CANNOT_DELETE;
		}
	}
	return ERR_OK;
}

int copyC2W(wchar_t *dest, size_t size, const char *src)
{
	size_t i = 0;
	size_t s = strlen(src);
	if (s > size)
	{
		return ERR_STR_SIZE_TOO_SMALL;
	}
	for (i = 0; i<size; i++)
	{
		if (src[i] == 0)
		{
			dest[i] = 0;
			break;
		}
		else
			dest[i] = src[i];
	}
	if (i >= size)
	{
		return ERR_STR_SIZE_TOO_SMALL;
	}
	else
	{
		return ERR_OK;
	}
}

int catC2W(wchar_t *dest, size_t size, const char *src)
{
	size_t i0 = wcslen(dest);
	size_t s = strlen(src);
	size_t i = 0;
	if (size < i0 + s)
	{
		return ERR_STR_SIZE_TOO_SMALL;
	}
	size -= i0;
	for (i = 0; i<size; i++)
	{
		if (src[i] == 0)
		{
			dest[i + i0] = 0;
			break;
		}
		else
			dest[i + i0] = src[i];
	}
	if (i >= size)
	{
		return ERR_STR_SIZE_TOO_SMALL;
	}
	else
	{
		return ERR_OK;
	}
}
int copyW2C(char *dest, size_t size, const wchar_t *src)
{
	size_t i;
	for (i = 0; i<size; i++)
	{
		if (src[i] == 0)
		{
			dest[i] = 0;
			break;
		}
		else
			dest[i] = (char)src[i];
	}
	if (i >= size)
	{
		return ERR_STR_SIZE_TOO_SMALL;
	}
	else
	{
		return ERR_OK;
	}
}

int formDataFileName(char *filename, size_t size, const char *basename, size_t timeIndex)
{
#ifdef __linux__
	return snprintf(filename, size, "%s%Ilu.em", basename, timeIndex) <=0?ERR_STR_PRF:ERR_OK;
#elif _WIN32
	return sprintf_s(filename, size, "%s%Iu.em", basename, timeIndex) <=0?ERR_STR_PRF:ERR_OK;
#else
	return 0;
#endif
	
	
}

int formFieldFileName(char *filename, size_t size, const char *basename, size_t timeIndex)
{
	
#ifdef __linux__
	return snprintf(filename, size, "%s%Ilu.field", basename, timeIndex) <=0?ERR_STR_PRF:ERR_OK;
#elif _WIN32
	return sprintf_s(filename, size, "%s%Iu.field", basename, timeIndex) <=0?ERR_STR_PRF:ERR_OK;
#else
	return 0;
#endif
	
}
#define NAMESIZE 100
double *parseDecimals(char *name_argv, int *argc, int *ret)
{
	int i, j, k, f;
	double *params = NULL;
	char argv[NAMESIZE];
	//
	*ret = ERR_OK;
	*argc = 0;
	if (name_argv != NULL)
	{
		i = j = 0;
		f = -1;
		argv[0] = 0;
		while (true)
		{
			if (name_argv[i] == ';' || name_argv[i] == 0)
			{
				double a;
				double *dd;
				argv[j] = 0;
				if (j == 0)
					a = 0;
				else
					a = atof(argv);
				*argc = *argc + 1;
				f++;
				dd = (double*)malloc((f + 1)*sizeof(double));
				if (f > 0)
				{
					for (k = 0; k<f; k++)
					{
						dd[k] = params[k];
					}
					free(params);
				}
				params = dd;
				params[f] = a;
				argv[0] = 0;
				j = 0;
				if (name_argv[i] == 0) break;
			}
			else
			{
				argv[j] = name_argv[i];
				argv[j + 1] = 0;
				j++;
				if (j >= NAMESIZE)
				{
					*ret = ERR_STR_SIZE_TOO_SMALL;
					if (params != NULL)
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




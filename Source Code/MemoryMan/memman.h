#ifndef __MEMMAN_H__
#define __MEMMAN_H__
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/
#include "..\ProcessMonitor\workProcess.h"

#define ERR_OK                 0
#define ERR_MEM_CREATE_FILE    6001
#define ERR_MEM_CREATE_MAP     6002
#define ERR_MEM_CREATE_VIEW    6003
#define ERR_MEM_ADDR_NOT_FOUND 6004
#define ERR_MEM_EINVAL         6005
#define ERR_MEM_WRITE_FILE     6006
#define ERR_MEM_OPEN_FILE      6007
#define ERR_INVALID_DLLPARAM   6008
#define ERR_INVALID_DLLPATH    6009
#define ERR_DLL_FUNC_NOTFOUND  6010
#define ERR_DLL_FUNC_ERROR     6011
#define ERR_INVALID_PARAMS     6012
#define ERR_MEM_GET_FILE_SIZE  6013
#define ERR_STR_SIZE_TOO_SMALL 6014
#define ERR_FOLDER_NOTEXIST    6015
#define ERR_MEM_MAN_NULL       6016
#define ERR_MEM_MAN_APP_PATH   6017
#define ERR_MEM_OUTOFMEMORY    6018
#define ERR_MEM_DIR_NOTEXIST   6019
#define ERR_MEM_UNKNOWN        6030


#define LODWORD(l)           ((DWORD)(((size_t)(l)) & 0xffffffff))
#define HIDWORD(l)           ((DWORD)((((size_t)(l)) >> 32) & 0xffffffff))

extern unsigned long osErrorCode;

class FileWriter
{
private:
	void *filehandle;
public:
	FileWriter();
	int OpenBinaryFileWrite(wchar_t *filename, unsigned long *err);
	void CloseBineryFile();
	int WriteDataToBineryFile(void *data, size_t itemSize, size_t itemCount);

};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

unsigned long GetTimeTick();
void RememberOSerror();
void ClearOSerror();
unsigned long GetLastWinError();
bool DirectoryExists(const wchar_t* szPath);
bool DirectoryExists_c(const char *szPath, int *ret);
int copyC2W(wchar_t *dest, int size, const char *src);
int catC2W(wchar_t *dest, int size, const char *src);
int copyW2C(char *dest, int size, const wchar_t *src);
int WriteToBinaryFile(wchar_t *filename, void *data, size_t itemSize, size_t itemCount, unsigned long *err, fnProgressReport progressReport);

__int64 WinFileSize(const wchar_t* name);
__int64 FileSize(const char* name);
__int64 FileSizeW(wchar_t* wname);
bool FileExists(const wchar_t *fileName);

void *LoadPlugin(wchar_t *libname, char *name_argv, int *ret);
void ClearPluginInstances(int *ret);

double *parseDecimals(char *name_argv, int *argc, int *ret);
int GetAppFolder(char *appfolder, unsigned long size);

int formDataFileName(char *filename, size_t size, const char *basename, size_t timeIndex);
int formDataFileNameW(wchar_t *filename, size_t size, const wchar_t *basename, size_t timeIndex);
int formFieldFileNameW(wchar_t *filename, size_t size, const wchar_t *basename, size_t timeIndex);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility

********************************************************************/
#include <cstdio>

#define _DEBUG_DETAIL_

#define ERR_OK 0

#define ERR_FILE_OPEN_READ_EACCES   1001
#define ERR_FILE_OPEN_READ_EEXIST   1002
#define ERR_FILE_OPEN_READ_EINVAL   1003
#define ERR_FILE_OPEN_READ_EMFILE   1004
#define ERR_FILE_OPEN_READ_ENOENT   1005
#define ERR_FILE_OPEN_READ_UNDEFI   1006

#define ERR_FILE_OPEN_WRIT_EACCES   1011
#define ERR_FILE_OPEN_WRIT_EEXIST   1012
#define ERR_FILE_OPEN_WRIT_EINVAL   1013
#define ERR_FILE_OPEN_WRIT_EMFILE   1014
#define ERR_FILE_OPEN_WRIT_ENOENT   1015
#define ERR_FILE_OPEN_WRIT_UNDEFI   1016

#define ERR_FILE_WRITE_FAIL    1020
#define ERR_FILE_WRITE_LESS    1021
#define ERR_FILE_CANNOT_DELETE 1022

#define ERR_FILE_READ_FAIL     1030
#define ERR_FILE_READ_LESS     1031
#define ERR_FILE_READ_EOF      1032        

#define ERR_FILE_OPEN          1040

#define ERR_GET_FILESIZE       1050
#define ERR_FILE_NOTEXIST      1051
#define ERR_FILE_NAMING        1052
#define ERR_FILENAME_LONG      1053
#define ERR_FILESIZE_MISMATCH  1054
#define ERR_FILENAME_MISSING   1055

#define ERR_STR_CPY            1056
#define ERR_STR_CAT            1057
#define ERR_STR_PRF            1058
#define ERR_STR_SIZE_TOO_SMALL 1059

#define ERR_DIR_WORK           1070
#define ERR_FILE_MAP           1071

//the first error code for exceptions
#define ERR_CODE_EXCEPT_START     30000
//the last unused error code for exceptions.
//every time a new exception code is needed, use this code and update this code by 1 
#define ERR_CODE_EXCEPT_LAST      30002


int formFilePath(char *filepath, size_t size, const char *path, const char *name);
int getFilename(const char *filepath, char * filename, size_t targetSize);
int WriteToFile(char *filename, void* data, size_t itemSize, size_t count, int *errCode);

int openfileRead(const char *file, FILE **handle);
int openfileWrite(const char *file, FILE **handle);
int openTextfileWrite(const char *file, FILE **handle);
int openTextfileAppend(const char *file, FILE **handle);
int openTextfileRead(const char *file, FILE **handle);
int writefile(FILE *handle, const void *data, unsigned int size);
int readfile(FILE *handle, void *data, unsigned int size);
void closefile(FILE *handle);

int deleteFile(const char * file);

bool directoryExists(const char *dir);
bool createdirectory(const char *dir);
bool fileexists(const char *fileName);
bool FileExists(const wchar_t *fileName);
long long int fileSize(const char* name);

int GetCurrentFolder(char *appfolder, unsigned long size);

int formDataFileName(char *filename, size_t size, const char *basename, size_t timeIndex);
int formFieldFileName(char *filename, size_t size, const char *basename, size_t timeIndex);

double *parseDecimals(char *name_argv, int *argc, int *ret);

int copyC2W(wchar_t *dest, size_t size, const char *src);
int catC2W(wchar_t *dest, size_t size, const char *src);
int copyW2C(char *dest, size_t size, const wchar_t *src);

int strcpy_0(char *dest, size_t size, const char *src);
int strcat_0(char *dest, size_t size, const char *src);
int strcmpi_0(const char * str1, const char * str2);
//int sprintf_0(char *buff, size_t size, const char *format, ...);
size_t strnlen_0(char *buff, size_t size);

unsigned long getTimeTick();
bool setProcessHighPtiority();

//
//#ifdef __linux__
//#define sprintf_1(buff, size, format, ...) (snprintf(buff, size, format, ...)<=0?0:ERR_STR_PRF)
//#elif _WIN32
//#define sprintf_1(buff, size, format, ...) (sprintf_s(buff, size, format, ...)<=0?0:ERR_STR_PRF)
//#else
//#endif

#ifdef __linux__
#define sprintf_1 snprintf
#elif _WIN32
#define sprintf_1 sprintf_s
#else
#endif
#endif
/*
#ifdef __linux__

#elif _WIN32

#else
#endif
<=0?ERR_STR_PRF:ERR_OK
*/
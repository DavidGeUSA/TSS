#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
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

#define ERR_FILE_READ_FAIL     1030
#define ERR_FILE_READ_LESS     1031
#define ERR_FILE_READ_EOF      1032        

#define ERR_FILE_OPEN     1040

#define ERR_GET_FILESIZE  1050
#define ERR_FILE_NOTEXIST 1051
#define ERR_FILE_NAMING   1052
#define ERR_FILENAME_LONG 1053
#define ERR_FILESIZE_MISMATCH 1054
#define ERR_FILENAME_MISSING  1055

int formFilePath(char *filepath, size_t size, const char *path, const char *name);
int getFilename(const char *filepath, char * filename, size_t targetSize);
int WriteToFile(char *filename, void* data, size_t itemSize, size_t count, int *errCode);

int openfileRead(const char *file, int *handle);
int openfileWrite(const char *file, int *handle);
int openTextfileWrite(const char *file, int *handle);
int openTextfileAppend(const char *file, int *handle);
int openTextfileRead(const char *file, int *handle);
int writefile(int handle, const void *data, unsigned int size);
int readfile(int handle, void *data, unsigned int size);
void closefile(int handle);

#endif

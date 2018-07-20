/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/
//#include "stdafx.h"
#include "fileutil.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <share.h>
#include <io.h>
#include <errno.h> 
#include <stdio.h>
#include <string.h>

/*
	merge file path and file name to form a fule file path and name
*/
int formFilePath(char *filepath, size_t size, const char *path, const char *name)
{
	int ret = ERR_OK;
	errno_t err;
	if(name != NULL && strlen(name) > 0)
	{
		size_t n = strlen(path);
		if(path != NULL && n > 0)
		{
			err = strcpy_s(filepath, size, path);
			if(err == -1)
			{
				ret = ERR_FILENAME_LONG;
			}
			else
			{
				if(path[n-1] != '\\' && path[n-1] != '/')
				{
					err = strcat_s(filepath, size, "\\");
					if(err == -1)
					{
						ret = ERR_FILENAME_LONG;
					}
				}
				if(ret == ERR_OK)
				{
					err = strcat_s(filepath, size, name);
					if(err == -1)
					{
						ret = ERR_FILENAME_LONG;
					}
				}
			}
		}
		else
		{
			err = strcpy_s(filepath, size, name);
			if(err == -1)
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
int openfileRead(const char* file, int* handle)
{
	errno_t err = _sopen_s(handle, file, _O_RDONLY | _O_SEQUENTIAL | _O_BINARY, _SH_DENYRW, _S_IREAD);
	if (err)
	{
		switch (err)
		{
		case EACCES:
			return ERR_FILE_OPEN_READ_EACCES;
		case EEXIST:
			return ERR_FILE_OPEN_READ_EEXIST;
		case EINVAL:
			return ERR_FILE_OPEN_READ_EINVAL;
		case EMFILE:
			return ERR_FILE_OPEN_READ_EMFILE;
		case ENOENT:
			return ERR_FILE_OPEN_READ_ENOENT;
		default:
			return ERR_FILE_OPEN_READ_UNDEFI;
		}
	}
	return 0;
}
int openfileWrite(const char *file, int *handle)
{
	errno_t err =  _sopen_s(handle, file, _O_WRONLY | _O_CREAT | _O_BINARY, _SH_DENYRW, _S_IWRITE);
	if (err)
	{
		switch (err)
		{
		case EACCES:
			return ERR_FILE_OPEN_WRIT_EACCES;
		case EEXIST:
			return ERR_FILE_OPEN_WRIT_EEXIST;
		case EINVAL:
			return ERR_FILE_OPEN_WRIT_EINVAL;
		case EMFILE:
			return ERR_FILE_OPEN_WRIT_EMFILE;
		case ENOENT:
			return ERR_FILE_OPEN_WRIT_ENOENT;
		default:
			return ERR_FILE_OPEN_WRIT_UNDEFI;
		}
	}
	return ERR_OK;
}

int writefile(int handle, const void* data, unsigned int size)
{
	int ret = _write(handle, data, size);
	if(ret < 0)
	{
		return ERR_FILE_WRITE_FAIL;
	}
	if((unsigned int)ret != size)
		return ERR_FILE_WRITE_LESS;
	return ERR_OK;
}
int readfile(int handle, void *data, unsigned int size)
{
	int ret = _read(handle, data, size);
	if( ret < 0)
	{
		return ERR_FILE_READ_FAIL;
	}
	if((unsigned int)ret < size)
	{
		if(ret == 0)
			return ERR_FILE_READ_EOF;
		return ERR_FILE_READ_LESS;
	}
	return ERR_OK;
}
void closefile(int handle)
{
	_close(handle);
}

int WriteToFile(char *filename, void* data, size_t itemSize, size_t count, int *errCode)
{
	int ret = ERR_OK;
	FILE *stream;
	errno_t err = fopen_s(&stream, filename, "wb");
	if(err != 0)
	{
		*errCode = err;
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
int openTextfileWrite(const char *file, int *handle)
{
	errno_t err =  _sopen_s(handle, file, _O_WRONLY | _O_CREAT | _O_TEXT, _SH_DENYRW, _S_IWRITE);
	if (err)
	{
		switch (err)
		{
		case EACCES:
			return ERR_FILE_OPEN_WRIT_EACCES;
		case EEXIST:
			return ERR_FILE_OPEN_WRIT_EEXIST;
		case EINVAL:
			return ERR_FILE_OPEN_WRIT_EINVAL;
		case EMFILE:
			return ERR_FILE_OPEN_WRIT_EMFILE;
		case ENOENT:
			return ERR_FILE_OPEN_WRIT_ENOENT;
		default:
			return ERR_FILE_OPEN_WRIT_UNDEFI;
		}
	}
	return ERR_OK;
}
int openTextfileAppend(const char *file, int *handle)
{
	errno_t err =  _sopen_s(handle, file, _O_WRONLY | _O_CREAT |_O_APPEND| _O_TEXT, _SH_DENYRW, _S_IWRITE);
	if (err)
	{
		switch (err)
		{
		case EACCES:
			return ERR_FILE_OPEN_WRIT_EACCES;
		case EEXIST:
			return ERR_FILE_OPEN_WRIT_EEXIST;
		case EINVAL:
			return ERR_FILE_OPEN_WRIT_EINVAL;
		case EMFILE:
			return ERR_FILE_OPEN_WRIT_EMFILE;
		case ENOENT:
			return ERR_FILE_OPEN_WRIT_ENOENT;
		default:
			return ERR_FILE_OPEN_WRIT_UNDEFI;
		}
	}
	return ERR_OK;
}
//
int openTextfileRead(const char *file, int *handle)
{
	errno_t err =  _sopen_s(handle, file, _O_RDONLY | _O_TEXT, _SH_DENYNO, _S_IREAD);
	if (err)
	{
		switch (err)
		{
		case EACCES:
			return ERR_FILE_OPEN_READ_EACCES;
		case EEXIST:
			return ERR_FILE_OPEN_READ_EEXIST;
		case EINVAL:
			return ERR_FILE_OPEN_READ_EINVAL;
		case EMFILE:
			return ERR_FILE_OPEN_READ_EMFILE;
		case ENOENT:
			return ERR_FILE_OPEN_READ_ENOENT;
		default:
			return ERR_FILE_OPEN_READ_UNDEFI;
		}
	}
	return ERR_OK;
}



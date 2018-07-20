
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/

#include "taskFile.h"
#include "fileutil.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//array size increment
#define BLOCKSIZE 10

/*
	this constructor reads parameters in the task file into array _lines
*/
TaskFile::TaskFile(const char *taskfile)
{
	int i,k;
	string line;
	ifstream file(taskfile);
	ret = ERR_OK;
	unfoundName = NULL;
	nameBadVal = NULL;
	_lines = NULL;
	count = 0;
	lineArraySize = 0;
	if(file.is_open())
	{
		while(file.good())
		{
			std::getline(file, line);
			if(line.length() > 0)
			{
				if(line.length() > 1)
				{
					if(line[0] == '/' && line[1] == '/')
					{
						//it is a comment
						continue;
					}
				}
				if(count >= lineArraySize)
				{
					allocateLines();
					if(ret != ERR_OK)
					{
						break;
					}
				}
				for(i=0;i<line.length();i++)
				{
					if(line[i] == '=')
					{
						break;
					}
				}
				if(i > 0 && i < line.length())
				{
					_lines[count].name = (char *)malloc(i+1);
					if(_lines[count].name == NULL)
					{
						ret = ERR_OUTOFMEMORY;
						break;
					}
					for(k=0;k<i;k++)
					{
						_lines[count].name[k] = line[k];
					}
					_lines[count].name[i] = 0;
					if(i + 1 < line.length())
					{
						_lines[count].value = (char *)malloc(line.length() - i);
						if(_lines[count].value == NULL)
						{
							ret = ERR_OUTOFMEMORY;
							break;
						}
						for(k=i+1;k<line.length();k++)
						{
							_lines[count].value[k-i-1] = line[k];
						}
						_lines[count].value[line.length()-i-1] = 0;
						count++;
					}
				}
				else
				{
					if(i == 0)
					{
						ret = ERR_TASKFIILE_INVALID;
						break;
					}
				}
			}
		}
	}
	else
	{
		ret = ERR_TASKFIILE_NOTEXIST;
	}
	file.close();
}

/*
	free the memory taken by _lines
*/
TaskFile::~TaskFile()
{
	if(unfoundName != NULL)
	{
		free(unfoundName);
	}
	if(nameBadVal != NULL)
	{
		free(nameBadVal);
	}
	if(_lines != NULL)
	{
		for(int i=0;i<count;i++)
		{
			if(_lines[i].name != NULL)
			{
				free(_lines[i].name);
			}
			if(_lines[i].value != NULL)
			{
				free(_lines[i].value);
			}
		}
		free(_lines);
	}
}

/*
	after calling get???(name, false), ret can be ERR_TASK_PARAMETER_NAME if the "name" does not exist.
	call this function to reset ret to ERR_OK
*/
void TaskFile::resetErrorCode(void)
{
	ret = ERR_OK;
}

/*
	increase array size of _lines by BLOCKSIZE
*/
void TaskFile::allocateLines()
{
	if(lineArraySize < count + BLOCKSIZE)
	{
		lineArraySize += BLOCKSIZE;
		NameValue * lines = (NameValue *)malloc(sizeof(NameValue) * lineArraySize);
		if(lines == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		else
		{
			for(int i=0;i<lineArraySize;i++)
			{
				lines[i].name = NULL;
				lines[i].value = NULL;
			}
			if(_lines != NULL)
			{
				for(int i=0;i<count;i++)
				{
					lines[i].name = _lines[i].name;
					lines[i].value = _lines[i].value;
				}
				free(_lines);
			}
			_lines = lines;
		}
	}
}

void TaskFile::setNameOfInvalidValue(const char *name)
{
	if(nameBadVal != NULL)
	{
		size_t sz = strlen(name);
		if(sz > 0)
		{
			nameBadVal = (char *)malloc(sz+1);
			strcpy_s(nameBadVal, sz+1, name);
		}
	}
}


/*
	search the "name" in the parameter array;
	returns the array index if the name is found;
	returns -1 if the name is not found.
*/
int TaskFile::getIndexByName(const char *name)
{
	int n = -1;
	if(ret == ERR_OK)
	{
		if(_lines != NULL)
		{
			for(int i=0;i<count;i++)
			{
				if(strcmp(name, _lines[i].name) == 0)
				{
					if(n != -1)
					{
						ret = ERR_TASKFIILE_DUPLICATE;
						break;
					}
					else
					{
						n = i;
					}
				}
			}
		}
		if(n == -1)
		{
			if(unfoundName != NULL)
			{
				free(unfoundName);
				unfoundName = NULL;
			}
			size_t sz = strlen(name);
			if(sz > 0)
			{
				unfoundName = (char *)malloc(sz+1);
				strcpy_s(unfoundName, sz+1, name);
			}
			ret = ERR_TASK_PARAMETER_NAME;
		}
	}
	return n;
}

/*
	these functions read task parameter values by name
*/
double TaskFile::getDouble(const char *name, bool optional)
{
	int n = getIndexByName(name);
	if(ret == ERR_OK)
	{
		if(n >= 0)
		{
			if(_lines[n].value != NULL)
			{
				char *q = strchr(_lines[n].value, '/');
				if(q == NULL)
				{
					return atof(_lines[n].value);
				}
				else
				{
					double qv = atof( (q+1) );
					if(qv == 0.0)
					{
						ret = ERR_TASK_INVALID_VALUE;
						setNameOfInvalidValue(name);
						return 0.0;
					}
					else
					{
						double v;
						size_t sz = strlen(_lines[n].value);
						char *d = (char *)malloc(sz+1);
						for(size_t k=0;k<sz;k++)
						{
							if(_lines[n].value[k] == '/')
							{
								d[k] = 0;
								break;
							}
							d[k] = _lines[n].value[k];
						}
						v = atof(d) / qv;
						free(d);
						return v;
					}
				}
			}
		}
	}
	if(optional)
	{
		if(ret == ERR_TASK_PARAMETER_NAME)
		{
			ret = ERR_OK;
		}
	}
	return 0.0;
}
int TaskFile::getInt(const char *name, bool optional)
{
	int n = getIndexByName(name);
	if(ret == ERR_OK)
	{
		if(n >= 0)
		{
			if(_lines[n].value != NULL)
			{
				return atoi(_lines[n].value);
			}
		}
	}
	if(optional)
	{
		if(ret == ERR_TASK_PARAMETER_NAME)
		{
			ret = ERR_OK;
		}
	}
	return 0;
}
unsigned int TaskFile::getUInt(const char *name, bool optional)
{
	int n = getIndexByName(name);
	if(ret == ERR_OK)
	{
		if(n >= 0)
		{
			if(_lines[n].value != NULL)
			{
				return (unsigned int)atoi(_lines[n].value);
			}
		}
	}
	if(optional)
	{
		if(ret == ERR_TASK_PARAMETER_NAME)
		{
			ret = ERR_OK;
		}
	}
	return (unsigned int)0;
}
long int TaskFile::getLong(const char *name, bool optional)
{
	int n = getIndexByName(name);
	if(ret == ERR_OK)
	{
		if(n >= 0)
		{
			if(_lines[n].value != NULL)
			{
				return atol(_lines[n].value);
			}
		}
	}
	if(optional)
	{
		if(ret == ERR_TASK_PARAMETER_NAME)
		{
			ret = ERR_OK;
		}
	}
	return 0;
}
char *TaskFile::getString(const char *name, bool optional)
{
	int n = getIndexByName(name);
	if(ret == ERR_OK)
	{
		if(n >= 0)
		{
			return _lines[n].value;
		}
	}
	if(optional)
	{
		if(ret == ERR_TASK_PARAMETER_NAME)
		{
			ret = ERR_OK;
		}
	}
	return NULL;
}
bool TaskFile::getBoolean(const char *name, bool optional)
{
	int n = getIndexByName(name);
	if(ret == ERR_OK)
	{
		if(n >= 0)
		{
			if(_lines[n].value != NULL)
			{
				if(_strcmpi(_lines[n].value, "true") == 0)
				{
					return true;
				}
				if(_strcmpi(_lines[n].value, "false") == 0)
				{
					return false;
				}
				return (atoi(_lines[n].value) != 0);
			}
		}
	}
	if(optional)
	{
		if(ret == ERR_TASK_PARAMETER_NAME)
		{
			ret = ERR_OK;
		}
	}
	return false;
}

/*
	convert a double number to two integers. It is for forming a file name.
	One integer is by removing digits after the decimal point
	One integer is using digits after the decimal point
*/
void doubleToIntegers(double d, int *i1, int *i2)
{
	int k;
	*i1 = (int)d;
	d = d - (double)(*i1);
	*i2 = 0;
	while(d > 0)
	{
		d = d * 10.0;
		k = (int)d;
		*i2 = k + 10 * (*i2);
		d = d - (double)k;
	}
}






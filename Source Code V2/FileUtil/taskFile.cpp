
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility

********************************************************************/
#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

#include "taskFile.h"
#include "fileutil.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//array size increment
#define BLOCKSIZE 10

/*
	this constructor reads parameters in the task file into array _lines
*/
TaskFile::TaskFile(const char *taskfile)
{
	size_t i,k;
	bool isDisabled;
	string line;
	ifstream file(taskfile);
	ret = ERR_OK;
	isDisabled = false;
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
					if (isDisabled)
					{
						//looking for * and /
						if (line[0] == '*' && line[1] == '/')
						{
							isDisabled = false;
							continue;
						}
					}
					else
					{
						//looking for / and *
						if (line[0] == '/' && line[1] == '*')
						{
							isDisabled = true;
							continue;
						}
						else if (line[0] == '/' && line[1] == '/')
						{
							//it is a single line comment
							continue;
						}
					}
				}
				if (!isDisabled)
				{
					if (count >= lineArraySize)
					{
						allocateLines();
						if (ret != ERR_OK)
						{
							break;
						}
					}
					for (i = 0; i < line.length(); i++)
					{
						if (line[i] == '=')
						{
							break;
						}
					}
					if (i > 0 && i < line.length())
					{
						_lines[count].name = (char *)malloc(i + 1);
						if (_lines[count].name == NULL)
						{
							ret = ERR_OUTOFMEMORY;
							break;
						}
						for (k = 0; k < i; k++)
						{
							if (line[k] == '\n' || line[k] == '\r')
							{
								_lines[count].name[k] = 0;
								break;
							}
							else
							{
								_lines[count].name[k] = line[k];
							}
						}
						_lines[count].name[i] = 0;
						if (i + 1 < line.length())
						{
							_lines[count].value = (char *)malloc(line.length() - i);
							if (_lines[count].value == NULL)
							{
								ret = ERR_OUTOFMEMORY;
								break;
							}
							for (k = i + 1; k < line.length(); k++)
							{
								if (line[k] == '\n' || line[k] == '\r')
								{
									_lines[count].value[k - i - 1] = 0;
									break;
								}
								else
								{
									_lines[count].value[k - i - 1] = line[k];
								}
							}
							_lines[count].value[line.length() - i - 1] = 0;
							count++;
						}
					}
					else
					{
						if (i == 0)
						{
							ret = ERR_TASKFIILE_INVALID;
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		ret = ERR_TASKFIILE_CANNOT_OPEN;
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
			if (_lines[i].data != NULL)
			{
				free(_lines[i].data);
			}
		}
		free(_lines);
	}
}

/*
	save a copy of this file
*/
#define ERR_STRINGFORM_FAILED       29
#define MESSAGELINE 218
int TaskFile::saveToFile(char *file)
{
	int ret = ERR_OK;
	FILE *fh = NULL;
	if (_lines != NULL)
	{
		ret = openTextfileWrite(file, &fh);
		if (ret == ERR_OK)
		{
			size_t sz = MESSAGELINE;
			char *msg = (char*)malloc(sz*sizeof(char));
			if (msg == NULL)
			{
				ret = ERR_OUTOFMEMORY;
			}
			else
			{
				for (int i = 0; i < count; i++)
				{
					if (sz < strlen(_lines[i].name) + strlen(_lines[i].value) + 3)
					{
						free(msg);
						sz = strlen(_lines[i].name) + strlen(_lines[i].value) + 3;
						msg = (char*)malloc(sz*sizeof(char));
						if (msg == NULL)
						{
							ret = ERR_OUTOFMEMORY;
							break;
						}
					}
					ret = sprintf_1(msg, sz, "%s=%s\n", _lines[i].name, _lines[i].value) <=0?ERR_STR_PRF:ERR_OK;
					if (ret != ERR_OK)
					{
						ret = ERR_STRINGFORM_FAILED;
					}
					else
					{
						ret = writefile(fh, msg, (unsigned int)strlen(msg));
					}
					if (ret != ERR_OK)
					{
						break;
					}
				}
				free(msg);
			}
		}
		closefile(fh);
	}
	return ret;
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
				lines[i].data = NULL;
				lines[i].dataSize = 0;
			}
			if(_lines != NULL)
			{
				for(int i=0;i<count;i++)
				{
					lines[i].name = _lines[i].name;
					lines[i].value = _lines[i].value;
					lines[i].data = _lines[i].data;
					lines[i].dataSize = _lines[i].dataSize;
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
			strcpy_0(nameBadVal, sz+1, name);
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
				strcpy_0(unfoundName, sz+1, name);
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
				if(strcmpi_0(_lines[n].value, "true") == 0)
				{
					return true;
				}
				if(strcmpi_0(_lines[n].value, "false") == 0)
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

*/
FieldOutputStruct *TaskFile::getOutputFormats(const char *name, bool optional, unsigned int *count)
{
	int n = getIndexByName(name);
	if (ret == ERR_OK)
	{
		if (n >= 0)
		{
			if (_lines[n].value != NULL)
			{
				if (strlen(_lines[n].value) > 0)
				{
					if (_lines[n].data == NULL)
					{
						std::vector<FieldOutputStruct> v;
						std::string str(_lines[n].value);
						while (ret == ERR_OK && str.length() > 0)
						{
							std::string::size_type loc = str.find_first_of('(');
							if (loc == std::string::npos)
								break;
							else
							{
								std::string::size_type loc2 = str.find_first_of(')');
								if (loc2 < loc)
								{
									ret = ERR_TASK_INVALID_VALUE;
									break;
								}
								else
								{
									FieldOutputStruct gd;
									gd.point.i = gd.point.j = gd.point.k = 0;
									gd.elements = None;
									string str0 = str.substr(loc2 + 1, string::npos);
									string str1 = str.substr(loc + 1, loc2 - loc - 1);
									string::size_type l0 = str1.find_first_of(',');
									str = str0;
									if (l0 != string::npos)
									{
										string sv = str1.substr(0, l0);
										int i = std::stoi(sv);
										if (i >= 0)
										{
											gd.point.i = (unsigned int)i;
											sv = str1.substr(l0 + 1, string::npos);
											str1 = sv;
											l0 = str1.find_first_of(',');
											if (l0 != string::npos)
											{
												sv = str1.substr(0, l0);
												i = std::stoi(sv);
												if (i >= 0)
												{
													gd.point.j = (unsigned int)i;
													sv = str1.substr(l0 + 1, string::npos);
													str1 = sv;
													l0 = str1.find_first_of(',');
													if (l0 != string::npos)
													{
														sv = str1.substr(0, l0);
														i = std::stoi(sv);
														if (i >= 0)
														{
															int f = 0;
															gd.point.k = (unsigned int)i;
															for (int k = 1; k <= 6; k++)
															{
																//k-th element
																sv = str1.substr(l0 + 1, string::npos);
																str1 = sv;
																l0 = str1.find_first_of('|');
																if (l0 != string::npos)
																{
																	sv = str1.substr(0, l0);
																	if (sv.compare("Ex") == 0)
																	{
																		f = f | Ex;
																	}
																	else if (sv.compare("Ey") == 0)
																	{
																		f = f | Ey;
																	}
																	else if (sv.compare("Ez") == 0)
																	{
																		f = f | Ez;
																	}
																	else if (sv.compare("Hx") == 0)
																	{
																		f = f | Hx;
																	}
																	else if (sv.compare("Hy") == 0)
																	{
																		f = f | Hy;
																	}
																	else if (sv.compare("Hz") == 0)
																	{
																		f = f | Hz;
																	}
																	else
																	{
																		ret = ERR_TASK_INVALID_VALUE;
																		break;
																	}
																}
																else
																{
																	if (sv.compare("Ex") == 0)
																	{
																		f = f | Ex;
																	}
																	else if (sv.compare("Ey") == 0)
																	{
																		f = f | Ey;
																	}
																	else if (sv.compare("Ez") == 0)
																	{
																		f = f | Ez;
																	}
																	else if (sv.compare("Hx") == 0)
																	{
																		f = f | Hx;
																	}
																	else if (sv.compare("Hy") == 0)
																	{
																		f = f | Hy;
																	}
																	else if (sv.compare("Hz") == 0)
																	{
																		f = f | Hz;
																	}
																	else
																	{
																		ret = ERR_TASK_INVALID_VALUE;
																	}
																	break;
																}
															}
															if (ret == ERR_OK)
															{
																bool bExist = false;
																gd.elements = (FieldElements)f;
																for (size_t h = 0; h < v.size(); h++)
																{
																	if (gd.elements == v[h].elements)
																	{
																		if (gd.point.i == v[h].point.i)
																		{
																			if (gd.point.j == v[h].point.j)
																			{
																				if (gd.point.k == v[h].point.k)
																				{
																					bExist = true;
																					break;
																				}
																			}
																		}
																	}
																}
																if (!bExist)
																{
																	v.push_back(gd);
																}
															}
														}
														else
														{
															ret = ERR_TASK_INVALID_VALUE;
														}
													}
													else
													{
														ret = ERR_TASK_INVALID_VALUE;
													}
												}
												else
												{
													ret = ERR_TASK_INVALID_VALUE;
												}
											}
											else
											{
												ret = ERR_TASK_INVALID_VALUE;
											}
										}
										else
										{
											ret = ERR_TASK_INVALID_VALUE;
										}
									}
									else
									{
										ret = ERR_TASK_INVALID_VALUE;
									}
								}
							}
						}
						if (ret == ERR_OK && v.size() > 0)
						{
							FieldOutputStruct *d = (FieldOutputStruct *)malloc(v.size()*sizeof(FieldOutputStruct));
							if (d == NULL)
								ret = ERR_OUTOFMEMORY;
							else
							{
								std::copy(v.begin(), v.end(), d);
								_lines[n].dataSize = v.size();
								_lines[n].data = d;
							}
						}
					}
					if (ret == ERR_OK)
					{
						*count = (unsigned int)(_lines[n].dataSize);
						return (FieldOutputStruct *)(_lines[n].data);
					}
				}
			}
		}
	}
	if (optional)
	{
		if (ret == ERR_TASK_PARAMETER_NAME)
		{
			ret = ERR_OK;
		}
	}
	return NULL;
}

/*
	data format:
	(i,j,k),(i,j,k),...
*/
GridNode3Dstruct *TaskFile::getGridNodes(const char *name, bool optional, size_t *size)
{
	int n = getIndexByName(name);
	if (ret == ERR_OK)
	{
		if (n >= 0)
		{
			if (_lines[n].value != NULL)
			{
				if (strlen(_lines[n].value) > 0)
				{
					if (_lines[n].data == NULL)
					{
						std::vector<GridNode3Dstruct> v;
						std::string str(_lines[n].value);
						while (ret == ERR_OK && str.length() > 0)
						{
							std::string::size_type loc = str.find_first_of('(');
							if (loc == std::string::npos)
								break;
							else
							{
								std::string::size_type loc2 = str.find_first_of(')');
								if (loc2 < loc)
								{
									ret = ERR_TASK_INVALID_VALUE;
									break;
								}
								else
								{
									GridNode3Dstruct gd;
									gd.i = gd.j = gd.k = 0;
									string str0 = str.substr(loc2 + 1, string::npos);
									string str1 = str.substr(loc + 1, loc2 - loc-1);
									string::size_type l0 = str1.find_first_of(',');
									str = str0;
									if (l0 != string::npos)
									{
										string sv = str1.substr(0, l0);
										int i = std::stoi(sv);
										if (i >= 0)
										{
											gd.i = (unsigned int)i;
											sv = str1.substr(l0 + 1, string::npos);
											str1 = sv;
											l0 = str1.find_first_of(',');
											if (l0 != string::npos)
											{
												sv = str1.substr(0, l0);
												i = std::stoi(sv);
												if (i >= 0)
												{
													gd.j = (unsigned int)i;
													sv = str1.substr(l0 + 1, string::npos);
													i = std::stoi(sv);
													if (i >= 0)
													{
														gd.k = (unsigned int)i;
														v.push_back(gd);
													}
													else
													{
														ret = ERR_TASK_INVALID_VALUE;
													}
												}
												else
												{
													ret = ERR_TASK_INVALID_VALUE;
												}
											}
											else
											{
												ret = ERR_TASK_INVALID_VALUE;
											}
										}
										else
										{
											ret = ERR_TASK_INVALID_VALUE;
										}
									}
									else
									{
										ret = ERR_TASK_INVALID_VALUE;
									}
								}
							}
						}
						if (ret == ERR_OK && v.size() > 0)
						{
							GridNode3Dstruct *d = (GridNode3Dstruct *)malloc(v.size()*sizeof(GridNode3Dstruct));
							if (d == NULL)
								ret = ERR_OUTOFMEMORY;
							else
							{
								std::copy(v.begin(), v.end(), d);
								_lines[n].dataSize = v.size();
								_lines[n].data = d;
							}
						}
					}
					if (ret == ERR_OK)
					{
						*size = _lines[n].dataSize;
						return (GridNode3Dstruct *)(_lines[n].data);
					}
				}
			}
		}
	}
	if (optional)
	{
		if (ret == ERR_TASK_PARAMETER_NAME)
		{
			ret = ERR_OK;
		}
	}
	return NULL;
}

unsigned *TaskFile::getUIntArray(const char *name, bool optional, size_t *size)
{
	int n = getIndexByName(name);
	if (ret == ERR_OK)
	{
		if (n >= 0)
		{
			if (_lines[n].value != NULL)
			{
				if (strlen(_lines[n].value) > 0)
				{
					if (_lines[n].data == NULL)
					{
						std::vector<unsigned int> v;
						string str(_lines[n].value);
						while (ret == ERR_OK && str.length() > 0)
						{
							string s;
							string::size_type l0 = str.find_first_of(',');
							if (l0 != string::npos)
							{
								s = str.substr(0, l0);
								string s0 = str.substr(l0 + 1, string::npos);
								str = s0;
							}
							else
							{
								s = str;
								str.clear();
							}
							int i = std::stoi(s);
							if (i >= 0)
							{
								v.push_back(i);
							}
							else
							{
								ret = ERR_TASK_INVALID_VALUE;
							}
						}
						if (ret == ERR_OK && v.size()>0)
						{
							unsigned int *d = (unsigned int *)malloc(v.size()*sizeof(unsigned int));
							if (d == NULL)
								ret = ERR_OUTOFMEMORY;
							else
							{
								std::copy(v.begin(), v.end(), d);
								_lines[n].data = d;
								_lines[n].dataSize = v.size();
							}
						}
					}
					*size = _lines[n].dataSize;
					return (unsigned int *)_lines[n].data;
				}
			}
		}
	}
	if (optional)
	{
		if (ret == ERR_TASK_PARAMETER_NAME)
		{
			ret = ERR_OK;
		}
	}
	return NULL;
}

int TaskFile::getIntVector(const char *name, bool optional, vector<int> &v)
{
	int n = getIndexByName(name);
	if (ret == ERR_OK)
	{
		if (n >= 0)
		{
			if (_lines[n].value != NULL)
			{
				if (strlen(_lines[n].value) > 0)
				{
					if (_lines[n].data == NULL)
					{
						string str(_lines[n].value);
						while (ret == ERR_OK && str.length() > 0)
						{
							string s;
							string::size_type l0 = str.find_first_of(',');
							if (l0 != string::npos)
							{
								s = str.substr(0, l0);
								string s0 = str.substr(l0 + 1, string::npos);
								str = s0;
							}
							else
							{
								s = str;
								str.clear();
							}
							int i = std::stoi(s);
							v.push_back(i);
						}
					}
				}
			}
		}
	}
	if (optional)
	{
		if (ret == ERR_TASK_PARAMETER_NAME)
		{
			ret = ERR_OK;
		}
	}
	return ret;
}

/*
	convert a double number to two integers. It is for forming a file name.
	One integer is by removing digits after the decimal point
	One integer is using digits after the decimal point
*/
void doubleToIntegers(double d, int *i1, int *i2, int *en)
{
	int k, e = 0;
	bool gotDigit = false;
	*en = 0;
	*i1 = (int)d;
	d = d - (double)(*i1);
	*i2 = 0;
	while (d > 0)
	{
		d = d * 10.0;
		k = (int)d;
		if (k == 0)
		{
			if (!gotDigit)
				e++;
		}
		else
		{
			gotDigit = true;
			if (e > 0)
				e++;
		}
		*i2 = k + 10 * (*i2);
		d = d - (double)k;
	}
	*en = e;
}

int doubleToIntegersString(double d, char *str, unsigned int size)
{
	int i1, i2, en;
	doubleToIntegers(d, &i1, &i2, &en);
	if (en > 0)
	{
		int ret = sprintf_1(str, size, "%dD%de%d", i1, i2, en) <=0?ERR_STR_PRF:ERR_OK;
		if (ret != ERR_OK)
			return ret;
	}
	else
	{
		int ret = sprintf_1(str, size, "%dD%d", i1, i2) <=0?ERR_STR_PRF:ERR_OK;
		if (ret != ERR_OK)
			return ret;
	}
	return ERR_OK;
}





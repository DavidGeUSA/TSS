/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/
/*
	Each subclass of FieldSource should be a class in a DLL like this one.
	A DLL may contain several classes. each class should provide a parameterless constructor. 
	To use a class, a program specifies DLL file path and a class name.
*/
#include "FieldSourceEz.h"
#include "SourceX.h"

#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	__declspec (dllexport) void* CreatePluginInstance(char *name, double *params);
	__declspec (dllexport) void RemovePluginInstances();

#ifdef __cplusplus
}
#endif /* __cplusplus */

FieldSourceEz **srcezList = NULL;
unsigned int srcezCount = 0;

SourceX **sourcexList = NULL;
unsigned int sourcexCount = 0;

__declspec (dllexport) void RemovePluginInstances()
{
	REMOVEALLPLUGINS(FieldSourceEz, srcezCount, srcezList);
	REMOVEALLPLUGINS(SourceX, sourcexCount, sourcexList);
}

__declspec (dllexport) void* CreatePluginInstance(char *name, double *params)
{
    if(strcmp(name,"FieldSourceEz") == 0)
	{
		FieldSource *p = NULL;
		CREATEPLUGININSTANCE(FieldSourceEz, srcezCount, srcezList);
		if (p != NULL)
		{
			p->setClassName(name);
		}
		return p;
	}
	else if (strcmp(name, "SourceX") == 0)
	{
		SourceX *p = NULL;
		CREATEPLUGININSTANCE(SourceX, sourcexCount, sourcexList);
		if (p != NULL)
		{
			p->setClassName(name);
		}
		return p;
	}
	return NULL;
}


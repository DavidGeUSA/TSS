/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Each subclass of FDTD should be a class in a DLL like this one.
	A DLL may contain several classes. Each class should provide a parameterless contructor.
	To use a class, a program specifies DLL file path and class name.
********************************************************************/

#include "YeeFDTD.h"
#include "YeeFDTDSpaceSynched.h"

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

YeeFDTD **yeeList = NULL;
unsigned int yeeCount = 0;
YeeFDTDSpaceSynched **yeesynchList = NULL;
unsigned int yeesynchCount = 0;

__declspec (dllexport) void RemovePluginInstances()
{
	REMOVEALLPLUGINS(YeeFDTD, yeeCount, yeeList);
	REMOVEALLPLUGINS(YeeFDTDSpaceSynched, yeesynchCount, yeesynchList);
}

__declspec (dllexport) void* CreatePluginInstance(char *name, double *params)
{
	FDTD *p = NULL;
    if(strcmp(name,"YeeFDTD") == 0)
	{
		CREATEPLUGININSTANCE(YeeFDTD, yeeCount, yeeList);
	}
	else if(strcmp(name, "YeeFDTDSpaceSynched") == 0)
	{
		CREATEPLUGININSTANCE(YeeFDTDSpaceSynched, yeesynchCount, yeesynchList);
	}
	if(p != NULL)
	{
		//class name will be used in forming data file names
		p->setClassName(name);
	}
	return p;
}


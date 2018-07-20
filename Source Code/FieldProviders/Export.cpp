/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

	Each Field initializer should be a class in a DLL like this one.
	A DLL may contain several classes. each class should provide a parameterless constructor. 
	To use a class, a program specifies DLL file path and a class name.
********************************************************************/
#include "..\EMField\EMField.h"
#include <string.h>
#include <stdlib.h>

#include "Gaussion.h"
#include "ZeroFields.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	__declspec (dllexport) void* CreatePluginInstance(char *name, double *params);
	__declspec (dllexport) void RemovePluginInstances();

#ifdef __cplusplus
}
#endif /* __cplusplus */

GaussianFields **gaussList = NULL;
unsigned int gaussCount = 0;
ZeroFields **zeroList = NULL;
unsigned int zeroCount = 0;

__declspec (dllexport) void RemovePluginInstances()
{
	REMOVEALLPLUGINS(GaussianFields, gaussCount, gaussList);
	REMOVEALLPLUGINS(ZeroFields, zeroCount, zeroList);
}


/*
	this function creates a field initializer object.

	a field initializer is for populating fields before starting a simulation;
	a field initializer object is responsible for initializing fields to desired values.

	the class name of every field initializer class contained in this DLL should be listed
	in this function so that other modules can create objects of it by name at runtime

	name   - class name for the field initializer object to be created
	params - constructor parameters for the field initializer object, it is not used
*/
__declspec (dllexport) void* CreatePluginInstance(char *name, double *params)
{
	FieldsInitializer *p = NULL;
    if(strcmp(name,"GaussianFields") == 0)
	{
		CREATEPLUGININSTANCE(GaussianFields, gaussCount, gaussList);
	}
	else if(strcmp(name, "ZeroFields") == 0)
	{
		CREATEPLUGININSTANCE(ZeroFields, zeroCount, zeroList);
	}
	if(p != NULL)
	{
		p->setClassName(name);
	}
	return p;
}


/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/

#include "tfsfEz.h"

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

TfsfEz **tfsfezList = NULL;
unsigned int tfsfezCount = 0;

__declspec (dllexport) void RemovePluginInstances()
{
	REMOVEALLPLUGINS(TfsfEz, tfsfezCount, tfsfezList);
}

__declspec (dllexport) void* CreatePluginInstance(char *name, double *params)
{
	TotalFieldScatteredFieldBoundary *p = NULL;
    if(strcmp(name,"TfsfEz") == 0)
	{
		CREATEPLUGININSTANCE(TfsfEz, tfsfezCount, tfsfezList);
	}
	if(p != NULL)
	{
		p->setClassName(name);
	}
	return p;
}


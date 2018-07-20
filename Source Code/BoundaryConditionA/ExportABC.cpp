/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by Bob Limnor

********************************************************************/

#include "AbcFirstOrder.h"
#include "VoidCondition.h"

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

AbcFirstOrder **abc1stList;
unsigned int abc1stCount;
VoidCondition **abc0List;
unsigned int abc0Count;

__declspec (dllexport) void RemovePluginInstances()
{
	REMOVEALLPLUGINS(AbcFirstOrder, abc1stCount, abc1stList);
	REMOVEALLPLUGINS(VoidCondition, abc0Count, abc0List);
}


__declspec (dllexport) void* CreatePluginInstance(char *name, double *params)
{
	BoundaryCondition *p = NULL;
	if(strcmp(name,"AbcFirstOrder") == 0)
	{
		CREATEPLUGININSTANCE(AbcFirstOrder, abc1stCount, abc1stList);
	}
	else if(strcmp(name, "VoidCondition") == 0)
	{
		CREATEPLUGININSTANCE(VoidCondition, abc0Count, abc0List);
	}
	if(p != NULL)
	{
		p->setClassName(name);
	}
	return p;
}


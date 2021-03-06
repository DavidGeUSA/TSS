
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/
/*
	Each subclass of FDTD should be a class in a DLL like this one.
	A DLL may contain several classes. each class should provide a parameterless constructor. 
	To use a class, a program specifies DLL file path and a class name.
*/
#include "TssFDTD.h"

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

TssFDTD **tssList = NULL;
unsigned int tssCount = 0;
TssFDTDinhomo **tssinhomoList = NULL;
unsigned int tssInhomoCOunt = 0;
FourthSpaceOrderTss **tss4List = NULL;
unsigned int tss4Count = 0;

__declspec (dllexport) void RemovePluginInstances()
{
	REMOVEALLPLUGINS(TssFDTD, tssCount, tssList);
	REMOVEALLPLUGINS(TssFDTDinhomo, tssInhomoCOunt, tssinhomoList);
	REMOVEALLPLUGINS(FourthSpaceOrderTss, tss4Count, tss4List);
}
__declspec (dllexport) void* CreatePluginInstance(char *name, double *params)
{
	FDTD *p = NULL;
    if(strcmp(name,"TssFDTD") == 0)
	{
		CREATEPLUGININSTANCE(TssFDTD, tssCount, tssList);
	}
	else if(strcmp(name, "TssFDTDinhomo") == 0)
	{
		CREATEPLUGININSTANCE(TssFDTDinhomo, tssInhomoCOunt, tssinhomoList);
	}
	else if (strcmp(name, "FourthSpaceOrderTss") == 0)
	{
		CREATEPLUGININSTANCE(FourthSpaceOrderTss, tss4Count, tss4List);
	}
	if(p != NULL)
	{
		//class name will be used in forming data file names
		p->setClassName(name);
	}
	return p;
}


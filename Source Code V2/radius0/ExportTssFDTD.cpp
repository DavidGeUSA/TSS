
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

#include "radius0.h"

FDTD* CreateRadiusFDTDInstance(char *name)
{
	if (name == NULL) return NULL;
	//class name will be used in forming data file names
    if(strcmp(name,"TssFDTD") == 0)
	{
		TssFDTD * p = new TssFDTD();
		p->setClassName(name);
		return dynamic_cast<TssFDTD *>(p);
	}
	else if(strcmp(name, "TssFDTDinhomo") == 0)
	{
		TssFDTDinhomo *p= new TssFDTDinhomo();
		p->setClassName(name);
		return dynamic_cast<TssFDTDinhomo *>(p);
	}
	else if (strcmp(name, "FourthSpaceOrderTss") == 0)
	{
		FourthSpaceOrderTss *p = new FourthSpaceOrderTss();
		p->setClassName(name);
		return dynamic_cast<FourthSpaceOrderTss *>(p);
	}
	else if (strcmp(name, "YeeFDTD") == 0)
	{
		YeeFDTD *p = new YeeFDTD();
		p->setClassName(name);
		return dynamic_cast<YeeFDTD *>(p);
	}
	else if (strcmp(name, "YeeFDTDSpaceSynched") == 0)
	{
		YeeFDTDSpaceSynched *p = new YeeFDTDSpaceSynched();
		p->setClassName(name);
		return dynamic_cast<YeeFDTDSpaceSynched *>(p);
	}

	return NULL;
}


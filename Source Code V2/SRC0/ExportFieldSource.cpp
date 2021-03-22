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
#include "DipoleSource.h"

#include <string.h>
#include <stdlib.h>

#include "SRC0.h"

FieldSource* CreateRadiusSourceInstance(char *name)
{
	if (name == NULL) return NULL;
    if(strcmp(name,"FieldSourceEz") == 0)
	{
		FieldSourceEz *p = new FieldSourceEz();
		p->setClassName(name);
		return dynamic_cast<FieldSourceEz *>(p);
	}
	else if (strcmp(name, "SourceX") == 0)
	{
		SourceX *p = new SourceX();
		p->setClassName(name);
		return dynamic_cast<SourceX *>(p);
	}
	else if (strcmp(name, "DipoleSource") == 0)
	{
		DipoleSource *p = new DipoleSource();
		p->setClassName(name);
		return dynamic_cast<DipoleSource *>(p);
	}
	return NULL;
}


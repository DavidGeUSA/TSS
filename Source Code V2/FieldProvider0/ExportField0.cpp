/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Each Field initializer should be a class in a DLL like this one.
	A DLL may contain several classes. each class should provide a parameterless constructor. 
	To use a class, a program specifies DLL file path and a class name.
********************************************************************/
#include "../EMField/EMField.h"
#include <string.h>
#include <stdlib.h>

#include "Gaussion.h"
#include "ZeroFields.h"
#include "../EMField/FieldsInitializer.h"

#include "IV.h"

FieldsInitializer *CreateIVInstance(const char *className)
{
	if (className == NULL)
		return NULL;
	if (strcmp(className, "GaussianFields") == 0)
	{
		GaussianFields *p = new GaussianFields();
		p->setClassName(className);
		return dynamic_cast<GaussianFields *>(p);
	}
	else if (strcmp(className, "ZeroFields") == 0)
	{
		ZeroFields *p = new ZeroFields();
		p->setClassName(className);
		return dynamic_cast<ZeroFields *>(p);
	}
	return NULL;

}
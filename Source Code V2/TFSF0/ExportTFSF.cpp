/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/

#include "tfsfEz.h"
#include "TFSF0.h"
#include "../EMField/TotalFieldScatteredFieldBoundary.h"
#include "../EMField/Plugin.h"
#include <string.h>
#include <stdlib.h>


TotalFieldScatteredFieldBoundary * CreateTFSTInstance(char *name)
{
	if (name == NULL) return NULL;
    if(strcmp(name,"TfsfEz") == 0)
	{
		TfsfEz *p = new TfsfEz();
		//class name will be used in forming data file names
		p->setClassName(name);
		return dynamic_cast<TfsfEz *>(p);
	}
	return NULL;
}


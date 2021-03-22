/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility
********************************************************************/

#include "AbcFirstOrder.h"
#include "VoidCondition.h"
#include "PemcTss.h"

#include <string.h>
#include <stdlib.h>

#include "BC.h"


BoundaryCondition *CreateBCInstance(char *className)
{
	if (className == NULL)
		return NULL;
	if (strcmp(className, "AbcFirstOrder") == 0)
	{
		AbcFirstOrder *p = new AbcFirstOrder();
		p->setClassName(className);
		return dynamic_cast<AbcFirstOrder *>(p);
	}
	else if (strcmp(className, "VoidCondition") == 0)
	{
		VoidCondition *p = new VoidCondition();
		p->setClassName(className);
		return dynamic_cast<VoidCondition *>(p);
	}
	else if (strcmp(className, "PemcTss") == 0)
	{
		PemcTss *p = new PemcTss();
		p->setClassName(className);
		return dynamic_cast<PemcTss *>(p);
	}
	return NULL;

}

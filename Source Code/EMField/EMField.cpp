/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "EMField.h"
#include "RadiusIndex.h"

///////FieldsFiller///////////////////////////////////////////////////////////////
FieldsFiller::FieldsFiller(FieldsInitializer* fields0, FieldPoint3D *fields)
{
	_fields0 = fields0;
	_fields = fields;
	index = 0;
}

/*
	_fields0 is a plug-in module loaded from a dynimic-link library
*/
void FieldsFiller::handleData(double x, double y, double z)
{
	_fields0->getField(x, y, z, &(_fields[index]));
	index++;
}

//////////////////////////////////////////

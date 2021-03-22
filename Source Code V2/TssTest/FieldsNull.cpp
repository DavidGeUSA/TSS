/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

initial fields to 0
********************************************************************/
#include "FieldsNull.h"


FieldsNull::FieldsNull()
{
}


FieldsNull::~FieldsNull()
{
}
void FieldsNull::SetFields(double x, double y, double z, Point3Dstruct *pE, Point3Dstruct *pH)
{
	pE->x = 0.0;
	pE->y = 0.0;
	pE->z = 0.0;
	pH->x = 0.0;
	pH->y = 0.0;
	pH->z = 0.0;
}
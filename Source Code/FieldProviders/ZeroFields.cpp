/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "ZeroFields.h"

ZeroFields::ZeroFields(void)
{
}


ZeroFields::~ZeroFields()
{
}
int ZeroFields::initialize(TaskFile *taskParameters)
{
	return ERR_OK;
}
void ZeroFields::getField(double x, double y, double z, FieldPoint3D *f)
{
	f->E.x = 0.0;
	f->E.y = 0.0;
	f->E.z = 0.0;
	f->H.x = 0.0;
	f->H.y = 0.0;
	f->H.z = 0.0;
}
double ZeroFields::funcE0x(double x, double y, double z)         //Initial Ex on the point
{
	return 0.0;
}
double ZeroFields::funcE0y(double x, double y, double z)         //Initial Ey on the point
{
	return 0.0;
}
double ZeroFields::funcE0z(double x, double y, double z)         //Initial Ez on the point
{
	return 0.0;
}
double ZeroFields::funcB0x(double x, double y, double z)         //Initial Bx on the point
{
	return 0.0;
}
double ZeroFields::funcB0y(double x, double y, double z)         //Initial By on the point
{
	return 0.0;
}
double ZeroFields::funcB0z(double x, double y, double z)         //Initial Bz on the point
{
	return 0.0;
}

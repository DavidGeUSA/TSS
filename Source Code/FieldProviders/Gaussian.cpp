/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "Gaussion.h"

#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  

#define EX (b  * _x->y * _x->z * ek / 2.0)
#define EY (-b * _x->x * _x->z * ek)
#define EZ (b  * _x->x * _x->y * ek / 2.0)
#define INITPOINT 	x2 = _x->x * _x->x; \
	y2 = _x->y * _x->y; \
	z2 = _x->z * _x->z; \
	_r2 = x2 + y2 + z2; \
	ek = pow(M_E, -k * _r2); \
	dd = bk * _x->x * _x->y * _x->z * ek;
#define SETPOINT(x, y, z) _x->x = (x);\
	_x->y = (y);\
	_x->z = (z);

GaussianFields::GaussianFields()
{
}


GaussianFields::~GaussianFields()
{
}
int GaussianFields::initialize(TaskFile *taskParameters)
{
	int ret = ERR_OK;
	k = taskParameters->getDouble(TP_IV_WIDTH, false);
	b = taskParameters->getDouble(TP_IV_MAGNITUDE, false);
	ret = taskParameters->getErrorCode();
	if(ret == ERR_OK)
	{
		_x = &defP;
		kp2 = 2.0 * k;
		bk = b * k;
	}
	return ret;
}
void GaussianFields::getField(double x, double y, double z, FieldPoint3D *f)
{
	SETPOINT(x, y, z)
	INITPOINT
	f->E.x = EX;
	f->E.y = EY;
	f->E.z = EZ;
	f->H.x = 0;
	f->H.y = 0;
	f->H.z = 0;
}
double GaussianFields::funcE0x(double x, double y, double z)         //Initial Ex on the point
{
	SETPOINT(x, y, z)
	INITPOINT
	return EX;
}
double GaussianFields::funcE0y(double x, double y, double z)         //Initial Ey on the point
{
	SETPOINT(x, y, z)
	INITPOINT
	return EY;
}
double GaussianFields::funcE0z(double x, double y, double z)         //Initial Ez on the point
{
	SETPOINT(x, y, z)
	INITPOINT
	return EZ;
}
double GaussianFields::funcB0x(double x, double y, double z)         //Initial Bx on the point
{
	return 0.0;
}
double GaussianFields::funcB0y(double x, double y, double z)         //Initial By on the point
{
	return 0.0;
}
double GaussianFields::funcB0z(double x, double y, double z)         //Initial Bz on the point
{
	return 0.0;
}



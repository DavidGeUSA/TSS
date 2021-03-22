/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

initial fields for curl test
********************************************************************/
#include "../Tss/Tss.h"
#include "CurlXYZ2.h"

CurlXYZ2::CurlXYZ2()
{
}


CurlXYZ2::~CurlXYZ2()
{
}

int CurlXYZ2::initialize(double b0, double k0)
{
	int ret = ERR_OK;
	a = b0;
	//
	//
	return ret;
}
int CurlXYZ2::initializeByConfig(TaskFile *configs)
{
	int ret = ERR_OK;
	double b0 = configs->getDouble(CURLXYZ2_MAGNITUDE, false);
	ret = initialize(b0, 0);
	return ret;
}

void CurlXYZ2::curl1(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = a*x*x*y*z*z*z + 2.0*a*x*x* y*y*y* z;//a*x*z*z / 2.0 + a*x*y*y;
	curl->y = a*x*x*x *y*y *z - a*x*y*y*z*z*z;//a*y*x*x / 2.0 - a*y*z*z / 2.0;
	curl->z = -2.0*a*x*y*y*y *z*z - a*y*z*z* x*x*x;//-a*z*y*y - a*z*x*x / 2.0;
}

void CurlXYZ2::curl2(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = -3.0*a*x*y*y* z*z - a*z*z* x*x*x - a*x*x*x* y*y;
	curl->y = 6.0*a*x*x* y*z*z + 2.0*a*x*x* y*y*y + 2.0*a*y*y*y* z*z;
	curl->z = -a*y*y* z*z*z - a*x*x* z*z*z - 3.0*a*x*x* y*y* z;
}
void CurlXYZ2::curl3(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = -18.0*a*x *x* y*z - 4.0*a*y*y*y* z - 2.0*a*y*z*z*z;
	curl->y = -2.0*a*z*x*x*x + 2.0*a*x*z*z*z;
	curl->z = 18.0*a*x*y*z*z + 4.0*a*x*y*y*y + 2.0*a*x*x*x* y;
}
void CurlXYZ2::curl4(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = 12.0*a*x*z*z + 12.0*a*x*y*y + 4.0*a*x*x*x;
	curl->y = -24.0*a*x*x* y - 24.0*a*y*z*z - 8.0*a*y*y*y;
	curl->z = 12.0*a*x*x* z + 12.0*a*y*y* z + 4.0*a*z*z*z;
}
void CurlXYZ2::curl5(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = 72.0*a*y*z;
	curl->y = 0.0;
	curl->z = -72.0*a*x*y;
}void CurlXYZ2::curl6(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = -72.0*a*x;
	curl->y = 144.0*a*y;
	curl->z = -72.0*a*z;
}void CurlXYZ2::curl7(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = 0.0;
	curl->y = 0.0;
	curl->z = 0.0;
}
void CurlXYZ2::SetFields(double x, double y, double z, Point3Dstruct *pE, Point3Dstruct *pH)
{
	pE->x = a*y*y*z*z*x*x*x / 2.0;
	pE->y = -a*x*x*z*z*y*y*y;
	pE->z = a*x*x*y*y*z*z*z / 2.0;
	//
	pH->x = 0.0;
	pH->y = 0.0;
	pH->z = 0.0;
}

double CurlXYZ2::funcE0x(double x, double y, double z)         //Initial Ex on the point
{
	return  a*y*y*z*z*x*x*x / 2.0;
}
double CurlXYZ2::funcE0y(double x, double y, double z)         //Initial Ey on the point
{
	return -a*x*x*z*z*y*y*y;
}
double CurlXYZ2::funcE0z(double x, double y, double z)         //Initial Ez on the point
{
	return a*x*x*y*y*z*z*z / 2.0;
}
double CurlXYZ2::funcB0x(double x, double y, double z)         //Initial Bx on the point
{
	return 0.0;
}
double CurlXYZ2::funcB0y(double x, double y, double z)         //Initial By on the point
{
	return 0.0;
}
double CurlXYZ2::funcB0z(double x, double y, double z)         //Initial Bz on the point
{
	return 0.0;
}
//////////////////////////////////////////////////////////////////
double CurlXYZ2::fnDx1eR(double x, double y, double z)
{
	return 0;
}
double CurlXYZ2::fnDy1eR(double x, double y, double z)
{
	return 0;
}
double CurlXYZ2::fnDz1eR(double x, double y, double z)
{

	return 0;
}
double CurlXYZ2::fnDxEx(double x, double y, double z)
{
	return (3.0/2.0)*a*x*x*y*y*z*z;
}
double CurlXYZ2::fnDyEy(double x, double y, double z)
{
	return -3.0*a*x*x*y*y*z*z;
}
double CurlXYZ2::fnDzEz(double x, double y, double z)
{
	return (3.0/2.0)*a*x*x*y*y*z*z;
}

double CurlXYZ2::fnDyEx(double x, double y, double z)
{
	return a*y*z*z*x*x*x;
}
double CurlXYZ2::fnDzEy(double x, double y, double z)
{
	return -2.0*x*x*y*y*y*z;
}
double CurlXYZ2::fnDxEz(double x, double y, double z)
{
	return a*x*y*y*z*z*z;
}
double CurlXYZ2::fnDzEx(double x, double y, double z)
{
	return a*x*x*x*y*y*z;
}
double CurlXYZ2::fnDxEy(double x, double y, double z)
{
	return -2.0*a*x*y*y*y*z*z;
}
double CurlXYZ2::fnDyEz(double x, double y, double z)
{
	return a*x*x*y*z*z*z;
}
//////////////////////////////////////////
double CurlXYZ2::fnDx2eR(double x, double y, double z)
{
	return 0;
}
double CurlXYZ2::fnDy2eR(double x, double y, double z)
{
	return 0;
}
double CurlXYZ2::fnDz2eR(double x, double y, double z)
{
	return 0;
}
double CurlXYZ2::fnD2xEx(double x, double y, double z)
{
	return 3.0*a*x*y*y*z*z;
}
double CurlXYZ2::fnD2yEy(double x, double y, double z)
{
	return -6.0*a*x*x*y*z*z;
}
double CurlXYZ2::fnD2zEz(double x, double y, double z)
{
	return 3.0*a*x*x*y*y*z;
}
//
double CurlXYZ2::fnD2yEx(double x, double y, double z)
{
	return a*z*z*x*x*x;
}
double CurlXYZ2::fnD2zEy(double x, double y, double z)
{
	return -2.0*a*x*x*y*y*y;
}
double CurlXYZ2::fnD2xEz(double x, double y, double z)
{
	return a*y*y*z*z*z;
}
//
double CurlXYZ2::fnD2zEx(double x, double y, double z)
{
	return a*x*x*x*y*y;
}
double CurlXYZ2::fnD2xEy(double x, double y, double z)
{
	return -2.0*a*y*y*y*z*z;
}
double CurlXYZ2::fnD2yEz(double x, double y, double z)
{
	return a*x*x*z*z*z;
}



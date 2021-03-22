/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

initial fields for curl test
********************************************************************/
#include "CurlXYZ.h"
#include "GaussianCurl.h"

CurlXYZ::CurlXYZ()
{
}


CurlXYZ::~CurlXYZ()
{
}

int CurlXYZ::initialize(double b0, double k0)
{
	int ret = ERR_OK;
	a = b0;
	//
	//
	return ret;
}
int CurlXYZ::initializeByConfig(TaskFile *configs)
{
	int ret = ERR_OK;
	double b0 = configs->getDouble(CURLXYZ_MAGNITUDE, false);
	//double k0 = configs->getDouble(GAUSSIAN_EXPO, false);
	ret = initialize(b0, 0);
	return ret;
}
void CurlXYZ::curl1(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = a*x*z*z / 2.0 + a*x*y*y;
	curl->y = a*y*x*x/2.0 - a*y*z*z/2.0;
	curl->z = -a*z*y*y - a*z*x*x/2.0;
}

void CurlXYZ::curl2(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = -a*z*y;
	curl->y = 2.0*a*z*x;
	curl->z = -a*x*y;
}
void CurlXYZ::curl3(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = -3.0*a*x;
	curl->y = 0.0;
	curl->z = 3.0*a*z;
}
void CurlXYZ::curl4(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = 0.0;
	curl->y = 0.0;
	curl->z = 0.0;
}
void CurlXYZ::SetFields(double x, double y, double z, Point3Dstruct *pE, Point3Dstruct *pH)
{
	pE->x = a*y*z*x*x / 2.0;
	pE->y = -a*x*z*y*y;
	pE->z = a*x*y*z*z / 2.0;
	//
	pH->x = 0.0;
	pH->y = 0.0;
	pH->z = 0.0;
}
double CurlXYZ::funcE0x(double x, double y, double z)         //Initial Ex on the point
{
	return  a*y*z*x*x/2.0;
}
double CurlXYZ::funcE0y(double x, double y, double z)         //Initial Ey on the point
{
	return -a*x*z*y*y;
}
double CurlXYZ::funcE0z(double x, double y, double z)         //Initial Ez on the point
{
	return a*x*y*z*z / 2.0;
}
double CurlXYZ::funcB0x(double x, double y, double z)         //Initial Bx on the point
{
	return 0.0;
}
double CurlXYZ::funcB0y(double x, double y, double z)         //Initial By on the point
{
	return 0.0;
}
double CurlXYZ::funcB0z(double x, double y, double z)         //Initial Bz on the point
{
	return 0.0;
}
//////////////////////////////////////////////////////////////////
double CurlXYZ::fnDx1eR(double x, double y, double z)
{
	return 0;
}
double CurlXYZ::fnDy1eR(double x, double y, double z)
{
	return 0;
}
double CurlXYZ::fnDz1eR(double x, double y, double z)
{

	return 0;
}
double CurlXYZ::fnDxEx(double x, double y, double z)
{
	return a*x*y*z;
}
double CurlXYZ::fnDyEy(double x, double y, double z)
{
	return -2.0*a*x*y*z;
}
double CurlXYZ::fnDzEz(double x, double y, double z)
{
	return a*x*y*z;
}
/*
d_y E_x=1/2 azx^2;d_z E_y=-axy^2;d_x E_z=1/2 ayz^2
*/
double CurlXYZ::fnDyEx(double x, double y, double z)
{
	return  a*z*x*x/2.0;
}
double CurlXYZ::fnDzEy(double x, double y, double z)
{
	return -a*x*y*y;
}
double CurlXYZ::fnDxEz(double x, double y, double z)
{
	return  a*y*z*z/2.0;
}
/*
d_z E_x=1/2 ayx^2;d_x E_y=-azy^2;d_y E_z=1/2 axz^2
*/
double CurlXYZ::fnDzEx(double x, double y, double z)
{
	return a*y*x*x/ 2.0;
}
double CurlXYZ::fnDxEy(double x, double y, double z)
{
	return -a*z*y*y;
}
double CurlXYZ::fnDyEz(double x, double y, double z)
{
	return a*x*z*z / 2.0;
}
//////////////////////////////////////////
double CurlXYZ::fnDx2eR(double x, double y, double z)
{

	return 0;
}
double CurlXYZ::fnDy2eR(double x, double y, double z)
{

	return 0;
}
double CurlXYZ::fnDz2eR(double x, double y, double z)
{

	return 0;
}
double CurlXYZ::fnD2xEx(double x, double y, double z)
{
	return a * y * z;
}
double CurlXYZ::fnD2yEy(double x, double y, double z)
{
	return -2.0 * a * x * z;
}
double CurlXYZ::fnD2zEz(double x, double y, double z)
{
	return a * x * y;
}
//
double CurlXYZ::fnD2yEx(double x, double y, double z)
{
	return  0.0;
}
double CurlXYZ::fnD2zEy(double x, double y, double z)
{
	return 0.0;
}
double CurlXYZ::fnD2xEz(double x, double y, double z)
{
	return 0.0;
}
//
double CurlXYZ::fnD2zEx(double x, double y, double z)
{
	return 0.0;
}
double CurlXYZ::fnD2xEy(double x, double y, double z)
{
	return 0.0;
}
double CurlXYZ::fnD2yEz(double x, double y, double z)
{
	return 0.0;
}



/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

initial fields using Gaussian function
********************************************************************/
#include "GaussianTest.h"
#include "GaussianCurl.h"

#define r (x*x+y*y+z*z)
#define R (-k*r)
#define eR (pow(M_E, R))

#define E_x (b*y*z*eR/2.0)
#define E_y (-b*x*z*eR)
#define E_z (b*x*y*eR/2.0)
///1 st/////////////////////////////
#define D_xR (-2.0*k*x)
#define D_yR (-2.0*k*y)
#define D_zR (-2.0*k*z)

#define D_xeR (-2.0*k*x*eR)
#define D_yeR (-2.0*k*y*eR)
#define D_zeR (-2.0*k*z*eR)

#define D_xE_x (0.5* b * y*z*D_xeR)
#define D_yE_y (-b*x*z*D_yeR)
#define D_zE_z (0.5* b*x*y*D_zeR)
#define D_yE_x (0.5* b*z*eR + 0.5* b*y*z*D_yeR)
#define D_zE_y (-b*x*eR - b*x*z*D_zeR)
#define D_xE_z (0.5* b*y*eR + 0.5* b*x*y*D_xeR)
#define D_zE_x (0.5* b*y*eR + 0.5* b*y*z*D_zeR)
#define D_xE_y (-b*z*eR - b*x*z*D_xeR)
#define D_yE_z (0.5* b*x*eR + 0.5* b*x*y*D_yeR)

///2 nd ////////////////////////////
#define D_x2eR (-2.0*k*eR - 2.0*k*x*D_xeR)
#define D_y2eR (-2.0*k*eR - 2.0*k*y*D_yeR)
#define D_z2eR (-2.0*k*eR - 2.0*k*z*D_zeR)

#define D_yD_xeR ( -2.0*k*x*D_yeR)
#define D_zD_yeR ( -2.0*k*y*D_zeR)
#define D_xD_zeR ( -2.0*k*z*D_xeR)
#define D_zD_xeR ( -2.0*k*x*D_zeR)
#define D_xD_yeR ( -2.0*k*y*D_xeR)
#define D_yD_zeR ( -2.0*k*z*D_yeR)


#define D_x2E_x (0.5* b*y*z*D_x2eR)
#define D_y2E_y (-b*x*z*D_y2eR)
#define D_z2E_z (0.5* b*x*y*D_z2eR)
#define D_y2E_x (b*z*D_yeR + 0.5* b*y*z*D_y2eR)
#define D_z2E_y (-2.0*b*x*D_zeR - b*x*z*D_z2eR)
#define D_x2E_z (b*y*D_xeR + 0.5* b*x*y*D_x2eR)
#define D_z2E_x (b*y*D_zeR + 0.5* b*y*z*D_z2eR)
#define D_x2E_y (-2.0*b*z*D_xeR - b*x*z*D_x2eR)
#define D_y2E_z (b*x*D_yeR + 0.5* b*x*y*D_y2eR)

///3 rd ///////////////////////////
#define D_x3eR ( -4.0*k*D_xeR - 2.0*k*x*D_x2eR)
#define D_y3eR ( -4.0*k*D_yeR - 2.0*k*y*D_y2eR)
#define D_z3eR ( -4.0*k*D_zeR - 2.0*k*z*D_z2eR)
#define D_yD_x2eR ( -2.0*k*D_yeR - 2.0*k*x*D_yD_xeR)
#define D_zD_y2eR ( -2.0*k*D_zeR - 2.0*k*y*D_zD_yeR)
#define D_xD_z2eR ( -2.0*k*D_xeR - 2.0*k*z*D_xD_zeR)
#define D_zD_x2eR ( -2.0*k*D_zeR - 2.0*k*x*D_zD_xeR)
#define D_xD_y2eR ( -2.0*k*D_xeR - 2.0*k*y*D_xD_yeR)
#define D_yD_z2eR ( -2.0*k*D_yeR - 2.0*k*z*D_yD_zeR)

#define D_x3E_x ( 0.5* b*y*z*D_x3eR)
#define D_y3E_y ( -b*x*z*D_y3eR)
#define D_z3E_z ( 0.5* b*x*y*D_z3eR)

#define D_yD_x2E_x ( 0.5* b*z*D_x2eR + 0.5* b*y*z*D_yD_x2eR)
#define D_zD_y2E_y ( -b*x*D_y2eR - b*x*z*D_zD_y2eR)
#define D_xD_z2E_z ( 0.5* b*y*D_z2eR + 0.5* b*x*y*D_xD_z2eR)
#define D_zD_x2E_x ( 0.5* b*y*D_x2eR + 0.5* b*y*z*D_zD_x2eR)
#define D_xD_y2E_y ( -b*z*D_y2eR - b*x*z*D_xD_y2eR)
#define D_yD_z2E_z ( 0.5* b*x*D_z2eR + 0.5* b*x*y*D_yD_z2eR)

#define D_xD_y2E_x ( b*z*D_xD_yeR + 0.5* b*y*z*D_xD_y2eR)
#define D_yD_z2E_y ( -2.0*b*x*D_yD_zeR - b*x*z*D_yD_z2eR)
#define D_zD_x2E_z ( b*y*D_zD_xeR + 0.5* b*x*y*D_zD_x2eR)
#define D_y3E_x ( 1.5* b*z*D_y2eR + 0.5* b*y*z*D_y3eR)
#define D_z3E_y ( -3.0*b*x*D_z2eR - b*x*z*D_z3eR)
#define D_x3E_z ( 1.5* b*y*D_x2eR + 0.5* b*x*y*D_x3eR)
#define D_zD_y2E_x ( b*D_yeR + b*z*D_zD_yeR + 0.5* b*y*D_y2eR + 0.5* b*y*z*D_y3eR)
#define D_xD_z2E_y ( -2.0*b*D_zeR - 2.0*b*x*D_xD_zeR - b*z*D_z2eR - b*x*z*D_xD_z2eR)
#define D_yD_x2E_z ( b*D_xeR + b*y*D_yD_xeR + 0.5* b*x*D_x2eR + 0.5* b*x*y*D_yD_x2eR)
#define D_z3E_x ( 1.5* b*y*D_z2eR + 0.5* b*y*z*D_z3eR)
#define D_x3E_y ( -3.0* b*z*D_x2eR - b*x*z*D_x3eR)
#define D_y3E_z ( 1.5* b*x*D_y2eR + 0.5* b*x*y*D_y3eR)

#define D_yD_z2E_x ( b*D_zeR + b*y*D_yD_zeR + 0.5* b*z*D_z2eR + 0.5* b*y*z*D_yD_z2eR)
#define D_zD_x2E_y ( -2.0*b*D_xeR - 2.0*b*z*D_zD_xeR - b*x*D_x2eR - b*x*z*D_zD_x2eR)
#define D_xD_y2E_z ( b*D_yeR + b*x*D_xD_yeR + 0.5* b*y*D_y2eR + 0.5* b*x*y*D_xD_y2eR)

///////////////////////////////////
GaussianTest::GaussianTest()
{
}


GaussianTest::~GaussianTest()
{
}

int GaussianTest::initialize(double b0, double k0)
{
	int ret = ERR_OK;
	b = b0;
	k = k0;
	//
	//
	return ret;
}
int GaussianTest::initializeByConfig(TaskFile *configs)
{
	int ret = ERR_OK;
	double b0 = configs->getDouble(GAUSSIAN_MAGN, false);
	double k0 = configs->getDouble(GAUSSIAN_EXPO, false);
	ret = initialize(b0, k0);
	return ret;
}
void GaussianTest::curl1(double x, double y, double z, Point3Dstruct* curl)
{
	/*
	curl->x = D_y (E_z ) - D_z (E_y)
	curl->y = D_z (E_x ) - D_x (E_z )
	curl->z = D_x (E_y) - D_y (E_x )
	*/

	curl->x = D_yE_z - D_zE_y;
	curl->y = D_zE_x - D_xE_z;
	curl->z = D_xE_y - D_yE_x;
}

void GaussianTest::curl2(double x, double y, double z, Point3Dstruct* curl)
{
	/*
	-∇^{2} ×E=[(〖D_x^2 (E〗_x)+〖D_y^2 (E〗_x)+〖D_z^2 (E〗_x)
	〖D_x^2 (E〗_y)+〖D_y^2 (E〗_y)+〖D_z^2 (E〗_y)
	〖D_x^2 (E〗_z)+〖D_y^2 (E〗_z)+〖D_z^2 (E〗_z))]
	*/
	curl->x = -(D_x2E_x + D_y2E_x + D_z2E_x);
	curl->y = -(D_x2E_y + D_y2E_y + D_z2E_y);
	curl->z = -(D_x2E_z + D_y2E_z + D_z2E_z);
}
void GaussianTest::curl3(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = -(D_yD_x2E_z + D_y3E_z + D_yD_z2E_z - D_zD_x2E_y - D_zD_y2E_y - D_z3E_y);
	curl->y = -(D_zD_x2E_x + D_zD_y2E_x + D_z3E_x - D_x3E_z - D_xD_y2E_z - D_xD_z2E_z);
	curl->z = -(D_x3E_y + D_xD_y2E_y + D_xD_z2E_y - D_yD_x2E_x - D_y3E_x - D_yD_z2E_x);
}
void GaussianTest::curl4(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = 0.0;
	curl->y = 0.0;
	curl->z = 0.0;
}
void GaussianTest::SetFields(double x, double y, double z, Point3Dstruct *pE, Point3Dstruct *pH)
{
	//double eR = pow(M_E, -k * (x*x + y*y + z*z));
	pE->x = b*y*z*eR / 2.0;
	pE->y = -b*x*z*eR;
	pE->z = b*y*x*eR / 2.0;
	//
	pH->x = 0.0;
	pH->y = 0.0;
	pH->z = 0.0;
}

double GaussianTest::funcE0x(double x, double y, double z)         //Initial Ex on the point
{
	return E_x;
}
double GaussianTest::funcE0y(double x, double y, double z)         //Initial Ey on the point
{
	return E_y;
}
double GaussianTest::funcE0z(double x, double y, double z)         //Initial Ez on the point
{
	return E_z;
}
double GaussianTest::funcB0x(double x, double y, double z)         //Initial Bx on the point
{
	return 0.0;
}
double GaussianTest::funcB0y(double x, double y, double z)         //Initial By on the point
{
	return 0.0;
}
double GaussianTest::funcB0z(double x, double y, double z)         //Initial Bz on the point
{
	return 0.0;
}
//////////////////////////////////////////////////////////////////
double GaussianTest::fnDx1eR(double x, double y, double z)
{
	return D_xeR;
}
double GaussianTest::fnDy1eR(double x, double y, double z)
{
	return D_yeR;
}
double GaussianTest::fnDz1eR(double x, double y, double z)
{
	return D_zeR;
}
double GaussianTest::fnDxEx(double x, double y, double z)
{
	return D_xE_x;
}
double GaussianTest::fnDyEy(double x, double y, double z)
{
	return D_yE_y;
}
double GaussianTest::fnDzEz(double x, double y, double z)
{
	return D_zE_z;
}

double GaussianTest::fnDyEx(double x, double y, double z)
{
	return D_yE_x;
}
double GaussianTest::fnDzEy(double x, double y, double z)
{
	return D_zE_y;
}
double GaussianTest::fnDxEz(double x, double y, double z)
{
	return D_xE_z;
}
double GaussianTest::fnDzEx(double x, double y, double z)
{
	return D_zE_x;
}
double GaussianTest::fnDxEy(double x, double y, double z)
{
	return D_xE_y;
}
double GaussianTest::fnDyEz(double x, double y, double z)
{
	return D_yE_z;
}
//////////////////////////////////////////
double GaussianTest::fnDx2eR(double x, double y, double z)
{
	return D_x2eR;
}
double GaussianTest::fnDy2eR(double x, double y, double z)
{
	return  D_y2eR;
}
double GaussianTest::fnDz2eR(double x, double y, double z)
{
	return D_z2eR;
}
double GaussianTest::fnD2xEx(double x, double y, double z)
{
	return D_x2E_x;
}
double GaussianTest::fnD2yEy(double x, double y, double z)
{
	return  D_y2E_y;
}
double GaussianTest::fnD2zEz(double x, double y, double z)
{
	return D_z2E_z;
}
//
double GaussianTest::fnD2yEx(double x, double y, double z)
{
	return  D_y2E_x;
}
double GaussianTest::fnD2zEy(double x, double y, double z)
{
	return D_z2E_y;
}
double GaussianTest::fnD2xEz(double x, double y, double z)
{
	return D_x2E_z;
}
//
double GaussianTest::fnD2zEx(double x, double y, double z)
{
	return D_z2E_x;
}
double GaussianTest::fnD2xEy(double x, double y, double z)
{
	return D_xE_y;
}
double GaussianTest::fnD2yEz(double x, double y, double z)
{
	return D_y2E_z;
}

///////////////////////////////////////////
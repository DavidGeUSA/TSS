/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

initial fields using Gaussian function
********************************************************************/
#include "GaussianCurl.h"

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

//#define eR (pow(M_E, -k * (x*x + y*y + z*z)))

GaussianCurl::GaussianCurl()
{

}


GaussianCurl::~GaussianCurl()
{
}
//int GaussianCurl::initialize(double b, double k, unsigned int nx0, unsigned int ny0, unsigned int nz0, double xmin0, double xmax0, double ymin0, double ymax0, double zmin0, double zmax0)
//{
//	int ret = ERR_OK;
//	b0 = b;
//	k0 = k;
//	//
//	nx = nx0; ny = ny0; nz = nz0;
//	xmin = xmin0; xmax = xmax0; ymin = ymin0; ymax = ymax0; zmin = zmin0; zmax = zmax0;
//	dx = (xmax - xmin) / nx; dy = (ymax - ymin) / ny; dz = (zmin - zmax) / nz;
//	//
//	return ret;
//}
int GaussianCurl::initializeByConfig(TaskFile *configs)
{
	int ret = ERR_OK;
	double b0 = configs->getDouble(GAUSSIAN_MAGN, false);
	double k0 = configs->getDouble(GAUSSIAN_EXPO, false);
	ret = initialize(b0, k0);
	return ret;
}
int GaussianCurl::initialize(double b0, double k0)
{
	int ret = ERR_OK;
	b = b0;
	k = k0;
	_x = &defP;
	//
	//
	return ret;
}

void GaussianCurl::curl1(double x, double y, double z, Point3Dstruct* curl)
{
	/*
	D_x (e^R )=-2kxe^R
	D_y (e^R )=-2kye^R
	D_z (e^R )=-2kze^R

	D_y (E_x )=1/2 bze^R+1/2 byzD_y (e^R)
	D_z (E_y)=-bxe^R-bxzD_z (e^R)
	D_x (E_z )=1/2 bye^R+1/2 bxyD_x (e^R)
	D_z (E_x )=1/2 bye^R+1/2 byzD_z (e^R)
	D_x (E_y)=-bze^R-bxzD_x (e^R)
	D_y (E_z )=1/2 bxe^R+1/2 bxyD_y (e^R)
	curl->x = D_y (E_z ) - D_z (E_y)
	curl->y = D_z (E_x ) - D_x (E_z )
	curl->z = D_x (E_y) - D_y (E_x )
	*/
	double eR = pow(M_E, -k * (x*x+y*y+z*z));
	DxeR = -2.0*k*x*eR;
	DyeR = -2.0*k*y*eR;
	DzeR = -2.0*k*z*eR;

	curl->x =   (b*x / 2.0)*(eR + y*DyeR) + b*x*(eR + z*DzeR);
	curl->y = (b*y / 2.0)*(z*DzeR - x*DxeR);
	curl->z = -((b*z / 2.0)*(eR + y*DyeR) + b*z*(eR + x*DxeR));
}

void GaussianCurl::curl2(double x, double y, double z, Point3Dstruct* curl)
{
	/*
	-∇^{2} ×E=[(〖D_x^2 (E〗_x)+〖D_y^2 (E〗_x)+〖D_z^2 (E〗_x)
				 〖D_x^2 (E〗_y)+〖D_y^2 (E〗_y)+〖D_z^2 (E〗_y)
				 〖D_x^2 (E〗_z)+〖D_y^2 (E〗_z)+〖D_z^2 (E〗_z))]
	*/
	//curl->x = -(fnD2xEx(x, y, z) + fnD2yEx(x, y, z) + fnD2zEx(x, y, z));
	//curl->y = -(fnD2xEy(x, y, z) + fnD2yEy(x, y, z) + fnD2zEy(x, y, z));
	//curl->z = -(fnD2xEz(x, y, z) + fnD2yEz(x, y, z) + fnD2zEz(x, y, z));
	//
	/*
	∇^{2} ×E=-[((2k(x^2+y^2+z^2 )-7)bkyze^R
	            (7-2k(x^2+y^2+z^2))2bkxze^R
				(2k(x^2+y^2+z^2)-7)bkxye^R )]
	*/
	double r2 = x*x + y*y + z*z;
	double eR = pow(M_E, -k * r2);
	double r7 = (7.0 - 2.0 *k*r2)*b*k*eR;
	curl->x = r7*y*z;
	curl->y = -r7*2.0*x*z;
	curl->z = r7*x*y;
}
void GaussianCurl::curl3(double x, double y, double z, Point3Dstruct* curl)
{
	/*
	f=(7 - 2k(x^2+y^2+z^2))bke^(-k(x^2+y^2+z^2 ) )
	x : (3-2ky^2-4kz^2)xf-(y^2+2z^2)4bk^2 xe^R
	y : 2ky(x^2-z^2)(f+2bke^R)
	z : (4kx^2+2ky^2-3)zf+(2x^2+y^2)4bk^2 ze^R
	*/
	double r2 = x*x + y*y + z*z;
	double eR = pow(M_E, -k*r2);
	double f = (7.0 - 2.0*k*r2)*b*k*eR;
	double v = 2.0*k*(y*y + 2.0*z*z), w = 2.0*k*(y*y + 2.0*x*x);
	//
	curl->x = (3.0 - v)*x*f - 2.0*b*k*v*x*eR;
	curl->y = 2.0*k*y*(x*x - z*z)*(f + 2.0*b*k*eR);
	curl->z = (-3.0 + w)*z*f + 2.0*b*k*w*z*eR;
}
void GaussianCurl::curl4(double x, double y, double z, Point3Dstruct* curl)
{
	curl->x = 0.0;
	curl->y = 0.0;
	curl->z = 0.0;
}
void GaussianCurl::SetFields(double x, double y, double z, Point3Dstruct *pE, Point3Dstruct *pH)
{
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	pE->x = b*y*z*eR / 2.0;
	pE->y = -b*x*z*eR;
	pE->z = b*y*x*eR / 2.0;
	//
	pH->x = 0.0;
	pH->y = 0.0;
	pH->z = 0.0;
}
/*
double GaussianCurl::funcE0x(double x, double y, double z)         //Initial Ex on the point
{
	//SETPOINT(x, y, z)
	//	INITPOINT
	//	return EX;
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	return b*y*z*eR / 2.0;
}
double GaussianCurl::funcE0y(double x, double y, double z)         //Initial Ey on the point
{
	//SETPOINT(x, y, z)
	//	INITPOINT
	//	return EY;
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	return -b*x*z*eR;
}
double GaussianCurl::funcE0z(double x, double y, double z)         //Initial Ez on the point
{
	//SETPOINT(x, y, z)
	//	INITPOINT
	//	return EZ;
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	return b*y*x*eR / 2.0;
}
double GaussianCurl::funcB0x(double x, double y, double z)         //Initial Bx on the point
{
	return 0.0;
}
double GaussianCurl::funcB0y(double x, double y, double z)         //Initial By on the point
{
	return 0.0;
}
double GaussianCurl::funcB0z(double x, double y, double z)         //Initial Bz on the point
{
	return 0.0;
}
*/
//////////////////////////////////////////////////////////////////
double GaussianCurl::fnDx1eR(double x, double y, double z)
{
	/*
	-2kxe^R
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	return -2.0*k*x*eR;
}
double GaussianCurl::fnDy1eR(double x, double y, double z)
{
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	return -2.0*k*y*eR;
}
double GaussianCurl::fnDz1eR(double x, double y, double z)
{
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	return -2.0*k*z*eR;
}
double GaussianCurl::fnDxEx(double x, double y, double z)
{
	/*
	D_x (e^R )=-2kxe^R
	1/2 byzD_x (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	double DxeR = -2.0*k*x*eR;
	return b*y*z*DxeR / 2.0;
}
double GaussianCurl::fnDyEy(double x, double y, double z)
{
	/*
	D_y (e^R )=-2kye^R
	-bxzD_y (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	double DyeR = -2.0*k*y*eR;
	return -b*x*z*DyeR;
}
double GaussianCurl::fnDzEz(double x, double y, double z)
{
	/*
	D_z (e^R )=-2kze^R
	1/2 byxD_z (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	double DzeR = -2.0*k*z*eR;
	return b*y*x*DzeR / 2.0;
}

double GaussianCurl::fnDyEx(double x, double y, double z)
{
	/*
	D_y (E_x )=1/2 bze^R+1/2 byzD_y (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	double DyeR = -2.0*k*y*eR;
	return (b*z*eR + b*y*z*DyeR) / 2.0;
}
double GaussianCurl::fnDzEy(double x, double y, double z)
{
	/*
	D_z (E_y)=-bxe^R-bxzD_z (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	double DzeR = -2.0*k*z*eR;
	return -b*x*eR - b*x*z*DzeR;
}
double GaussianCurl::fnDxEz(double x, double y, double z)
{
	/*
	D_x (E_z )=1/2 bye^R+1/2 bxyD_x (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	double DxeR = -2.0*k*x*eR;
	return (b*y*eR + b*x*y*DxeR) / 2.0;
}
double GaussianCurl::fnDzEx(double x, double y, double z)
{
	/*
	D_z (E_x )=1/2 bye^R+1/2 byzD_z (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	double DzeR = -2.0*k*z*eR;
	return (b*y*eR + b*y*z*DzeR) / 2.0;
}
double GaussianCurl::fnDxEy(double x, double y, double z)
{
	/*
	D_x (E_y)=-bze^R-bxzD_x (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	double DxeR = -2.0*k*x*eR;
	return -b*z*eR - b*x*z*DxeR;
}
double GaussianCurl::fnDyEz(double x, double y, double z)
{
	/*
	D_y (E_z )=1/2 bxe^R+1/2 bxyD_y (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	double DyeR = -2.0*k*y*eR;
	return ( b*x*eR + b*x*y*DyeR) / 2.0;
}
//////////////////////////////////////////
double GaussianCurl::fnDx2eR(double x, double y, double z)
{
	/*
	D_x^2 (e^R )=-2ke^R-2kxD_x (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	return -2.0*k*eR - 2.0*k*x*fnDx1eR(x,y,z);
}
double GaussianCurl::fnDy2eR(double x, double y, double z)
{
	/*
	D_y^2 (e^R )=-2ke^R-2kyD_y (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	return  -2.0*k*eR - 2.0*k*y*fnDy1eR(x,y,z);
}
double GaussianCurl::fnDz2eR(double x, double y, double z)
{
	/*
	D_z^2 (e^R )=-2ke^R-2kzD_z (e^R)
	*/
	double eR = pow(M_E, -k * (x*x + y*y + z*z));
	return -2.0*k*eR - 2.0*k*z*fnDz1eR(x, y, z);
}
double GaussianCurl::fnD2xEx(double x, double y, double z)
{
	/*
	〖D_x^2 (E〗_x)=1/2 byzD_x^2 (e^R)
	*/
	return b*y*z*fnDx2eR(x,y,z) / 2.0;
}
double GaussianCurl::fnD2yEy(double x, double y, double z)
{
	/*
	D_y^2 (E_y)=-bxzD_y^2 (e^R)
	*/
	return  -b*x*z*fnDy2eR(x,y,z);
}
double GaussianCurl::fnD2zEz(double x, double y, double z)
{
	/*
	D_z^2 (E_z)=1/2 bxyD_z^2 (e^R)
	*/
	return b*x*y*fnDz2eR(x,y,z)/2.0;
}
//
double GaussianCurl::fnD2yEx(double x, double y, double z)
{
	/*
	D_y^2 (E_x )=bzD_y (e^R)+1/2 byzD_y^2 (e^R)
	*/
	return  b*z*fnDy1eR(x, y, z) + b*y*z*fnDy2eR(x,y,z) / 2.0;
}
double GaussianCurl::fnD2zEy(double x, double y, double z)
{
	/*
	D_z^2 (E_y)=-2bxD_z (e^R)-bxzD_z^2 (e^R)
	*/
	return -2.0*b*x*fnDz1eR(x,y,z) - b*x*z*fnDz2eR(x,y,z);
}
double GaussianCurl::fnD2xEz(double x, double y, double z)
{
	/*
	D_x^2 (E_z )=byD_x (e^R)+1/2 bxyD_x^2 (e^R)
	*/
	return b*y*fnDx1eR(x,y,z) + b*x*y*fnDx2eR(x,y,z)/2.0;
}
//
double GaussianCurl::fnD2zEx(double x, double y, double z)
{
	/*
	D_z^2 (E_x )=byD_z (e^R)+1/2 byzD_z^2 (e^R)
	*/
	return b*y*fnDz1eR(x,y,z) + b*y*z*fnDz2eR(x,y,z) / 2.0;
}
double GaussianCurl::fnD2xEy(double x, double y, double z)
{
	/*
	D_x^2 (E_y)=-2bzD_x (e^R)-bxzD_x^2 (e^R)
	*/
	return -2.0*b*z*fnDx1eR(x,y,z) - b*x*z*fnDx2eR(x,y,z);
}
double GaussianCurl::fnD2yEz(double x, double y, double z)
{
	/*
	D_y^2 (E_z )=bxD_y (e^R)+1/2 bxyD_y^2 (e^R)
	*/
	return b*x*fnDy1eR(x,y,z) + b*x*y*fnDy2eR(x,y,z) / 2.0;
}

///////////////////////////////////////////
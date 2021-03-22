#pragma once
#include "FieldsSetter.h"

#define GAUSSIAN_MAGN "GAUSSIAN.MAGNITUDE"
#define GAUSSIAN_EXPO "GAUSSIAN.EXPO"

class GaussianCurl : virtual public FieldsSetter
{
private:
	Point3D* _x;
	Point3D defP;
	//configurations to be read from a task file
	double k, b;
	//calculated from k and b
	double bk, kp2;
	//values determined by location
	double x2, y2, z2;
	double _r2;
	double ek;
	double dd;

	//
	//for calculating curls
	double R; //-k/r
	double DxR, DyR, DzR, DxeR, DyeR, DzeR;
	double DxEx, DyEy, DzEz, DyEx, DzEy, DxEz, DzEx, DxEy, DyEz;
public:
	GaussianCurl();
	~GaussianCurl();
	//
	virtual int initializeByConfig(TaskFile *configs);              //read back configurations from a task file
	int initialize(double b, double k);
	///////////////////////////////////////////////
	double fnDx1eR(double x, double y, double z);
	double fnDy1eR(double x, double y, double z);
	double fnDz1eR(double x, double y, double z);
	//
	double fnDxEx(double x, double y, double z);
	double fnDyEy(double x, double y, double z);
	double fnDzEz(double x, double y, double z);
	//
	double fnDyEx(double x, double y, double z);
	double fnDzEy(double x, double y, double z);
	double fnDxEz(double x, double y, double z);
	//
	double fnDzEx(double x, double y, double z);
	double fnDxEy(double x, double y, double z);
	double fnDyEz(double x, double y, double z);
	/////////////////////////////////////////////////
	double fnDx2eR(double x, double y, double z);
	double fnDy2eR(double x, double y, double z);
	double fnDz2eR(double x, double y, double z);
	//
	double fnD2xEx(double x, double y, double z);
	double fnD2yEy(double x, double y, double z);
	double fnD2zEz(double x, double y, double z);
	//
	double fnD2yEx(double x, double y, double z);
	double fnD2zEy(double x, double y, double z);
	double fnD2xEz(double x, double y, double z);
	//
	double fnD2zEx(double x, double y, double z);
	double fnD2xEy(double x, double y, double z);
	double fnD2yEz(double x, double y, double z);
	//
	void curl1(double x, double y, double z, Point3Dstruct* curl);
	void curl2(double x, double y, double z, Point3Dstruct* curl);
	void curl3(double x, double y, double z, Point3Dstruct* curl);
	void curl4(double x, double y, double z, Point3Dstruct* curl);
	void curl5(double x, double y, double z, Point3Dstruct* curl){}
	void curl6(double x, double y, double z, Point3Dstruct* curl){}
	void curl7(double x, double y, double z, Point3Dstruct* curl){}
	//
	//virtual double funcE0x(double x, double y, double z);               //Initial Ex at the point
	//virtual double funcE0y(double x, double y, double z);               //Initial Ey at the point
	//virtual double funcE0z(double x, double y, double z);               //Initial Ez at the point
	//virtual double funcB0x(double x, double y, double z);               //Initial Bx at the point
	//virtual double funcB0y(double x, double y, double z);               //Initial By at the point
	//virtual double funcB0z(double x, double y, double z);               //Initial Bz at the point
	//virtual void getField(double x, double y, double z, FieldPoint3D *f){} //initialize EM fields at the point
	//
	virtual void SetFields(double x, double y, double z, Point3Dstruct *pE, Point3Dstruct *pH);
};


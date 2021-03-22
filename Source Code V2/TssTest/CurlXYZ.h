#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

initial fields for curl test
********************************************************************/
#include "../Tss/FieldsSetter.h"
#include "FieldsTester.h"

#define CURLXYZ_MAGNITUDE "CURLXYZ.MAGNITUDE"

class CurlXYZ :
	virtual public FieldsTester
{
private:
	double a;
public:
	CurlXYZ();
	~CurlXYZ();
	virtual int initializeByConfig(TaskFile *configs);
	int initialize(double b, double k);
	///////////////////////////////////////////////
	virtual double fnDx1eR(double x, double y, double z);
	virtual double fnDy1eR(double x, double y, double z);
	virtual double fnDz1eR(double x, double y, double z);
	//
	virtual double fnDxEx(double x, double y, double z);
	virtual double fnDyEy(double x, double y, double z);
	virtual double fnDzEz(double x, double y, double z);
	//
	virtual double fnDyEx(double x, double y, double z);
	virtual double fnDzEy(double x, double y, double z);
	virtual double fnDxEz(double x, double y, double z);
	//
	virtual double fnDzEx(double x, double y, double z);
	virtual double fnDxEy(double x, double y, double z);
	virtual double fnDyEz(double x, double y, double z);
	/////////////////////////////////////////////////
	double fnDx2eR(double x, double y, double z);
	double fnDy2eR(double x, double y, double z);
	double fnDz2eR(double x, double y, double z);
	//
	virtual double fnD2xEx(double x, double y, double z);
	virtual double fnD2yEy(double x, double y, double z);
	virtual double fnD2zEz(double x, double y, double z);
	//
	virtual double fnD2yEx(double x, double y, double z);
	virtual double fnD2zEy(double x, double y, double z);
	virtual double fnD2xEz(double x, double y, double z);
	//
	virtual double fnD2zEx(double x, double y, double z);
	virtual double fnD2xEy(double x, double y, double z);
	virtual double fnD2yEz(double x, double y, double z);
	//
	virtual void curl1(double x, double y, double z, Point3Dstruct* curl);
	virtual void curl2(double x, double y, double z, Point3Dstruct* curl);
	virtual void curl3(double x, double y, double z, Point3Dstruct* curl);
	virtual void curl4(double x, double y, double z, Point3Dstruct* curl);
	virtual void curl5(double x, double y, double z, Point3Dstruct* curl){}
	virtual void curl6(double x, double y, double z, Point3Dstruct* curl){}
	virtual void curl7(double x, double y, double z, Point3Dstruct* curl){}
	//
	virtual double funcE0x(double x, double y, double z);               //Initial Ex at the point
	virtual double funcE0y(double x, double y, double z);               //Initial Ey at the point
	virtual double funcE0z(double x, double y, double z);               //Initial Ez at the point
	virtual double funcB0x(double x, double y, double z);               //Initial Bx at the point
	virtual double funcB0y(double x, double y, double z);               //Initial By at the point
	virtual double funcB0z(double x, double y, double z);               //Initial Bz at the point
	virtual void getField(double x, double y, double z, FieldPoint3D *f){} //initialize EM fields at the point

	virtual void SetFields(double x, double y, double z, Point3Dstruct *pE, Point3Dstruct *pH);
};


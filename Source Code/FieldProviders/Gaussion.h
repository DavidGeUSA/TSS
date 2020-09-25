#pragma once
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "..\EMField\EMField.h"

#define TP_IV_MAGNITUDE "IV.MAGNITUDE"
#define TP_IV_WIDTH     "IV.WIDTH"

/*
	space distribution of field using Gaussian function
*/
class GaussianFields:public FieldsInitializer
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
public:
	GaussianFields();
	~GaussianFields();
	//FieldsInitializer members
	virtual void getField(double x, double y, double z, FieldPoint3D *f); //initialize EM fields at the point
	virtual int initialize(TaskFile *taskParameters);                     //read back configurations from a task file
	virtual double funcE0x(double x, double y, double z);                 //Initial Ex on the point
	virtual double funcE0y(double x, double y, double z);                 //Initial Ey on the point
	virtual double funcE0z(double x, double y, double z);                 //Initial Ez on the point
	virtual double funcB0x(double x, double y, double z);                 //Initial Bx on the point
	virtual double funcB0y(double x, double y, double z);                 //Initial By on the point
	virtual double funcB0z(double x, double y, double z);                 //Initial Bz on the point
};


#pragma once
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "..\EMField\EMField.h"


/*
	set fields to 0
*/
class ZeroFields:public FieldsInitializer
{
private:
	Point3D* _x;
	Point3D defP;
public:
	ZeroFields(void);
	~ZeroFields();
	//FieldsInitializer members
	virtual void getField(double x, double y, double z, FieldPoint3D *f);
	virtual int initialize(TaskFile *taskParameters);
	virtual double funcE0x(double x, double y, double z);         //Initial Ex on the point
	virtual double funcE0y(double x, double y, double z);         //Initial Ey on the point
	virtual double funcE0z(double x, double y, double z);         //Initial Ez on the point
	virtual double funcB0x(double x, double y, double z);         //Initial Bx on the point
	virtual double funcB0y(double x, double y, double z);         //Initial By on the point
	virtual double funcB0z(double x, double y, double z);         //Initial Bz on the point
};


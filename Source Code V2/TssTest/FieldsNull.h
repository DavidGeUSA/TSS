#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

initial fields to 0
********************************************************************/
#include "FieldsTester.h"

class FieldsNull :
	virtual public FieldsTester
{
public:
	FieldsNull();
	~FieldsNull();
	virtual int initializeByConfig(TaskFile *configs){ return 0; }
	virtual void SetFields(double x, double y, double z, Point3Dstruct *pE, Point3Dstruct *pH);
	virtual double fnDxEx(double x, double y, double z){ return 0; }
	virtual double fnDyEy(double x, double y, double z){ return 0; }
	virtual double fnDzEz(double x, double y, double z){ return 0; }
	//
	virtual double fnDyEx(double x, double y, double z){ return 0; }
	virtual double fnDzEy(double x, double y, double z){ return 0; }
	virtual double fnDxEz(double x, double y, double z){ return 0; }
	//
	virtual double fnDzEx(double x, double y, double z){ return 0; }
	virtual double fnDxEy(double x, double y, double z){ return 0; }
	virtual double fnDyEz(double x, double y, double z){ return 0; }
	/////////////////////////////////////////////////
	virtual double fnD2xEx(double x, double y, double z){ return 0; }
	virtual double fnD2yEy(double x, double y, double z){ return 0; }
	virtual double fnD2zEz(double x, double y, double z){ return 0; }
	//
	virtual double fnD2yEx(double x, double y, double z){ return 0; }
	virtual double fnD2zEy(double x, double y, double z){ return 0; }
	virtual double fnD2xEz(double x, double y, double z){ return 0; }
	//
	virtual double fnD2zEx(double x, double y, double z){ return 0; }
	virtual double fnD2xEy(double x, double y, double z){ return 0; }
	virtual double fnD2yEz(double x, double y, double z){ return 0; }
	//
	virtual void curl1(double x, double y, double z, Point3Dstruct* curl){}
	virtual void curl2(double x, double y, double z, Point3Dstruct* curl){}
	virtual void curl3(double x, double y, double z, Point3Dstruct* curl){}
	virtual void curl4(double x, double y, double z, Point3Dstruct* curl){}
	virtual void curl5(double x, double y, double z, Point3Dstruct* curl){}
	virtual void curl6(double x, double y, double z, Point3Dstruct* curl){}
	virtual void curl7(double x, double y, double z, Point3Dstruct* curl){}

};


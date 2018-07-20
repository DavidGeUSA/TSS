#pragma once
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/

#include "..\EMField\EMField.h"
#include "..\EMField\RadiusIndex.h"

/*
	fill fields at time=0
	The field components in a Yee's algorithm are at different space locations
	This class populates fields according to the requirements of Yee's algorithm
*/
class PopulateYeeFieldsTime0:public virtual GoThroughSphereBySpaces
{
private:
	FieldsInitializer* datasource;
	FieldPoint3D *_field;
	double ds2;
	double dx,dy,dz; //space shift
public:
	PopulateYeeFieldsTime0(FieldsInitializer* source, FieldPoint3D *field, double yeeCellSize);
	void setShifted(double shiftX, double shiftY, double shiftZ);
	virtual void handleData(double x, double y, double z);
};

#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 03/31/2018
Allrights reserved by David Ge

********************************************************************/
#include "Tss.h"
#include "../EMField/Plugin.h"
/*
	set values to E and H fields 
*/
class FieldsSetter: virtual public Plugin
{
public:
	FieldsSetter();
	~FieldsSetter();
	//give derived classes a chance to read configurations from the task file
	virtual int initializeByConfig(TaskFile *configs){ return 0; }
	//major functions
	virtual void SetFields(double x, double y, double z, Point3Dstruct *pE, Point3Dstruct *pH){}
};


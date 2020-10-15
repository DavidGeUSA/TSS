#ifndef __FIELDSOURCE_H__
#define __FIELDSOURCE_H__

/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "EMField.h"
#include "RadiusIndex.h"
#include "Plugin.h"
#include "..\FileUtil\taskFile.h"
#include "..\MemoryMan\MemoryManager.h"

/*
	abstract class for field source. a source should be implemented in a dynamic link library 
	to be plugged into a simulation system at runtime.
*/
class FieldSource: public GoThroughSphereByIndexes, public virtual MemoryManUser, public Plugin
{
protected:
	double Cdtds;          //Courant number = 1.0 / sqrt(3.0)
	double dt;             //time step = (ds / c0) / sqrt(3.0);
	double ds;             //space step
	FieldPoint3D *_fields; //EM field. the source will be applied to it
	size_t _timeIndex;     //time step index, in case a source may need it
	double _time;          //time value, in case a source may need it
public:
	FieldSource(void);
	//
	/*
		initialize the source. a subclass overrides this function to do more initializations, including reading back task parameters.
	*/
	virtual int initialize(double Courant,double timeStep, double spaceStep, int maximumRadius, TaskFile *taskParameters);

	/*
		prepare variables for the source to work
	*/
	virtual void reset(FieldPoint3D *fields, size_t timeIndex, double time){_fields=fields; _timeIndex=timeIndex; _time = time;}
	
};

#endif
#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 03/31/2018
Allrights reserved by David Ge

********************************************************************/

#include "../MemoryMan/MemoryManager.h"
#include "Plugin.h"
#include "EMField.h"
/*
abstract class to provide fields at time=0.
a subclass should be implemented in a dynamic link library
to be plugged into a simulation system at runtime.
*/
class FieldsInitializer : public virtual MemoryManUser, public Plugin
{
public:
	virtual int initialize(TaskFile *taskParameters) = 0;               //read back configurations from a task file
	virtual double funcE0x(double x, double y, double z) = 0;               //Initial Ex at the point
	virtual double funcE0y(double x, double y, double z) = 0;               //Initial Ey at the point
	virtual double funcE0z(double x, double y, double z) = 0;               //Initial Ez at the point
	virtual double funcB0x(double x, double y, double z) = 0;               //Initial Bx at the point
	virtual double funcB0y(double x, double y, double z) = 0;               //Initial By at the point
	virtual double funcB0z(double x, double y, double z) = 0;               //Initial Bz at the point
	virtual void getField(double x, double y, double z, FieldPoint3D *f) = 0; //initialize EM fields at the point
};

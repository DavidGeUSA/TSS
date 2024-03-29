#pragma once
#include "../EMField/FieldSource.h"
class SourceX :
	virtual public FieldSource
{
public:
	SourceX();
	~SourceX();
	virtual int initialize(TaskFile *configs){ return 0; }
	virtual int initialize(double Courant, double timeStep, double spaceStep, int maximumRadius, TaskFile *taskParameters);
	//
	virtual RadiusHandleType setRadius(int radius);
	virtual void handleData(int m, int n, int p);
};


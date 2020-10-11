#pragma once
#include "C:\Users\dge66\Documents\TSS\EMField\FieldSource.h"
class SourceX :
	public FieldSource
{
public:
	SourceX();
	~SourceX();

	virtual int initialize(double Courant, int maximumRadius, TaskFile *taskParameters);
	//
	virtual RadiusHandleType setRadius(int radius);
	virtual void handleData(int m, int n, int p);
};


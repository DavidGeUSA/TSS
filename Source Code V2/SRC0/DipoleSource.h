#pragma once
#include "../EMField/FieldSource.h"
class DipoleSource :
	virtual public FieldSource
{
private:
	double rtau;
protected:
	double c0;   //light speed
	double mu0;  //Permeability
	double eps0; //Permittivity
public:
	DipoleSource();
	~DipoleSource();
	virtual int initialize(TaskFile *configs){ return 0; }
	virtual int initialize(double Courant, double timeStep, double spaceStep, int maximumRadius, TaskFile *taskParameters);
	//
	virtual RadiusHandleType setRadius(int radius);
	virtual void handleData(int m, int n, int p);
};


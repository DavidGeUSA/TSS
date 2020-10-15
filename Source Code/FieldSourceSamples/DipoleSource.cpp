#include "DipoleSource.h"


DipoleSource::DipoleSource()
{
	//physics constants ==============
	c0 = 299792458.0; //speed of light
	mu0 = 4.0 * M_PI * 1.0e-7;
	eps0 = 1.0 / (mu0 * c0 * c0);
	//=================================

	rtau = 2.0e-9;
}


DipoleSource::~DipoleSource()
{
}

int DipoleSource::initialize(double Courant, double timeStep, double spaceStep, int maximumRadius, TaskFile *taskParameters)
{
	int ret = FieldSource::initialize(Courant, timeStep, spaceStep, maximumRadius, taskParameters);
	if (ret == ERR_OK)
	{
		//other initialization code here
	}
	return ret;
}

RadiusHandleType DipoleSource::setRadius(int radius)
{
	return NeedProcess;
}

void DipoleSource::handleData(int m, int n, int p)
{
	double a = (_time - 3.0 * rtau) / rtau;
	double dp = -(dt / (eps0 * (ds*ds*ds)));
	double pn = 1.0e-10 * exp(-a * a);
	double sn = dp * pn;
	_fields[0].E.z += sn;
}


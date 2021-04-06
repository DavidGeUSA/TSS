/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

field source of Ricker function
********************************************************************/
#include "RickerSource.h"

RickerSource::RickerSource()
{
	courant = 1.0 / sqrt(3.0);
}


RickerSource::~RickerSource()
{
}

#define TP_FS_PPW "FS.PPW"
int RickerSource::initialize(TaskFile *configs)
{
	int ret = ERR_OK;
	ppw = configs->getDouble(TP_FS_PPW, false);
	ret = configs->getErrorCode();
	if (ret == ERR_OK)
	{
		if (ppw <= 0.0)
		{
			ret = ERR_TASK_INVALID_VALUE;
			configs->setNameOfInvalidValue(TP_FS_PPW);
		}
	}
	return ret;
}
int RickerSource::initialize(SimStruct *params)
{
	int ret = FieldSourceTss::initialize(params);
	i0 = pams->nx / 2;
	j0 = pams->ny / 2;
	k0 = pams->nz / 2;
	w0 = k0 + (pams->nz + 1)*(j0 + (pams->ny + 1)*i0);
	return ret;
}
bool RickerSource::isInSource(unsigned int i, unsigned int j, unsigned int k)
{
	return (i == i0 && j == j0 && k == k0);
}
int RickerSource::applySources(double t, size_t tIndex, Point3Dstruct *efile, Point3Dstruct *hfile)
{
	int ret = ERR_OK;
	//H = C.F.dJm + C.G.dJe + ...
	//E = C.U.dJe + C.W.dJm + ...
	if (pams->kmax == 0)
	{
		//g[0] = 0
		//u[0]*Je
		//apply at center
		//int i = pams->nx / 2;
		//int j = pams->ny / 2;
		//int k = pams->nz / 2;
		double arg;
		arg = M_PI * ((courant * (double)tIndex - 0.0) / ppw - 1.0);
		arg = arg * arg;
		arg = (1.0 - 2.0 * arg) * exp(-arg);
		efile[w0].z += arg;
	}
	else //if (pams->kmax == 1)
	{
		//int i = pams->nx / 2;
		//int j = pams->ny / 2;
		//int k = pams->nz / 2;
		double arg;
		arg = M_PI * ((courant * (double)tIndex - 0.0) / ppw - 1.0);
		arg = arg * arg;
		arg = (1.0 - 2.0 * arg) * exp(-arg);
		efile[w0].z += arg;

	}
	return ret;
}

int RickerSource::applyToZrotateSymmetry(double t, size_t tIndex, RotateSymmetryField *efile, RotateSymmetryField *hfile)
{
	int ret = ERR_OK;
	//H = C.F.dJm + C.G.dJe + ...
	//E = C.U.dJe + C.W.dJm + ...
	//
	//g[0] = 0
	//u[0]*Je
	//apply at center
	//int i = pams->nx / 2;
	//int j = pams->ny / 2;
	//int k = pams->nz / 2;
	double arg;
	arg = M_PI * ((courant * (double)tIndex - 0.0) / ppw - 1.0);
	arg = arg * arg;
	arg = (1.0 - 2.0 * arg) * exp(-arg);
	//efile[w0].z += arg;
	(efile->getFieldOnPlane(i0, k0))->z += arg;
	return ret;
}
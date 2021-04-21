/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 04/17/2021
Allrights reserved by David Ge

********************************************************************/
#include "DipoleSourceTss.h"


DipoleSourceTss::DipoleSourceTss()
{
}


DipoleSourceTss::~DipoleSourceTss()
{
}

/*
use task file to read parameters if so desired
*/
int DipoleSourceTss::initialize(TaskFile *configs)
{
	int ret = FieldSourceTss::initialize(configs);
	if (ret == ERR_OK)
	{
		x00 = configs->getDouble(DIPOLE_X, false);
		y00 = configs->getDouble(DIPOLE_Y, false);
		z10 = configs->getDouble(DIPOLE_Z10, false);
		z11 = configs->getDouble(DIPOLE_Z11, false);
		z20 = configs->getDouble(DIPOLE_Z20, false);
		z21 = configs->getDouble(DIPOLE_Z21, false);
		magnitude = configs->getDouble(SOURCE_MAGNITUDE, false);
		centerFrequency = configs->getDouble(GAUSSIAN_FRE, false);
	}
	return ret;
}
/*
initialize based on simulation parameters
*/
int DipoleSourceTss::initialize(SimStruct *params)
{
	int ret = FieldSourceTss::initialize(params);
	if (ret == ERR_OK)
	{
		i00 = (unsigned int)round((x00 - pams->xmin) / pams->ds);
		j00 = (unsigned int)round((y00 - pams->ymin) / pams->ds);
		//
		k10 = (unsigned int)round((z10 - pams->zmin) / pams->ds);
		k11 = (unsigned int)round((z11 - pams->zmin) / pams->ds);
		if (k10 > k11)
		{
			unsigned int k = k11;
			k11 = k10;
			k10 = k;
		}
		//
		k20 = (unsigned int)round((z20 - pams->zmin) / pams->ds);
		k21 = (unsigned int)round((z21 - pams->zmin) / pams->ds);
		if (k20 > k21)
		{
			unsigned int k = k21;
			k21 = k20;
			k20 = k;
		}
		//
		iN = i00 - pams->smax;
		iP = i00 + pams->smax;
		jN = j00 - pams->smax;
		jP = j00 + pams->smax;
		k10N = k10 - pams->smax;
		//k10P = k10 + pams->smax;
		//k11N = k11 - pams->smax;
		k11P = k11 + pams->smax;
		k20N = k20 - pams->smax;
		//k20P = k20 + pams->smax;
		//k21N = k21 - pams->smax;
		k21P = k21 + pams->smax;
		//
		//use Gaussian waveform. In the future, other wave forms can be used
		GaussianWaveForm *p = new GaussianWaveForm();
		p->setParameters(magnitude / (pams->ds*pams->ds), centerFrequency);
		wave = dynamic_cast<GaussianWaveForm *>(p);
	}
	return ret;
}
/*
	test if the point is at the source
*/
bool DipoleSourceTss::isInSource(unsigned int i, unsigned int j, unsigned int k)
{
	return (iN <= i && i <= iP && jN <= j && j <= jP && ((k >= k10N && k <= k11P) || (k >= k20N && k <= k21P)));
}
/*
	apply the field source
*/
int DipoleSourceTss::applySources(double tValue, size_t tIndex, Point3Dstruct *efile, Point3Dstruct *hfile)
{
	//H += Curl.F.dJm+Curl.G.dJe
	//E += Curl.U.dJe+Curl.W.dJm
	//Jm = 0
	//H += Curl.G.dJe
	//E += Curl.U.dJe
	//Je is at (x00,y00,z), in node indexing: 
	//								(i00,j00,k10)-(i00,j00,k11)
	//								(i00,j00,k20)-(i00,j00,k21)
	double v = wave->value(tValue);
	if (pams->kmax == 0)
	{
		for (unsigned int k = k10; k <= k11; k++)
		{
			size_t w = IDX(i00, j00, k);
			//hfile[w].z += g0[0] * v;
			efile[w].z += u0[0] * v;
			//assume the rod is a PEC, magnetic field cannot go through
			hfile[w].x = 0;
			hfile[w].y = 0;
			hfile[w].z = 0;
		}
		for (unsigned int k = k20; k <= k21; k++)
		{
			size_t w = IDX(i00, j00, k);
			//hfile[w].z += g0[0] * v;
			efile[w].z += u0[0] * v;
			//assume the rod is a PEC, magnetic field cannot go through
			hfile[w].x = 0;
			hfile[w].y = 0;
			hfile[w].z = 0;
		}
	}
	else
	{
		//using pams->kmax==1
		//G, U: 3X3
		//
		//G: curl0:[0    0    0]
		//   curl1:[g21  g22  0]
		//   curl2:[0    0    0]
		//
		//U: curl0:[u11  u12  u13]
		//   curl1:[0    0    0  ]
		//   curl2:[u31  0    0  ]
		//H += Curl.G.dJe = g21*Curl(Je)+g22*Curl(dJe/dt))
		//E += Curl.U.dJe = u11*Je+u12*(dJe/dt)+u13*(d^2Je/dt^2) + u31*Curl(Curl(Je))
		//Curl(Curl(Je) = 0 ->
		//E += u11*Je+u12*(dJe/dt)+u13*(d^2Je/dt^2)
		//Je=[ 0 ]    dJe/dt=[ 0     ]   d^2Je/dt^2=[ 0         ]
		//   [ 0 ]           [ 0     ]              [ 0         ]
		//   [ I ]           [ dI/dt ]              [ d^2I/dt^2 ]
		double dv_dt = wave->dv_dt(tValue);
		double dv2_dt2 = wave->dv2_dt2(tValue);
		for (unsigned int k = k10; k <= k11; k++)
		{
			size_t w = IDX(i00, j00, k);
			efile[w].z += u0[0] * v + u0[1] * dv_dt + u0[2] * dv2_dt2;
			//assume the rod is a PEC, magnetic field cannot go through
			hfile[w].x = 0;
			hfile[w].y = 0;
			hfile[w].z = 0;
		}
	}
	return ERR_OK;
}

#include "HeidlerCurrent.h"


HeidlerCurrent::HeidlerCurrent()
{
	//use default values
	factor = 1;
	c0 = 299792458.0; //speed of light
	n1 = 2.0;
	t11 = 1.8e-06;
	t12 = 95.0e-06;
	v = c0 / 3.0;
	I1 = 28.0e+03;
	lamdaH = 2000.0;
	ita = exp(-(t11 / t12)*pow((n1*t12 / t11),1.0/n1));
	ez = 1.0 / lamdaH - 1.0 / (v*t12);
	i1ita = I1 / ita;
	n1t11 = n1 / t11;
	i1ita_f = i1ita*factor;
}


HeidlerCurrent::~HeidlerCurrent()
{
}

int HeidlerCurrent::initialize(TaskFile *configs)
{
	int ret;
	factor = configs->getDouble(TP_SPACE_FACTOR, false);
	ret = configs->getErrorCode();
	if (ret == ERR_OK)
	{
		i1ita_f = i1ita*factor;
	}
	return ret;
}

int HeidlerCurrent::initialize(SimStruct *params)
{
	int ret = FieldSourceTss::initialize(params);
	if (ret == ERR_OK)
	{
		//assume nx and ny are even
		i0 = params->nx / 2;
		j0 = params->ny / 2;
	}
	return ret;
}
/*
J_1 (z,t)=((t-z/v)/τ_11 )^(n_1 )/(((t-z/v)/τ_11 )^(n_1 )+1)
*/
//(x,y,z) = (1/ks)(u,v,w)
//z is w, factor=1/ks, (1/ks)w is z meter
double HeidlerCurrent::J1()
{
	double ta = (t - factor*z / v);
	if (ta >= 0)
	{
		double a1 = pow(ta / t11, n1);
		return a1 / (a1 + 1.0);
	}
	else
		return 0;
}
double HeidlerCurrent::J2()
{
	double a2 = (t / t12 + ez * factor*z);
	return exp(-a2);
}
double HeidlerCurrent::dJ1_dt()
{
	double ta = (t - factor*z / v);
	if (ta > 0)
	{
		double a = ta / t11;
		double j1 = J1();
		return n1t11*(j1 - j1*j1) / a;
	}
	else
		return 0;
}
double HeidlerCurrent::d2J1_dt2()
{
	double ta = (t - factor*z / v);
	if (ta > 0)
	{
		double a = ta / t11;
		double j1 = J1();
		return n1t11*((1.0 - 2.0*j1)*dJ1_dt() + (j1 / t11)*(j1 - 1.0) / a) / a;
	}
	else
		return 0;
}
double HeidlerCurrent::dJ2_dt()
{
	return -J2() / t12;
}
double HeidlerCurrent::d2J2_dt2()
{
	return J2() / (t12*t12);
}

/*
I_1/η_1   ((t-z/v)/τ_11 )^(n_1 )/(((t-z/v)/τ_11 )^(n_1 )+1) e^(-(t/τ_12 +(1/λ_H -1/(vτ_12 ))z) )
*/
double HeidlerCurrent::I()
{
	return i1ita_f*J1()*J2();
}

double HeidlerCurrent::dI_dt()
{
	return i1ita_f*(dJ1_dt()*J2() + J1()*dJ2_dt());
}
double HeidlerCurrent::d2I_dt2()
{
	return i1ita_f*(d2J1_dt2()*J2() + 2.0*dJ1_dt()*dJ2_dt()+J1()*d2J2_dt2());
}
bool HeidlerCurrent::isInSource(unsigned int i, unsigned int j, unsigned int k)
{
	return (i == i0 && j == j0);
}
int HeidlerCurrent::applySources(double tValue, size_t tIndex, Point3Dstruct *efile, Point3Dstruct *hfile)
{
	int ret = ERR_OK;
	t = tValue;
	//H += Curl.F.dJm+Curl.G.dJe
	//E += Curl.U.dJe+Curl.W.dJm
	//Jm = 0
	//H += Curl.G.dJe
	//E += Curl.U.dJe
	//assume Je is at (0,0,z)
	if (pams->kmax == 0)
	{
		//G, U: 1X1
		//H += g[0] * Je
		//E += u[0] * Je
		//because Je(x,y,z) = 0, Je only applies to H(0,0,z) and E(0,0,z)
		//because Je(x,y,z)'s direction points to positive z, Je only applies to H(0,0,z).z and E(0,0,z).z
		double hc;
		size_t w;
		z = pams->zmin;
		for (unsigned int k = 0; k <= pams->nz; k++)
		{
			w = IDX(i0,j0,k);
			hc = I();
			hfile[w].z += g0[0] * hc;
			efile[w].z += u0[0] * hc;
			z += pams->ds;
		}
	}
	else if (pams->kmax == 1) //estimation order = 2*(kmax+1)=4
	{
		//G, U: 3X3
		//G: 0   0   0
		//   g21 g22 0
		//   0   0   0
		//U: u11  u12  u13
		//   0    0    0
		//   u31  0    0
		//H += Curl.G.dJe = Curl(g21*Je+g22*(dJe/dt))
		//E += Curl.U.dJe = u11*Je+u12*(dJe/dt)+u13*(d^2Je/dt^2) + Curl(Curl(u31*Je))
		//Curl(Curl(Je) = 0
		size_t w;
		double Ivalue, dIdtValue;
		double IvalueDs, dIdtValueDs;
		z = pams->zmin;
		for (unsigned int k = 0; k <= pams->nz; k++)
		{
			w = IDX(i0,j0,k);
			//
			Ivalue = I();
			dIdtValue = dI_dt();
			IvalueDs = Ivalue / pams->ds;
			dIdtValueDs = dIdtValue / pams->ds;
			//H += g21 * Curl(Je) + g22 * Curl(dJe/dt)
			//at (x=0,y=0, z):
			//Curl(Je)    =[-Iz/ds] 
			//             [ Iz/ds]
			//             [ 0    ]
			//Curl(dJe/dt)=[-(dIz/dt)/ds]
			//             [ (dIz/dt)/ds]
			//             [ 0          ]
			//g21 = g0[3];  g22=g0[4]
			hfile[w].x += -g0[3] * IvalueDs - g0[4] * dIdtValueDs;
			hfile[w].y +=  g0[3] * IvalueDs + g0[4] * dIdtValueDs;
			//E += u11*Je+u12*(dJe/dt)+u13*(d^2Je/dt^2)
			//Je=[ 0 ]    dJe/dt=[ 0     ]   d^2Je/dt^2=[ 0         ]
			//   [ 0 ]           [ 0     ]              [ 0         ]
			//   [ I ]           [ dI/dt ]              [ d^2I/dt^2 ]
			efile[w].z += u0[0] * Ivalue + u0[1] * dIdtValue + u0[2] * d2I_dt2();
			//
			z += pams->ds;
		}
	}
	else
	{
		ret = ERR_NOT_IMPLEMENT;
	}
	return ret;
}

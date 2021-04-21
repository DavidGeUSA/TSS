/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 03/31/2018
Allrights reserved by David Ge

implementation of perfect match layer

Date		Author			Description
--------------------------------------------------------------------
04/03/2021	David Ge		Support z-rotation symmetry
********************************************************************/
#include "PmlTss.h"
#include "Tss.h"
#include "../FileUtil/fileutil.h"

PmlTss::PmlTss()
{
}


PmlTss::~PmlTss()
{

}

int PmlTss::initialize(SimStruct *p)
{
	int ret = ERR_OK;
	if (p == NULL)
	{
		throw;
	}
	pams = p;
	ic = pams->nx / 2;
	//
	pmlLLL = p->pml.Pxl || p->pml.Pyl || p->pml.Pzl;
	pmlHLL = p->pml.Pxh || p->pml.Pyl || p->pml.Pzl;
	pmlLHL = p->pml.Pxl || p->pml.Pyh || p->pml.Pzl;
	pmlLLH = p->pml.Pxl || p->pml.Pyl || p->pml.Pzh;
	pmlLHH = p->pml.Pxl || p->pml.Pyh || p->pml.Pzh;
	pmlHLH = p->pml.Pxh || p->pml.Pyl || p->pml.Pzh;
	pmlHHL = p->pml.Pxh || p->pml.Pyh || p->pml.Pzl;
	pmlHHH = p->pml.Pxh || p->pml.Pyh || p->pml.Pzh;
	bUsePML = !p->pml.disable && (pmlLLL || pmlHLL || pmlLHL || pmlLLH || pmlLHH || pmlHLH || pmlHHL || pmlHHH);
	nx1 = p->nx + 1;
	ny1 = p->ny + 1;
	nz1 = p->nz + 1;
	//
	if (!usePML())
	{
		return ret;
	}
	//
	if (p->pml.power < 1)
	{
		ret = ERR_PML_POWER_0;
	}
	if (p->pml.alphaMax < 0 || p->pml.betaMax < 0)
	{
		return ERR_PML_MAGNITUDE;
	}
	unsigned int Ln3 = 3 * p->pml.Ln;
	if (Ln3 > pams->nx || Ln3 > pams->ny || Ln3 > pams->nz)
	{
		ret = ERR_PML_LN_BIG;
	}
	else
	{
		nxLn = pams->nx - p->pml.Ln;
		nyLn = pams->ny - p->pml.Ln;
		nzLn = pams->nz - p->pml.Ln;
		//
		dt1 = pams->dt;
		dt2 = pams->dt*pams->dt / 2.0;
		dt3 = pams->dt*pams->dt*pams->dt / 6.0;
		eu = 1.0 / (pams->eps * pams->mu);
	}

	if (ret == ERR_OK)
	{
		for (int i = 0; i < CORNERCOUNT; i++)
		{
			ret = corners[i].initializePmlRange(pams, i);
			if (ret != ERR_OK)
			{
				break;
			}
		}
		if (ret == ERR_OK)
		{
			for (int i = 0; i < EDGCOUNT; i++)
			{
				ret = edgbars[i].initializePmlRange(pams, i);
				if (ret != ERR_OK)
				{
					break;
				}
			}
		}
		if (ret == ERR_OK)
		{
			for (int i = 0; i < SIDECOUNT; i++)
			{
				ret = sides[i].initializePmlRange(pams, i);
				if (ret != ERR_OK)
				{
					break;
				}
			}
		}
		if (ret == ERR_OK)
		{
			calculateCornerCoefficients();
			calculateEdgeCoefficients();
			calculateSideCoefficients();
		}
	}
	return ret;
}

/*
	m: 1 ... Ln, depth into the PML range
*/
double PmlTss::effectStrength(unsigned int m)
{
	return pow((double)m / (double)pams->pml.Ln, pams->pml.power);
}

double PmlTss::alpha(unsigned int m)
{
	return 1.0 + pams->pml.alphaMax*effectStrength(m);
}

double PmlTss::beta(unsigned int m)
{
	return pams->pml.betaMax*effectStrength(m);
}

unsigned int PmlTss::mx(unsigned int i)
{
	if (i < pams->pml.Ln)
	{
		if (pams->pml.Pxl)
		{
			return pams->pml.Ln - i;
		}
	}
	else if (i > nxLn)
	{
		if (pams->pml.Pxh)
		{
			return i + pams->pml.Ln - pams->nx;
		}
	}
	return 0;
}
unsigned int PmlTss::my(unsigned int j)
{
	if (j < pams->pml.Ln)
	{
		if (pams->pml.Pyl)
		{
			return pams->pml.Ln - j;
		}
	}
	else if (j > nyLn)
	{
		if (pams->pml.Pyh)
		{
			return j + pams->pml.Ln - pams->ny;
		}
	}
	return 0;
}
unsigned int PmlTss::mz(unsigned int k)
{
	if (k < pams->pml.Ln)
	{
		if (pams->pml.Pzl)
		{
			return pams->pml.Ln - k;
		}
	}
	else if (k > nzLn)
	{
		if (pams->pml.Pzh)
		{
			return k + pams->pml.Ln - pams->nz;
		}
	}
	return 0;
}
///////0////////////////////////////////////////////////////////////////////////
double PmlTss::p_01x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//((α_z β_y+α_y β_z ))/(α_y α_z )
	return (alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z)) / (alpha(m_y) * alpha(m_z));
}
double PmlTss::p_01y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//((α_x β_z+α_z β_x ))/(α_z α_x )
	return (alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x)) / (alpha(m_z) * alpha(m_x));
}
double PmlTss::p_01z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//((α_y β_x+α_x β_y ))/(α_x α_y )
	return (alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y)) / (alpha(m_x) * alpha(m_y));
}
double PmlTss::p_02x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//((α_z β_y+α_y β_z)/(α_y α_z ))^2-(β_y β_z)/(α_y α_z )
	double a = ((alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z)) / (alpha(m_y) * alpha(m_z)));
	return a * a - (beta(m_y) * beta(m_z)) / (alpha(m_y) * alpha(m_z));
}
double PmlTss::p_02y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//((α_x β_z+α_z β_x)/(α_z α_x ))^2-(β_z β_x)/(α_z α_x )
	double a = ((alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x)) / (alpha(m_z) * alpha(m_x)));
	return a * a - (beta(m_z) * beta(m_x)) / (alpha(m_z) * alpha(m_x));
}
double PmlTss::p_02z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//((α_y β_x+α_x β_y)/(α_x α_y ))^2-(β_x β_y)/(α_x α_y )
	double a = ((alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y)) / (alpha(m_x) * alpha(m_y)));
	return a * a - (beta(m_x) * beta(m_y)) / (alpha(m_x) * alpha(m_y));
}
double PmlTss::p_03x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2β_y β_z (α_z β_y+α_y β_z ))/(α_y α_z )^2 -((α_z β_y+α_y β_z)/(α_y α_z ))^3
	double a = alpha(m_y) * alpha(m_z);
	double b = (alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z)) / (alpha(m_y) * alpha(m_z));
	return (2 * beta(m_y) * beta(m_z)*(alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z))) / (a * a) - (b*b*b);
}
double PmlTss::p_03y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2β_z β_x (α_x β_z+α_z β_x ))/(α_z α_x )^2 -((α_x β_z+α_z β_x)/(α_z α_x ))^3
	double a = alpha(m_z) * alpha(m_x);
	double b = (alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x)) / (alpha(m_z) * alpha(m_x));
	return (2 * beta(m_z) * beta(m_x)*(alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x))) / (a * a) - (b*b*b);
}
double PmlTss::p_03z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2β_x β_y (α_y β_x+α_x β_y ))/(α_x α_y )^2 -((α_y β_x+α_x β_y)/(α_x α_y ))^3
	double a = alpha(m_x) * alpha(m_y);
	double b = (alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y)) / (alpha(m_x) * alpha(m_y));
	return (2 * beta(m_x) * beta(m_y)*(alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y))) / (a * a) - (b*b*b);
}

////1//////////////
double PmlTss::p_11x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//α_x/(α_y α_z )
	return alpha(m_x) / (alpha(m_y) * alpha(m_z));
}
double PmlTss::p_11y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//α_y/(α_z α_x )
	return alpha(m_y) / (alpha(m_z) * alpha(m_x));
}
double PmlTss::p_11z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//α_z/(α_x α_y )
	return alpha(m_z) / (alpha(m_x) * alpha(m_y));
}
double PmlTss::p_12x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2α_x (α_z β_y+α_y β_z ))/(α_y α_z )^2 -β_x/(α_y α_z )
	double a = alpha(m_y) * alpha(m_z);
	return (2 * alpha(m_x)*(alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z))) / (a*a) - beta(m_x) / (alpha(m_y) * alpha(m_z));
}
double PmlTss::p_12y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2α_y (α_x β_z+α_z β_x ))/(α_z α_x )^2 -β_y/(α_z α_x )
	double a = alpha(m_z) * alpha(m_x);
	return (2 * alpha(m_y)*(alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x))) / (a*a) - beta(m_y) / (alpha(m_z) * alpha(m_x));
}
double PmlTss::p_12z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2α_z (α_y β_x+α_x β_y ))/(α_x α_y )^2 -β_z/(α_x α_y )
	double a = alpha(m_x) * alpha(m_y);
	return (2 * alpha(m_z)*(alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y))) / (a*a) - beta(m_z) / (alpha(m_x) * alpha(m_y));
}
double PmlTss::p_13x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(3α_x (α_z β_y+α_y β_z )^2)/(α_y α_z )^3 -2 (β_x (α_z β_y+α_y β_z )+α_x β_y β_z)/(α_y α_z )^2 
	double a = alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z);
	double b = alpha(m_y) * alpha(m_z);
	return (3 * alpha(m_x)*(a*a)) / (b*b*b) - 2 * (beta(m_x)*(a) + alpha(m_x) * beta(m_y) * beta(m_z)) / (b*b);
}
double PmlTss::p_13y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(3α_y (α_x β_z+α_z β_x )^2)/(α_z α_x )^3 -2 (β_y (α_x β_z+α_z β_x )+α_y β_z β_x)/(α_z α_x )^2 
	double a = alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x);
	double b = alpha(m_z) * alpha(m_x);
	return (3 * alpha(m_y)*(a*a)) / (b*b*b) - 2 * (beta(m_y)*(a) + alpha(m_y) * beta(m_z) * beta(m_x)) / (b*b);
}
double PmlTss::p_13z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(3α_z (α_y β_x+α_x β_y )^2)/(α_x α_y )^3 -2 (β_z (α_y β_x+α_x β_y )+α_z β_x β_y)/(α_x α_y )^2 
	double a = alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y);
	double b = alpha(m_x) * alpha(m_y);
	return (3 * alpha(m_z)*(a*a)) / (b*b*b) - 2 * (beta(m_z)*(a) + alpha(m_z) * beta(m_x) * beta(m_y)) / (b*b);
}

////2,3//////////////////
double PmlTss::p_22x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_x/(α_y α_z ))^2
	double a = alpha(m_x) / (alpha(m_y) * alpha(m_z));
	return a * a;
}
double PmlTss::p_22y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_y/(α_z α_x ))^2
	double a = alpha(m_y) / (alpha(m_z) * alpha(m_x));
	return a * a;
}
double PmlTss::p_22z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_z/(α_x α_y ))^2
	double a = alpha(m_z) / (alpha(m_x) * alpha(m_y));
	return a * a;
}
double PmlTss::p_23x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(3α_x^2 (α_z β_y+α_y β_z ))/(α_y α_z )^3 -(2α_x β_x)/(α_y α_z )^2 
	double a = alpha(m_y) * alpha(m_z);
	return (3 * alpha(m_x)* alpha(m_x)* (alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z))) / (a*a*a) - (2 * alpha(m_x) * beta(m_x)) / (a*a);
}
double PmlTss::p_23y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(3α_y^2 (α_x β_z+α_z β_x ))/(α_z α_x )^3 -(2α_y β_y)/(α_z α_x )^2 
	double a = alpha(m_z) * alpha(m_x);
	return (3 * alpha(m_y) * alpha(m_y)* (alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x))) / (a*a*a) - (2 * alpha(m_y) * beta(m_y)) / (a*a);
}
double PmlTss::p_23z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(3α_z^2 (α_y β_x+α_x β_y ))/(α_x α_y )^3 -(2α_z β_z)/(α_x α_y )^2 
	double a = alpha(m_x) * alpha(m_y);
	return (3 * alpha(m_z)* alpha(m_z) *(alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y))) / (a*a*a) - (2 * alpha(m_z) * beta(m_z)) / (a*a);
}
double PmlTss::p_33x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_x/(α_y α_z ))^3
	double a = alpha(m_x) / (alpha(m_y) * alpha(m_z));
	return a*a*a;
}
double PmlTss::p_33y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_y/(α_z α_x ))^3
	double a = alpha(m_y) / (alpha(m_z) * alpha(m_x));
	return a*a*a;
}
double PmlTss::p_33z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_z/(α_x α_y ))^3
	double a = alpha(m_z) / (alpha(m_x) * alpha(m_y));
	return a*a*a;
}
///////////////////////////////////////////////////////////////////////////
////////integral coefficients///////////////////////////////////////////
//0
double PmlTss::q_01x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_y β_z)/(α_y α_z )
	return (beta(m_y) * beta(m_z)) / (alpha(m_y) * alpha(m_z));
}
double PmlTss::q_01y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_z β_x)/(α_z α_x )
	return (beta(m_z) * beta(m_x)) / (alpha(m_z) * alpha(m_x));
}
double PmlTss::q_01z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_x β_y)/(α_x α_y )
	return (beta(m_x) * beta(m_y)) / (alpha(m_x) * alpha(m_y));
}
double PmlTss::q_02x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_y β_z (α_z β_y+α_y β_z ))/(α_y α_z )^2 
	double a = alpha(m_y) * alpha(m_z);
	return (beta(m_y) * beta(m_z)*(alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z))) / (a * a);
}
double PmlTss::q_02y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_z β_x (α_x β_z+α_z β_x ))/(α_z α_x )^2 
	double a = alpha(m_z) * alpha(m_x);
	return (beta(m_z) * beta(m_x)*(alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x))) / (a * a);
}
double PmlTss::q_02z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_x β_y (α_y β_x+α_x β_y ))/(α_x α_y )^2 
	double a = alpha(m_x) * alpha(m_y);
	return (beta(m_x) * beta(m_y)*(alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y))) / (a * a);
}
double PmlTss::q_03x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_y β_z)/(α_y α_z ) ((α_z β_y+α_y β_z )^2/(α_y α_z )^2 -(β_y β_z)/(α_y α_z ))
	double a = alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z);
	double b = alpha(m_y) * alpha(m_z);
	return ( beta(m_y) * beta(m_z) / (alpha(m_y) * alpha(m_z)) ) * ( (a*a) / (b*b) - (beta(m_y) * beta(m_z)) / (alpha(m_y) * alpha(m_z)) );
}
double PmlTss::q_03y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_z β_x)/(α_z α_x ) ((α_x β_z+α_z β_x )^2/(α_z α_x )^2 -(β_z β_x)/(α_z α_x ))
	double a = alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x);
	double b = alpha(m_z) * alpha(m_x);
	return ( beta(m_z) * beta(m_x) / (alpha(m_z) * alpha(m_x)) ) * ( (a*a) / (b*b) - (beta(m_z) * beta(m_x)) / (alpha(m_z) * alpha(m_x)) );
}
double PmlTss::q_03z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_x β_y)/(α_x α_y ) ((α_y β_x+α_x β_y )^2/(α_x α_y )^2 -(β_x β_y)/(α_x α_y ))
	double a = alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y);
	double b = alpha(m_x) * alpha(m_y);
	return ( beta(m_x) * beta(m_y) / (alpha(m_x) * alpha(m_y)) ) * ( (a*a) / (b*b) - (beta(m_x) * beta(m_y)) / (alpha(m_x) * alpha(m_y)) );
}
//1
double PmlTss::q_11x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//β_x/(α_y α_z )
	return beta(m_x) / (alpha(m_y) * alpha(m_z));
}
double PmlTss::q_11y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//β_y/(α_z α_x )
	return beta(m_y) / (alpha(m_z) * alpha(m_x));
}
double PmlTss::q_11z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//β_z/(α_x α_y )
	return beta(m_z) / (alpha(m_x) * alpha(m_y));
}
double PmlTss::q_12x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_x (α_z β_y+α_y β_z )+α_x β_y β_z)/(α_y α_z )^2 
	double a = alpha(m_y) * alpha(m_z);
	return ( beta(m_x) * (alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z)) + alpha(m_x) * beta(m_y) * beta(m_z) ) / (a*a);
}
double PmlTss::q_12y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_y (α_x β_z+α_z β_x )+α_y β_z β_x)/(α_z α_x )^2 
	double a = alpha(m_z) * alpha(m_x);
	return ( beta(m_y) * (alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x)) + alpha(m_y) * beta(m_z) * beta(m_x) ) / (a*a);
}
double PmlTss::q_12z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(β_z (α_y β_x+α_x β_y )+α_z β_x β_y)/(α_x α_y )^2 
	double a = alpha(m_x) * alpha(m_y);
	return ( beta(m_z)*(alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y)) + alpha(m_z) * beta(m_x) * beta(m_y) ) / (a*a);
}
double PmlTss::q_13x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2α_x β_y β_z+β_x (α_z β_y+α_y β_z ))(α_z β_y+α_y β_z )/(α_y α_z )^3 -(2β_x β_y β_z)/(α_y α_z )^2 
	double a = alpha(m_y) * alpha(m_z);
	double b = alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z);
	return ( 2 * alpha(m_x) * beta(m_y) * beta(m_z) + beta(m_x)*b )*b / (a*a*a) - (2 * beta(m_x) * beta(m_y) * beta(m_z)) / (a*a);
}
double PmlTss::q_13y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2α_y β_z β_x+β_y (α_x β_z+α_z β_x ))(α_x β_z+α_z β_x )/(α_z α_x )^3 -(2β_x β_y β_z)/(α_z α_x )^2 
	double a = alpha(m_z) * alpha(m_x);
	double b = alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x);
	return ( 2 * alpha(m_y) * beta(m_z) * beta(m_x) + beta(m_y)*b )*b / (a*a*a) - (2 * beta(m_x) * beta(m_y) * beta(m_z)) / (a*a);
}
double PmlTss::q_13z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2α_z β_x β_y+β_z (α_y β_x+α_x β_y ))(α_y β_x+α_x β_y )/(α_x α_y )^3 -(2β_x β_y β_z)/(α_x α_y )^2 
	double a = alpha(m_x) * alpha(m_y);
	double b = alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y);
	return ( 2 * alpha(m_z) * beta(m_x) * beta(m_y) + beta(m_z)*b )*b / (a*a*a) - (2 * beta(m_x) * beta(m_y) * beta(m_z)) / (a*a);
}
//2,3
double PmlTss::q_22x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_x β_x)/(α_y α_z )^2 
	double a = alpha(m_y) * alpha(m_z);
	return (alpha(m_x) * beta(m_x)) / (a*a);
}
double PmlTss::q_22y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_y β_y)/(α_z α_x )^2 
	double a = alpha(m_z) * alpha(m_x);
	return (alpha(m_y) * beta(m_y)) / (a*a);
}
double PmlTss::q_22z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_z β_z)/(α_x α_y )^2 
	double a = alpha(m_x) * alpha(m_y);
	return (alpha(m_z) * beta(m_z)) / (a*a);
}
double PmlTss::q_23x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2α_x β_x (α_z β_y+α_y β_z )+α_x^2 β_y β_z)/(α_y α_z )^3 -(β_x/(α_y α_z ))^2
	double a = alpha(m_y) * alpha(m_z);
	double b = beta(m_x) / a;
	return (2 * alpha(m_x) * beta(m_x)*(alpha(m_z) * beta(m_y) + alpha(m_y) * beta(m_z)) + alpha(m_x) * alpha(m_x) * beta(m_y) * beta(m_z)) / (a*a*a) - (b*b);
}
double PmlTss::q_23y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2α_y β_y (α_x β_z+α_z β_x )+α_y^2 β_z β_x)/(α_z α_x )^3 -(β_y/(α_z α_x ))^2
	double a = alpha(m_z) * alpha(m_x);
	double b = beta(m_y) / a;
	return (2 * alpha(m_y) * beta(m_y)*(alpha(m_x) * beta(m_z) + alpha(m_z) * beta(m_x)) + alpha(m_y) * alpha(m_y) * beta(m_z) * beta(m_x)) / (a*a*a) - (b*b);
}
double PmlTss::q_23z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(2α_z β_z (α_y β_x+α_x β_y )+α_z^2 β_x β_y)/(α_x α_y )^3 -(β_z/(α_x α_y ))^2
	double a = alpha(m_x) * alpha(m_y);
	double b = beta(m_z) / a;
	return (2 * alpha(m_z) * beta(m_z)*(alpha(m_y) * beta(m_x) + alpha(m_x) * beta(m_y)) + alpha(m_z) * alpha(m_z) * beta(m_x) * beta(m_y)) / (a*a*a) - (b*b);
}
double PmlTss::q_33x(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_x^2 β_x)/(α_y α_z )^3 
	double a = alpha(m_y) * alpha(m_z);
	return (alpha(m_x) * alpha(m_x) * beta(m_x)) / (a*a*a);
}
double PmlTss::q_33y(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_y^2 β_y)/(α_z α_x )^3 
	double a = alpha(m_z) * alpha(m_x);
	return (alpha(m_y) * alpha(m_y) * beta(m_y)) / (a*a*a);
}
double PmlTss::q_33z(unsigned int m_x, unsigned int m_y, unsigned int m_z)
{
	//(α_z^2 β_z)/(α_x α_y )^3 
	double a = alpha(m_x) * alpha(m_y);
	return (alpha(m_z) * alpha(m_z) * beta(m_z)) / (a*a*a);
}
////////////////////////////////////////////////////////////////////////
#define ijk mx(i), my(j), mz(k)
void PmlTss::calculateCoefficients(PmlCoefficients **coeff, size_t w, unsigned int i, unsigned int j, unsigned int k)
{
	coeff[0][w].w.x = 1 - dt1*p_01x(ijk) + dt2*p_02x(ijk) + dt3*p_03x(ijk);
	coeff[0][w].w.y = 1 - dt1*p_01y(ijk) + dt2*p_02y(ijk) + dt3*p_03y(ijk);
	coeff[0][w].w.z = 1 - dt1*p_01z(ijk) + dt2*p_02z(ijk) + dt3*p_03z(ijk);
	coeff[0][w].e.x = dt1*p_11x(ijk) - dt2*p_12x(ijk) + dt3*p_13x(ijk);
	coeff[0][w].e.y = dt1*p_11y(ijk) - dt2*p_12y(ijk) + dt3*p_13y(ijk);
	coeff[0][w].e.z = dt1*p_11z(ijk) - dt2*p_12z(ijk) + dt3*p_13z(ijk);
	coeff[0][w].e.x = coeff[0][w].e.x / pams->eps;
	coeff[0][w].e.y = coeff[0][w].e.y / pams->eps;
	coeff[0][w].e.z = coeff[0][w].e.z / pams->eps;
	coeff[0][w].u.x = -dt1*q_01x(ijk) + dt2*q_02x(ijk) - dt3*q_03x(ijk);
	coeff[0][w].u.y = -dt1*q_01y(ijk) + dt2*q_02y(ijk) - dt3*q_03y(ijk);
	coeff[0][w].u.z = -dt1*q_01z(ijk) + dt2*q_02z(ijk) - dt3*q_03z(ijk);
	coeff[0][w].u.x = dt1*coeff[0][w].u.x;
	coeff[0][w].u.y = dt1*coeff[0][w].u.y;
	coeff[0][w].u.z = dt1*coeff[0][w].u.z;
	coeff[0][w].f.x = dt1*q_11x(ijk) - dt2*q_12x(ijk) + dt3*q_13x(ijk);
	coeff[0][w].f.y = dt1*q_11y(ijk) - dt2*q_12y(ijk) + dt3*q_13y(ijk);
	coeff[0][w].f.z = dt1*q_11z(ijk) - dt2*q_12z(ijk) + dt3*q_13z(ijk);
	coeff[0][w].f.x = dt1*coeff[0][w].f.x / pams->eps;
	coeff[0][w].f.y = dt1*coeff[0][w].f.y / pams->eps;
	coeff[0][w].f.z = dt1*coeff[0][w].f.z / pams->eps;
	coeff[0][w].h.x = -dt1*p_11x(ijk) + dt2*p_12x(ijk) - dt3*p_13x(ijk);
	coeff[0][w].h.y = -dt1*p_11y(ijk) + dt2*p_12y(ijk) - dt3*p_13y(ijk);
	coeff[0][w].h.z = -dt1*p_11z(ijk) + dt2*p_12z(ijk) - dt3*p_13z(ijk);
	coeff[0][w].h.x = coeff[0][w].h.x / pams->mu;
	coeff[0][w].h.y = coeff[0][w].h.y / pams->mu;
	coeff[0][w].h.z = coeff[0][w].h.z / pams->mu;
	coeff[0][w].g.x = -dt1*q_11x(ijk) + dt2*q_12x(ijk) - dt3*q_13x(ijk);
	coeff[0][w].g.y = -dt1*q_11y(ijk) + dt2*q_12y(ijk) - dt3*q_13y(ijk);
	coeff[0][w].g.z = -dt1*q_11z(ijk) + dt2*q_12z(ijk) - dt3*q_13z(ijk);
	coeff[0][w].g.x = dt1*coeff[0][w].g.x / pams->mu;
	coeff[0][w].g.y = dt1*coeff[0][w].g.y / pams->mu;
	coeff[0][w].g.z = dt1*coeff[0][w].g.z / pams->mu;
	//
	coeff[1][w].w.x = -dt2*p_22x(ijk) + dt3*p_23x(ijk);
	coeff[1][w].w.y = -dt2*p_22y(ijk) + dt3*p_23y(ijk);
	coeff[1][w].w.z = -dt2*p_22z(ijk) + dt3*p_23z(ijk);
	coeff[1][w].w.x = coeff[1][w].w.x * eu;
	coeff[1][w].w.y = coeff[1][w].w.y * eu;
	coeff[1][w].w.z = coeff[1][w].w.z * eu;
	coeff[1][w].e.x = -dt3 * p_33x(ijk)*eu / pams->eps;
	coeff[1][w].e.y = -dt3 * p_33y(ijk)*eu / pams->eps;
	coeff[1][w].e.z = -dt3 * p_33z(ijk)*eu / pams->eps;
	coeff[1][w].u.x = -dt2*q_22x(ijk) + dt3*q_23x(ijk);
	coeff[1][w].u.y = -dt2*q_22y(ijk) + dt3*q_23y(ijk);
	coeff[1][w].u.z = -dt2*q_22z(ijk) + dt3*q_23z(ijk);
	coeff[1][w].u.x = coeff[1][w].u.x * eu;
	coeff[1][w].u.y = coeff[1][w].u.y * eu;
	coeff[1][w].u.z = coeff[1][w].u.z * eu;
	coeff[1][w].f.x = -dt3 * q_33x(ijk);
	coeff[1][w].f.y = -dt3 * q_33y(ijk);
	coeff[1][w].f.z = -dt3 * q_33z(ijk);
	coeff[1][w].f.x = coeff[1][w].f.x * eu / pams->eps;
	coeff[1][w].f.y = coeff[1][w].f.y * eu / pams->eps;
	coeff[1][w].f.z = coeff[1][w].f.z * eu / pams->eps;
	coeff[1][w].h.x = dt3 * p_33x(ijk);
	coeff[1][w].h.y = dt3 * p_33y(ijk);
	coeff[1][w].h.z = dt3 * p_33z(ijk);
	coeff[1][w].h.x = coeff[1][w].h.x * eu / pams->mu;
	coeff[1][w].h.y = coeff[1][w].h.y * eu / pams->mu;
	coeff[1][w].h.z = coeff[1][w].h.z * eu / pams->mu;
	coeff[1][w].g.x = dt3*q_33x(ijk);
	coeff[1][w].g.y = dt3*q_33y(ijk);
	coeff[1][w].g.z = dt3*q_33z(ijk);
	coeff[1][w].g.x = coeff[1][w].g.x * eu / pams->mu;
	coeff[1][w].g.y = coeff[1][w].g.y * eu / pams->mu;
	coeff[1][w].g.z = coeff[1][w].g.z * eu / pams->mu;
}
void PmlTss::calculateSideCoefficients()
{
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	size_t w = 0;
	for (int h = 0; h < SIDECOUNT; h++)
	{
		coeff = sides[h].Coefficients();
		i0 = sides[h].iStart();
		i1 = sides[h].iEnd();
		j0 = sides[h].jStart();
		j1 = sides[h].jEnd();
		k0 = sides[h].kStart();
		k1 = sides[h].kEnd();
		w = 0;
		if (sides[h].sideX())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				calculateCoefficients(coeff, w, i, pams->pml.Ln + 1, pams->pml.Ln + 1);
				w++;
			}
		}
		else if (sides[h].sideY())
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				calculateCoefficients(coeff, w, pams->pml.Ln + 1, j, pams->pml.Ln + 1);
				w++;
			}
		}
		else if (sides[h].sideZ())
		{
			for (unsigned int k = k0; k <= k1; k++)
			{
				calculateCoefficients(coeff, w, pams->pml.Ln + 1, pams->pml.Ln + 1,k);
				w++;
			}
		}
	}
}
/*
	the coefficients are defined for the 2-D area; 
	the coefficients are the same along the edge length;
	when using the coefficients, pay attention to the sequence of the 2-D indexing used here;
	the same 2-D indexing must be used when applying the coefficients
*/
void PmlTss::calculateEdgeCoefficients()
{
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	size_t w = 0;
	for (int h = 0; h < EDGCOUNT; h++)
	{
		coeff = edgbars[h].Coefficients();
		i0 = edgbars[h].iStart();
		i1 = edgbars[h].iEnd();
		j0 = edgbars[h].jStart();
		j1 = edgbars[h].jEnd();
		k0 = edgbars[h].kStart();
		k1 = edgbars[h].kEnd();
		w = 0;
		if (edgbars[h].parallelX())
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					calculateCoefficients(coeff, w, pams->pml.Ln+1, j, k);
					w++;
				}
			}
		}
		else if (edgbars[h].parallelY())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					calculateCoefficients(coeff, w, i, pams->pml.Ln + 1, k);
					w++;
				}
			}
		}
		else if (edgbars[h].parallelZ())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int j = j0; j <= j1; j++)
				{
					calculateCoefficients(coeff, w, i, j, pams->pml.Ln + 1);
					w++;
				}
			}
		}
	}
}
/*
	there is one set of coefficients for every point in a corner cube
*/
void PmlTss::calculateCornerCoefficients()
{
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	size_t w = 0;
	for (int h = 0; h < CORNERCOUNT; h++)
	{
		coeff = corners[h].Coefficients();
		i0 = corners[h].iStart();
		i1 = corners[h].iEnd();
		j0 = corners[h].jStart();
		j1 = corners[h].jEnd();
		k0 = corners[h].kStart();
		k1 = corners[h].kEnd();
		w = 0;
		for (unsigned int i = i0; i <= i1; i++)
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					calculateCoefficients(coeff, w, i, j, k);
					w++;
				}
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////
///apply PML 
/// in each PML range, there are 3 indexes:
/// wS is the 1-D index into the 3-D range memory used for summations,
/// wF is the 1-D index into the 3-D field memory
/// w is a 1-D index into a 3-D, 2-D or 1-D coefficients array.
/// for corner cubes, w==wS.
/// for edge bars, check function calculateEdgeCoefficients to make sure
///		that the same 2-D indexing is used;
/// for side boards, check function calculateSideCoefficients to make sure
///     that the same 1-D indexing is used
////////////////////////////////////////////////////////////////////////
/////apply curl to corners//////////////////////////////////////////////
/*
	apply curl-0 and curl-1 to the 8-corners
*/
void PmlTss::apply1stCurlCorners(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	size_t w,wF;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	//
	for (int h = 0; h < CORNERCOUNT; h++)
	{
		sumH = corners[h].SmH();
		sumE = corners[h].SmE();
		coeff = corners[h].Coefficients();
		i0 = corners[h].iStart();
		i1 = corners[h].iEnd();
		j0 = corners[h].jStart();
		j1 = corners[h].jEnd();
		k0 = corners[h].kStart();
		k1 = corners[h].kEnd();
		w = 0;
		for (unsigned int i = i0; i <= i1; i++)
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					wF = IdxField(i, j, k);
					//integrations updated first before fields are changed
					sumE[w].x += coeff[0][w].u.x * efield[wF].x + coeff[0][w].f.x * hCurl[wF].x;
					sumE[w].y += coeff[0][w].u.y * efield[wF].y + coeff[0][w].f.y * hCurl[wF].y;
					sumE[w].z += coeff[0][w].u.z * efield[wF].z + coeff[0][w].f.z * hCurl[wF].z;
					//
					sumH[w].x += coeff[0][w].u.x * hfield[wF].x + coeff[0][w].g.x * eCurl[wF].x;
					sumH[w].y += coeff[0][w].u.y * hfield[wF].y + coeff[0][w].g.y * eCurl[wF].y;
					sumH[w].z += coeff[0][w].u.z * hfield[wF].z + coeff[0][w].g.z * eCurl[wF].z;
					//
					//fields updated next
					efield[wF].x = coeff[0][w].w.x * efield[wF].x + coeff[0][w].e.x * hCurl[wF].x;
					efield[wF].y = coeff[0][w].w.y * efield[wF].y + coeff[0][w].e.y * hCurl[wF].y;
					efield[wF].z = coeff[0][w].w.z * efield[wF].z + coeff[0][w].e.z * hCurl[wF].z;
					//
					hfield[wF].x = coeff[0][w].w.x * hfield[wF].x + coeff[0][w].h.x * eCurl[wF].x;
					hfield[wF].y = coeff[0][w].w.y * hfield[wF].y + coeff[0][w].h.y * eCurl[wF].y;
					hfield[wF].z = coeff[0][w].w.z * hfield[wF].z + coeff[0][w].h.z * eCurl[wF].z;
					w++;
				}
			}
		}
	}
}

/*
apply curl-2 to the 8-corners
*/
void PmlTss::apply2ndCurlCorners(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	size_t w, wF;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	//
	for (int h = 0; h < CORNERCOUNT; h++)
	{
		sumH = corners[h].SmH();
		sumE = corners[h].SmE();
		coeff = corners[h].Coefficients();
		i0 = corners[h].iStart();
		i1 = corners[h].iEnd();
		j0 = corners[h].jStart();
		j1 = corners[h].jEnd();
		k0 = corners[h].kStart();
		k1 = corners[h].kEnd();
		w = 0;
		for (unsigned int i = i0; i <= i1; i++)
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					wF = IdxField(i, j, k);
					//integrations updated first before fields are changed
					sumE[w].x += coeff[1][w].u.x * eCurl[wF].x;
					sumE[w].y += coeff[1][w].u.y * eCurl[wF].y;
					sumE[w].z += coeff[1][w].u.z * eCurl[wF].z;
					//
					sumH[w].x += coeff[1][w].u.x * hCurl[wF].x;
					sumH[w].y += coeff[1][w].u.y * hCurl[wF].y;
					sumH[w].z += coeff[1][w].u.z * hCurl[wF].z;
					//
					//fields updated next
					efield[wF].x += coeff[1][w].w.x * eCurl[wF].x;
					efield[wF].y += coeff[1][w].w.y * eCurl[wF].y;
					efield[wF].z += coeff[1][w].w.z * eCurl[wF].z;
					//
					hfield[wF].x += coeff[1][w].w.x * hCurl[wF].x;
					hfield[wF].y += coeff[1][w].w.y * hCurl[wF].y;
					hfield[wF].z += coeff[1][w].w.z * hCurl[wF].z;
					w++;
				}
			}
		}
	}
}

/*
apply curl-3 to the 8-corners
*/
void PmlTss::apply3rdCurlCorners(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	size_t w, wF;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	//
	for (int h = 0; h < CORNERCOUNT; h++)
	{
		sumH = corners[h].SmH();
		sumE = corners[h].SmE();
		coeff = corners[h].Coefficients();
		i0 = corners[h].iStart();
		i1 = corners[h].iEnd();
		j0 = corners[h].jStart();
		j1 = corners[h].jEnd();
		k0 = corners[h].kStart();
		k1 = corners[h].kEnd();
		w = 0;
		for (unsigned int i = i0; i <= i1; i++)
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					wF = IdxField(i, j, k);
					//integrations updated first before fields are changed
					sumE[w].x += coeff[1][w].f.x * hCurl[wF].x;
					sumE[w].y += coeff[1][w].f.y * hCurl[wF].y;
					sumE[w].z += coeff[1][w].f.z * hCurl[wF].z;
					//
					sumH[w].x += coeff[1][w].g.x * eCurl[wF].x;
					sumH[w].y += coeff[1][w].g.y * eCurl[wF].y;
					sumH[w].z += coeff[1][w].g.z * eCurl[wF].z;
					//
					//fields updated next
					efield[wF].x += coeff[1][w].e.x * hCurl[wF].x + sumE[w].x;
					efield[wF].y += coeff[1][w].e.y * hCurl[wF].y + sumE[w].y;
					efield[wF].z += coeff[1][w].e.z * hCurl[wF].z + sumE[w].z;
					//
					hfield[wF].x += coeff[1][w].h.x * eCurl[wF].x + sumH[w].x;
					hfield[wF].y += coeff[1][w].h.y * eCurl[wF].y + sumH[w].y;
					hfield[wF].z += coeff[1][w].h.z * eCurl[wF].z + sumH[w].z;
					w++;
				}
			}
		}
	}
}
/////end of corners//////////////////////////////////////////////////////////////////////
/////apply curls to edges////////////////////////////////////////////////////////////////
//check calculateEdgeCoefficients to make sure the same 2-D indexing is used
void PmlTss::apply1stCurlEdges(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	size_t wS, wF, w;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	//
	for (int h = 0; h < EDGCOUNT; h++)
	{
		sumH = edgbars[h].SmH();
		sumE = edgbars[h].SmE();
		coeff = edgbars[h].Coefficients();
		i0 = edgbars[h].iStart();
		i1 = edgbars[h].iEnd();
		j0 = edgbars[h].jStart();
		j1 = edgbars[h].jEnd();
		k0 = edgbars[h].kStart();
		k1 = edgbars[h].kEnd();
		w = 0;
		wS = 0;
		if (edgbars[h].parallelX())
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					//for all i, coefficients are the same
					for (unsigned int i = i0; i <= i1; i++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[0][w].u.x * efield[wF].x + coeff[0][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[0][w].u.y * efield[wF].y + coeff[0][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[0][w].u.z * efield[wF].z + coeff[0][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[0][w].u.x * hfield[wF].x + coeff[0][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[0][w].u.y * hfield[wF].y + coeff[0][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[0][w].u.z * hfield[wF].z + coeff[0][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x = coeff[0][w].w.x * efield[wF].x + coeff[0][w].e.x * hCurl[wF].x;
						efield[wF].y = coeff[0][w].w.y * efield[wF].y + coeff[0][w].e.y * hCurl[wF].y;
						efield[wF].z = coeff[0][w].w.z * efield[wF].z + coeff[0][w].e.z * hCurl[wF].z;
						//
						hfield[wF].x = coeff[0][w].w.x * hfield[wF].x + coeff[0][w].h.x * eCurl[wF].x;
						hfield[wF].y = coeff[0][w].w.y * hfield[wF].y + coeff[0][w].h.y * eCurl[wF].y;
						hfield[wF].z = coeff[0][w].w.z * hfield[wF].z + coeff[0][w].h.z * eCurl[wF].z;
						//
						wS++; //index into summation memory
					}
					w++; //index into coefficients memory
				}
			}
		}
		else if (edgbars[h].parallelY())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					for (unsigned int j = j0; j <= j1; j++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[0][w].u.x * efield[wF].x + coeff[0][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[0][w].u.y * efield[wF].y + coeff[0][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[0][w].u.z * efield[wF].z + coeff[0][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[0][w].u.x * hfield[wF].x + coeff[0][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[0][w].u.y * hfield[wF].y + coeff[0][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[0][w].u.z * hfield[wF].z + coeff[0][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x = coeff[0][w].w.x * efield[wF].x + coeff[0][w].e.x * hCurl[wF].x;
						efield[wF].y = coeff[0][w].w.y * efield[wF].y + coeff[0][w].e.y * hCurl[wF].y;
						efield[wF].z = coeff[0][w].w.z * efield[wF].z + coeff[0][w].e.z * hCurl[wF].z;
						//
						hfield[wF].x = coeff[0][w].w.x * hfield[wF].x + coeff[0][w].h.x * eCurl[wF].x;
						hfield[wF].y = coeff[0][w].w.y * hfield[wF].y + coeff[0][w].h.y * eCurl[wF].y;
						hfield[wF].z = coeff[0][w].w.z * hfield[wF].z + coeff[0][w].h.z * eCurl[wF].z;
						//
						wS++;
					}
					w++;
				}
			}
		}
		else if (edgbars[h].parallelZ())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int j = j0; j <= j1; j++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[0][w].u.x * efield[wF].x + coeff[0][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[0][w].u.y * efield[wF].y + coeff[0][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[0][w].u.z * efield[wF].z + coeff[0][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[0][w].u.x * hfield[wF].x + coeff[0][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[0][w].u.y * hfield[wF].y + coeff[0][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[0][w].u.z * hfield[wF].z + coeff[0][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x = coeff[0][w].w.x * efield[wF].x + coeff[0][w].e.x * hCurl[wF].x;
						efield[wF].y = coeff[0][w].w.y * efield[wF].y + coeff[0][w].e.y * hCurl[wF].y;
						efield[wF].z = coeff[0][w].w.z * efield[wF].z + coeff[0][w].e.z * hCurl[wF].z;
						//
						hfield[wF].x = coeff[0][w].w.x * hfield[wF].x + coeff[0][w].h.x * eCurl[wF].x;
						hfield[wF].y = coeff[0][w].w.y * hfield[wF].y + coeff[0][w].h.y * eCurl[wF].y;
						hfield[wF].z = coeff[0][w].w.z * hfield[wF].z + coeff[0][w].h.z * eCurl[wF].z;
						//
						wS++;
					}
					w++;
				}
			}
		}
	}
}
void PmlTss::apply2ndCurlEdges(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	size_t w, wF, wS;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	//
	for (int h = 0; h < EDGCOUNT; h++)
	{
		sumH = edgbars[h].SmH();
		sumE = edgbars[h].SmE();
		coeff = edgbars[h].Coefficients();
		i0 = edgbars[h].iStart();
		i1 = edgbars[h].iEnd();
		j0 = edgbars[h].jStart();
		j1 = edgbars[h].jEnd();
		k0 = edgbars[h].kStart();
		k1 = edgbars[h].kEnd();
		w = 0;
		wS = 0;
		if (edgbars[h].parallelX())
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					for (unsigned int i = i0; i <= i1; i++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].u.x * eCurl[wF].x;
						sumE[wS].y += coeff[1][w].u.y * eCurl[wF].y;
						sumE[wS].z += coeff[1][w].u.z * eCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].u.x * hCurl[wF].x;
						sumH[wS].y += coeff[1][w].u.y * hCurl[wF].y;
						sumH[wS].z += coeff[1][w].u.z * hCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].w.x * eCurl[wF].x;
						efield[wF].y += coeff[1][w].w.y * eCurl[wF].y;
						efield[wF].z += coeff[1][w].w.z * eCurl[wF].z;
						//
						hfield[wF].x += coeff[1][w].w.x * hCurl[wF].x;
						hfield[wF].y += coeff[1][w].w.y * hCurl[wF].y;
						hfield[wF].z += coeff[1][w].w.z * hCurl[wF].z;
						//
						wS++;
					}
					w++;
				}
			}
		}
		else if (edgbars[h].parallelY())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					for (unsigned int j = j0; j <= j1; j++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].u.x * eCurl[wF].x;
						sumE[wS].y += coeff[1][w].u.y * eCurl[wF].y;
						sumE[wS].z += coeff[1][w].u.z * eCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].u.x * hCurl[wF].x;
						sumH[wS].y += coeff[1][w].u.y * hCurl[wF].y;
						sumH[wS].z += coeff[1][w].u.z * hCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].w.x * eCurl[wF].x;
						efield[wF].y += coeff[1][w].w.y * eCurl[wF].y;
						efield[wF].z += coeff[1][w].w.z * eCurl[wF].z;
						//
						hfield[wF].x += coeff[1][w].w.x * hCurl[wF].x;
						hfield[wF].y += coeff[1][w].w.y * hCurl[wF].y;
						hfield[wF].z += coeff[1][w].w.z * hCurl[wF].z;
						//
						wS++;
					}
					w++;
				}
			}
		}
		else if (edgbars[h].parallelZ())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int j = j0; j <= j1; j++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].u.x * eCurl[wF].x;
						sumE[wS].y += coeff[1][w].u.y * eCurl[wF].y;
						sumE[wS].z += coeff[1][w].u.z * eCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].u.x * hCurl[wF].x;
						sumH[wS].y += coeff[1][w].u.y * hCurl[wF].y;
						sumH[wS].z += coeff[1][w].u.z * hCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].w.x * eCurl[wF].x;
						efield[wF].y += coeff[1][w].w.y * eCurl[wF].y;
						efield[wF].z += coeff[1][w].w.z * eCurl[wF].z;
						//
						hfield[wF].x += coeff[1][w].w.x * hCurl[wF].x;
						hfield[wF].y += coeff[1][w].w.y * hCurl[wF].y;
						hfield[wF].z += coeff[1][w].w.z * hCurl[wF].z;
						//
						wS++;
					}
					w++;
				}
			}
		}
	}
}
void PmlTss::apply3rdCurlEdges(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	size_t w, wF, wS;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	//
	for (int h = 0; h < EDGCOUNT; h++)
	{
		sumH = edgbars[h].SmH();
		sumE = edgbars[h].SmE();
		coeff = edgbars[h].Coefficients();
		i0 = edgbars[h].iStart();
		i1 = edgbars[h].iEnd();
		j0 = edgbars[h].jStart();
		j1 = edgbars[h].jEnd();
		k0 = edgbars[h].kStart();
		k1 = edgbars[h].kEnd();
		w = 0;
		wS = 0; //1-D index into the range
		if (edgbars[h].parallelX())
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					for (unsigned int i = i0; i <= i1; i++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[1][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[1][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[1][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[1][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].e.x * hCurl[wF].x + sumE[wS].x;
						efield[wF].y += coeff[1][w].e.y * hCurl[wF].y + sumE[wS].y;
						efield[wF].z += coeff[1][w].e.z * hCurl[wF].z + sumE[wS].z;
						//
						hfield[wF].x += coeff[1][w].h.x * eCurl[wF].x + sumH[wS].x;
						hfield[wF].y += coeff[1][w].h.y * eCurl[wF].y + sumH[wS].y;
						hfield[wF].z += coeff[1][w].h.z * eCurl[wF].z + sumH[wS].z;
						//
						wS++;
					}
					w++;
				}
			}
		}
		else if (edgbars[h].parallelY())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					for (unsigned int j = j0; j <= j1; j++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[1][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[1][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[1][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[1][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].e.x * hCurl[wF].x + sumE[wS].x;
						efield[wF].y += coeff[1][w].e.y * hCurl[wF].y + sumE[wS].y;
						efield[wF].z += coeff[1][w].e.z * hCurl[wF].z + sumE[wS].z;
						//
						hfield[wF].x += coeff[1][w].h.x * eCurl[wF].x + sumH[wS].x;
						hfield[wF].y += coeff[1][w].h.y * eCurl[wF].y + sumH[wS].y;
						hfield[wF].z += coeff[1][w].h.z * eCurl[wF].z + sumH[wS].z;
						//
						wS++;
					}
					w++;
				}
			}
		}
		else if (edgbars[h].parallelZ())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int j = j0; j <= j1; j++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[1][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[1][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[1][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[1][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].e.x * hCurl[wF].x + sumE[wS].x;
						efield[wF].y += coeff[1][w].e.y * hCurl[wF].y + sumE[wS].y;
						efield[wF].z += coeff[1][w].e.z * hCurl[wF].z + sumE[wS].z;
						//
						hfield[wF].x += coeff[1][w].h.x * eCurl[wF].x + sumH[wS].x;
						hfield[wF].y += coeff[1][w].h.y * eCurl[wF].y + sumH[wS].y;
						hfield[wF].z += coeff[1][w].h.z * eCurl[wF].z + sumH[wS].z;
						//
						wS++;
					}
					w++;
				}
			}
		}
	}
}
/////end of edges////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////sides///////////////////////////////////////////////////////////////////////////////
void PmlTss::apply1stCurlSides(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	size_t w, wF, wS;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	//
	for (int h = 0; h < SIDECOUNT; h++)
	{
		sumH = sides[h].SmH();
		sumE = sides[h].SmE();
		coeff = sides[h].Coefficients();
		i0 = sides[h].iStart();
		i1 = sides[h].iEnd();
		j0 = sides[h].jStart();
		j1 = sides[h].jEnd();
		k0 = sides[h].kStart();
		k1 = sides[h].kEnd();
		w = 0;
		wS = 0;
		if (sides[h].sideX())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int j = j0; j <= j1; j++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[0][w].u.x * efield[wF].x + coeff[0][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[0][w].u.y * efield[wF].y + coeff[0][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[0][w].u.z * efield[wF].z + coeff[0][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[0][w].u.x * hfield[wF].x + coeff[0][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[0][w].u.y * hfield[wF].y + coeff[0][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[0][w].u.z * hfield[wF].z + coeff[0][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x = coeff[0][w].w.x * efield[wF].x + coeff[0][w].e.x * hCurl[wF].x;
						efield[wF].y = coeff[0][w].w.y * efield[wF].y + coeff[0][w].e.y * hCurl[wF].y;
						efield[wF].z = coeff[0][w].w.z * efield[wF].z + coeff[0][w].e.z * hCurl[wF].z;
						//
						hfield[wF].x = coeff[0][w].w.x * hfield[wF].x + coeff[0][w].h.x * eCurl[wF].x;
						hfield[wF].y = coeff[0][w].w.y * hfield[wF].y + coeff[0][w].h.y * eCurl[wF].y;
						hfield[wF].z = coeff[0][w].w.z * hfield[wF].z + coeff[0][w].h.z * eCurl[wF].z;
						//
						wS++;
					}
				}
				w++;
			}
		}
		else if (sides[h].sideY())
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[0][w].u.x * efield[wF].x + coeff[0][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[0][w].u.y * efield[wF].y + coeff[0][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[0][w].u.z * efield[wF].z + coeff[0][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[0][w].u.x * hfield[wF].x + coeff[0][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[0][w].u.y * hfield[wF].y + coeff[0][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[0][w].u.z * hfield[wF].z + coeff[0][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x = coeff[0][w].w.x * efield[wF].x + coeff[0][w].e.x * hCurl[wF].x;
						efield[wF].y = coeff[0][w].w.y * efield[wF].y + coeff[0][w].e.y * hCurl[wF].y;
						efield[wF].z = coeff[0][w].w.z * efield[wF].z + coeff[0][w].e.z * hCurl[wF].z;
						//
						hfield[wF].x = coeff[0][w].w.x * hfield[wF].x + coeff[0][w].h.x * eCurl[wF].x;
						hfield[wF].y = coeff[0][w].w.y * hfield[wF].y + coeff[0][w].h.y * eCurl[wF].y;
						hfield[wF].z = coeff[0][w].w.z * hfield[wF].z + coeff[0][w].h.z * eCurl[wF].z;
						//
						wS++;
					}
				}
				w++;
			}
		}
		else if (sides[h].sideZ())
		{
			for (unsigned int k = k0; k <= k1; k++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					for (unsigned int j = j0; j <= j1; j++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[0][w].u.x * efield[wF].x + coeff[0][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[0][w].u.y * efield[wF].y + coeff[0][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[0][w].u.z * efield[wF].z + coeff[0][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[0][w].u.x * hfield[wF].x + coeff[0][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[0][w].u.y * hfield[wF].y + coeff[0][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[0][w].u.z * hfield[wF].z + coeff[0][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x = coeff[0][w].w.x * efield[wF].x + coeff[0][w].e.x * hCurl[wF].x;
						efield[wF].y = coeff[0][w].w.y * efield[wF].y + coeff[0][w].e.y * hCurl[wF].y;
						efield[wF].z = coeff[0][w].w.z * efield[wF].z + coeff[0][w].e.z * hCurl[wF].z;
						//
						hfield[wF].x = coeff[0][w].w.x * hfield[wF].x + coeff[0][w].h.x * eCurl[wF].x;
						hfield[wF].y = coeff[0][w].w.y * hfield[wF].y + coeff[0][w].h.y * eCurl[wF].y;
						hfield[wF].z = coeff[0][w].w.z * hfield[wF].z + coeff[0][w].h.z * eCurl[wF].z;
						//
						wS++;
					}
				}
				w++;
			}
		}
	}
}
void PmlTss::apply2ndCurlSides(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	size_t w, wF, wS;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	//
	for (int h = 0; h < SIDECOUNT; h++)
	{
		sumH = sides[h].SmH();
		sumE = sides[h].SmE();
		coeff = sides[h].Coefficients();
		i0 = sides[h].iStart();
		i1 = sides[h].iEnd();
		j0 = sides[h].jStart();
		j1 = sides[h].jEnd();
		k0 = sides[h].kStart();
		k1 = sides[h].kEnd();
		w = 0;
		wS = 0;
		if (sides[h].sideX())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int j = j0; j <= j1; j++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].u.x * eCurl[wF].x;
						sumE[wS].y += coeff[1][w].u.y * eCurl[wF].y;
						sumE[wS].z += coeff[1][w].u.z * eCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].u.x * hCurl[wF].x;
						sumH[wS].y += coeff[1][w].u.y * hCurl[wF].y;
						sumH[wS].z += coeff[1][w].u.z * hCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].w.x * eCurl[wF].x;
						efield[wF].y += coeff[1][w].w.y * eCurl[wF].y;
						efield[wF].z += coeff[1][w].w.z * eCurl[wF].z;
						//
						hfield[wF].x += coeff[1][w].w.x * hCurl[wF].x;
						hfield[wF].y += coeff[1][w].w.y * hCurl[wF].y;
						hfield[wF].z += coeff[1][w].w.z * hCurl[wF].z;
						//
						wS++;
					}
				}
				w++;
			}
		}
		else if (sides[h].sideY())
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].u.x * eCurl[wF].x;
						sumE[wS].y += coeff[1][w].u.y * eCurl[wF].y;
						sumE[wS].z += coeff[1][w].u.z * eCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].u.x * hCurl[wF].x;
						sumH[wS].y += coeff[1][w].u.y * hCurl[wF].y;
						sumH[wS].z += coeff[1][w].u.z * hCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].w.x * eCurl[wF].x;
						efield[wF].y += coeff[1][w].w.y * eCurl[wF].y;
						efield[wF].z += coeff[1][w].w.z * eCurl[wF].z;
						//
						hfield[wF].x += coeff[1][w].w.x * hCurl[wF].x;
						hfield[wF].y += coeff[1][w].w.y * hCurl[wF].y;
						hfield[wF].z += coeff[1][w].w.z * hCurl[wF].z;
						//
						wS++;
					}
				}
				w++;
			}
		}
		else if (sides[h].sideZ())
		{
			for (unsigned int k = k0; k <= k1; k++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					for (unsigned int j = j0; j <= j1; j++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].u.x * eCurl[wF].x;
						sumE[wS].y += coeff[1][w].u.y * eCurl[wF].y;
						sumE[wS].z += coeff[1][w].u.z * eCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].u.x * hCurl[wF].x;
						sumH[wS].y += coeff[1][w].u.y * hCurl[wF].y;
						sumH[wS].z += coeff[1][w].u.z * hCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].w.x * eCurl[wF].x;
						efield[wF].y += coeff[1][w].w.y * eCurl[wF].y;
						efield[wF].z += coeff[1][w].w.z * eCurl[wF].z;
						//
						hfield[wF].x += coeff[1][w].w.x * hCurl[wF].x;
						hfield[wF].y += coeff[1][w].w.y * hCurl[wF].y;
						hfield[wF].z += coeff[1][w].w.z * hCurl[wF].z;
						//
						wS++;
					}
				}
				w++;
			}
		}
	}
}
void PmlTss::apply3rdCurlSides(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	size_t w, wF, wS;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	//
	for (int h = 0; h < SIDECOUNT; h++)
	{
		sumH = sides[h].SmH();
		sumE = sides[h].SmE();
		coeff = sides[h].Coefficients();
		i0 = sides[h].iStart();
		i1 = sides[h].iEnd();
		j0 = sides[h].jStart();
		j1 = sides[h].jEnd();
		k0 = sides[h].kStart();
		k1 = sides[h].kEnd();
		w = 0;
		wS = 0;
		if (sides[h].sideX())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int j = j0; j <= j1; j++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[1][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[1][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[1][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[1][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].e.x * hCurl[wF].x + sumE[wS].x;
						efield[wF].y += coeff[1][w].e.y * hCurl[wF].y + sumE[wS].y;
						efield[wF].z += coeff[1][w].e.z * hCurl[wF].z + sumE[wS].z;
						//
						hfield[wF].x += coeff[1][w].h.x * eCurl[wF].x + sumH[wS].x;
						hfield[wF].y += coeff[1][w].h.y * eCurl[wF].y + sumH[wS].y;
						hfield[wF].z += coeff[1][w].h.z * eCurl[wF].z + sumH[wS].z;
						//
						wS++;
					}
				}
				w++;
			}
		}
		else if (sides[h].sideY())
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[1][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[1][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[1][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[1][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].e.x * hCurl[wF].x + sumE[wS].x;
						efield[wF].y += coeff[1][w].e.y * hCurl[wF].y + sumE[wS].y;
						efield[wF].z += coeff[1][w].e.z * hCurl[wF].z + sumE[wS].z;
						//
						hfield[wF].x += coeff[1][w].h.x * eCurl[wF].x + sumH[wS].x;
						hfield[wF].y += coeff[1][w].h.y * eCurl[wF].y + sumH[wS].y;
						hfield[wF].z += coeff[1][w].h.z * eCurl[wF].z + sumH[wS].z;
						//
						wS++;
					}
				}
				w++;
			}
		}
		else if (sides[h].sideZ())
		{
			for (unsigned int k = k0; k <= k1; k++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					for (unsigned int j = j0; j <= j1; j++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[1][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[1][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[1][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[1][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].e.x * hCurl[wF].x + sumE[wS].x;
						efield[wF].y += coeff[1][w].e.y * hCurl[wF].y + sumE[wS].y;
						efield[wF].z += coeff[1][w].e.z * hCurl[wF].z + sumE[wS].z;
						//
						hfield[wF].x += coeff[1][w].h.x * eCurl[wF].x + sumH[wS].x;
						hfield[wF].y += coeff[1][w].h.y * eCurl[wF].y + sumH[wS].y;
						hfield[wF].z += coeff[1][w].h.z * eCurl[wF].z + sumH[wS].z;
						//
						wS++;
					}
				}
				w++;
			}
		}
	}
}
/////end of sides////////////////////////////////////////////////////////////////////////
/////Apply curls/////////////////////////////////////////////////////////////////////////
/*
	Appy curl-0 and curl-1
*/
void PmlTss::apply1stCurl(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	apply1stCurlCorners(efield, hfield, eCurl, hCurl);
	apply1stCurlEdges(efield, hfield, eCurl, hCurl);
	apply1stCurlSides(efield, hfield, eCurl, hCurl);
}

/*
	Appy curl-2
*/
void PmlTss::apply2ndCurl(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	apply2ndCurlCorners(efield, hfield, eCurl, hCurl);
	apply2ndCurlEdges(efield, hfield, eCurl, hCurl);
	apply2ndCurlSides(efield, hfield, eCurl, hCurl);
}

/*
	Appy curl-3
*/
void PmlTss::apply3rdCurl(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl)
{
	apply3rdCurlCorners(efield, hfield, eCurl, hCurl);
	apply3rdCurlEdges(efield, hfield, eCurl, hCurl);
	apply3rdCurlSides(efield, hfield, eCurl, hCurl);
}
void PmlTss::writeCoefficients(FILE *fileHandle, PmlCoefficients **coeff, size_t w)
{
	unsigned int size = 1024;
	char buff[1024];
	sprintf_1(buff, size, "\nfield:<w(0)>=<%g, %g, %g>, <e(0)>=<%g, %g, %g>, <h(0)>=<%g, %g, %g>\n"
		, coeff[0][w].w.x, coeff[0][w].w.y, coeff[0][w].w.z
		, coeff[0][w].e.x, coeff[0][w].e.y, coeff[0][w].e.z
		, coeff[0][w].h.x, coeff[0][w].h.y, coeff[0][w].h.z);
	writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));

	sprintf_1(buff, size, "\nintegral:<u(0)>=<%g, %g, %g>, <f(0)>=<%g, %g, %g>, <g(0)>=<%g, %g, %g>\n"
		, coeff[0][w].u.x, coeff[0][w].u.y, coeff[0][w].u.z
		, coeff[0][w].f.x, coeff[0][w].f.y, coeff[0][w].f.z
		, coeff[0][w].g.x, coeff[0][w].g.y, coeff[0][w].g.z);
	writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));

	sprintf_1(buff, size, "\nfield:<w(1)>=<%g, %g, %g>, <e(1)>=<%g, %g, %g>, <h(1)>=<%g, %g, %g>\n"
		, coeff[0][w].w.x, coeff[1][w].w.y, coeff[1][w].w.z
		, coeff[0][w].e.x, coeff[1][w].e.y, coeff[1][w].e.z
		, coeff[0][w].h.x, coeff[1][w].h.y, coeff[1][w].h.z);
	writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));

	sprintf_1(buff, size, "\nintegral:<u(1)>=<%g, %g, %g>, <f(1)>=<%g, %g, %g>, <g(1)>=<%g, %g, %g>\n"
		, coeff[0][w].u.x, coeff[1][w].u.y, coeff[1][w].u.z
		, coeff[0][w].f.x, coeff[1][w].f.y, coeff[1][w].f.z
		, coeff[0][w].g.x, coeff[1][w].g.y, coeff[1][w].g.z);
	writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));

}
void PmlTss::writeCoefficientsToFile(FILE *fileHandle)
{
	PmlCoefficients **coeff;
	unsigned int i0, i1, j0, j1, k0, k1;
	size_t w;
	unsigned int size = 1024;
	char buff[1024];
	sprintf_1(buff, size, "\r\nPML setting\nPML usage: (x-Low:%s, x-High:%s) (y-Low:%s, y-High:%s) (z-Low:%s, z-High:%s)\n" 
		, pams->pml.Pxl ? "true" : "false", pams->pml.Pxh ? "true" : "false"
		, pams->pml.Pyl ? "true" : "false", pams->pml.Pyh ? "true" : "false"
		, pams->pml.Pzl ? "true" : "false", pams->pml.Pzh ? "true" : "false");
	writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));
	sprintf_1(buff, size, "Thickness:%u, maximum loss: %g + %g / (iw)\n", pams->pml.Ln, pams->pml.alphaMax, pams->pml.betaMax);
	writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));
	sprintf_1(buff, size, "\nField Update Coefficients:\n");
	writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));
	//coefficients for the 8 corners
	for (unsigned int h = 0; h < CORNERCOUNT; h++)
	{
		coeff = corners[h].Coefficients();
		i0 = corners[h].iStart();
		i1 = corners[h].iEnd();
		j0 = corners[h].jStart();
		j1 = corners[h].jEnd();
		k0 = corners[h].kStart();
		k1 = corners[h].kEnd();
		sprintf_1(buff, size, "\nCorner %u %s:\n", h, corners[h].name());
		writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));
		w = 0;
		for (unsigned int i = i0; i < i1; i++)
		{
			for (unsigned int j = j0; j < j1; j++)
			{
				for (unsigned int k = k0; k < k1; k++)
				{
					sprintf_1(buff, size, "i:%u, j:%u, k:%u\n", i,j,k);
					writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));

					writeCoefficients(fileHandle, coeff, w);
					w++;
				}
			}
		}
	}
	//coefficients for the 12 edge bars
	for (int h = 0; h < EDGCOUNT; h++)
	{
		coeff = edgbars[h].Coefficients();
		i0 = edgbars[h].iStart();
		i1 = edgbars[h].iEnd();
		j0 = edgbars[h].jStart();
		j1 = edgbars[h].jEnd();
		k0 = edgbars[h].kStart();
		k1 = edgbars[h].kEnd();
		sprintf_1(buff, size, "\nEdge bar %d %s:\n", h, edgbars[h].name());
		writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));
		w = 0;
		if (edgbars[h].parallelX())
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					sprintf_1(buff, size, "parallel to x-axis j:%u, k:%u\n", j, k);
					writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));

					writeCoefficients(fileHandle, coeff, w);
					w++;
				}
			}
		}
		else if (edgbars[h].parallelY())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					sprintf_1(buff, size, "parallel to y-axis i:%u, k:%u\n", i, k);
					writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));

					writeCoefficients(fileHandle, coeff, w);
					w++;
				}
			}
		}
		else if (edgbars[h].parallelZ())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int j = j0; j <= j1; j++)
				{
					sprintf_1(buff, size, "parallel to z-axis i:%u, j:%u\n", i, j);
					writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));

					writeCoefficients(fileHandle, coeff, w);
					w++;
				}
			}
		}
	}
	//coefficients for the 6 side boards
	for (int h = 0; h < SIDECOUNT; h++)
	{
		coeff = sides[h].Coefficients();
		i0 = sides[h].iStart();
		i1 = sides[h].iEnd();
		j0 = sides[h].jStart();
		j1 = sides[h].jEnd();
		k0 = sides[h].kStart();
		k1 = sides[h].kEnd();
		sprintf_1(buff, size, "\nSide board %d %s:\n", h, sides[h].name());
		writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));
		w = 0;
		if (sides[h].sideX())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				sprintf_1(buff, size, "%s of x-axis i:%u\n",sides[h].isLowEnd()?"Low-end":"High-end", i);
				writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));

				writeCoefficients(fileHandle, coeff, w);
				w++;
			}
		}
		else if (sides[h].sideY())
		{
			for (unsigned int j = j0; j <= j1; j++)
			{
				sprintf_1(buff, size, "%s of y-axis j:%u\n", sides[h].isLowEnd() ? "Low-end" : "High-end", j);
				writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));

				writeCoefficients(fileHandle, coeff, w);
				w++;
			}
		}
		else if (sides[h].sideZ())
		{
			for (unsigned int k = k0; k <= k1; k++)
			{
				sprintf_1(buff, size, "%s of z-axis k:%u\n", sides[h].isLowEnd() ? "Low-end" : "High-end", k);
				writefile(fileHandle, buff, (unsigned int)strnlen_0(buff, size));

				writeCoefficients(fileHandle, coeff, w);
				w++;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// z-rotation symmetry
///   only edge bars and side boards, no corner cubes
//////////////////////////////////////////////////////////////////////////////
/////apply curls to edges////////////////////////////////////////////////////////////////
void PmlTss::apply1stCurlEdgesZrotateSymmetry(RotateSymmetryField *efield, RotateSymmetryField *hfield, RotateSymmetryField *eCurl, RotateSymmetryField *hCurl)
{
	size_t w, wS;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, k0, k1;
	//
	unsigned int j = ic; //y=0
	Point3Dstruct ef;
	Point3Dstruct hf;
	Point3Dstruct hc;
	Point3Dstruct ec;
	Point3Dstruct v0;
	//
	for (int h = 0; h < EDGCOUNT; h++)
	{
		sumH = edgbars[h].SmH();
		sumE = edgbars[h].SmE();
		coeff = edgbars[h].Coefficients();
		i0 = edgbars[h].iStart();
		i1 = edgbars[h].iEnd();
		if (i0 > ic)
		{
			continue; //only apply edges at low-end
		}
		//j0 = edgbars[h].jStart();
		//j1 = edgbars[h].jEnd();
		k0 = edgbars[h].kStart();
		k1 = edgbars[h].kEnd();
		w = 0;
		wS = 0;
		if (edgbars[h].parallelX())
		{
			/* z-rotation symmetry does not include this case
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					for (unsigned int i = i0; i <= i1; i++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[0][w].u.x * efield[wF].x + coeff[0][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[0][w].u.y * efield[wF].y + coeff[0][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[0][w].u.z * efield[wF].z + coeff[0][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[0][w].u.x * hfield[wF].x + coeff[0][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[0][w].u.y * hfield[wF].y + coeff[0][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[0][w].u.z * hfield[wF].z + coeff[0][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x = coeff[0][w].w.x * efield[wF].x + coeff[0][w].e.x * hCurl[wF].x;
						efield[wF].y = coeff[0][w].w.y * efield[wF].y + coeff[0][w].e.y * hCurl[wF].y;
						efield[wF].z = coeff[0][w].w.z * efield[wF].z + coeff[0][w].e.z * hCurl[wF].z;
						//
						hfield[wF].x = coeff[0][w].w.x * hfield[wF].x + coeff[0][w].h.x * eCurl[wF].x;
						hfield[wF].y = coeff[0][w].w.y * hfield[wF].y + coeff[0][w].h.y * eCurl[wF].y;
						hfield[wF].z = coeff[0][w].w.z * hfield[wF].z + coeff[0][w].h.z * eCurl[wF].z;
						//
						wS++;
					}
					w++;
				}
			}
			*/
		}
		else if (edgbars[h].parallelY())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					//for (unsigned int j = j0; j <= j1; j++)
					//{
					efield->getField(i, j, k, &ef);
					hfield->getField(i, j, k, &hf);
					hCurl->getField(i, j, k, &hc);
					eCurl->getField(i, j, k, &ec);
					//wF = IdxField(i, j, k);
					//integrations updated first before fields are changed
					sumE[wS].x += coeff[0][w].u.x * ef.x + coeff[0][w].f.x * hc.x;
					sumE[wS].y += coeff[0][w].u.y * ef.y + coeff[0][w].f.y * hc.y;
					sumE[wS].z += coeff[0][w].u.z * ef.z + coeff[0][w].f.z * hc.z;
					//
					sumH[wS].x += coeff[0][w].u.x * hf.x + coeff[0][w].g.x * ec.x;
					sumH[wS].y += coeff[0][w].u.y * hf.y + coeff[0][w].g.y * ec.y;
					sumH[wS].z += coeff[0][w].u.z * hf.z + coeff[0][w].g.z * ec.z;
					//
					//fields updated next
					v0.x = coeff[0][w].w.x * ef.x + coeff[0][w].e.x * hc.x;
					v0.y = coeff[0][w].w.y * ef.y + coeff[0][w].e.y * hc.y;
					v0.z = coeff[0][w].w.z * ef.z + coeff[0][w].e.z * hc.z;
					efield->setField(i, j, k, &v0);
					//
					v0.x = coeff[0][w].w.x * hf.x + coeff[0][w].h.x * ec.x;
					v0.y = coeff[0][w].w.y * hf.y + coeff[0][w].h.y * ec.y;
					v0.z = coeff[0][w].w.z * hf.z + coeff[0][w].h.z * ec.z;
					hfield->setField(i, j, k, &v0);
					//
					wS++;
					//}
					w++;
				}
			}
		}
		else if (edgbars[h].parallelZ())
		{
			/* z-rotation symmetry does not include this case
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int j = j0; j <= j1; j++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[0][w].u.x * efield[wF].x + coeff[0][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[0][w].u.y * efield[wF].y + coeff[0][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[0][w].u.z * efield[wF].z + coeff[0][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[0][w].u.x * hfield[wF].x + coeff[0][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[0][w].u.y * hfield[wF].y + coeff[0][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[0][w].u.z * hfield[wF].z + coeff[0][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x = coeff[0][w].w.x * efield[wF].x + coeff[0][w].e.x * hCurl[wF].x;
						efield[wF].y = coeff[0][w].w.y * efield[wF].y + coeff[0][w].e.y * hCurl[wF].y;
						efield[wF].z = coeff[0][w].w.z * efield[wF].z + coeff[0][w].e.z * hCurl[wF].z;
						//
						hfield[wF].x = coeff[0][w].w.x * hfield[wF].x + coeff[0][w].h.x * eCurl[wF].x;
						hfield[wF].y = coeff[0][w].w.y * hfield[wF].y + coeff[0][w].h.y * eCurl[wF].y;
						hfield[wF].z = coeff[0][w].w.z * hfield[wF].z + coeff[0][w].h.z * eCurl[wF].z;
						//
						wS++;
					}
					w++;
				}
			}
			*/
		}
	}
}
void PmlTss::apply2ndCurlEdgesZrotateSymmetry(RotateSymmetryField *efield, RotateSymmetryField *hfield, RotateSymmetryField *eCurl, RotateSymmetryField *hCurl)
{
	size_t w, wS;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, k0, k1;
	//
	unsigned int j = ic; //y=0
	Point3Dstruct ef;
	Point3Dstruct hf;
	Point3Dstruct hc;
	Point3Dstruct ec;
	Point3Dstruct v0;
	//
	for (int h = 0; h < EDGCOUNT; h++)
	{
		sumH = edgbars[h].SmH();
		sumE = edgbars[h].SmE();
		coeff = edgbars[h].Coefficients();
		i0 = edgbars[h].iStart();
		i1 = edgbars[h].iEnd();
		if (i0 > ic)
		{
			continue; //only apply edges of low-end
		}
		//j0 = edgbars[h].jStart();
		//j1 = edgbars[h].jEnd();
		k0 = edgbars[h].kStart();
		k1 = edgbars[h].kEnd();
		w = 0;
		wS = 0;
		if (edgbars[h].parallelX())
		{
			/* z-rotation symmetry does not include this case
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					for (unsigned int i = i0; i <= i1; i++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].u.x * eCurl[wF].x;
						sumE[wS].y += coeff[1][w].u.y * eCurl[wF].y;
						sumE[wS].z += coeff[1][w].u.z * eCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].u.x * hCurl[wF].x;
						sumH[wS].y += coeff[1][w].u.y * hCurl[wF].y;
						sumH[wS].z += coeff[1][w].u.z * hCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].w.x * eCurl[wF].x;
						efield[wF].y += coeff[1][w].w.y * eCurl[wF].y;
						efield[wF].z += coeff[1][w].w.z * eCurl[wF].z;
						//
						hfield[wF].x += coeff[1][w].w.x * hCurl[wF].x;
						hfield[wF].y += coeff[1][w].w.y * hCurl[wF].y;
						hfield[wF].z += coeff[1][w].w.z * hCurl[wF].z;
						//
						wS++;
					}
					w++;
				}
			}
			*/
		}
		else if (edgbars[h].parallelY())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					//for (unsigned int j = j0; j <= j1; j++)
					//{
					//wF = IdxField(i, j, k);
					efield->getField(i, j, k, &ef);
					hfield->getField(i, j, k, &hf);
					hCurl->getField(i, j, k, &hc);
					eCurl->getField(i, j, k, &ec);
					//integrations updated first before fields are changed
					sumE[wS].x += coeff[1][w].u.x * ec.x;
					sumE[wS].y += coeff[1][w].u.y * ec.y;
					sumE[wS].z += coeff[1][w].u.z * ec.z;
					//
					sumH[wS].x += coeff[1][w].u.x * hc.x;
					sumH[wS].y += coeff[1][w].u.y * hc.y;
					sumH[wS].z += coeff[1][w].u.z * hc.z;
					//
					//fields updated next
					v0.x = ef.x + coeff[1][w].w.x * ec.x;
					v0.y = ef.y + coeff[1][w].w.y * ec.y;
					v0.z = ef.y + coeff[1][w].w.z * ec.z;
					efield->setField(i, j, k, &v0);
					//
					v0.x = hf.x + coeff[1][w].w.x * hc.x;
					v0.y = hf.y + coeff[1][w].w.y * hc.y;
					v0.z = hf.z + coeff[1][w].w.z * hc.z;
					hfield->setField(i, j, k, &v0);
					//
					wS++;
					//}
					w++;
				}
			}
		}
		else if (edgbars[h].parallelZ())
		{
			/* z-rotation symmetry does not include this case
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int j = j0; j <= j1; j++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].u.x * eCurl[wF].x;
						sumE[wS].y += coeff[1][w].u.y * eCurl[wF].y;
						sumE[wS].z += coeff[1][w].u.z * eCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].u.x * hCurl[wF].x;
						sumH[wS].y += coeff[1][w].u.y * hCurl[wF].y;
						sumH[wS].z += coeff[1][w].u.z * hCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].w.x * eCurl[wF].x;
						efield[wF].y += coeff[1][w].w.y * eCurl[wF].y;
						efield[wF].z += coeff[1][w].w.z * eCurl[wF].z;
						//
						hfield[wF].x += coeff[1][w].w.x * hCurl[wF].x;
						hfield[wF].y += coeff[1][w].w.y * hCurl[wF].y;
						hfield[wF].z += coeff[1][w].w.z * hCurl[wF].z;
						//
						wS++;
					}
					w++;
				}
			}
			*/
		}
	}
}
void PmlTss::apply3rdCurlEdgesZrotateSymmetry(RotateSymmetryField *efield, RotateSymmetryField *hfield, RotateSymmetryField *eCurl, RotateSymmetryField *hCurl)
{
	size_t w, wS;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, k0, k1;
	//
	unsigned int j = ic; //y=0
	Point3Dstruct ef;
	Point3Dstruct hf;
	Point3Dstruct hc;
	Point3Dstruct ec;
	Point3Dstruct v0;
	//
	for (int h = 0; h < EDGCOUNT; h++)
	{
		sumH = edgbars[h].SmH();
		sumE = edgbars[h].SmE();
		coeff = edgbars[h].Coefficients();
		i0 = edgbars[h].iStart();
		i1 = edgbars[h].iEnd();
		if (i0 > ic)
		{
			continue; //only apply lower-end edges
		}
		//j0 = edgbars[h].jStart();
		//j1 = edgbars[h].jEnd();
		k0 = edgbars[h].kStart();
		k1 = edgbars[h].kEnd();
		w = 0;
		wS = 0;
		if (edgbars[h].parallelX())
		{
			/* z-rotation symmetry does not include this case
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					for (unsigned int i = i0; i <= i1; i++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[1][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[1][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[1][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[1][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].e.x * hCurl[wF].x + sumE[w].x;
						efield[wF].y += coeff[1][w].e.y * hCurl[wF].y + sumE[w].y;
						efield[wF].z += coeff[1][w].e.z * hCurl[wF].z + sumE[w].z;
						//
						hfield[wF].x += coeff[1][w].h.x * eCurl[wF].x + sumH[w].x;
						hfield[wF].y += coeff[1][w].h.y * eCurl[wF].y + sumH[w].y;
						hfield[wF].z += coeff[1][w].h.z * eCurl[wF].z + sumH[w].z;
						//
						wS++;
					}
					w++;
				}
			}
			*/
		}
		else if (edgbars[h].parallelY())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int k = k0; k <= k1; k++)
				{
					//for (unsigned int j = j0; j <= j1; j++)
					//{
					//wF = IdxField(i, j, k);
					efield->getField(i, j, k, &ef);
					hfield->getField(i, j, k, &hf);
					hCurl->getField(i, j, k, &hc);
					eCurl->getField(i, j, k, &ec);
					//integrations updated first before fields are changed
					sumE[wS].x += coeff[1][w].f.x * hc.x;
					sumE[wS].y += coeff[1][w].f.y * hc.y;
					sumE[wS].z += coeff[1][w].f.z * hc.z;
					//
					sumH[wS].x += coeff[1][w].g.x * ec.x;
					sumH[wS].y += coeff[1][w].g.y * ec.y;
					sumH[wS].z += coeff[1][w].g.z * ec.z;
					//
					//fields updated next
					v0.x = ef.x + coeff[1][w].e.x * hc.x + sumE[wS].x;
					v0.y = ef.y + coeff[1][w].e.y * hc.y + sumE[wS].y;
					v0.z = ef.z + coeff[1][w].e.z * hc.z + sumE[wS].z;
					efield->setField(i, j, k, &v0);
					//
					v0.x = hf.x + coeff[1][w].h.x * ec.x + sumH[wS].x;
					v0.y = hf.y + coeff[1][w].h.y * ec.y + sumH[wS].y;
					v0.z = hf.z + coeff[1][w].h.z * ec.z + sumH[wS].z;
					hfield->setField(i, j, k, &v0);
					//
					wS++;
					//}
					w++;
				}
			}
		}
		else if (edgbars[h].parallelZ())
		{
			/* z-rotation symmetry does not include this case
			for (unsigned int i = i0; i <= i1; i++)
			{
				for (unsigned int j = j0; j <= j1; j++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[1][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[1][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[1][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[1][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].e.x * hCurl[wF].x + sumE[wS].x;
						efield[wF].y += coeff[1][w].e.y * hCurl[wF].y + sumE[wS].y;
						efield[wF].z += coeff[1][w].e.z * hCurl[wF].z + sumE[wS].z;
						//
						hfield[wF].x += coeff[1][w].h.x * eCurl[wF].x + sumH[wS].x;
						hfield[wF].y += coeff[1][w].h.y * eCurl[wF].y + sumH[wS].y;
						hfield[wF].z += coeff[1][w].h.z * eCurl[wF].z + sumH[wS].z;
						//
						wS++;
					}
					w++;
				}
			}
			*/
		}
	}
}
/////end of edges////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////sides///////////////////////////////////////////////////////////////////////////////
void PmlTss::apply1stCurlSidesZrotateSymmetry(RotateSymmetryField *efield, RotateSymmetryField *hfield, RotateSymmetryField *eCurl, RotateSymmetryField *hCurl)
{
	size_t w, wS;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, k0, k1;
	//
	unsigned int j = ic; //y=0
	Point3Dstruct ef;
	Point3Dstruct hf;
	Point3Dstruct hc;
	Point3Dstruct ec;
	Point3Dstruct v0;
	//
	for (int h = 0; h < SIDECOUNT; h++)
	{
		sumH = sides[h].SmH();
		sumE = sides[h].SmE();
		coeff = sides[h].Coefficients();
		i0 = sides[h].iStart();
		i1 = sides[h].iEnd();
		if (i0 > ic)
		{
			continue; //only apply lower-end side board
		}
		//j0 = sides[h].jStart();
		//j1 = sides[h].jEnd();
		k0 = sides[h].kStart();
		k1 = sides[h].kEnd();
		w = 0;
		wS = 0;
		if (sides[h].sideX())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				//for (unsigned int j = j0; j <= j1; j++)
				//{
				for (unsigned int k = k0; k <= k1; k++)
				{
					//wF = IdxField(i, j, k);
					efield->getField(i, j, k, &ef);
					hfield->getField(i, j, k, &hf);
					hCurl->getField(i, j, k, &hc);
					eCurl->getField(i, j, k, &ec);
					//integrations updated first before fields are changed
					sumE[wS].x += coeff[0][w].u.x * ef.x + coeff[0][w].f.x * hc.x;
					sumE[wS].y += coeff[0][w].u.y * ef.y + coeff[0][w].f.y * hc.y;
					sumE[wS].z += coeff[0][w].u.z * ef.z + coeff[0][w].f.z * hc.z;
					//
					sumH[wS].x += coeff[0][w].u.x * hf.x + coeff[0][w].g.x * ec.x;
					sumH[wS].y += coeff[0][w].u.y * hf.y + coeff[0][w].g.y * ec.y;
					sumH[wS].z += coeff[0][w].u.z * hf.z + coeff[0][w].g.z * ec.z;
					//
					//fields updated next
					v0.x = coeff[0][w].w.x * ef.x + coeff[0][w].e.x * hc.x;
					v0.y = coeff[0][w].w.y * ef.y + coeff[0][w].e.y * hc.y;
					v0.z = coeff[0][w].w.z * ef.z + coeff[0][w].e.z * hc.z;
					efield->setField(i, j, k, &v0);
					//
					v0.x = coeff[0][w].w.x * hf.x + coeff[0][w].h.x * ec.x;
					v0.y = coeff[0][w].w.y * hf.y + coeff[0][w].h.y * ec.y;
					v0.z = coeff[0][w].w.z * hf.z + coeff[0][w].h.z * ec.z;
					hfield->setField(i, j, k, &v0);
					//
					wS++;
				}
				//}
				w++;
			}
		}
		else if (sides[h].sideY())
		{
			/* z-rotation symmetry does not include this case
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[0][w].u.x * efield[wF].x + coeff[0][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[0][w].u.y * efield[wF].y + coeff[0][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[0][w].u.z * efield[wF].z + coeff[0][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[0][w].u.x * hfield[wF].x + coeff[0][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[0][w].u.y * hfield[wF].y + coeff[0][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[0][w].u.z * hfield[wF].z + coeff[0][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x = coeff[0][w].w.x * efield[wF].x + coeff[0][w].e.x * hCurl[wF].x;
						efield[wF].y = coeff[0][w].w.y * efield[wF].y + coeff[0][w].e.y * hCurl[wF].y;
						efield[wF].z = coeff[0][w].w.z * efield[wF].z + coeff[0][w].e.z * hCurl[wF].z;
						//
						hfield[wF].x = coeff[0][w].w.x * hfield[wF].x + coeff[0][w].h.x * eCurl[wF].x;
						hfield[wF].y = coeff[0][w].w.y * hfield[wF].y + coeff[0][w].h.y * eCurl[wF].y;
						hfield[wF].z = coeff[0][w].w.z * hfield[wF].z + coeff[0][w].h.z * eCurl[wF].z;
						//
						wS++;
					}
				}
				w++;
			}
			*/
		}
		else if (sides[h].sideZ())
		{
			for (unsigned int k = k0; k <= k1; k++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					//for (unsigned int j = j0; j <= j1; j++)
					//{
					//wF = IdxField(i, j, k);
					efield->getField(i, j, k, &ef);
					hfield->getField(i, j, k, &hf);
					hCurl->getField(i, j, k, &hc);
					eCurl->getField(i, j, k, &ec);
					//integrations updated first before fields are changed
					sumE[wS].x += coeff[0][w].u.x * ef.x + coeff[0][w].f.x * hc.x;
					sumE[wS].y += coeff[0][w].u.y * ef.y + coeff[0][w].f.y * hc.y;
					sumE[wS].z += coeff[0][w].u.z * ef.z + coeff[0][w].f.z * hc.z;
					//
					sumH[wS].x += coeff[0][w].u.x * hf.x + coeff[0][w].g.x * ec.x;
					sumH[wS].y += coeff[0][w].u.y * hf.y + coeff[0][w].g.y * ec.y;
					sumH[wS].z += coeff[0][w].u.z * hf.z + coeff[0][w].g.z * ec.z;
					//
					//fields updated next
					v0.x = coeff[0][w].w.x * ef.x + coeff[0][w].e.x * hc.x;
					v0.y = coeff[0][w].w.y * ef.y + coeff[0][w].e.y * hc.y;
					v0.z = coeff[0][w].w.z * ef.z + coeff[0][w].e.z * hc.z;
					efield->setField(i, j, k, &v0);
					//
					v0.x = coeff[0][w].w.x * hf.x + coeff[0][w].h.x * ec.x;
					v0.y = coeff[0][w].w.y * hf.y + coeff[0][w].h.y * ec.y;
					v0.z = coeff[0][w].w.z * hf.z + coeff[0][w].h.z * ec.z;
					hfield->setField(i, j, k, &v0);
					//
					wS++;
					//}
				}
				w++;
			}
		}
	}
}
void PmlTss::apply2ndCurlSidesZrotateSymmetry(RotateSymmetryField *efield, RotateSymmetryField *hfield, RotateSymmetryField *eCurl, RotateSymmetryField *hCurl)
{
	size_t w, wS;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, k0, k1;
	//
	unsigned int j = ic; //y=0
	Point3Dstruct ef;
	Point3Dstruct hf;
	Point3Dstruct hc;
	Point3Dstruct ec;
	Point3Dstruct v0;
	//
	for (int h = 0; h < SIDECOUNT; h++)
	{
		sumH = sides[h].SmH();
		sumE = sides[h].SmE();
		coeff = sides[h].Coefficients();
		i0 = sides[h].iStart();
		i1 = sides[h].iEnd();
		if (i0 > ic)
		{
			continue; //only apply lower-end side board
		}
		//j0 = sides[h].jStart();
		//j1 = sides[h].jEnd();
		k0 = sides[h].kStart();
		k1 = sides[h].kEnd();
		w = 0;
		wS = 0;
		if (sides[h].sideX())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				//for (unsigned int j = j0; j <= j1; j++)
				//{
				for (unsigned int k = k0; k <= k1; k++)
				{
					//wF = IdxField(i, j, k);
					efield->getField(i, j, k, &ef);
					hfield->getField(i, j, k, &hf);
					hCurl->getField(i, j, k, &hc);
					eCurl->getField(i, j, k, &ec);
					//integrations updated first before fields are changed
					sumE[wS].x += coeff[1][w].u.x * ec.x;
					sumE[wS].y += coeff[1][w].u.y * ec.y;
					sumE[wS].z += coeff[1][w].u.z * ec.z;
					//
					sumH[wS].x += coeff[1][w].u.x * hc.x;
					sumH[wS].y += coeff[1][w].u.y * hc.y;
					sumH[wS].z += coeff[1][w].u.z * hc.z;
					//
					//fields updated next
					v0.x = ef.x + coeff[1][w].w.x * ec.x;
					v0.y = ef.y + coeff[1][w].w.y * ec.y;
					v0.z = ef.z + coeff[1][w].w.z * ec.z;
					efield->setField(i, j, k, &v0);
					//
					v0.x = hf.x + coeff[1][w].w.x * hc.x;
					v0.y = hf.y + coeff[1][w].w.y * hc.y;
					v0.z = hf.z + coeff[1][w].w.z * hc.z;
					hfield->setField(i, j, k, &v0);
					//
					wS++;
				}
				//}
				w++;
			}
		}
		else if (sides[h].sideY())
		{
			/* z-rotation symmetry does not include this case
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].u.x * eCurl[wF].x;
						sumE[wS].y += coeff[1][w].u.y * eCurl[wF].y;
						sumE[wS].z += coeff[1][w].u.z * eCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].u.x * hCurl[wF].x;
						sumH[wS].y += coeff[1][w].u.y * hCurl[wF].y;
						sumH[wS].z += coeff[1][w].u.z * hCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].w.x * eCurl[wF].x;
						efield[wF].y += coeff[1][w].w.y * eCurl[wF].y;
						efield[wF].z += coeff[1][w].w.z * eCurl[wF].z;
						//
						hfield[wF].x += coeff[1][w].w.x * hCurl[wF].x;
						hfield[wF].y += coeff[1][w].w.y * hCurl[wF].y;
						hfield[wF].z += coeff[1][w].w.z * hCurl[wF].z;
						///
						wS++;
					}
				}
				w++;
			}
			*/
		}
		else if (sides[h].sideZ())
		{
			for (unsigned int k = k0; k <= k1; k++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					//for (unsigned int j = j0; j <= j1; j++)
					//{
					//wF = IdxField(i, j, k);
					efield->getField(i, j, k, &ef);
					hfield->getField(i, j, k, &hf);
					hCurl->getField(i, j, k, &hc);
					eCurl->getField(i, j, k, &ec);
					//integrations updated first before fields are changed
					sumE[wS].x += coeff[1][w].u.x * ec.x;
					sumE[wS].y += coeff[1][w].u.y * ec.y;
					sumE[wS].z += coeff[1][w].u.z * ec.z;
					//
					sumH[wS].x += coeff[1][w].u.x * hc.x;
					sumH[wS].y += coeff[1][w].u.y * hc.y;
					sumH[wS].z += coeff[1][w].u.z * hc.z;
					//
					//fields updated next
					v0.x = ef.x + coeff[1][w].w.x * ec.x;
					v0.y = ef.y + coeff[1][w].w.y * ec.y;
					v0.z = ef.z + coeff[1][w].w.z * ec.z;
					efield->setField(i, j, k, &v0);
					//
					v0.x = hf.x + coeff[1][w].w.x * hc.x;
					v0.y = hf.y + coeff[1][w].w.y * hc.y;
					v0.z = hf.z + coeff[1][w].w.z * hc.z;
					hfield->setField(i, j, k, &v0);
					//
					wS++;
					//}
				}
				w++;
			}
		}
	}
}
void PmlTss::apply3rdCurlSidesZrotateSymmetry(RotateSymmetryField *efield, RotateSymmetryField *hfield, RotateSymmetryField *eCurl, RotateSymmetryField *hCurl)
{
	size_t w, wS;
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	PmlCoefficients **coeff;
	unsigned int i0, i1, k0, k1;
	//
	unsigned int j = ic; //y=0
	Point3Dstruct ef;
	Point3Dstruct hf;
	Point3Dstruct hc;
	Point3Dstruct ec;
	Point3Dstruct v0;
	//
	for (int h = 0; h < SIDECOUNT; h++)
	{
		sumH = sides[h].SmH();
		sumE = sides[h].SmE();
		coeff = sides[h].Coefficients();
		i0 = sides[h].iStart();
		i1 = sides[h].iEnd();
		if (i0 > ic)
		{
			continue; //only apply lower-end side board
		}
		//j0 = sides[h].jStart();
		//j1 = sides[h].jEnd();
		k0 = sides[h].kStart();
		k1 = sides[h].kEnd();
		w = 0;
		if (sides[h].sideX())
		{
			for (unsigned int i = i0; i <= i1; i++)
			{
				//for (unsigned int j = j0; j <= j1; j++)
				//{
				for (unsigned int k = k0; k <= k1; k++)
				{
					//wF = IdxField(i, j, k);
					efield->getField(i, j, k, &ef);
					hfield->getField(i, j, k, &hf);
					hCurl->getField(i, j, k, &hc);
					eCurl->getField(i, j, k, &ec);
					//integrations updated first before fields are changed
					sumE[wS].x += coeff[1][w].f.x * hc.x;
					sumE[wS].y += coeff[1][w].f.y * hc.y;
					sumE[wS].z += coeff[1][w].f.z * hc.z;
					//
					sumH[wS].x += coeff[1][w].g.x * ec.x;
					sumH[wS].y += coeff[1][w].g.y * ec.y;
					sumH[wS].z += coeff[1][w].g.z * ec.z;
					//
					//fields updated next
					v0.x = ef.x + coeff[1][w].e.x * hc.x + sumE[wS].x;
					v0.y = ef.y + coeff[1][w].e.y * hc.y + sumE[wS].y;
					v0.z = ef.z + coeff[1][w].e.z * hc.z + sumE[wS].z;
					efield->setField(i, j, k, &v0);
					//
					v0.x = hf.x + coeff[1][w].h.x * ec.x + sumH[wS].x;
					v0.y = hf.y + coeff[1][w].h.y * ec.y + sumH[wS].y;
					v0.z = hf.z + coeff[1][w].h.z * ec.z + sumH[wS].z;
					hfield->setField(i, j, k, &v0);
					//
					wS++;
				}
				//}
				w++;
			}
		}
		else if (sides[h].sideY())
		{
			/* z-rotation symmetry does not include this case
			for (unsigned int j = j0; j <= j1; j++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					for (unsigned int k = k0; k <= k1; k++)
					{
						wF = IdxField(i, j, k);
						//integrations updated first before fields are changed
						sumE[wS].x += coeff[1][w].f.x * hCurl[wF].x;
						sumE[wS].y += coeff[1][w].f.y * hCurl[wF].y;
						sumE[wS].z += coeff[1][w].f.z * hCurl[wF].z;
						//
						sumH[wS].x += coeff[1][w].g.x * eCurl[wF].x;
						sumH[wS].y += coeff[1][w].g.y * eCurl[wF].y;
						sumH[wS].z += coeff[1][w].g.z * eCurl[wF].z;
						//
						//fields updated next
						efield[wF].x += coeff[1][w].e.x * hCurl[wF].x + sumE[w].x;
						efield[wF].y += coeff[1][w].e.y * hCurl[wF].y + sumE[w].y;
						efield[wF].z += coeff[1][w].e.z * hCurl[wF].z + sumE[w].z;
						//
						hfield[wF].x += coeff[1][w].h.x * eCurl[wF].x + sumH[w].x;
						hfield[wF].y += coeff[1][w].h.y * eCurl[wF].y + sumH[w].y;
						hfield[wF].z += coeff[1][w].h.z * eCurl[wF].z + sumH[w].z;
						//
						wS++;
					}
				}
				w++;
			}
			*/
		}
		else if (sides[h].sideZ())
		{
			for (unsigned int k = k0; k <= k1; k++)
			{
				for (unsigned int i = i0; i <= i1; i++)
				{
					//for (unsigned int j = j0; j <= j1; j++)
					//{
					//wF = IdxField(i, j, k);
					efield->getField(i, j, k, &ef);
					hfield->getField(i, j, k, &hf);
					hCurl->getField(i, j, k, &hc);
					eCurl->getField(i, j, k, &ec);
					//integrations updated first before fields are changed
					sumE[wS].x += coeff[1][w].f.x * hc.x;
					sumE[wS].y += coeff[1][w].f.y * hc.y;
					sumE[wS].z += coeff[1][w].f.z * hc.z;
					//
					sumH[wS].x += coeff[1][w].g.x * ec.x;
					sumH[wS].y += coeff[1][w].g.y * ec.y;
					sumH[wS].z += coeff[1][w].g.z * ec.z;
					//
					//fields updated next
					v0.x = ef.x + coeff[1][w].e.x * hc.x + sumE[wS].x;
					v0.y = ef.y + coeff[1][w].e.y * hc.y + sumE[wS].y;
					v0.z = ef.z + coeff[1][w].e.z * hc.z + sumE[wS].z;
					efield->setField(i, j, k, &v0);
					//
					v0.x = hf.x + coeff[1][w].h.x * ec.x + sumH[wS].x;
					v0.y = hf.y + coeff[1][w].h.y * ec.y + sumH[wS].y;
					v0.z = hf.z + coeff[1][w].h.z * ec.z + sumH[wS].z;
					hfield->setField(i, j, k, &v0);
					//
					wS++;
					//}
				}
				w++;
			}
		}
	}
}
/////end of sides////////////////////////////////////////////////////////////////////////
/////Apply curls/////////////////////////////////////////////////////////////////////////
/*
Appy curl-0 and curl-1
*/
void PmlTss::apply1stCurlZrotateSymmetry(RotateSymmetryField *efield, RotateSymmetryField *hfield, RotateSymmetryField *eCurl, RotateSymmetryField *hCurl)
{
	apply1stCurlEdgesZrotateSymmetry(efield, hfield, eCurl, hCurl);
	apply1stCurlSidesZrotateSymmetry(efield, hfield, eCurl, hCurl);
}

/*
Appy curl-2
*/
void PmlTss::apply2ndCurlZrotateSymmetry(RotateSymmetryField *efield, RotateSymmetryField *hfield, RotateSymmetryField *eCurl, RotateSymmetryField *hCurl)
{
	apply2ndCurlEdgesZrotateSymmetry(efield, hfield, eCurl, hCurl);
	apply2ndCurlSidesZrotateSymmetry(efield, hfield, eCurl, hCurl);
}

/*
Appy curl-3
*/
void PmlTss::apply3rdCurlZrotateSymmetry(RotateSymmetryField *efield, RotateSymmetryField *hfield, RotateSymmetryField *eCurl, RotateSymmetryField *hCurl)
{
	apply3rdCurlEdgesZrotateSymmetry(efield, hfield, eCurl, hCurl);
	apply3rdCurlSidesZrotateSymmetry(efield, hfield, eCurl, hCurl);
}
//////////////////////////////////////////////////////////////////////////////
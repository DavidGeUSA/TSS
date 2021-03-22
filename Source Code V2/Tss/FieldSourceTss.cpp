/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

field source
********************************************************************/
#include "FieldSourceTss.h"

#include <boost/multiprecision/cpp_dec_float.hpp>

#define ih(i, h) ((h)*eMax+(i))

FieldSourceTss::FieldSourceTss()
{
	f0 = g0 = w0 = u0 = NULL;
	_initialized = false;
}


FieldSourceTss::~FieldSourceTss()
{
	cleanup();
}

void FieldSourceTss::cleanup()
{
	if (f0 != NULL)
	{
		free(f0); f0 = NULL;
	}
	if (g0 != NULL)
	{
		free(g0); g0 = NULL;
	}
	if (w0 != NULL)
	{
		free(w0); w0 = NULL;
	}
	if (u0 != NULL)
	{
		free(u0); u0 = NULL;
	}
}

int FieldSourceTss::initialize(SimStruct *params)
{
	int ret = ERR_OK;
	//size_t emMax2;
	size_t srcDim2;
	cleanup();
	pams = params;
	nx1 = pams->nx + 1; ny1 = pams->ny + 1; nz1 = pams->nz + 1;
	emMax = 2 * pams->kmax + 3;
	//emMax2 = emMax * emMax;
	srcDim = 2 * pams->kmax + 1;
	srcDim2 = srcDim * srcDim;
	f0 = (double *)malloc(srcDim2*sizeof(double));
	g0 = (double *)malloc(srcDim2*sizeof(double));
	w0 = (double *)malloc(srcDim2*sizeof(double));
	u0 = (double *)malloc(srcDim2*sizeof(double));
	if (f0 == NULL || g0 == NULL || w0 == NULL || u0 == NULL)
	{
		ret = ERR_OUTOFMEMORY;
	}
	else
	{
		for (unsigned int k = 0; k < srcDim2; k++)
		{
			f0[k] = g0[k] = w0[k] = u0[k] = 0.0;
		}
		
	}
	_initialized = true;
	return ret;
}

/*
	calculate field fource coefficients
*/
int FieldSourceTss::onInitialized(void *e0, void *m0)
{
	using namespace boost::multiprecision;
	int ret = ERR_OK;
	size_t srcDim2 = srcDim * srcDim;
	cpp_dec_float_100 *f  = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *g  = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *w  = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *u  = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *f2 = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *g2 = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *w2 = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *u2 = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *f1 = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *g1 = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *w1 = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *u1 = (cpp_dec_float_100 *)malloc(srcDim2*sizeof(cpp_dec_float_100));
	//
	cpp_dec_float_100 *sr = (cpp_dec_float_100 *)malloc((emMax + 2)*sizeof(cpp_dec_float_100));
	if (sr == NULL || f == NULL || g == NULL || w == NULL || u == NULL 
		|| f1 == NULL || g1 == NULL || w1 == NULL || u1 == NULL
		|| f2 == NULL || g2 == NULL || w2 == NULL || u2 == NULL)
		ret = ERR_OUTOFMEMORY;
	else
	{
		cpp_dec_float_100 *e = (cpp_dec_float_100 *)e0;
		cpp_dec_float_100 *m = (cpp_dec_float_100 *)m0;
		for (unsigned int k = 0; k < srcDim2; k++)
		{
			f[k]  = 0.0; g[k]  = 0.0; w[k]  = 0.0; u[k]  = 0.0;
		}
		cpp_dec_float_100 XN = 1.0;
		cpp_dec_float_100 X = 0.0;
		cpp_dec_float_100 dtN = 1.0;
		//sr[k] = (∆_t^k)/ k!
		for (unsigned int k = 0; k < emMax + 2; k++)
		{
			sr[k] = dtN / XN;
			X += 1.0;
			XN = XN * X;
			dtN = dtN * pams->dt;
		}
		/*
		S_m (2k+1)=∑_(h=0)^k▒〖∇^{2(k-h)} ×∑_(i=0)^2h▒〖m_i^{2(k-h)}   (d^(2h-i) J_m)/(dt^(2h-i) )〗〗
			      +∑_(h=0)^(k-1)▒〖∇^{2(k-h)-1} ×∑_(i=0)^(2h+1)▒〖e_i^{2(k-h)-1}   (d^(2h-i+1) J_e)/(dt^(2h-i+1) )〗〗
		S_e (2k+1)=∑_(h=0)^k▒〖∇^{2(k-h)} ×∑_(i=0)^2h▒〖e_i^{2(k-h)}   (d^(2h-i) J_e)/(dt^(2h-i) )〗〗
			      +∑_(h=0)^(k-1)▒〖∇^{2(k-h)-1} ×∑_(i=0)^(2h+1)▒〖m_i^{2(k-h)-1}   (d^(2h-i+1) J_m)/(dt^(2h-i+1) )〗〗
		S_m (2k)  =∑_(h=0)^(k-1)▒(∇^{2(k-1-h)} ×∑_(i=0)^(2h+1)▒〖m_i^{2(k-1-h)}   (d^(2h+1-i) J_m)/(dt^(2h+1-i) )〗
				  +∇^{2(k-h)-1} ×∑_(i=0)^2h▒〖e_i^{2(k-h)-1}   (d^(2h-i) J_e)/(dt^(2h-i) )〗)
		S_e (2k)  =∑_(h=0)^(k-1)▒(∇^{2(k-1-h)} ×∑_(i=0)^(2h+1)▒〖e_i^{2(k-1-h)}   (d^(2h+1-i) J_e)/(dt^(2h+1-i) )〗
				  +∇^{2(k-h)-1} ×∑_(i=0)^2h▒〖m_i^{2(k-h)-1}   (d^(2h-i) J_m)/(dt^(2h-i) )〗)

		S^h (k,∆_t)=(∆_t^2k)/(2k)! S_m (2k) + (∆_t^(2k+1))/(2k+1)! S_m (2k+1)
		S^e (k,∆_t)=(∆_t^2k)/(2k)! S_e (2k) + (∆_t^(2k+1))/(2k+1)! S_e (2k+1)

		SH=∑_(k=0)^(k_max)▒〖S^h (k,∆_t ) 〗-> get f and g -> C.F.dJm + C.G.dJe
		SE=∑_(k=0)^(k_max)▒〖S^e (k,∆_t ) 〗-> get u and w -> C.U.dJe + C.W.dJm

		S_m (2k)   -> f2, g2;  S_m (2k+1) -> f1, g1;
		S_e (2k)   -> u2, w2;  S_e (2k+1) -> u1, w1
		f = sr[2k]*f2 + sr[2k+1]*f1; g = sr[2k]*g2 + sr[2k+1]*g1
		u = sr[2k]*u2 + sr[2k+1]*u1; w = sr[2k]*w2 + sr[2k+1]*w1
		*/
		size_t t,t1=0;
		unsigned int kmax = pams->kmax;
		t = 0;
		for (unsigned int k = 0; k <= kmax; k++)
		{
			//temporary summation holders
			for (t = 0; t < srcDim2; t++)
			{
				f1[t] = 0.0; g1[t] = 0.0; w1[t] = 0.0; u1[t] = 0.0;
				f2[t] = 0.0; g2[t] = 0.0; w2[t] = 0.0; u2[t] = 0.0;
			}
			//H = C.F.dJm + C.G.dJe + ...
			//E = C.U.dJe + C.W.dJm + ...
			for (unsigned int h = 0; h <= k; h++)
			{
				//first part of S_m (2k+1)->f1:Jm, S_e (2k+1)->u1:Je
				//∇^{2(k-h)} -> row number 0...2kmax
				for (unsigned int i = 0; i <= 2 * h; i++)
				{
					//d^(2h-i), column number 2 * h - i 
					t = Sr_ih(2 * h - i, 2 * (k - h));  //0...2kmax, 0...,2kmax
					t1 = I_ih(i, 2 * (k - h)); //m_i^{2(k-h)}, e_i^{2(k-h)}
					f1[t] += m[t1];
					u1[t] += e[t1];
				}
			}
			for (unsigned int h = 0; h < k; h++)
			{
				for (unsigned int i = 0; i <= 2 * h + 1; i++)
				{
					//second part of S_m (2k+1)->g1:Je, S_e (2k+1)->w1:Jm
					//∇^{2(k-h)-1} ->row number
					//column number 2h-i+1
					t = Sr_ih(2 * h - i + 1, 2 * (k - h) - 1);
					t1 = I_ih(i, 2 * (k - h) - 1); //e_i^{2(k-h)-1}, m_i^{2(k-h)-1}
					g1[t] += e[t1];
					w1[t] += m[t1];
					//first part of S_m (2k)->f2:Jm, S_e (2k)->u2:Je
					//∇^{2(k-1-h)} -> row number; column number d^(2h+1-i)
					t = Sr_ih(2 * h + 1 - i, 2 * (k - 1 - h));
					t1 = I_ih(i, 2 * (k - 1 - h)); //m_i^{2(k-1-h)}, e_i^{2(k-1-h)}
					f2[t] += m[t1];
					u2[t] += e[t1];
				}
				//second part of  S_m (2k)->g2:Je, S_e (2k)->w2:Jm
				//∇^{2(k-h)-1} -> row number; column number d^(2h-i)
				for (unsigned int i = 0; i <= 2 * h; i++)
				{
					t = Sr_ih(2 * h - i, 2 * (k - h) - 1);
					t1 = I_ih(i, 2 * (k - h) - 1); //e_i^{2(k-h)-1}, m_i^{2(k-h)-1}
					g2[t] += e[t1];
					w2[t] += m[t1];
				}
			}
			//add to the results
			for (t = 0; t < srcDim2; t++)
			{
				f[t] += f2[t] * sr[2 * k] + f1[t] * sr[2 * k + 1];
				g[t] += g2[t] * sr[2 * k] + g1[t] * sr[2 * k + 1];
				u[t] += u2[t] * sr[2 * k] + u1[t] * sr[2 * k + 1];
				w[t] += w2[t] * sr[2 * k] + w1[t] * sr[2 * k + 1];
			}
		}
		//convert to double
		for (t = 0; t < srcDim2; t++)
		{
			f0[t] = f[t].convert_to<double>();
			g0[t] = g[t].convert_to<double>();
			w0[t] = w[t].convert_to<double>();
			u0[t] = u[t].convert_to<double>();
		}
	}
	if(f != NULL) free(f); 
	if(g != NULL) free(g);
	if(w != NULL) free(w);
	if(u != NULL) free(u);
	//
	if (f1 != NULL) free(f1);
	if (g1 != NULL) free(g1);
	if (w1 != NULL) free(w1);
	if (u1 != NULL) free(u1);
	//
	if (f2 != NULL) free(f2);
	if (g2 != NULL) free(g2);
	if (w2 != NULL) free(w2);
	if (u2 != NULL) free(u2);
	//
	if(sr != NULL) free(sr);
	return ret;
}


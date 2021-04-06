/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

base class for time advancement estimation module
********************************************************************/
#include "TimeTssBase.h"
#include "../FileUtil/fileutil.h"
#include "../ProcessMonitor/ProcessMonitor.h"
#include "../ProcessMonitor/workProcess.h"

#include <thread>

#include <boost/multiprecision/cpp_dec_float.hpp>

bool voidReport(){ return false; }
TimeTssBase::TimeTssBase()
{
	GehP = GhhP = GeeP = GheP = NULL;
	space = NULL;
	H = E = curlH = curlE = NULL; // workH = workE = NULL;
	currentTime = 0.0;
	tIndex = 0;
	reporter = NULL;
	operationCanceled = voidReport;
}


TimeTssBase::~TimeTssBase()
{
	cleanup();
}
void TimeTssBase::cleanup()
{
	//
	if (GehP != NULL)
	{
		free(GehP); GehP = NULL;
	}
	if (GhhP != NULL)
	{
		free(GhhP); GhhP = NULL;
	}
	if (GeeP != NULL)
	{
		free(GeeP); GeeP = NULL;
	}
	if (GheP != NULL)
	{
		free(GheP); GheP = NULL;
	}
	//
	if (H != NULL)
	{
		FreeMemory(H); H = NULL;
	}
	if (E != NULL)
	{
		FreeMemory(E); E = NULL;
	}
	if (curlH != NULL)
	{
		FreeMemory(curlH); curlH = NULL;
	}
	if (curlE != NULL)
	{
		FreeMemory(curlE); curlE = NULL;
	}
}
int TimeTssBase::initializeTimeModule(Space *s, SimStruct *pams0, FieldSourceTss *src)
{
	using namespace boost::multiprecision;
	int ret = ERR_OK;
	puts("\r\nCalculating update coefficients...");
	pams = pams0;
	if (src != NULL)
	{
		if (!src->Initialized())
		{
			return ERR_SRC_NOT_INIT;
		}
	}
	space = s;
	source = src;
	cpp_dec_float_100 *G_ehP = (cpp_dec_float_100 *)malloc((pams->kmax + 1)*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *G_hhP = (cpp_dec_float_100 *)malloc((pams->kmax + 1)*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *G_eeP = (cpp_dec_float_100 *)malloc((pams->kmax + 1)*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *G_heP = (cpp_dec_float_100 *)malloc((pams->kmax + 1)*sizeof(cpp_dec_float_100));
	//
	GehP = (double*)malloc((pams->kmax + 1)*sizeof(double));
	GhhP = (double*)malloc((pams->kmax + 1)*sizeof(double));
	GeeP = (double*)malloc((pams->kmax + 1)*sizeof(double));
	GheP = (double*)malloc((pams->kmax + 1)*sizeof(double));
	if (G_ehP == NULL || G_hhP == NULL || G_eeP == NULL || G_heP == NULL || GehP == NULL || GhhP == NULL || GeeP == NULL || GheP == NULL)
	{
		ret = ERR_OUTOFMEMORY;
	}
	else
	{
		printf("\r\nUpdate coefficients allocated. Array size:%u ", pams->kmax + 1);
		for (unsigned int k = 0; k <= pams->kmax; k++)
		{
			G_ehP[k] = G_hhP[k] = G_eeP[k] = G_heP[k] = 0.0;
			GehP[k]  = GhhP[k]  = GeeP[k]  = GheP[k]  = 0.0;
		}
	}
	if (ret == ERR_OK)
	{
		/*
		p[h=0..2k_max+2,i=0..2k_max+2]=p_i^{h} 
		q[h=0..2k_max+2,i=0..2k_max+2]=q_i^{h} 
		*/
		size_t pMax = 2 * pams->kmax + 3; // 0 ... 2kmax+2
		size_t pMax2 = pMax * pMax;
		//p_i^{h}
#define Idx_ih(i, h) ((h)*pMax+(i))
		cpp_dec_float_100 *p = (cpp_dec_float_100 *)malloc(pMax2*sizeof(cpp_dec_float_100));
		cpp_dec_float_100 *q = (cpp_dec_float_100 *)malloc(pMax2*sizeof(cpp_dec_float_100));
		cpp_dec_float_100 *dtR = (cpp_dec_float_100 *)malloc(pMax*sizeof(cpp_dec_float_100));
		cpp_dec_float_100 *dsR = (cpp_dec_float_100 *)malloc(pMax*sizeof(cpp_dec_float_100));
		if (p == NULL || q == NULL || dtR == NULL || dsR == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		if (ret == ERR_OK)
		{
			cpp_dec_float_100 gde = pams->sie;
			cpp_dec_float_100 geps = pams->eps;
			cpp_dec_float_100 gmu = pams->mu;
			cpp_dec_float_100 gdm = pams->sim;
			cpp_dec_float_100 g1 = 1.0;
			cpp_dec_float_100 f;
			//values for k=0
			cpp_dec_float_100 dt2 = 1.0;
			cpp_dec_float_100 XN = 0.0;
			cpp_dec_float_100 X = 1.0;
			cpp_dec_float_100 ds2 = 1.0;
			for (unsigned int k = 0; k < pMax2; k++)
			{
				p[k] = 0.0; q[k] = 0.0;
			}
			//p_0^0=1, q_0^0 = 1
			p[0] = 1.0; q[0] = 1.0;
			for (unsigned int k = 0; k < pMax; k++)
			{
				f = dt2 / X;
				dtR[k] = f; //dt^k/k!
				dsR[k] = ds2; //ds^k
				//prepare for k+1
				XN = XN + 1.0;
				X = X * XN;
				dt2 = dt2 * pams->dt;
				ds2 = ds2 * pams->ds;
			}
			/*
			p_0^{1} =1/ε,p_1^{1} =-σ/ε
			q_0^{1} =-1/μ,q_1^{1} =-σ_m/μ
			*/
			p[Idx_ih(0, 1)] = g1 / geps; p[Idx_ih(1, 1)] = -gde / geps;
			q[Idx_ih(0, 1)] = -g1 / gmu; q[Idx_ih(1, 1)] = -gdm / gmu;
			/*
			p_0^{2k}  =q_0^{1}  p_0^{2k-1} 
			p_2k^{2k} =p_(2k-1)^{2k-1}  p_1^{1}
			q_0^{2k}  =p_0^{1}  q_0^{2k-1} 
			q_2k^{2k} =q_(2k-1)^{2k-1}  q_1^{1}

			p_0^{2k+1}      =p_0^{1}  p_0^{2k} 
			p_(2k+1)^{2k+1} =p_2k^{2k}  p_1^{1} 
			q_0^{2k+1}      =q_0^{1}  q_0^{2k} 
			q_(2k+1)^{2k+1} =q_2k^{2k}  q_1^{1}
			*/
			//from (0,1), (1,1) get
			//(0,2)...(0,2kmax+2); (2,2) ... (2kmax+2,2kmax+2)
			//(0,3)...(0,2kmax+1); (3,3) ... (2kmax+1,2kmax+1)
			for (unsigned int k = 1; k <= pams->kmax + 1; k++)
			{
				p[Idx_ih(0, 2 * k)] = q[Idx_ih(0, 1)] * p[Idx_ih(0, 2 * k - 1)];
				q[Idx_ih(0, 2 * k)] = p[Idx_ih(0, 1)] * q[Idx_ih(0, 2 * k - 1)];
				p[Idx_ih(2 * k, 2 * k)] = p[Idx_ih(2 * k - 1, 2 * k - 1)] * p[Idx_ih(1, 1)];
				q[Idx_ih(2 * k, 2 * k)] = q[Idx_ih(2 * k - 1, 2 * k - 1)] * q[Idx_ih(1, 1)];
				if (k <= pams->kmax)
				{
					p[Idx_ih(0, 2 * k + 1)] = p[Idx_ih(0, 1)] * p[Idx_ih(0, 2 * k)];
					q[Idx_ih(0, 2 * k + 1)] = q[Idx_ih(0, 1)] * q[Idx_ih(0, 2 * k)];
					p[Idx_ih(2 * k + 1, 2 * k + 1)] = p[Idx_ih(2 * k, 2 * k)] * p[Idx_ih(1, 1)];
					q[Idx_ih(2 * k + 1, 2 * k + 1)] = q[Idx_ih(2 * k, 2 * k)] * q[Idx_ih(1, 1)];
				}
			}
			/*
			p_(2i+1)^{2k} =q_1^{1}  p_2i^{2k-1} +p_0^{1}  p_(2i+1)^{2k-1} ;    i=0,1,…,k-1;k_max+1≥k>0
			q_(2i+1)^{2k} =p_1^{1}  q_2i^{2k-1} +q_0^{1}  q_(2i+1)^{2k-1} ;    i=0,1,…,k-1;k_max+1≥k>0
			p_(2i+2)^{2k} =p_1^{1}  p_(2i+1)^{2k-1} +q_0^{1}  p_(2i+2)^{2k-1} ;i=0,1,…,k-2;k_max+1≥k>1
			q_(2i+2)^{2k} =q_1^{1}  q_(2i+1)^{2k-1} +p_0^{1}  q_(2i+2)^{2k-1} ;i=0,1,…,k-2;k_max+1≥k>1
			*/
			for (unsigned int k = 1; k <= pams->kmax + 1; k++)
			{
				for (unsigned int i = 0; i < k; i++)
				{
					p[Idx_ih(2 * i + 1, 2 * k)] = q[Idx_ih(1, 1)] * p[Idx_ih(2 * i, 2 * k - 1)] + p[Idx_ih(0, 1)] * p[Idx_ih(2 * i + 1, 2 * k - 1)];
					q[Idx_ih(2 * i + 1, 2 * k)] = p[Idx_ih(1, 1)] * q[Idx_ih(2 * i, 2 * k - 1)] + q[Idx_ih(0, 1)] * q[Idx_ih(2 * i + 1, 2 * k - 1)];
					if (k>1 && i < k - 1)
					{
						p[Idx_ih(2 * i + 2, 2 * k)] = p[Idx_ih(1, 1)] * p[Idx_ih(2 * i + 1, 2 * k - 1)] + q[Idx_ih(0, 1)] * p[Idx_ih(2 * i + 2, 2 * k - 1)];
						q[Idx_ih(2 * i + 2, 2 * k)] = q[Idx_ih(1, 1)] * q[Idx_ih(2 * i + 1, 2 * k - 1)] + p[Idx_ih(0, 1)] * q[Idx_ih(2 * i + 2, 2 * k - 1)];
					}
				}
				if (k <= pams->kmax)
				{
					/*
					p_(2i-1)^{2k+1} =p_1^{1}  p_(2i-2)^{2k} +q_0^{1}  p_(2i-1)^{2k}
					p_2i    ^{2k+1} =q_1^{1}  p_(2i-1)^{2k} +p_0^{1}  p_2i^{2k}
					q_(2i-1)^{2k+1} =q_1^{1}  q_(2i-2)^{2k} +p_0^{1}  q_(2i-1)^{2k}
					q_2i    ^{2k+1} =p_1^{1}  q_(2i-1)^{2k} +q_0^{1}  q_2i^{2k}
					i=1,2,…,k;k≥1
					*/
					for (unsigned int i = 1; i <= k; i++)
					{
						p[Idx_ih(2 * i - 1, 2 * k + 1)] = p[Idx_ih(1, 1)] * p[Idx_ih(2 * i - 2, 2 * k)] + q[Idx_ih(0, 1)] * p[Idx_ih(2 * i - 1, 2 * k)];
						q[Idx_ih(2 * i - 1, 2 * k + 1)] = q[Idx_ih(1, 1)] * q[Idx_ih(2 * i - 2, 2 * k)] + p[Idx_ih(0, 1)] * q[Idx_ih(2 * i - 1, 2 * k)];
						p[Idx_ih(2 * i, 2 * k + 1)] = q[Idx_ih(1, 1)] * p[Idx_ih(2 * i - 1, 2 * k)] + p[Idx_ih(0, 1)] * p[Idx_ih(2 * i, 2 * k)];
						q[Idx_ih(2 * i, 2 * k + 1)] = p[Idx_ih(1, 1)] * q[Idx_ih(2 * i - 1, 2 * k)] + q[Idx_ih(0, 1)] * q[Idx_ih(2 * i, 2 * k)];
					}
				}
			}
			////calculate G_e^h, G_h^h, G_h^e, G_e^e///////////////////////////////
			cpp_dec_float_100 a, b, c;
			/*
			i=0,1,...,kmax
			G_e^h (i,∆_t)=∑_(k=i)^(k_max)〖C_e^h (k,i,∆_t ) 〗
			G_h^h (i,∆_t)=∑_(k=i)^(k_max)〖C_h^h (k,i,∆_t ) 〗
			G_h^e (i,∆_t)=∑_(k=i)^(k_max)〖C_h^e (k,i,∆_t ) 〗
			G_e^e (i,∆_t)=∑_(k=i)^(k_max)〖C_e^e (k,i,∆_t ) 〗
			//
			sum k=i,i+1,...,kmax
			start from (∆_t^(2i+1))/(2i+1)! go up
			-------------------------------------
			C_e^h (k,i,∆_t )= ((∆_t^(2k+1))/(2k+1)! q_0^{2k+1}                                        ,i=k
							  (∆_t^2k)/(2k)! q_(2(k-i)-1)^{2k} + (∆_t^(2k+1))/(2k+1)! q_2(k-i)^{2k+1} ,0≤i<k
			//
			C_h^e (k,i,∆_t )= ((∆_t^(2k+1))/(2k+1)! p_0^{2k+1}                                        ,i=k
							  (∆_t^2k)/(2k)! p_(2(k-i)-1)^{2k} + (∆_t^(2k+1))/(2k+1)! p_2(k-i)^{2k+1} ,0≤i<k
			====================
			C_h^h (k,i,∆_t )=(∆_t^2k)/(2k)! q_2(k-i)^{2k} +(∆_t^(2k+1))/(2k+1)! q_(2(k-i)+1)^{2k+1} 
			C_e^e (k,i,∆_t )=(∆_t^2k)/(2k)! p_2(k-i)^{2k} +(∆_t^(2k+1))/(2k+1)! p_(2(k-i)+1)^{2k+1}
			--------------------------------
			*/
			for (unsigned int i = 0; i <= pams->kmax; i++)
			{
				G_ehP[i] = 0.0; G_hhP[i] = 0.0; G_eeP[i] = 0.0; G_heP[i] = 0.0;
				for (unsigned int k = i; k <= pams->kmax; k++)
				{
					a = q[Idx_ih(2 * (k - i), 2 * k)];
					b = p[Idx_ih(2 * (k - i), 2 * k)];
					G_hhP[i] += dtR[2*k] * a;
					G_eeP[i] += dtR[2*k] * b;
					//
					a = q[Idx_ih(2 * (k - i)+1, 2 * k+1)];
					b = p[Idx_ih(2 * (k - i)+1, 2 * k+1)];
					G_hhP[i] += dtR[2 * k+1] * a;
					G_eeP[i] += dtR[2 * k+1] * b;
					//
					if (k == i)
					{
						a = q[Idx_ih(0, 2 * k + 1)];
						b = p[Idx_ih(0, 2 * k + 1)];
						G_ehP[i] += dtR[2 * k + 1] * a;
						G_heP[i] += dtR[2 * k + 1] * b;
					}
					else
					{
						a = q[Idx_ih(2 * (k - i) - 1, 2 * k)];
						b = p[Idx_ih(2 * (k - i) - 1, 2 * k)];
						G_ehP[i] += dtR[2 * k] * a;
						G_heP[i] += dtR[2 * k] * b;
						//
						a = q[Idx_ih(2 * (k - i), 2 * k + 1)];
						b = p[Idx_ih(2 * (k - i), 2 * k + 1)];
						G_ehP[i] += dtR[2 * k+1] * a;
						G_heP[i] += dtR[2 * k+1] * b;
					}
				}
			}
			/*
			G_h^h (i,∆_t ) ∇ ̅^{2i} ×H+G_e^h (i,∆_t ) ∇ ̅^{2i+1} ×E
			G_e^e (i,∆_t ) ∇ ̅^{2i} ×E+G_h^e (i,∆_t ) ∇ ̅^{2i+1} ×H
			*/
			for (unsigned int i = 0; i <= pams->kmax; i++)
			{
				G_ehP[i] = G_ehP[i] / dsR[2 * i + 1];
				G_heP[i] = G_heP[i] / dsR[2 * i + 1];
				G_hhP[i] = G_hhP[i] / dsR[2 * i];
				G_eeP[i] = G_eeP[i] / dsR[2 * i];
			}
			for (unsigned int i = 0; i <= pams->kmax; i++)
			{
				GehP[i] = G_ehP[i].convert_to<double>();
				GheP[i] = G_heP[i].convert_to<double>();
				GhhP[i] = G_hhP[i].convert_to<double>();
				GeeP[i] = G_eeP[i].convert_to<double>();
			}
			free(G_ehP); free(G_heP); free(G_hhP); free(G_eeP);
			if (source != NULL)
			{
				if (ret == ERR_OK)
				{
					unsigned int emMax = source->GetemMax(); //2 * kmax + 3;
					size_t emMax2 = emMax*emMax;
					cpp_dec_float_100 *e = (cpp_dec_float_100 *)malloc(emMax2*sizeof(cpp_dec_float_100));
					cpp_dec_float_100 *m = (cpp_dec_float_100 *)malloc(emMax2*sizeof(cpp_dec_float_100));
					size_t t,t1;
					for (t = 0; t < emMax2; t++)
					{
						e[t] = 0.0; m[t] = 0.0;
					}
					/*
					Eq. (16), (17)
					e_0^{0} =-1/ε
					m_0^{0} =-1/μ
					*/
					e[I_ih(0, 0)] = -1.0 / pams->eps;
					m[I_ih(0, 0)] = -1.0 / pams->mu;
					/*
					Eq. (18)
					e_2(k-i)^{2i} =p_(2(k-i))^{2k}  e_0^{0} ,
					m_2(k-i)^{2i} =q_(2(k-i))^{2k}  m_0^{0} ,
					i=0,1,2,…,k, k>0					
					*/
					for (unsigned int k = 1; k <= pams->kmax + 1; k++)
					{
						for (unsigned int i = 0; i <= k; i++)
						{
							//0...2kmax+2
							t = I_ih(2 * (k - i), 2 * i); //[2kmax+2, (2kmax), ..., 0], {0, 2, ..., 2kmax+2}
							e[t] = p[Idx_ih(2 * (k - i), 2 * k)] * e[I_ih(0, 0)];
							m[t] = q[Idx_ih(2 * (k - i), 2 * k)] * m[I_ih(0, 0)];
						}
					}
					/*
					Eq. (19)
					m_(2(k-i)+1)^{2i-1} =p_(2(k-i)+1)^{2k}  m_0^{0} ,
					e_(2(k-i)+1)^{2i-1} =q_(2(k-i)+1)^{2k}  e_0^{0} ,
					i=1,2,…,k,k>0
					*/
					for (unsigned int k = 1; k <= pams->kmax + 1; k++)
					{
						for (unsigned int i = 1; i <= k; i++)
						{
							t = I_ih(2 * (k - i) + 1, 2 * i - 1);//[2kmax+1,2kmax-1,...,1], {1,3,...,2kmax+1}
							m[t] = p[Idx_ih(2 * (k - i) + 1, 2 * k)] * m[I_ih(0, 0)];
							e[t] = q[Idx_ih(2 * (k - i) + 1, 2 * k)] * e[I_ih(0, 0)];
						}
					}
					/*
					Eq. (26)
					e_(2(k-i)-1)^{2i} =p_(2(k-i)-1)^{2k-1}  e_0^{0} ,
					m_(2(k-i)-1)^{2i} =q_(2(k-i)-1)^{2k-1}  m_0^{0} ,
					i=0,1,2,…,k-1, k>0
					Eq. (27)
					m_(2(k-i-1))^{2i+1} =p_(2(k-i-1))^{2k-1}  m_0^{0} ,
					e_(2(k-i-1))^{2i+1} =q_(2(k-i-1))^{2k-1}  e_0^{0} ,
					i=0,1,2,…,k-1, k>0
					*/
					for (unsigned int k = 1; k <= pams->kmax + 1; k++)
					{
						for (unsigned int i = 0; i < k; i++)
						{
							t = I_ih(2 * (k - i) - 1, 2 * i);//[2kmax+1,2kmax-1,...,1],{0,2,...,2kmax+2}
							t1 = Idx_ih(2 * (k - i) - 1, 2 * k - 1);
							e[t] = p[t1] * e[I_ih(0, 0)];
							m[t] = q[t1] * m[I_ih(0, 0)];
							t = I_ih(2 * (k - i - 1), 2 * i + 1);//[2kmax,2kmax-2,...,0],{1,2,...,2kmax+1}
							t1 = Idx_ih(2 * (k - i - 1), 2 * k - 1);
							m[t] = p[t1] * m[I_ih(0, 0)];
							e[t] = q[t1] * e[I_ih(0, 0)];
						}
					}
					ret = source->onInitialized(e, m);
					free(e); free(m);
				}
			}
		}
		if (p != NULL) { free(p); p = NULL; }
		if (q != NULL) { free(q); q = NULL; }
	}
	if (ret == ERR_OK)
	{
		startThreads();
		ret = pml.initialize(pams);
	}
	if (ret == ERR_OK)
	{
		puts("\r\nFinished calculating update coefficients.");
	}
	return ret;
}
int TimeTssBase::initFields(FieldsSetter* f0)
{
	int ret = ERR_OK;
	puts("\r\nAllocating field memories...");
	//
	cellCount = (pams->nx + 1)*(pams->ny + 1)*(pams->nz + 1);
	fieldMemorySize = cellCount*sizeof(Point3Dstruct);
	//all field memories used
	if (H != NULL) 
	{
		FreeMemory(H); H = NULL;
	}
	if (E != NULL)
	{
		FreeMemory(E); E = NULL;
	}
	if (curlH != NULL)
	{
		FreeMemory(curlH); curlH = NULL;
	}
	if (curlE != NULL)
	{
		FreeMemory(curlE); curlE = NULL;
	}
	H = (Point3Dstruct *)AllocateMemory(fieldMemorySize);
	if (H == NULL)
	{
		ret = MEMRETLAST;
		if (ret == ERR_OK)
		{
			ret = ERR_OUTOFMEMORY;
		}
	}
	if (ret == ERR_OK)
	{
		E = (Point3Dstruct *)AllocateMemory(fieldMemorySize);
		if (E == NULL)
		{
			ret = MEMRETLAST;
			if (ret == ERR_OK)
			{
				ret = ERR_OUTOFMEMORY;
			}
		}
	}
	if (ret == ERR_OK)
	{
		curlH = (Point3Dstruct *)AllocateMemory(fieldMemorySize);
		if (curlH == NULL)
		{
			ret = MEMRETLAST;
			if (ret == ERR_OK)
			{
				ret = ERR_OUTOFMEMORY;
			}
		}
	}
	if (ret == ERR_OK)
	{
		curlE = (Point3Dstruct *)AllocateMemory(fieldMemorySize);
		if (curlE == NULL)
		{
			ret = MEMRETLAST;
			if (ret == ERR_OK)
			{
				ret = ERR_OUTOFMEMORY;
			}
		}
	}
	if (ret == ERR_OK)
	{
		size_t w = 0;
		double x = pams->xmin, y, z;
		puts("\r\nInitializing fields...");
		for (unsigned int i = 0; i <= pams->nx; i++)
		{
			y = pams->ymin;
			for (unsigned int j = 0; j <= pams->ny; j++)
			{
				z = pams->zmin;
				for (unsigned int k = 0; k <= pams->nz; k++)
				{
					if (f0 != NULL)
					{
						f0->SetFields(x, y, z, &(E[w]), &(H[w]));
					}
					else
					{
						E[w].x = 0.0;
						E[w].y = 0.0;
						E[w].z = 0.0;
						H[w].x = 0.0;
						H[w].y = 0.0;
						H[w].z = 0.0;
					}
					z += pams->ds;
					w++;
				}
				y += pams->ds;
			}
			x += pams->ds;
		}
	}
	return ret;
}

int TimeTssBase::writeCoefficientsToFile(char *file, unsigned int maxTimeSteps, double courantFactor, unsigned int dataFileSteps)
{
	int ret = ERR_OK;
	FILE *retFileHandle = 0;
	if (file != NULL)
	{
		ret = deleteFile(file);
		if (ret == ERR_OK)
		{
			ret = openTextfileWrite(file, &retFileHandle);
		}
	}
	else
	{
		ret = ERR_FILENAME_MISSING;
	}
	if (retFileHandle != 0)
	{
		unsigned int size = 1024;
		char buff[1024];
		sprintf_1(buff, size, "kmax=%u, time estimation order = 2(kmax+1)=%u \nds=%g, dt=%g, courant factor=%g, total time=%g, data file interval=%g\n", pams->kmax, 2 * (pams->kmax + 1), pams->ds, pams->dt, courantFactor, pams->dt*maxTimeSteps, pams->dt*dataFileSteps);
		writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
		sprintf_1(buff, size, "material parameters: eps=%g, mu=%g, sigma_e=%g, sigma_h=%g\r\n", pams->eps, pams->mu, pams->sie, pams->sim);
		writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
		////////////H/////////////////////////////////////////////
		sprintf_1(buff, size, "H=C.F.dJm+C.G.dJe+");
		writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
		//Curl X H -> H
		for (unsigned int k = 0; k <= pams->kmax; k++)
		{
			if (k == 0)
			{
				if (k == pams->kmax)
				{
					sprintf_1(buff, size, "[%g]C2xH+", GhhP[k]);
				}
				else
				{
					sprintf_1(buff, size, "[%g,", GhhP[k]);
				}
			}
			else
			{
				if (k == pams->kmax)
				{
					sprintf_1(buff, size, " %g]C2xH+", GhhP[k]);
				}
				else
				{
					sprintf_1(buff, size, " %g,", GhhP[k]);
				}
			}
			writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
		}
		//CE->H
		for (unsigned int k = 0; k <= pams->kmax; k++)
		{
			if (k == 0)
			{
				if (k == pams->kmax)
				{
					sprintf_1(buff, size, "[%g]C1xE\r\n", GehP[k]);
				}
				else
				{
					sprintf_1(buff, size, "[%g,", GehP[k]);
				}
			}
			else
			{
				if (k == pams->kmax)
				{
					sprintf_1(buff, size, " %g]C1xE\n", GehP[k]);
				}
				else
				{
					sprintf_1(buff, size, " %g,", GehP[k]);
				}
			}
			writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
		}
		///////////////E///////////
		sprintf_1(buff, size, "\nE=C.U.dJe+C.W.dJm+");
		writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
		//Curl X E -> E
		for (unsigned int k = 0; k <= pams->kmax; k++)
		{
			if (k == 0)
			{
				if (k == pams->kmax)
				{
					sprintf_1(buff, size, "[%g]C2xE+", GeeP[k]);
				}
				else
				{
					sprintf_1(buff, size, "[%g,", GeeP[k]);
				}
			}
			else
			{
				if (k == pams->kmax)
				{
					sprintf_1(buff, size, " %g]C2xE+", GeeP[k]);
				}
				else
				{
					sprintf_1(buff, size, " %g,", GeeP[k]);
				}
			}
			writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
		}
		//CH->E
		for (unsigned int k = 0; k <= pams->kmax; k++)
		{
			if (k == 0)
			{
				if (k == pams->kmax)
				{
					sprintf_1(buff, size, "[%g]C1xH\n", GheP[k]);
				}
				else
				{
					sprintf_1(buff, size, "[%g,", GheP[k]);
				}
			}
			else
			{
				if (k == pams->kmax)
				{
					sprintf_1(buff, size, " %g]C1xH\n", GheP[k]);
				}
				else
				{
					sprintf_1(buff, size, " %g,", GheP[k]);
				}
			}
			writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
		}
		if (source != NULL)
		{
			double *f = source->GetJmH();
			double *g = source->GetJeH();
			double *u = source->GetJeE();
			double *w = source->GetJmE();
			unsigned int srcDim = source->GetsrcDim();
			unsigned int t = 0;
			//SH=============================================
			sprintf_1(buff, size, "\nF[ %u x %u ]=\n", srcDim, srcDim);
			writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
			t = 0;
			for (unsigned int i = 0; i < srcDim; i++)
			{
				for (unsigned int j = 0; j < srcDim; j++)
				{
					if (j == srcDim - 1)
					{
						sprintf_1(buff, size, " %g\n", f[t]);
					}
					else
					{
						sprintf_1(buff, size, " %g,\t", f[t]);
					}
					writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
					t++;
				}
			}
			sprintf_1(buff, size, " \nG[ %u x %u ]=\n", srcDim, srcDim);
			writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
			t = 0;
			for (unsigned int i = 0; i < srcDim; i++)
			{
				for (unsigned int j = 0; j < srcDim; j++)
				{
					if (j == srcDim - 1)
					{
						sprintf_1(buff, size, " %g\n", g[t]);
					}
					else
					{
						sprintf_1(buff, size, " %g,\t", g[t]);
					}
					writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
					t++;
				}
			}
			//SE=============================================
			sprintf_1(buff, size, "\nU[ %u x %u ]=\n", srcDim, srcDim);
			writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
			t = 0;
			for (unsigned int i = 0; i < srcDim; i++)
			{
				for (unsigned int j = 0; j < srcDim; j++)
				{
					if (j == srcDim - 1)
					{
						sprintf_1(buff, size, " %g\n", u[t]);
					}
					else
					{
						sprintf_1(buff, size, " %g,\t", u[t]);
					}
					writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
					t++;
				}
			}
			sprintf_1(buff, size, "\nW[ %u x %u ]=\n", srcDim, srcDim);
			writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
			t = 0;
			for (unsigned int i = 0; i < srcDim; i++)
			{
				for (unsigned int j = 0; j < srcDim; j++)
				{
					if (j == srcDim - 1)
					{
						sprintf_1(buff, size, " %g\n", w[t]);
					}
					else
					{
						sprintf_1(buff, size, " %g,\t", w[t]);
					}
					writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
					t++;
				}
			}
		}//source
		if (pml.usePML())
		{
			pml.writeCoefficientsToFile(retFileHandle);
		}
	}
	if (retFileHandle != 0)
	{
		closefile(retFileHandle);
	}
	return ret;
}

int TimeTssBase::saveFieldToFile(char *filename, FIELD_EMTYPE fieldToSave)
{
	int ret = ERR_OK;
	size_t cc = spacePointCount();
	FILE *fp = NULL;
	ret = openfileWrite(filename, &fp);
	if (ret != ERR_OK || fp == NULL)
	{
		if (ret == ERR_OK)
		ret = ERR_FILE_OPEN_WRIT_EACCES;
	}
	else
	{
		size_t sizew; 
		if (fieldToSave == Field_E)
		{
			sizew = fwrite(getRawMemoryE(), sizeof(Point3Dstruct), cc, fp);
		}
		else
		{
			sizew = fwrite(getRawMemoryH(), sizeof(Point3Dstruct), cc, fp);
		}
		fclose(fp);
		if (sizew != cc)
		{
			ret = ERR_FILE_WRITE_LESS;
		}
	}
	return ret;
}


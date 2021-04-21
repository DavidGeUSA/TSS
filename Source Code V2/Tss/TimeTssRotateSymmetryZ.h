#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 04/02/2021
Allrights reserved by David Ge

time advancement estimation module using rotation symmetry around z-axis
********************************************************************/
#include "TimeTssBase.h"
#include "LoopWorkDivider.h"
#include "RotateSymmetryField.h"
#include "CurlOneSymmetryZ.h"

class ThreadPool;

class TimeTssRotateSymmetryZ : virtual public TimeTssBase
{
private:
	unsigned int ic;            //center index
	RotateSymmetryField vE;     //replace E
	RotateSymmetryField vH;     //replace H
	RotateSymmetryField *curlZE; //replace curlE
	RotateSymmetryField *curlZH; //replace curlH
	//work variables
	Point3Dstruct v0;
	Point3Dstruct *v1;
	Point3Dstruct *v2;
	//
	ThreadPool *pool;
	int retH;
	int retE;
	unsigned int h;
	//work fields, different threads work on different parts
	//curlZE points to work0E and work1E alternatively
	//curlZH points to work0H and work1H alternatively
	RotateSymmetryField *wk,*pNextE,*pNextH;
	RotateSymmetryField work0E;
	RotateSymmetryField work0H;
	RotateSymmetryField work1E;
	RotateSymmetryField work1H;
	//arrays for threads, array size is workCount
	CurlOneSymmetryZ **curl_OneE;
	CurlOneSymmetryZ **curl_OneH;
	LoopWorkDividerSymmetryZ divider;
	unsigned int workCount;
	unsigned int workCountN1; //workCount-1
protected:
	virtual void cleanup();
	virtual void onSettingSimParams();
public:
	TimeTssRotateSymmetryZ();
	virtual ~TimeTssRotateSymmetryZ();
	virtual FIELD_TYPE FieldType(){ return Field_type_z_rotateSymmetry; }
	//
	virtual void startThreads();
	virtual void removeThreads();
	
	virtual int initFields(FieldsSetter* f0);
	virtual int GetFirstCurls();
	virtual int GetNextCurls();
	virtual int AdvanceOneStep();
	//
	virtual Point3Dstruct *getRawMemoryE(){ return vE.getRawMemory(); }
	virtual Point3Dstruct *getRawMemoryH(){ return vH.getRawMemory(); }
	//
	virtual RotateSymmetryField *GetFieldZrotateSymmetryH(){ return &vH; }
	virtual RotateSymmetryField *GetFieldZrotateSymmetryE(){ return &vE; }
	virtual DivergenceStatistic *CreateDefaultStatisticsMaker(){ DivergenceStatisticZRotateSymmetry *g = new DivergenceStatisticZRotateSymmetry(); return dynamic_cast<DivergenceStatisticZRotateSymmetry*>(g); }

	//////////////////////////////////////////////////////////////////////
	void apply1stCurlE(unsigned int th)
	{
		unsigned int i, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wE = divider.StartIndexOneDim(th);
		unsigned int k0 = 0;
		unsigned int k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wE = (pams->nz + 1) * i0;
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			k0 = pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		//
		for (i = i0; i <= i1; i++)
		{
			for (k = 0; k <= pams->nz; k++)
			{
				if (k >= k0 && k <= k1)
				{
					v1 = vE.getFieldOnPlane(wE);
					v2 = curlZH->getFieldOnPlane(wE);
					v0.x = GeeP[h] * v1->x + GheP[h] * v2->x;
					v0.y = GeeP[h] * v1->y + GheP[h] * v2->y;
					v0.z = GeeP[h] * v1->z + GheP[h] * v2->z;
					vE.setFieldOnPlane(wE, &v0);
				}
				wE++;
			}
		}
#ifdef TIMESTEPPROFILING
		endTE = GetTimeTick();
#endif
	}
	void apply1stCurlH(unsigned int th)
	{
#ifdef TIMESTEPPROFILING
		startTH = GetTimeTick();
#endif
		unsigned int i, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wH = divider.StartIndexOneDim(th);
		unsigned int k0 = 0;
		unsigned int k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wH = (pams->nz + 1) * i0;
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			k0 = pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		for (i = i0; i <= i1; i++)
		{
			for (k = 0; k <= pams->nz; k++)
			{
				if (k >= k0 && k <= k1)
				{
					v1 = vH.getFieldOnPlane(wH);
					v2 = curlZE->getFieldOnPlane(wH);
					v0.x = GhhP[h] * v1->x + GehP[h] * v2->x;
					v0.y = GhhP[h] * v1->y + GehP[h] * v2->y;
					v0.z = GhhP[h] * v1->z + GehP[h] * v2->z;
					vH.setFieldOnPlane(wH, &v0);
				}
				wH++;
			}
		}
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
#endif
	}
	void apply2ndCurlE(unsigned int th)
	{
#ifdef TIMESTEPPROFILING
		startTE = GetTimeTick();
#endif
		unsigned int i, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wE = divider.StartIndexOneDim(th);
		unsigned int k0 = 0;
		unsigned int k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wE = (pams->nz + 1) * i0;
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			k0 = pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		for (i = i0; i <= i1; i++)
		{
			for (k = 0; k <= pams->nz; k++)
			{
				if (k >= k0 && k <= k1)
				{
					v1 = vE.getFieldOnPlane(wE);
					v2 = curlZE->getFieldOnPlane(wE);
					v0.x = v1->x + GeeP[h] * v2->x;
					v0.y = v1->y + GeeP[h] * v2->y;
					v0.z = v1->z + GeeP[h] * v2->z;
					vE.setFieldOnPlane(wE, &v0);
				}
				//
				wE++;
			}
		}
#ifdef TIMESTEPPROFILING
		endTE = GetTimeTick();
#endif
	}
	void apply2ndCurlH(unsigned int th)
	{
#ifdef TIMESTEPPROFILING
		startTH = GetTimeTick();
#endif
		unsigned int i, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wH = divider.StartIndexOneDim(th);
		unsigned int k0 = 0;
		unsigned int k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wH = (pams->nz + 1) * i0;
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			k0 = pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		for (i = i0; i <= i1; i++)
		{
			for (k = 0; k <= pams->nz; k++)
			{
				if (k >= k0 && k <= k1)
				{
					v1 = vH.getFieldOnPlane(wH);
					v2 = curlZH->getFieldOnPlane(wH);
					v0.x = v1->x + GhhP[h] * v2->x;
					v0.y = v1->y + GhhP[h] * v2->y;
					v0.z = v1->z + GhhP[h] * v2->z;
					vH.setFieldOnPlane(wH, &v0);
				}
				wH++;
			}
		}
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
#endif
	}
	void apply3rdCurlE(unsigned int th)
	{
#ifdef TIMESTEPPROFILING
		startTE = GetTimeTick();
#endif
		unsigned int i, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wE = divider.StartIndexOneDim(th);
		unsigned int k0 = 0;
		unsigned int k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wE = (pams->nz + 1) * i0;
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			k0 = pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		for (i = i0; i <= i1; i++)
		{
			for (k = 0; k <= pams->nz; k++)
			{
				if (k >= k0 && k <= k1)
				{
					v1 = vE.getFieldOnPlane(wE);
					v2 = curlZH->getFieldOnPlane(wE);
					v0.x = v1->x + GheP[h] * v2->x;
					v0.y = v1->y + GheP[h] * v2->y;
					v0.z = v1->z + GheP[h] * v2->z;
					vE.setFieldOnPlane(wE, &v0);
				}
				//
				wE++;
			}
		}
#ifdef TIMESTEPPROFILING
		endTE = GetTimeTick();
#endif
	}
	void apply3rdCurlH(unsigned int th)
	{
#ifdef TIMESTEPPROFILING
		startTH = GetTimeTick();
#endif
		unsigned int i, k;
		unsigned int i0 = divider.StartIndex(th);
		unsigned int i1 = divider.EndIndex(th);
		size_t wH = divider.StartIndexOneDim(th);
		unsigned int k0 = 0;
		unsigned int k1 = pams->nz;
		//adjust i0,i1 and wE if PML is used
		if (pml.usePML())
		{
			if (i0 < pams->pml.Ln)
			{
				i0 = pams->pml.Ln;
				wH = (pams->nz + 1) * i0;
			}
			if (i1 > pams->nx - pams->pml.Ln)
			{
				i1 = pams->nx - pams->pml.Ln;
			}
			k0 = pams->pml.Ln;
			k1 = pams->nz - pams->pml.Ln;
		}
		for (i = i0; i <= i1; i++)
		{
			for (k = 0; k <= pams->nz; k++)
			{
				//
				if (k >= k0 && k <= k1)
				{
					v1 = vH.getFieldOnPlane(wH);
					v2 = curlZE->getFieldOnPlane(wH);
					v0.x = v1->x + GehP[h] * v2->x;
					v0.y = v1->y + GehP[h] * v2->y;
					v0.z = v1->z + GehP[h] * v2->z;
					vH.setFieldOnPlane(wH, &v0);
				}
				wH++;
			}
		}
#ifdef TIMESTEPPROFILING
		endTH = GetTimeTick();
#endif
	}

};


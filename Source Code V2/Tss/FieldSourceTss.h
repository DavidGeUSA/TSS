#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

********************************************************************/
#include "Tss.h"
//#include "TimeTssBase.h"
#include "RotateSymmetryField.h"
#include "../EMField/Plugin.h"
#include "../FileUtil/taskFile.h"

#define I_ih(i, h) ((h)*emMax+(i))
#define Sr_ih(i, h) ((h)*srcDim+(i))
#define IDX(i,j,k) ((k) + nz1 * ((j) + ny1 * (i)))

class TimeTssBase;

/*
	form field source
*/
class FieldSourceTss:virtual public Plugin
{
private:
	bool _initialized;
protected:
	SimStruct *pams;
	unsigned int nx1, ny1, nz1;  //nx1=nx+1; ny1=ny+1; nz1=nz+1
	//
	//parameters
	/*
	e[h=0..2k_max+2,i=0..2k_max+2 ]=e_i^{h}
	m[h=0..2k_max+2,i=0..2k_max+2 ]=m_i^{h}
	sr[i=0..2k_max+2]=(∆_t^i)/i!
	*/
	unsigned int emMax;  //2kmax+3
	unsigned int srcDim; //2kmax+1
	//
	double *f0, *g0;      //source for H: (2kmax+1)X(2kmax+1)
	double *w0, *u0;      //source for E: (2kmax+1)X(2kmax+1)
	//
	void setInitialized(){ _initialized = true; }
public:
	FieldSourceTss();
	~FieldSourceTss();
	void cleanup();
	bool Initialized(){ return _initialized; }
	//give derived classes a chance to read more configurations
	virtual int initialize(TaskFile *configs){ return 0; }
	//allocate memories
	virtual int initialize(SimStruct *params);
	//calculate coefficients based on e0 and m0
	virtual int onInitialized(void *e0, void *m0);
	//for testing
	unsigned int GetemMax(){ return emMax; }
	unsigned int GetsrcDim(){ return srcDim; }
	double *GetJmH(){ return f0; }
	double *GetJeH(){ return g0; }
	double *GetJeE(){ return u0; }
	double *GetJmE(){ return w0; }
	//
	virtual bool isInSource(unsigned int i, unsigned int j, unsigned int k){ return true; }
	//major function
	virtual int applySources(double t,size_t tIndex, Point3Dstruct *efile, Point3Dstruct *hfile){ return 0; }
	virtual int applyToZrotateSymmetry(double t, size_t tIndex, RotateSymmetryField *efile, RotateSymmetryField *hfile){ return 0; }
	int applySourceToFields(TimeTssBase *timeModule);
};


#ifndef __TSSFDTD_H__
#define __TSSFDTD_H__

/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/

#include "../EMField/EMField.h"
#include "../TssInSphere/TssInSphere.h"
#include "../TssInSphere/TssInhomogeneous.h"
#include "../TssInSphere/FourthSpaceOrderTss.h"

class TssFDTD:virtual public TssInSphere
{
public:
	TssFDTD(void);
	~TssFDTD(void);
	virtual int initialize(TaskFile *configs){ return 0; }
};
///////////////////////////////////////////////////////////////
/*
	a sample implementation of applying TSS algorithm to inhomogeneous environments
*/
class TssFDTDinhomo:virtual public TssInhomogeneous
{
protected:
	/*
		a subclass override this function to assign values to space-location-dependent Permeability and Permittivity,
		that is, set values for mu[i] and eps[i]
	*/
	virtual int initializeInhomogeneous();
	/*
		it is called after onInitInhomogeneous. default is doing nothing.
		for this sample, free memories mu and eps because they are no longer needed
	*/
	virtual int onPreparedInhomogeneous();
public:
	TssFDTDinhomo(void);
	~TssFDTDinhomo(void);
	virtual int initialize(TaskFile *configs){ return 0; }
};

/*
	this is a class to assign values to space-location-dependent Permeability and Permittivity
*/
class InitInhomogeneousParameters: public GoThroughSphereByIndexes
{
protected:
	double c0;
	double mu0;
	double eps0;
	double *_mu;
	double *_eps;
public:
	InitInhomogeneousParameters(double *mu, double *eps);
	/*
		(m,n,p) indicates a space location. use this information to determine values for Permeability and Permittivity
	*/
	virtual void handleData(int m, int n, int p);
};


#endif
#pragma once
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "TssInSphere.h"


/*
	an abstract class to apply TSS algorithm in inhomogeneous environments
*/
class TssInhomogeneous: public TssInSphere
{
protected:
	//initialization should allocate memory and initialize space-related values of these two arrays
	double *mu;  //Permeability at each space point, in radius indexing
	double *eps; //Permittivity at each space point, in radius indexing
	double *dtmu;  //-dt/(ds*mu0) = - 1 / (c0 * sqrt(3) * mu0)
	double *dteps; // dt/(ds*eps0) = 1 / (c0 * sqrt(3) * eps0)
	//
	//factors to update fields, they are space-location dependent
	double *ae_a;
	double *ah_a;
	double *ae0_a;
	double *ah0_a; 
	//
	/*
		a subclass override this function to assign values to space-location-dependent Permeability and Permittivity,
		that is, set values for mu[i] and eps[i]
	*/
	virtual int initializeInhomogeneous()=0;
	/*
		prepare dtmu and dteps
	*/
	virtual int onInitInhomogeneous();
	/*
		create curl-estimators of desired classes
	*/
	virtual void createCurlGenerators();
	/*
		it is called after onInitInhomogeneous. default is doing nothing
	*/
	virtual int onPreparedInhomogeneous(){return ERR_OK;}
	//
	/*
		free memories
	*/
	virtual void cleanup();
	/*
		1. initialize memories for space-location-dependent Permeability, Permittivity, and coefficients
		2. call initializeInhomogeneous to assigned values to space-location-dependent Permeability and Permittivity 
		3. call onInitInhomogeneous
	*/
	virtual int onInitialized(TaskFile *taskParameters);
	/*
		use curls to estimate time-advancement in a space-location-dependent manner
	*/
	virtual int applyCurls(int k);
	//
public:
	TssInhomogeneous(void);
	~TssInhomogeneous(void);
};

//

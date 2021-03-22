#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 10/13/2020
Allrights reserved by David Ge

optimize 4-th order space curl estimation calculation.
for a 3D space point, the calculations are reduced to 60 additions.
multiplication operations are absorbed into constants.
********************************************************************/
#include "../EMField/FDTD.h"

/*
estimate curls using 4-th order space estimation
*/
class FourthOrderCurlEstimator :public virtual GoThroughSphereByIndexes, public virtual RadiusIndexCacheUser
{
private:
	FieldPoint3D *_fields; //fields for calculating curls of it
	FieldPoint3D *_curls;  //curls of _fields
	int forElectric;
protected:
	int ret;
	size_t idx, idx2;
public:
	FourthOrderCurlEstimator();
	void setParams(int isForElectric, int mR){ forElectric = isForElectric; maxRadius = mR; }
	void SetFields(FieldPoint3D *fields, FieldPoint3D *curls);
	//
	virtual RadiusHandleType setRadius(int radius){ r = radius; return NeedProcess; }
	virtual void handleData(int m, int n, int p);
};

/*
	4-th order TSS
*/
class FourthSpaceOrderTss :
	virtual public FDTD
{
private :
	double mu0;  //Permeability
	double eps0; //Permittivity
	//
	double *e_even=NULL, *e_odd=NULL, *h_even=NULL, *h_odd=NULL; //time advance coefficients for curls. each array if of size _maxOrderTimeAdvance+1
											 //e_even[0]=1; h_even[0]=1
	//e_even (k)=((-1)^k c_r^2k)/(2k)!  1/〖12〗^2k ,e_odd (k)=√(μ/ε)  ((-1)^k c_r^(2k+1))/(2k+1)!  1/〖12〗^(2k+1) 
	//h_even (k)=((-1)^k c_r^2k)/(2k)!  1/〖12〗^2k ,h_odd (k)=-√(ε/μ)   ((-1)^k c_r^(2k+1))/(2k+1)!  1/〖12〗^((2k+1) ) 
	//where c_r is the Courant number, variable name is courant

	//memory holding 12ds*Curl
	FieldPoint3D *curlEven, *curlOdd;
	FourthOrderCurlEstimator curlEstimator;
protected:
	virtual void cleanup();
	virtual int onInitialized(TaskFile *taskParameters); //called after initialize(...) returns ERR_OK
	//
	int ds12Curl();
public:
	FourthSpaceOrderTss();
	~FourthSpaceOrderTss();
	virtual int initialize(TaskFile *configs){ return 0; }
	virtual int updateFieldsToMoveForward();
	virtual void OnFinishSimulation();
};


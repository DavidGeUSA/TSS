#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 10/16/2020
Allrights reserved by David Ge

********************************************************************/
#include "..\EMField\EMField.h"
#include "..\EMField\RadiusIndex.h"
#include "ArbitrarySpaceOrder.h"

/*
	base class for calculating time advancement coefficients for curls
	derived classes: TimeOrderYee, TimeOrderTss
*/
class ArbitraryTimeOrder
{
protected:
	//constants
	double c0;   //light speed
	double mu0;  //Permeability
	double eps0; //Permittivity
	//
	unsigned int ht; //ht>0; half time advancement estimation order; temporal derivative estimation order = 2 * ht
	double *ae; //Electrical curl coefficients for temporal derivatives; array size = 2ht; 
	double *ah; //Magnetic curl coefficients for temporal derivatives; array size = 2ht;

	//
	void cleanup();
	virtual int allocateMemory(unsigned int halfOrder);

public:
	ArbitraryTimeOrder();
	~ArbitraryTimeOrder();
	unsigned int HalfOrder(){ return ht; }
	double *coefficientsE(){ return ae; }
	double *coefficientsH(){ return ah; }
	virtual int CalculateCoefficients(unsigned int halfOrder, double courant);
};

/*
	base class for time advancement
	derived classes: YeeTimeAdvanceHField, YeeTimeAdvanceEField, TssTimeAdvanceHField
*/
class ArbitraryOrderTimeAdvance :public virtual GoThroughSphereByIndexes, public virtual RadiusIndexCacheUser
{
protected:
	//---passed into here by an instance host----------
	ArbitraryTimeOrder *timeCoefficients;
	ArbitrarySpaceOrder *spaceCoefficients;
	FieldPoint3D *_field;
	FieldPoint3D *curlOdd;
	FieldPoint3D *curlEven;
	//-------------------------------------------------
	//
	FieldPoint3D *curl; //pointer only
	FieldPoint3D *field;//pointer only
	//
	//size_t *dxp, *dxn, *dyp, *dyn, *dzp, *dzn; //series indexes for calculating curls
	unsigned int hs; //half space order
	double *a; //space derivative coefficients
	size_t m1, n1, p1, m2, n2, p2;
	int maxRadius, maxRadiusNeg;
	//
	virtual RadiusHandleType setRadius(int radius) = 0;
	virtual void handleData(int m, int n, int p) = 0;
public:
	ArbitraryOrderTimeAdvance();
	~ArbitraryOrderTimeAdvance();
	void setMaxRadius(RadiusIndexToSeriesIndex *cache, int maxR, ArbitraryTimeOrder* i_timeCoefficients, ArbitrarySpaceOrder* i_spaceCoefficients);
	void reset(FieldPoint3D *field, FieldPoint3D *i_curlOdd, FieldPoint3D *i_curlEven);
	//
	virtual int moveForward() = 0;
};


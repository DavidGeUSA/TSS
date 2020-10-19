#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 10/15/2020
Allrights reserved by David Ge

********************************************************************/

/*
	base class for calculating space derivative estimation coefficients
	derived classes: SpaceOrderYee, SpaceOrderTss
*/
class ArbitrarySpaceOrder
{
protected:
	unsigned int hs; //hs>0; half space estimation order; space derivative estimation order = 2 * hs
	double *a; //coefficients for space derivative estimation; array size = hs; 

	double *A; //matrix for calculating the coefficients
	//
	void cleanup();
	virtual int allocateMemory(unsigned int halfOrder);
	virtual int fillMatrix() = 0;
public:
	ArbitrarySpaceOrder();
	~ArbitrarySpaceOrder();
	double *coefficients(){ return a; }
	unsigned int ArraySize(){ return hs; }
	int CalculateCoefficients(unsigned int halfOrder);
};


#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 10/15/2020
Allrights reserved by David Ge

********************************************************************/
#include "..\EMField\FDTD.h"
#include "..\ArbitraryOrder\ArbitrarySpaceOrder.h"

class SpaceOrderTss :virtual public ArbitrarySpaceOrder
{
protected:
	virtual int fillMatrix();
	SpaceOrderTss();
	~SpaceOrderTss();
};

class Tss :
	public FDTD
{
public:
	Tss();
	~Tss();
};


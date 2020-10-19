#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 10/15/2020
Allrights reserved by David Ge

********************************************************************/
#include "..\EMField\FDTD.h"
#include "..\ArbitraryOrder\ArbitrarySpaceOrder.h"
#include "..\ArbitraryOrder\ArbitraryTimeOrder.h"

class SpaceOrderYee:virtual public ArbitrarySpaceOrder
{
protected:
	virtual int fillMatrix();
public:
	SpaceOrderYee();
	~SpaceOrderYee();
};

class TimeOrderYee : virtual public ArbitraryTimeOrder
{
public:
	TimeOrderYee();
	~TimeOrderYee();
	virtual int CalculateCoefficients(unsigned int halfOrder, double courant);
};

/*
update magnetic field
*/
class YeeTimeAdvanceHField :public virtual ArbitraryOrderTimeAdvance
{
protected:
	virtual RadiusHandleType setRadius(int radius);
	virtual void handleData(int m, int n, int p);
public:
	virtual int moveForward();
};

/*
update electric field
*/
class YeeTimeAdvanceEField :public ArbitraryOrderTimeAdvance
{
protected:
	virtual RadiusHandleType setRadius(int radius);
	virtual void handleData(int m, int n, int p);
public:
	virtual int moveForward();
};


/*
	Arbitary even order FDTD algorithm applied to Yee Grid
*/
class HighYee :
	virtual public FDTD
{
private:
	double mu0;  //Permeability
	double eps0; //Permittivity
	TimeOrderYee timeCoefficients;
	SpaceOrderYee spaceCoefficients;
	YeeTimeAdvanceHField hfield;
	YeeTimeAdvanceEField efield;
	FieldPoint3D *curlOdd;
	FieldPoint3D *curlEven;
	//size_t *dxp, *dxn, *dyp, *dyn, *dzp, *dzn; //series indexes for calculating curls
protected:
	virtual void cleanup();
	virtual int onInitialized(TaskFile *taskParameters); //called after initialize(...) returns ERR_OK
	/*int createCurlIndex()*/;
public:
	HighYee();
	~HighYee();
	/*
	it populates the fields with space poisition shifts.
	it is used by YeeFDTDSpaceSynched to generate 6 YeeFDTD instances to get space-synchronized Yee estimations
	*/
	int setFieldValues(FieldsInitializer *fieldValues, double shiftX, double shiftY, double shiftZ);

	/*
	set field values.
	it is called by a simulator to initialize fields.
	if special initialization is needed then override this function
	*/
	virtual int PopulateFields(FieldsInitializer *fieldValues);

	virtual int updateFieldsToMoveForward();
	virtual void OnFinishSimulation();

};


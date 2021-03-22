#pragma once
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "../EMField/EMField.h"
#include "../EMField/FdtdMemory.h"
#include "../EMField/RadiusIndex.h"
#include "../EMField/FDTD.h"
#include "../ProcessMonitor/workProcess.h"
#include "FieldUpdator.h"

/*
	Yee's FDTD algorithm
*/
class YeeFDTD: public virtual FDTD
{
private:
	double mu0;  //Permeability
	double eps0; //Permittivity
	//
	double ch, ce; //ch = (dt/ds)/mu0, ce = (dt/ds)/eps0
	//
	UpdateHField updateH;
	UpdateEField updateE;
	//
protected:
	virtual void cleanup();
	virtual int onInitialized(TaskFile *taskParameters); //called after initialize(...) returns ERR_OK

public:
	YeeFDTD(void);
	virtual ~YeeFDTD(void);
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



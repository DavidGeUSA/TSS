#pragma once
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "YeeFDTD.h"

/*
	this class is for doing space-synchronized Yee simulation.
	it uses 6 YeeFDTD instances, each with certain space shifts.
	each YeeFDTD provides one field component which is at a sychronized location.
	together they provide all 6 field components at the synchronized location.
*/
class YeeFDTDSpaceSynched: public virtual FDTD
{
private:
	YeeFDTD *yeeEx; //shift Ex to (x,y,z)
	YeeFDTD *yeeEy; //shift Ey to (x,y,z)
	YeeFDTD *yeeEz; //shift Ez to (x,y,z)
	YeeFDTD *yeeHx; //shift Hx to (x,y,z)
	YeeFDTD *yeeHy; //shift Hy to (x,y,z)
	YeeFDTD *yeeHz; //shift Hz to (x,y,z)
	FieldPoint3D *fEx; //fields for yeeEx
	FieldPoint3D *fEy; //fields for yeeEy
	FieldPoint3D *fEz; //fields for yeeEz
	FieldPoint3D *fHx; //fields for yeeHx
	FieldPoint3D *fHy; //fields for yeeHy
	FieldPoint3D *fHz; //fields for yeeHz
	//
	double ds2; //space shift distance
	int ret;
protected:
	virtual void cleanup(void);
	virtual int onInitialized(TaskFile *taskParameters); //called after initialize(...) returns ERR_OK
public:
	YeeFDTDSpaceSynched(void);
	~YeeFDTDSpaceSynched(void);
	int formSpaceSynchedFields();
	//
	virtual int PopulateFields(FieldsInitializer *fieldValues);
	virtual int updateFieldsToMoveForward();
	virtual void OnFinishSimulation();
	//
};



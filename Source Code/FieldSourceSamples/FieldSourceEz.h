#ifndef __FIELDSOURCEEZ_H__
#define __FIELDSOURCEEZ_H__

/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "..\EMField\EMField.h"
#include "..\EMField\RadiusIndex.h"
#include "..\EMField\FieldSource.h"
#include "..\FileUtil\taskFile.h"

#define TP_FS_PPW "FS.PPW"

/*
	implement a ricker field source
	the code is taken from a book "understanding FDTD method" by John B. Schneider
	see https://github.com/john-b-schneider/uFDTD/blob/master/Code/Fdtd-3d/abc3dfirst.c
*/
class FieldSourceEz: public FieldSource
{
private:
	double ppw;
public:
	FieldSourceEz(void);
	//
	virtual int initialize(double Courant, int maximumRadius, TaskFile *taskParameters);
	//
	//GoThroughSphereByIndexes members
	virtual RadiusHandleType setRadius(int radius);
	virtual void handleData(int m, int n, int p);

};
#endif

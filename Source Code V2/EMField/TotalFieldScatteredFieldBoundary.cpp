/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/

#include "TotalFieldScatteredFieldBoundary.h"

TotalFieldScatteredFieldBoundary::TotalFieldScatteredFieldBoundary(void)
{
	gridSize = 0;
	maxRadius = 0;
	firstX = firstY = firstZ = lastX = lastY = lastZ = 0;
	_fields = NULL;
}

int TotalFieldScatteredFieldBoundary::initialize(double Courant, int maximumRadius, TaskFile *taskParameters)
{
	int ret = ERR_OK;
	Cdtds = Courant;
	maxRadius = maximumRadius;
	gridSize = 2 * maxRadius + 1;
	firstX = taskParameters->getInt("TFSF.firstX", false);
	firstY = taskParameters->getInt("TFSF.firstY", false);
	firstZ = taskParameters->getInt("TFSF.firstZ", false);
	lastX = taskParameters->getInt("TFSF.lastX", false);
	lastY = taskParameters->getInt("TFSF.lastY", false);
	lastZ = taskParameters->getInt("TFSF.lastZ", false);
	ret = taskParameters->getErrorCode();
	return ret;
}

/*
	go through each TFSF boundary plane and call applyOnPlane?? for each space point
	access EM fields by _fields[CINDEX(i,j,k)]
*/
int TotalFieldScatteredFieldBoundary::applyTFSF(FieldPoint3D *fields)
{
	int i,j,k;
	_fields = fields;
	//plane x0
	//i = firstX;
	for(j=0;j<gridSize;j++)
	{
		for(k=0;k<gridSize;k++)
		{
			applyOnPlaneX0(j,k);
		}
	}
	//plane x1
	//i = lastX;
	for(j=0;j<gridSize;j++)
	{
		for(k=0;k<gridSize;k++)
		{
			applyOnPlaneX1(j,k);
		}
	}
	//plane y0
	//j = firstY;
	for(i=0;i<gridSize;i++)
	{
		for(k=0;k<gridSize;k++)
		{
			applyOnPlaneY0(i,k);
		}
	}
	//plane y1
	//j = lastY;
	for(i=0;i<gridSize;i++)
	{
		for(k=0;k<gridSize;k++)
		{
			applyOnPlaneY1(i,k);
		}
	}
	//plane z0
	//k = firstZ;
	for(i=0;i<gridSize;i++)
	{
		for(j=0;j<gridSize;j++)
		{
			applyOnPlaneZ0(i,j);
		}
	}
	//plane z1
	//k = lastZ;
	for(i=0;i<gridSize;i++)
	{
		for(j=0;j<gridSize;j++)
		{
			applyOnPlaneZ1(i,j);
		}
	}
	return ERR_OK;
}



/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by Bob Limnor

********************************************************************/

#include "tfsfEz.h"
#include "..\MemoryMan\memman.h"
#include "..\EMField\RadiusIndex.h"

#include <malloc.h>

TfsfEz::TfsfEz(void)
{
	g1 = NULL;
}

/*
	see
	https://github.com/john-b-schneider/uFDTD/blob/master/Code/Fdtd-3d/Tfsf-new/tfsf-3d-ez.c
*/
int TfsfEz::initialize(double Courant, int maximumRadius, TaskFile *taskParameters)
{
	int ret = TotalFieldScatteredFieldBoundary::initialize(Courant, maximumRadius, taskParameters);
	if(ret == ERR_OK)
	{
		g1 = (Grid *)malloc(sizeof(Grid));
		if(g1 == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		else
		{
		}
	}
	return ret;
}

void TfsfEz::applyOnPlaneX0(int j, int k)
{
}
void TfsfEz::applyOnPlaneX1(int j, int k)
{
}
void TfsfEz::applyOnPlaneY0(int i, int k)
{
}
void TfsfEz::applyOnPlaneY1(int i, int k)
{
}
void TfsfEz::applyOnPlaneZ0(int i, int j)
{
}
void TfsfEz::applyOnPlaneZ1(int i, int j)
{
}

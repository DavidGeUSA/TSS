#ifndef __TFSFEZ_H__
#define __TFSFEZ_H__

/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/

#include "..\EMField\EMField.h"
#include "..\EMField\TotalFieldScatteredFieldBoundary.h"

/*
	implement TFSF boundary
	the code/algorithm is taken from from book "understanding FDTD method" by John B. Schneider
	see https://github.com/john-b-schneider/uFDTD/tree/master/Code/Fdtd-3d/Tfsf-new
*/

/*
	this struct is copied from "understanding FDTD method"
	see
	https://github.com/john-b-schneider/uFDTD/blob/master/Code/Fdtd-3d/Tfsf-new/fdtd-grid.h
*/
struct Grid {
  double *hx, *chxh, *chxe;
  double *hy, *chyh, *chye;
  double *hz, *chzh, *chze;
  double *ex, *cexe, *cexh;
  double *ey, *ceye, *ceyh;
  double *ez, *ceze, *cezh;
  int sizeX, sizeY, sizeZ;
  int time, maxTime;
  int type;
  double cdtds;
};

typedef struct Grid Grid;

/*
	most of code in this class is taken from
	https://github.com/john-b-schneider/uFDTD/tree/master/Code/Fdtd-3d/Tfsf-new
*/
class TfsfEz:public TotalFieldScatteredFieldBoundary
{
protected:
	Grid *g1; // 1D auxilliary grid
public:
	TfsfEz(void);

	virtual int initialize(double Courant, int maximumRadius, TaskFile *taskParameters);
	virtual void applyOnPlaneX0(int j, int k);
	virtual void applyOnPlaneX1(int j, int k);
	virtual void applyOnPlaneY0(int i, int k);
	virtual void applyOnPlaneY1(int i, int k);
	virtual void applyOnPlaneZ0(int i, int j);
	virtual void applyOnPlaneZ1(int i, int j);

};

#endif

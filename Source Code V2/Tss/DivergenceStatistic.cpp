/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

use field divergence to evaluate simulation accuracy
********************************************************************/
#include "DivergenceStatistic.h"


DivergenceStatistic::DivergenceStatistic()
{
}


DivergenceStatistic::~DivergenceStatistic()
{
}

void DivergenceStatistic::initialize(Space *sp, SimStruct *ps, FieldSourceTss *fieldSource, bool excluseBoundary)
{
	space = sp;
	pams = ps;
	src = fieldSource;
	if (excluseBoundary)
	{
		s0 = pams->smax + 2;
		endX = pams->nx - 2 * pams->smax;
		endY = pams->ny - 2 * pams->smax;
		endZ = pams->nz - 2 * pams->smax;
	}
	else
	{
		s0 = 0;
		endX = pams->nx;
		endY = pams->ny;
		endZ = pams->nz;
	}
}

size_t DivergenceStatistic::calculate(Point3Dstruct *efile, Point3Dstruct *hfile, FieldStatisticStruct *f)
{
	double emag, hmag, divg;
	size_t pointCount = 0;
	bool bDiv;
	fs = f;
	for (unsigned int i = s0; i < endX; i++)
	{
		for (unsigned int j = s0; j < endY; j++)
		{
			for (unsigned int k = s0; k < endZ; k++)
			{
				size_t w = space->Idx(i, j, k);
				//||E||^2
				emag = efile[w].x * efile[w].x + efile[w].y * efile[w].y + efile[w].z * efile[w].z;
				//||H||^2
				hmag = hfile[w].x * hfile[w].x + hfile[w].y * hfile[w].y + hfile[w].z * hfile[w].z;
				//
				bDiv = (src == NULL);
				if (!bDiv)
				{
					bDiv = src->isInSource(i, j, k);
				}
				if (!bDiv)
				{
					divg = abs(space->dx_Fx(efile, 1, i, j, k) + space->dy_Fy(efile, 1, i, j, k) + space->dz_Fz(efile, 1, i, j, k));
					//divg = abs(c1.Divergence(efile, i, j, k, w));
					divg = divg / pams->ds;
					if (divg > fs->maxDivergenceE)
					{
						fs->maxDivergenceE = divg;
					}
					fs->averageDivergenceE += divg;
					//
					divg = abs(space->dx_Fx(hfile, 1, i, j, k) + space->dy_Fy(hfile, 1, i, j, k) + space->dz_Fz(hfile, 1, i, j, k));
					//divg = abs(c1.Divergence(hfile, i, j, k, w));
					divg = divg / pams->ds;
					if (divg > fs->maxDivergenceH)
					{
						fs->maxDivergenceH = divg;
					}
					fs->averageDivergenceH += divg;
				}
				fs->energySum += pams->eps * emag / 2.0 + pams->mu * hmag / 2.0;
				fs->magSumE += emag;
				fs->magSumH += hmag;
				pointCount++;
			}
		}
	}
	return pointCount;
}


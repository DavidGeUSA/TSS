/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

use field divergence to evaluate simulation accuracy
********************************************************************/
#include "DivergenceStatistic.h"

void DivergenceStatistic::initialize(Space *sp, SimStruct *ps, FieldSourceTss *fieldSource, bool excludeBoundary)
{
	space = sp;
	pams = ps;
	src = fieldSource;
	_excludeBoundary = excludeBoundary;
	if (excludeBoundary)
	{
		s0 = 2 * pams->smax + 1;
		endX = pams->nx - 2 * pams->smax;
		endY = pams->ny - 2 * pams->smax;
		endZ = pams->nz - 2 * pams->smax;
		if (!pams->pml.disable)
		{
			s0 += pams->pml.Ln;
			endX -= pams->pml.Ln;
			endY -= pams->pml.Ln;
			endZ -= pams->pml.Ln;
		}
	}
	else
	{
		s0 = 0;
		endX = pams->nx;
		endY = pams->ny;
		endZ = pams->nz;
	}
	onInitialize();
}

DivergenceStatistic3D::DivergenceStatistic3D()
{
}


DivergenceStatistic3D::~DivergenceStatistic3D()
{
}

size_t DivergenceStatistic3D::calculate(Point3Dstruct *efile, Point3Dstruct *hfile, FieldStatisticStruct *f)
{
	double emag, hmag, divg;
	size_t pointCount = 0;
	bool bDiv;
	double emag0=0, hmag0 = 0;
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
				bDiv = (src == NULL); //if no field source then calculate divergence
				if (!bDiv) //field source is used
				{
					bDiv = !src->isInSource(i, j, k); //calculate divergence if it is not in field source area
				}
				if (bDiv)
				{
					//divg = abs(space->dx_Fx(efile, 1, i, j, k) + space->dy_Fy(efile, 1, i, j, k) + space->dz_Fz(efile, 1, i, j, k));
					divg = abs(c1.Divergence(efile, i, j, k, w));
					divg = divg / pams->ds;
					if (pams->relativeDivergence && pams->relDivergThreshold>0)
					{
						emag0 = sqrt(emag);
						if (emag0 > pams->relDivergThreshold)
						{
							divg = divg / emag0;
						}
					}
					if (divg > fs->maxDivergenceE)
					{
						fs->maxDivergenceE = divg;
						fs->magForMaxDivgE = sqrt(emag);
					}
					fs->averageDivergenceE += divg;
					//
					//divg = abs(space->dx_Fx(hfile, 1, i, j, k) + space->dy_Fy(hfile, 1, i, j, k) + space->dz_Fz(hfile, 1, i, j, k));
					divg = abs(c1.Divergence(hfile, i, j, k, w));
					divg = divg / pams->ds;
					if (pams->relativeDivergence && pams->relDivergThreshold>0)
					{
						hmag0 = sqrt(hmag);
						if (hmag0 > pams->relDivergThreshold)
						{
							divg = divg / hmag0;
						}
					}
					if (divg > fs->maxDivergenceH)
					{
						fs->maxDivergenceH = divg;
						fs->magForMaxDivgH = sqrt(hmag);
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

int DivergenceStatistic3D::onInitialize()
{
	c1.setSpaceRange(pams->nx, pams->ny, pams->nz);
	c1.setThreadWork(0, pams->nx, 0, 0);
	return ERR_OK;
}

int DivergenceStatisticZRotateSymmetry::onInitialize()
{
	if (_excludeBoundary)
	{
	}
	else
	{
	}
	ic = pams->nx / 2;
	endX = ic;
	c1.setSpaceRange(pams->nx, pams->ny, pams->nz);
	c1.setThreadWork(0, pams->nx, 0, 0);
	//
	return ERR_OK;
}

size_t DivergenceStatisticZRotateSymmetry::calculate(Point3Dstruct *efile, Point3Dstruct *hfile, FieldStatisticStruct *f)
{
	double emag, hmag, divg;
	size_t pointCount = 0;
	bool bDiv;
	RotateSymmetryField ef(pams,efile);
	RotateSymmetryField hf(pams,hfile);
	unsigned int j = ic;
	//ef.setRawMemory(efile);
	//hf.setRawMemory(hfile);
	fs = f;
	for (unsigned int i = s0; i < endX; i++)
	{
		//for (unsigned int j = s0; j < endY; j++)
		//{
			for (unsigned int k = s0; k < endZ; k++)
			{
				size_t w = ef.Idx(i, k); //space->Idx(i, j, k);
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
					//divg = abs(space->dx_Fx(efile, 1, i, j, k) + space->dy_Fy(efile, 1, i, j, k) + space->dz_Fz(efile, 1, i, j, k));
					divg = abs(c1.Divergence(&ef, i, j, k, w));
					divg = divg / pams->ds;
					if (divg > fs->maxDivergenceE)
					{
						fs->maxDivergenceE = divg;
					}
					fs->averageDivergenceE += divg;
					//
					//divg = abs(space->dx_Fx(hfile, 1, i, j, k) + space->dy_Fy(hfile, 1, i, j, k) + space->dz_Fz(hfile, 1, i, j, k));
					divg = abs(c1.Divergence(&hf, i, j, k, w));
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
		//}
	}
	return pointCount;
}

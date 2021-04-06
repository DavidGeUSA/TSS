/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/22/2020
Allrights reserved by David Ge

Implementation of PEMC for Time-Space-Synchronized FDTD simulation
********************************************************************/
#include "BoundaryTss.h"


BoundaryTss::BoundaryTss()
{
	ds = 0;
	nx = ny = nz = nx1 = ny1 = nz1 = 0;
	factor0 = 0;
	factor1 = 0;
	useDivergence = false;
	_initialized = false;
}


BoundaryTss::~BoundaryTss()
{
}
inline size_t BoundaryTss::Idx(unsigned i, unsigned j, unsigned k)
{
	return k + nz1 * (j + ny1 * i);
}

int BoundaryTss::setSpace(Space *spaceModule)
{
	space = spaceModule;
	ds = space->DS();
	nx = space->Nx(); ny = space->Ny(); nz = space->Nz();
	ic = nx / 2;
	smax = space->SMAX();
	M = 2 * smax;
	nx1 = nx + 1; ny1 = ny + 1; nz1 = nz + 1;
	A = space->GetMaxtrixes();
	//these two values are for applying divergence-restriction
	factor0 = factor1 = 0.0;
	for (unsigned int i = 0; i < M; i++)
	{
		factor0 += A->a[smax].r[0].c[i];
		factor1 += A->a[M].r[0].c[i];
	}
	_initialized = true;
	return ERR_OK;
}

//the following 3 functions are applying divergence restrictions
//do not use them because the idea is not proved to be working
/*
E_x (0,j,k)=1/M ∑_(i=1)^M〖a_1i^smax E_x (i,j,k) 〗
E_x (n_x,j,k)=1/M ∑_(i=1)^M〖a_1i^M E_x (n_x-i,j,k) 〗
*/
void BoundaryTss::setXboundary(unsigned int j, unsigned int k)
{
	a0 = Idx(0,j,k);
	am = Idx(nx,j,k);
	efield[a0].x = 0.0;
	efield[am].x = 0.0;
	hfield[a0].x = 0.0;
	hfield[am].x = 0.0;
	for (unsigned int i = 1; i <= M; i++)
	{
		aw = Idx(i,j,k);
		efield[a0].x += A->a[smax].r[0].c[i-1] * efield[aw].x;
		hfield[a0].x += A->a[smax].r[0].c[i-1] * hfield[aw].x;
		aw = Idx(nx-i,j,k);
		efield[am].x += A->a[M].r[0].c[i-1] * efield[aw].x;
		hfield[am].x += A->a[M].r[0].c[i-1] * hfield[aw].x;
	}
	efield[a0].x = efield[a0].x / factor0;
	efield[am].x = efield[am].x / factor1;
	hfield[a0].x = hfield[a0].x / factor0;
	hfield[am].x = hfield[am].x / factor1;
}

void BoundaryTss::setYboundary(unsigned int i, unsigned int k)
{
	a0 = Idx(i,0,k);
	am = Idx(i,ny,k);
	efield[a0].y = 0.0;
	efield[am].y = 0.0;
	hfield[a0].y = 0.0;
	hfield[am].y = 0.0;
	for (unsigned int j = 1; j <= M; j++)
	{
		aw = Idx(i,j,k);
		efield[a0].y += A->a[smax].r[0].c[j-1] * efield[aw].y;
		hfield[a0].y += A->a[smax].r[0].c[j-1] * hfield[aw].y;
		aw = Idx(i,ny-j,k);
		efield[am].y += A->a[M].r[0].c[j-1] * efield[aw].y;
		hfield[am].y += A->a[M].r[0].c[j-1] * hfield[aw].y;
	}
	efield[a0].y = efield[a0].y / factor0;
	efield[am].y = efield[am].y / factor1;
	hfield[a0].y = hfield[a0].y / factor0;
	hfield[am].y = hfield[am].y / factor1;
}

void BoundaryTss::setZboundary(unsigned int i, unsigned int j)
{
	a0 = Idx(i,j,0);
	am = Idx(i,j,nz);
	efield[a0].z = 0.0;
	efield[am].z = 0.0;
	hfield[a0].z = 0.0;
	hfield[am].z = 0.0;
	for (unsigned int k = 1; k <= M; k++)
	{
		aw = Idx(i,j,k);
		efield[a0].z += A->a[smax].r[0].c[k-1] * efield[aw].z;
		hfield[a0].z += A->a[smax].r[0].c[k-1] * hfield[aw].z;
		aw = Idx(i,j,nz-k);
		efield[am].z += A->a[M].r[0].c[k-1] * efield[aw].z;
		hfield[am].z += A->a[M].r[0].c[k-1] * hfield[aw].z;
	}
	efield[a0].z = efield[a0].z / factor0;
	efield[am].z = efield[am].z / factor1;
	hfield[a0].z = hfield[a0].z / factor0;
	hfield[am].z = hfield[am].z / factor1;
}

/*
	by default, use PEMC
*/
int BoundaryTss::apply(Point3Dstruct *E, Point3Dstruct* H)
{
	int ret = ERR_OK;
	if (E == NULL || H == NULL)
		ret = ERR_INVALID_CALL;
	else
	{
		efield = E;
		hfield = H;
		//
		size_t w;
		//(y, z) planes
		for (unsigned int j = 0; j <= ny; j++)
		{
			for (unsigned int k = 0; k <= nz; k++)
			{
				w = Idx(0,j,k);
				E[w].y = 0.0;
				E[w].z = 0.0;
				H[w].y = 0.0;
				H[w].z = 0.0;
				//
				w = Idx(nx,j,k);
				E[w].y = 0.0;
				E[w].z = 0.0;
				H[w].y = 0.0;
				H[w].z = 0.0;
				//
				if (useDivergence)
				{
					setXboundary(j, k);
				}
			}
		}
		//(x, z) planes
		for (unsigned int i = 0; i <= nx; i++)
		{
			for (unsigned int k = 0; k <= nz; k++)
			{
				w = Idx(i,0,k);
				E[w].x = 0.0;
				E[w].z = 0.0;
				H[w].x = 0.0;
				H[w].z = 0.0;
				//
				w = Idx(i,ny,k);
				E[w].x = 0.0;
				E[w].z = 0.0;
				H[w].x = 0.0;
				H[w].z = 0.0;
				//
				if (useDivergence)
				{
					setYboundary(i, k);
				}
			}
		}
		//(x,y) planes
		for (unsigned int i = 0; i <= nx; i++)
		{
			for (unsigned int j = 0; j <= ny; j++)
			{
				w = Idx(i,j,0);
				E[w].x = 0.0;
				E[w].y = 0.0;
				H[w].x = 0.0;
				H[w].y = 0.0;
				//
				w = Idx(i,j,nz);
				E[w].x = 0.0;
				E[w].y = 0.0;
				H[w].x = 0.0;
				H[w].y = 0.0;
				//
				if (useDivergence)
				{
					setZboundary(i, j);
				}
			}
		}
		//
	}
	return ret;
}
int BoundaryTss::applyToZrotateSymmetry(RotateSymmetryField *E, RotateSymmetryField *H)
{
	int ret = ERR_OK;
	if (E == NULL || H == NULL)
		ret = ERR_INVALID_CALL;
	else
	{
		Point3Dstruct *boundary;
		//unsigned int j = ic;
		//(y, z) planes reduce to one line (0,ic,0)-(0,ic,nz): i=0, j=ic, k=0,1,...,nz
		for (unsigned int k = 0; k <= nz; k++)
		{
			//w = Idx(0, j, k);
			boundary = E->getFieldOnPlane(0, k);
			boundary->y = 0.0;
			boundary->z = 0.0;
			boundary = H->getFieldOnPlane(0, k);
			boundary->y = 0.0;
			boundary->z = 0.0;
			//
		}
		//
		//(x, z) planes do not exist
		//
		//(x, y) planes reduce to two lines:
		//     (0,ic,0) - (ic,ic,0)
		//     (0,ic,nz) - (ic,ic,nz)
		for (unsigned int i = 0; i <= ic; i++)
		{
			//w = Idx(i, j, 0);
			boundary = E->getFieldOnPlane(i, 0);
			boundary->x = 0.0;
			boundary->y = 0.0;
			boundary = H->getFieldOnPlane(i, 0);
			boundary->x = 0.0;
			boundary->y = 0.0;
			//
			//w = Idx(i, j, nz);
			boundary = E->getFieldOnPlane(i, nz);
			boundary->x = 0.0;
			boundary->y = 0.0;
			boundary = H->getFieldOnPlane(i, nz);
			boundary->x = 0.0;
			boundary->y = 0.0;
			//
		}
		//
	}
	return ret;
}
int BoundaryTss::applyBoundaryToFields(TimeTssBase *timeModule)
{
	if (timeModule->FieldType() == Field_type_3D)
	{
		return apply(timeModule->GetFieldE(), timeModule->GetFieldH());
	}
	else if (timeModule->FieldType() == Field_type_z_rotateSymmetry)
	{
		return applyToZrotateSymmetry(timeModule->GetFieldZrotateSymmetryE(), timeModule->GetFieldZrotateSymmetryH());
	}
	else
	{
		return ERR_BOUNDARY_NOT_SUPPORT;
	}
}
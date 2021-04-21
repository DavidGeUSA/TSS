#include "RotateSymmetryField.h"
#include <cmath>

RotateSymmetryField::RotateSymmetryField()
{
	plane = NULL;
	memAllocated = false;
}
RotateSymmetryField::RotateSymmetryField(SimStruct *simp, Point3Dstruct *field)
{
	pams = simp;
	plane = field;
	memAllocated = false;
	_mem = NULL;
	nz1 = pams->nz + 1;
	ic = pams->nx / 2;
	ic1 = ic + 1;
	cellCount = ic1*nz1;
	fieldMemorySize = cellCount*sizeof(Point3Dstruct);
}

RotateSymmetryField::~RotateSymmetryField()
{
	cleanup();
}

/*
	map (x0, 0) to (x, y)
	fx0, fy0: values at (x0,0)
	pfx, pfy: values at (x, y)
	assuming x^2 + y^2 = x0^2
*/
void RotateSymmetryField::map(double fx0, double fy0, double x, double y, double *pfx, double *pfy)
{
	double r = sqrt(x*x + y*y);
	if (r == 0)
	{
		throw;
	}
	if (fy0 == 0)
	{
		*pfx = -x*fx0 / r;
		*pfy = -y*fx0 / r;
	}
	else if (fx0 == 0)
	{
		*pfx = -y*fy0 / r;
		*pfy = -x*fy0 / r;
	}
	else if ((fx0 < 0 && fy0 < 0)||(fx0 > 0 && fy0 > 0))
	{
		*pfx = -(x*fx0 - y*fy0) / r;
		*pfy = -(x*fy0 + y*fx0) / r;
	}
	else if (fx0 > 0 && fy0 < 0)
	{
		*pfx = -(x*fx0 + y*fy0) / r;
		*pfy = (x*fy0 - y*fx0) / r;
	}
	else if (fx0 < 0 && fy0 > 0)
	{
		*pfx = (x*fx0 + y*fy0) / r;
		*pfy = -(x*fy0 - y*fx0) / r;
	}
	else
	{
		throw;
	}
}

void RotateSymmetryField::mapToNegativeY(double fx0, double fy0, double *pfx, double *pfy)
{
	*pfx = -fy0;
	*pfy = fx0;
}
void RotateSymmetryField::mapToPositiveY(double fx0, double fy0, double *pfx, double *pfy)
{
	*pfx = fy0;
	*pfy = -fx0;
}
void RotateSymmetryField::mapToPositiveX(double fx0, double fy0, double *pfx, double *pfy)
{
	*pfx = -fx0;
	*pfy = -fy0;
}

int RotateSymmetryField::IsZrotateSymmetry(SimStruct *pams)
{
	int ret = ERR_OK;
	if (pams->ny != pams->nx)
	{
		ret = ERR_CYLINTRICAL_Y_N_X;
	}
	else
	{
		if (pams->nx % 2)
		{
			ret = ERR_CYLINTRICAL_NX_ODD;
		}
		else
		{
			if (pams->xmin >= 0)
			{
				ret = ERR_CYLINTRICAL_XMIN_0;
			}
			else
			{
				if (pams->nx * pams->ds / 2 + pams->xmin != 0)
				{
					ret = ERR_CYLINTRICAL_CENTER;
				}
			}
		}
	}
	return ret;
}

void RotateSymmetryField::cleanup()
{
	if (memAllocated)
	{
		if (plane != NULL)
		{
			FreeMemory(plane);
			plane = NULL;
			memAllocated = false;
		}
	}
}
int RotateSymmetryField::initialVirtualField(SimStruct *simp, MemoryManager *mem)
{
	VirtualField::initialVirtualField(simp, mem);
	int ret = IsZrotateSymmetry(pams);
	if (ret == ERR_OK)
	{
		nz1 = pams->nz + 1;
		ic = pams->nx / 2;
		ic1 = ic + 1;
		cellCount = ic1*nz1;
		fieldMemorySize = cellCount*sizeof(Point3Dstruct);
		plane = (Point3Dstruct *)AllocateMemory(fieldMemorySize);
		if (plane == NULL)
		{
			ret = MEMRETLAST;
		}
		else
		{
			size_t w = 0;
			memAllocated = true;
			for (unsigned int i = 0; i <= ic; i++)
			{
				for (unsigned int k = 0; k <= pams->nz; k++)
				{
					plane[w].x = 0;
					plane[w].y = 0;
					plane[w].z = 0;
					w++;
				}
			}
		}
	}
	return ret;
}

/*
	map 3D point to 2D plane
*/
void RotateSymmetryField::getField(unsigned int i, unsigned int j, unsigned int k, Point3Dstruct *val)
{
	int ir, jr;          //for calculate radius, ir = ic-i, jr = ic-j
	double r;            //radius = sqrt(ir^2+jr^2)
	unsigned int n0, n1; //integers covering r; n1=ceiling(r)>=r; n0=floor(r)<=r
	unsigned int dn;     //dn = n1-n0; if dn==0 then index=(ic-n0,ic,k) 
	//if dn !=0 then use weighted average between indexes (ic-n0,ic,k) and (ic-n1,ic,k) 
	double f1, f0;       //weights, f1=(n1-r)/dn, f0=(r-n0)/dn
	//Field(i,j,k) = f1 * Field(ic-n0,ic,k) + f0 * Field(ic-n1,ic,k)
	size_t idx1, idx0;
	//
	if (i > ic)
	{
		i = 2 * ic - i;
	}
	jr = ic - j;
	if (jr == 0) //at center
	{
		idx0 = Idx(i, k);
		val->x = plane[idx0].x;
		val->y = plane[idx0].y;
		val->z = plane[idx0].z;
		return;
	}
	ir = ic - i;
	r = sqrt(ir*ir + jr*jr);
	n1 = (int)ceil(r);
	n0 = (int)floor(r);
	if (n1 > ic) n1 = ic;
	if (n0 > ic) n0 = ic;
	//
	dn = n1 - n0;
	if (dn == 0)
	{
		idx0 = Idx(ic - n0, k);
		val->x = plane[idx0].x;
		val->y = plane[idx0].y;
		val->z = plane[idx0].z;
		return;
	}
	else
	{
		idx0 = Idx(ic-n0, k);
		idx1 = Idx(ic-n1, k);
		f1 = ((double)n1 - r) / (double)dn;
		f0 = (r - (double)n0) / (double)dn;
		val->x = f1*plane[idx0].x + f0*plane[idx1].x;
		val->y = f1*plane[idx0].y + f0*plane[idx1].y;
		val->z = f1*plane[idx0].z + f0*plane[idx1].z;
	}
}

double RotateSymmetryField::getFieldx(unsigned int i, unsigned int j, unsigned int k)
{
	int ir, jr;          //for calculate radius, ir = ic-i, jr = ic-j
	double r;            //radius = sqrt(ir^2+jr^2)
	unsigned int n0, n1; //integers covering r; n1=ceiling(r)>=r; n0=floor(r)<=r
	unsigned int dn;     //dn = n1-n0; if dn==0 then index=(ic-n0,ic,k) 
	//if dn !=0 then use weighted average between indexes (ic-n0,ic,k) and (ic-n1,ic,k) 
	double f1, f0;       //weights, f1=(n1-r)/dn, f0=(r-n0)/dn
	//Field(i,j,k) = f1 * Field(ic-n0,ic,k) + f0 * Field(ic-n1,ic,k)
	size_t idx1, idx0;
	//
	if (i > ic)
	{
		i = 2 * ic - i;
	}
	jr = ic - j;
	if (jr == 0) //at center
	{
		return	plane[Idx(i, k)].x;
	}
	ir = ic - i;
	r = sqrt(ir*ir + jr*jr);
	n1 = (int)ceil(r);
	n0 = (int)floor(r);
	if (n1 > ic) n1 = ic;
	if (n0 > ic) n0 = ic;
	//
	dn = n1 - n0;
	if (dn == 0)
	{
		return plane[Idx(ic - n0, k)].x;
	}
	else
	{
		idx0 = Idx(ic - n0, k);
		idx1 = Idx(ic - n1, k);
		f1 = ((double)n1 - r) / (double)dn;
		f0 = (r - (double)n0) / (double)dn;
		return f1*plane[idx0].x + f0*plane[idx1].x;
	}
}
double RotateSymmetryField::getFieldy(unsigned int i, unsigned int j, unsigned int k)
{
	int ir, jr;          //for calculate radius, ir = ic-i, jr = ic-j
	double r;            //radius = sqrt(ir^2+jr^2)
	unsigned int n0, n1; //integers covering r; n1=ceiling(r)>=r; n0=floor(r)<=r
	unsigned int dn;     //dn = n1-n0; if dn==0 then index=(ic-n0,ic,k) 
	//if dn !=0 then use weighted average between indexes (ic-n0,ic,k) and (ic-n1,ic,k) 
	double f1, f0;       //weights, f1=(n1-r)/dn, f0=(r-n0)/dn
	//Field(i,j,k) = f1 * Field(ic-n0,ic,k) + f0 * Field(ic-n1,ic,k)
	size_t idx1, idx0;
	//
	if (i > ic)
	{
		i = 2 * ic - i;
	}
	jr = ic - j;
	if (jr == 0) //at center
	{
		return	plane[Idx(i, k)].y;
	}
	ir = ic - i;
	r = sqrt(ir*ir + jr*jr);
	n1 = (int)ceil(r);
	n0 = (int)floor(r);
	if (n1 > ic) n1 = ic;
	if (n0 > ic) n0 = ic;
	//
	dn = n1 - n0;
	if (dn == 0)
	{
		return plane[Idx(ic - n0, k)].y;
	}
	else
	{
		idx0 = Idx(ic - n0, k);
		idx1 = Idx(ic - n1, k);
		f1 = ((double)n1 - r) / (double)dn;
		f0 = (r - (double)n0) / (double)dn;
		return f1*plane[idx0].y + f0*plane[idx1].y;
	}
}
double RotateSymmetryField::getFieldz(unsigned int i, unsigned int j, unsigned int k)
{
	int ir, jr;          //for calculate radius, ir = ic-i, jr = ic-j
	double r;            //radius = sqrt(ir^2+jr^2)
	unsigned int n0, n1; //integers covering r; n1=ceiling(r)>=r; n0=floor(r)<=r
	unsigned int dn;     //dn = n1-n0; if dn==0 then index=(ic-n0,ic,k) 
	//if dn !=0 then use weighted average between indexes (ic-n0,ic,k) and (ic-n1,ic,k) 
	double f1, f0;       //weights, f1=(n1-r)/dn, f0=(r-n0)/dn
	//Field(i,j,k) = f1 * Field(ic-n0,ic,k) + f0 * Field(ic-n1,ic,k)
	size_t idx1, idx0;
	//
	if (i > ic)
	{
		i = 2 * ic - i;
	}
	jr = ic - j;
	if (jr == 0) //at center
	{
		idx0 = Idx(i, k);
		return	plane[idx0].z;
	}
	ir = ic - i;
	r = sqrt(ir*ir + jr*jr);
	n1 = (int)ceil(r);
	n0 = (int)floor(r);
	if (n1 > ic) n1 = ic;
	if (n0 > ic) n0 = ic;
	//
	dn = n1 - n0;
	if (dn == 0)
	{
		idx0 = Idx(ic - n0, k);
		return plane[idx0].z;
	}
	else
	{
		idx0 = Idx(ic - n0, k);
		idx1 = Idx(ic - n1, k);
		f1 = ((double)n1 - r) / (double)dn;
		f0 = (r - (double)n0) / (double)dn;
		return f1*plane[idx0].z + f0*plane[idx1].z;
	}
}


/*
	initialize the 2D plane using 3D indexing
	i=0,1,2,...,ic
	j=ic
	k=0,1,2,...,nz
*/
void RotateSymmetryField::setField(unsigned int i, unsigned int j, unsigned int k, Point3Dstruct *val)
{
	if (j != ic)
	{
		throw;
	}
	if (i > ic)
	{
		throw;
	}
	size_t idx0 = Idx(i, k);
	plane[idx0].x = val->x;
	plane[idx0].y = val->y;
	plane[idx0].z = val->z;
}


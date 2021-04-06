#include "RotateSymmetryField.h"
#include <cmath>

RotateSymmetryField::RotateSymmetryField()
{
	plane = NULL;
}


RotateSymmetryField::~RotateSymmetryField()
{
	cleanup();
}
void RotateSymmetryField::cleanup()
{
	if (plane != NULL)
	{
		FreeMemory(plane);
		plane = NULL;
	}
}
int RotateSymmetryField::initialVirtualField(SimStruct *simp, MemoryManager *mem)
{
	VirtualField::initialVirtualField(simp, mem);
	int ret = ERR_OK;
	//verify it is good for z-axis rotation symmetry 
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
			ic = pams->nx / 2;
			ic1 = ic + 1;
			if (pams->xmin == 0)
			{
				ret = ERR_CYLINTRICAL_XMIN_0;
			}
			else
			{
				if (ic*pams->ds + pams->xmin != 0)
				{
					ret = ERR_CYLINTRICAL_CENTER;
				}
				else
				{
					plane = (Point3Dstruct *)AllocateMemory(ic1*(pams->nz + 1)*sizeof(Point3Dstruct));
					if (plane == NULL)
					{
						ret = MEMRETLAST;
					}
					else
					{
						size_t w = 0;
						for (unsigned int i = 0; i <= ic; i++)
						{
							for (unsigned int k = 0; k <= pams->ny; k++)
							{
								plane[w].x = 0;
								plane[w].y = 0;
								plane[w].z = 0;
								w++;
							}
						}
					}
				}
			}
		}
	}
	return ret;
}

/*
	map 3D point to 2D plane
*/
Point3Dstruct *RotateSymmetryField::getField(unsigned int i, unsigned int j, unsigned int k)
{
	jr = ic - j;
	if (jr == 0) //at center
	{
		return	&(plane[Idx(ic, k)]);
	}
	ir = ic - i;
	r = sqrt(ir*ir + jr*jr);
	n1 = (int)ceil(r);
	n0 = (int)floor(r);
	dn = n1 - n0;
	if (dn == 0)
	{
		return &(plane[Idx(ic-n0, k)]);
	}
	else
	{
		idx0 = Idx(ic-n0, k);
		idx1 = Idx(ic-n1, k);
		f1 = ((double)n1 - r) / (double)dn;
		f0 = (r - (double)n0) / (double)dn;
		val.x = f1*plane[idx0].x + f0*plane[idx1].x;
		val.y = f1*plane[idx0].y + f0*plane[idx1].y;
		val.z = f1*plane[idx0].z + f0*plane[idx1].z;
		return &val;
	}
}

double RotateSymmetryField::getFieldx(unsigned int i, unsigned int j, unsigned int k)
{
	jr = ic - j;
	if (jr == 0) //at center
	{
		return	plane[Idx(ic, k)].x;
	}
	ir = ic - i;
	r = sqrt(ir*ir + jr*jr);
	n1 = (int)ceil(r);
	n0 = (int)floor(r);
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
	jr = ic - j;
	if (jr == 0) //at center
	{
		return	plane[Idx(ic, k)].y;
	}
	ir = ic - i;
	r = sqrt(ir*ir + jr*jr);
	n1 = (int)ceil(r);
	n0 = (int)floor(r);
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
	jr = ic - j;
	if (jr == 0) //at center
	{
		return	plane[Idx(ic, k)].z;
	}
	ir = ic - i;
	r = sqrt(ir*ir + jr*jr);
	n1 = (int)ceil(r);
	n0 = (int)floor(r);
	dn = n1 - n0;
	if (dn == 0)
	{
		return plane[Idx(ic - n0, k)].z;
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
	idx0 = Idx(i, k);
	plane[idx0].x = val->x;
	plane[idx0].y = val->y;
	plane[idx0].z = val->z;
}


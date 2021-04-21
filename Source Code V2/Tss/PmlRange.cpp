/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 03/31/2018
Allrights reserved by David Ge

define types of PML ranges: corner cube, edge bar, side board

********************************************************************/
#include "PmlRange.h"


PmlRange::PmlRange()
{
	coeff = NULL;
	sumH = NULL;
	sumE = NULL;
}


PmlRange::~PmlRange()
{

}

int PmlRange::initializePmlRange(SimStruct *p, int rangeIndex0)
{
	pams = p; 
	rangeIndex = rangeIndex0;
	return onInitializePmlRange();
}
////////////--PmlCorner--/////////////////////////////////////////////////
PmlCorner::PmlCorner()
{
}


PmlCorner::~PmlCorner()
{
	if (coeff != NULL)
	{
		for (unsigned int h = 0; h <= pams->kmax; h++)
		{
			if (coeff[h] != NULL)
			{
				free(coeff[h]); coeff[h] = NULL;
			}
		}
		free(coeff);
	}
	if (sumH != NULL)
	{
		free(sumH);
	}
	if (sumE != NULL)
	{
		free(sumE);
	}
}
const char *PmlCorner::name()
{
	switch (rangeIndex)
	{
	case COR_LLL:
		return "COR_LLL";
	case COR_HLL:
		return "COR_HLL";
	case COR_LHL:
		return "COR_LHL";
	case COR_LLH:
		return "COR_LLH";
	case COR_LHH:
		return "COR_LHH";
	case COR_HLH:
		return "COR_HLH";
	case COR_HHL:
		return "COR_HHL";
	case COR_HHH:
		return "COR_HHH";
	}
	throw;
}
/*
	i,j,k range - indexes into full field
*/
unsigned int PmlCorner::iStart()
{
	switch (rangeIndex)
	{
	case COR_LLL:
		return 0;
	case COR_HLL:
		return pams->nx - pams->pml.Ln+1;
	case COR_LHL:
		return 0;
	case COR_LLH:
		return 0;
	case COR_LHH:
		return 0;
	case COR_HLH:
		return pams->nx - pams->pml.Ln + 1;
	case COR_HHL:
		return pams->nx - pams->pml.Ln + 1;
	case COR_HHH:
		return pams->nx - pams->pml.Ln + 1;
	}
	throw;
}
unsigned int PmlCorner::iEnd()
{
	switch (rangeIndex)
	{
	case COR_LLL:
		return pams->pml.Ln-1;
	case COR_HLL:
		return pams->nx;
	case COR_LHL:
		return pams->pml.Ln - 1;
	case COR_LLH:
		return pams->pml.Ln - 1;
	case COR_LHH:
		return pams->pml.Ln - 1;
	case COR_HLH:
		return pams->nx;
	case COR_HHL:
		return pams->nx;
	case COR_HHH:
		return pams->nx;
	}
	throw;
}
unsigned int PmlCorner::jStart()
{
	switch (rangeIndex)
	{
	case COR_LLL:
		return 0;
	case COR_HLL:
		return 0;
	case COR_LHL:
		return pams->ny - pams->pml.Ln + 1;
	case COR_LLH:
		return 0;
	case COR_LHH:
		return pams->ny - pams->pml.Ln + 1;
	case COR_HLH:
		return 0;
	case COR_HHL:
		return pams->ny - pams->pml.Ln + 1;
	case COR_HHH:
		return pams->ny - pams->pml.Ln + 1;
	}
	throw;

}
unsigned int PmlCorner::jEnd()
{
	switch (rangeIndex)
	{
	case COR_LLL:
		return pams->pml.Ln - 1;
	case COR_HLL:
		return pams->pml.Ln - 1;
	case COR_LHL:
		return pams->ny;
	case COR_LLH:
		return pams->pml.Ln - 1;
	case COR_LHH:
		return pams->ny;
	case COR_HLH:
		return pams->pml.Ln - 1;
	case COR_HHL:
		return pams->ny;
	case COR_HHH:
		return pams->ny;
	}
	throw;

}
unsigned int PmlCorner::kStart()
{
	switch (rangeIndex)
	{
	case COR_LLL:
		return 0;
	case COR_HLL:
		return 0;
	case COR_LHL:
		return 0;
	case COR_LLH:
		return pams->nz - pams->pml.Ln + 1;
	case COR_LHH:
		return pams->nz - pams->pml.Ln + 1;
	case COR_HLH:
		return pams->nz - pams->pml.Ln + 1;
	case COR_HHL:
		return 0;
	case COR_HHH:
		return pams->nz - pams->pml.Ln + 1;
	}
	throw;

}
unsigned int PmlCorner::kEnd()
{
	switch (rangeIndex)
	{
	case COR_LLL:
		return pams->pml.Ln - 1;
	case COR_HLL:
		return pams->pml.Ln - 1;
	case COR_LHL:
		return pams->pml.Ln - 1;
	case COR_LLH:
		return pams->nz;
	case COR_LHH:
		return pams->nz;
	case COR_HLH:
		return pams->nz;
	case COR_HHL:
		return pams->pml.Ln - 1;
	case COR_HHH:
		return pams->nz;
	}
	throw;

}
int PmlCorner::onInitializePmlRange()
{
	int ret = ERR_OK;
	//corner range: Ln*Ln*Ln
	size_t count = pams->pml.Ln * pams->pml.Ln * pams->pml.Ln;
	size_t size = count * sizeof(Point3Dstruct);
	sumH = (Point3Dstruct *)malloc(size);
	sumE = (Point3Dstruct *)malloc(size);
	if (sumH == NULL || sumE == NULL)
	{
		ret = ERR_OUTOFMEMORY;
	}
	if (ret == ERR_OK)
	{
		for (size_t w = 0; w < count; w++)
		{
			sumH[w].x = sumH[w].y = sumH[w].z = 0;
			sumE[w].x = sumE[w].y = sumE[w].z = 0;
		}
		coeff = (PmlCoefficients **)malloc((pams->kmax + 1)*sizeof(PmlCoefficients *));
		if (coeff == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		else
		{
			for (unsigned int h = 0; h <= pams->kmax; h++)
			{
				coeff[h] = NULL;
			}
			for (unsigned int h = 0; h <= pams->kmax; h++)
			{
				//the same number of sets of coefficients as the number of the spaces
				coeff[h] = (PmlCoefficients *)malloc(count*sizeof(PmlCoefficients));
				if (coeff[h] == NULL)
				{
					ret = ERR_OUTOFMEMORY;
					break;
				}
			}
		}
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
////////////--PmlEdgeBar--/////////////////////////////////////////////////
PmlEdgeBar::PmlEdgeBar()
{
}


PmlEdgeBar::~PmlEdgeBar()
{
	if (coeff != NULL)
	{
		for (unsigned int h = 0; h <= pams->kmax; h++)
		{
			if (coeff[h] != NULL)
			{
				free(coeff[h]); coeff[h] = NULL;
			}
		}
		free(coeff);
	}
	if (sumH != NULL)
	{
		free(sumH);
	}
	if (sumE != NULL)
	{
		free(sumE);
	}
}
const char *PmlEdgeBar::name()
{
	switch (rangeIndex)
	{
	case EDG_XLL:
		return "EDG_XLL";
	case EDG_XHL:
		return "EDG_XHL";
	case EDG_XLH:
		return "EDG_XLH";
	case EDG_XHH:
		return "EDG_XHH";
	case EDG_LYL:
		return "EDG_LYL";
	case EDG_HYL:
		return "EDG_HYL";
	case EDG_LYH:
		return "EDG_LYH";
	case EDG_HYH:
		return "EDG_HYH";
	case EDG_LLZ:
		return "EDG_LLZ";
	case EDG_HLZ:
		return "EDG_HLZ";
	case EDG_LHZ:
		return "EDG_LHZ";
	case EDG_HHZ:
		return "EDG_HHZ";
	}
	throw;
}
/*
i,j,k range - indexes into full field
*/
unsigned int PmlEdgeBar::iStart()
{
	switch (rangeIndex)
	{
	case EDG_XLL:
		return pams->pml.Ln;
	case EDG_XHL:
		return pams->pml.Ln;
	case EDG_XLH:
		return pams->pml.Ln;
	case EDG_XHH:
		return pams->pml.Ln;
	case EDG_LYL:
		return 0;
	case EDG_HYL:
		return pams->nx - pams->pml.Ln + 1;
	case EDG_LYH:
		return 0;
	case EDG_HYH:
		return pams->nx - pams->pml.Ln + 1;
	case EDG_LLZ:
		return 0;
	case EDG_HLZ:
		return pams->nx - pams->pml.Ln + 1;
	case EDG_LHZ:
		return 0;
	case EDG_HHZ:
		return pams->nx - pams->pml.Ln + 1;
	}
	throw;
}
unsigned int PmlEdgeBar::iEnd()
{
	switch (rangeIndex)
	{
	case EDG_XLL:
		return pams->nx - pams->pml.Ln;
	case EDG_XHL:
		return pams->nx - pams->pml.Ln;
	case EDG_XLH:
		return pams->nx - pams->pml.Ln;
	case EDG_XHH:
		return pams->nx - pams->pml.Ln;
	case EDG_LYL:
		return pams->pml.Ln - 1;
	case EDG_HYL:
		return pams->nx;
	case EDG_LYH:
		return pams->pml.Ln - 1;
	case EDG_HYH:
		return pams->nx;
	case EDG_LLZ:
		return pams->pml.Ln - 1;
	case EDG_HLZ:
		return pams->nx;
	case EDG_LHZ:
		return pams->pml.Ln - 1;
	case EDG_HHZ:
		return pams->nx;
	}
	throw;
}
unsigned int PmlEdgeBar::jStart()
{
	switch (rangeIndex)
	{
	case EDG_XLL:
		return 0;
	case EDG_XHL:
		return pams->ny - pams->pml.Ln + 1;
	case EDG_XLH:
		return 0;
	case EDG_XHH:
		return pams->ny - pams->pml.Ln + 1;
	case EDG_LYL:
		return pams->pml.Ln;
	case EDG_HYL:
		return pams->pml.Ln;
	case EDG_LYH:
		return pams->pml.Ln;
	case EDG_HYH:
		return pams->pml.Ln;
	case EDG_LLZ:
		return 0;
	case EDG_HLZ:
		return 0;
	case EDG_LHZ:
		return pams->ny - pams->pml.Ln + 1;
	case EDG_HHZ:
		return pams->ny - pams->pml.Ln + 1;
	}
	throw;
}
unsigned int PmlEdgeBar::jEnd()
{
	switch (rangeIndex)
	{
	case EDG_XLL:
		return pams->pml.Ln - 1;
	case EDG_XHL:
		return pams->ny;
	case EDG_XLH:
		return pams->pml.Ln - 1;
	case EDG_XHH:
		return pams->ny;
	case EDG_LYL:
		return pams->ny - pams->pml.Ln;
	case EDG_HYL:
		return pams->ny - pams->pml.Ln;
	case EDG_LYH:
		return pams->ny - pams->pml.Ln;
	case EDG_HYH:
		return pams->ny - pams->pml.Ln;
	case EDG_LLZ:
		return pams->pml.Ln - 1;
	case EDG_HLZ:
		return pams->pml.Ln - 1;
	case EDG_LHZ:
		return pams->ny;
	case EDG_HHZ:
		return pams->ny;
	}
	throw;
}
unsigned int PmlEdgeBar::kStart()
{
	switch (rangeIndex)
	{
	case EDG_XLL:
		return 0;
	case EDG_XHL:
		return 0;
	case EDG_XLH:
		return pams->nz - pams->pml.Ln + 1;
	case EDG_XHH:
		return pams->nz - pams->pml.Ln + 1;
	case EDG_LYL:
		return 0;
	case EDG_HYL:
		return 0;
	case EDG_LYH:
		return pams->nz - pams->pml.Ln + 1;
	case EDG_HYH:
		return pams->nz - pams->pml.Ln + 1;
	case EDG_LLZ:
		return pams->pml.Ln;
	case EDG_HLZ:
		return pams->pml.Ln;
	case EDG_LHZ:
		return pams->pml.Ln;
	case EDG_HHZ:
		return pams->pml.Ln;
	}
	throw;
}
unsigned int PmlEdgeBar::kEnd()
{
	switch (rangeIndex)
	{
	case EDG_XLL:
		return pams->pml.Ln - 1;
	case EDG_XHL:
		return pams->pml.Ln - 1;
	case EDG_XLH:
		return pams->nz;
	case EDG_XHH:
		return pams->nz;
	case EDG_LYL:
		return pams->pml.Ln - 1;
	case EDG_HYL:
		return pams->pml.Ln - 1;
	case EDG_LYH:
		return pams->nz;
	case EDG_HYH:
		return pams->nz;
	case EDG_LLZ:
		return pams->nz - pams->pml.Ln;
	case EDG_HLZ:
		return pams->nz - pams->pml.Ln;
	case EDG_LHZ:
		return pams->nz - pams->pml.Ln;
	case EDG_HHZ:
		return pams->nz - pams->pml.Ln;
	}
	throw;
}
unsigned int PmlEdgeBar::edgeLength()
{
	if (parallelX())
	{
		return pams->nx - pams->pml.Ln - pams->pml.Ln + 1;
	}
	if (parallelY())
	{
		return pams->ny - pams->pml.Ln - pams->pml.Ln + 1;
	}
	if (parallelZ())
	{
		return pams->nz - pams->pml.Ln - pams->pml.Ln + 1;
	}
	throw;
}
int PmlEdgeBar::onInitializePmlRange()
{
	int ret = ERR_OK;
	size_t areaPoints = pams->pml.Ln * pams->pml.Ln;
	size_t count = areaPoints * edgeLength();
	size_t size = count * sizeof(Point3Dstruct);
	sumH = (Point3Dstruct *)malloc(size);
	sumE = (Point3Dstruct *)malloc(size);
	if (sumH == NULL || sumE == NULL)
	{
		ret = ERR_OUTOFMEMORY;
	}
	if (ret == ERR_OK)
	{
		for (size_t w = 0; w < count; w++)
		{
			sumH[w].x = sumH[w].y = sumH[w].z = 0;
			sumE[w].x = sumE[w].y = sumE[w].z = 0;
		}
		coeff = (PmlCoefficients **)malloc((pams->kmax + 1)*sizeof(PmlCoefficients *));
		if (coeff == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		else
		{
			for (unsigned int h = 0; h <= pams->kmax; h++)
			{
				coeff[h] = NULL;
			}
			for (unsigned int h = 0; h <= pams->kmax; h++)
			{
				//coefficients are defined for the 2-D area; the coefficients are the same along the length 
				coeff[h] = (PmlCoefficients *)malloc(areaPoints*sizeof(PmlCoefficients));
				if (coeff[h] == NULL)
				{
					ret = ERR_OUTOFMEMORY;
					break;
				}
			}
		}
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
///sides
PmlSide::PmlSide()
{

}
PmlSide::~PmlSide()
{
	if (coeff != NULL)
	{
		for (unsigned int h = 0; h <= pams->kmax; h++)
		{
			if (coeff[h] != NULL)
			{
				free(coeff[h]); coeff[h] = NULL;
			}
		}
		free(coeff);
	}
	if (sumH != NULL)
	{
		free(sumH);
	}
	if (sumE != NULL)
	{
		free(sumE);
	}
}
const char *PmlSide::name()
{
	switch (rangeIndex)
	{
	case SIDE_XL:
		return "SIDE_XL";
	case SIDE_XH:
		return "SIDE_XH";
	case SIDE_YL:
		return "SIDE_YL";
	case SIDE_YH:
		return "SIDE_YH";
	case SIDE_ZL:
		return "SIDE_ZL";
	case SIDE_ZH:
		return "SIDE_ZH";
	}
	throw;
}
size_t PmlSide::sideArea()
{
	switch (rangeIndex)
	{
	case SIDE_XL:
		return (pams->ny - pams->pml.Ln - pams->pml.Ln + 1)*(pams->nz - pams->pml.Ln - pams->pml.Ln + 1);
	case SIDE_XH:
		return (pams->ny - pams->pml.Ln - pams->pml.Ln + 1)*(pams->nz - pams->pml.Ln - pams->pml.Ln + 1);
	case SIDE_YL:
		return (pams->nz - pams->pml.Ln - pams->pml.Ln + 1)*(pams->nx - pams->pml.Ln - pams->pml.Ln + 1);
	case SIDE_YH:
		return (pams->nz - pams->pml.Ln - pams->pml.Ln + 1)*(pams->nx - pams->pml.Ln - pams->pml.Ln + 1);
	case SIDE_ZL:
		return (pams->nx - pams->pml.Ln - pams->pml.Ln + 1)*(pams->ny - pams->pml.Ln - pams->pml.Ln + 1);
	case SIDE_ZH:
		return (pams->nx - pams->pml.Ln - pams->pml.Ln + 1)*(pams->ny - pams->pml.Ln - pams->pml.Ln + 1);
	}
	throw;
}
//range definition
unsigned int PmlSide::iStart()
{
	switch (rangeIndex)
	{
	case SIDE_XL:
		return 0;
	case SIDE_XH:
		return pams->nx - pams->pml.Ln + 1;
	case SIDE_YL:
		return pams->pml.Ln;
	case SIDE_YH:
		return pams->pml.Ln;
	case SIDE_ZL:
		return pams->pml.Ln;
	case SIDE_ZH:
		return pams->pml.Ln;
	}
	throw;
}
unsigned int PmlSide::iEnd()
{
	switch (rangeIndex)
	{
	case SIDE_XL:
		return pams->pml.Ln - 1;
	case SIDE_XH:
		return pams->nx;
	case SIDE_YL:
		return pams->nx - pams->pml.Ln;
	case SIDE_YH:
		return pams->nx - pams->pml.Ln;
	case SIDE_ZL:
		return pams->nx - pams->pml.Ln;
	case SIDE_ZH:
		return pams->nx - pams->pml.Ln;
	}
	throw;
}
unsigned int PmlSide::jStart()
{
	switch (rangeIndex)
	{
	case SIDE_XL:
		return pams->pml.Ln;
	case SIDE_XH:
		return pams->pml.Ln;
	case SIDE_YL:
		return 0;
	case SIDE_YH:
		return pams->ny - pams->pml.Ln + 1;
	case SIDE_ZL:
		return pams->pml.Ln;
	case SIDE_ZH:
		return pams->pml.Ln;
	}
	throw;
}
unsigned int PmlSide::jEnd()
{
	switch (rangeIndex)
	{
	case SIDE_XL:
		return pams->ny - pams->pml.Ln;
	case SIDE_XH:
		return pams->ny - pams->pml.Ln;
	case SIDE_YL:
		return pams->pml.Ln - 1;
	case SIDE_YH:
		return pams->ny;
	case SIDE_ZL:
		return pams->ny - pams->pml.Ln;
	case SIDE_ZH:
		return pams->ny - pams->pml.Ln;
	}
	throw;
}
unsigned int PmlSide::kStart()
{
	switch (rangeIndex)
	{
	case SIDE_XL:
		return pams->pml.Ln;
	case SIDE_XH:
		return pams->pml.Ln;
	case SIDE_YL:
		return pams->pml.Ln;
	case SIDE_YH:
		return pams->pml.Ln;
	case SIDE_ZL:
		return 0;
	case SIDE_ZH:
		return pams->nz - pams->pml.Ln + 1;
	}
	throw;
}
unsigned int PmlSide::kEnd()
{
	switch (rangeIndex)
	{
	case SIDE_XL:
		return pams->nz - pams->pml.Ln;
	case SIDE_XH:
		return pams->nz - pams->pml.Ln;
	case SIDE_YL:
		return pams->nz - pams->pml.Ln;
	case SIDE_YH:
		return pams->nz - pams->pml.Ln;
	case SIDE_ZL:
		return pams->pml.Ln - 1;
	case SIDE_ZH:
		return pams->nz;
	}
	throw;
}
int PmlSide::onInitializePmlRange()
{
	int ret = ERR_OK;
	size_t areaPoints = sideArea();
	size_t count = areaPoints * pams->pml.Ln;
	size_t size = count * sizeof(Point3Dstruct);
	sumH = (Point3Dstruct *)malloc(size);
	sumE = (Point3Dstruct *)malloc(size);
	if (sumH == NULL || sumE == NULL)
	{
		ret = ERR_OUTOFMEMORY;
	}
	if (ret == ERR_OK)
	{
		for (size_t w = 0; w < count; w++)
		{
			sumH[w].x = sumH[w].y = sumH[w].z = 0;
			sumE[w].x = sumE[w].y = sumE[w].z = 0;
		}
		coeff = (PmlCoefficients **)malloc((pams->kmax + 1)*sizeof(PmlCoefficients *));
		if (coeff == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		else
		{
			for (unsigned int h = 0; h <= pams->kmax; h++)
			{
				coeff[h] = NULL;
			}
			for (unsigned int h = 0; h <= pams->kmax; h++)
			{
				//the coefficients are defined along the depth of the PML; 
				//the coefficients are the same for the whole area at the same PML depth
				coeff[h] = (PmlCoefficients *)malloc(pams->pml.Ln*sizeof(PmlCoefficients));
				if (coeff[h] == NULL)
				{
					ret = ERR_OUTOFMEMORY;
					break;
				}
			}
		}
	}
	return ret;
}
///////////////////////////////////////////////////
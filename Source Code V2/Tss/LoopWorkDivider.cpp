/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

divide a for(...) loop into multiple parts to be executed by multiple threads
********************************************************************/
#include "LoopWorkDivider.h"
#include <malloc.h>

LoopWorkDivider::LoopWorkDivider()
{
	works = NULL;
}


LoopWorkDivider::~LoopWorkDivider()
{
	if (works != NULL)
	{
		free(works); works = NULL;
	}
}

void LoopWorkDivider::initializeDivider(unsigned int nx0, unsigned int ny0, unsigned int nz0, unsigned int n)
{
	nx = nx0;
	ny = ny0;
	nz = nz0;
	N = n;
	if (nx == 0 || ny == 0 || nz == 0 || N == 0)
	{
		throw;
	}
	works = (LoopWorkStruct *)malloc(N*sizeof(LoopWorkStruct));
	if (works == NULL)
	{
		throw;
	}
	unsigned int ndx = (unsigned int)(nx / N);
	for (unsigned int i = 0; i < N; i++)
	{
		works[i].StartIndex = i*ndx;
		if (i < N - 1)
			works[i].EndIndex = (i + 1)*ndx - 1;
		else
			works[i].EndIndex = nx;
		works[i].StartOneDimIndex = (nz + 1)*(ny + 1)*(i*ndx);
	}
}

void LoopWorkDividerSymmetryZ::initializeDivider(unsigned int nx0, unsigned int ny0, unsigned int nz0, unsigned int n)
{
	unsigned int ic = nx / 2;
	nx = nx0;
	ny = ny0;
	nz = nz0;
	N = n;
	if (nx == 0 || ny == 0 || nz == 0 || N == 0)
	{
		throw;
	}
	works = (LoopWorkStruct *)malloc(N*sizeof(LoopWorkStruct));
	if (works == NULL)
	{
		throw;
	}
	unsigned int ndx = (unsigned int)(ic / N);
	for (unsigned int i = 0; i < N; i++)
	{
		works[i].StartIndex = i*ndx;
		if (i < N - 1)
			works[i].EndIndex = (i + 1)*ndx - 1;
		else
			works[i].EndIndex = ic;
		works[i].StartOneDimIndex = (nz + 1) * (i * ndx);
	}
}
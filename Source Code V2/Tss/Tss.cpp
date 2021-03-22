/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

********************************************************************/
#include "Tss.h"
#include <stdlib.h>

int tss_allocate2Darray(Array2Dstruct *a, unsigned int dr, unsigned int dc)
{
	int ret = ERR_OK;
	a->r = (Array1Dstruct *)malloc(dr*sizeof(Array1Dstruct));
	if (a->r != NULL)
	{
		for (unsigned int j = 0; j < dr; j++)
		{
			a->r[j].c = NULL;
		}
		for (unsigned int j = 0; j < dr; j++)
		{
			a->r[j].c = (double *)malloc(dc*sizeof(double));
			if (a->r[j].c == NULL)
			{
				ret = ERR_OUTOFMEMORY;
				break;
			}
		}
	}
	else
	{
		ret = ERR_OUTOFMEMORY;
	}
	return ret;
}

void tss_free2Darray(Array2Dstruct *a, unsigned int dr)
{
	if (a->r != NULL)
	{
		for (unsigned int j = 0; j < dr; j++)
		{
			if (a->r[j].c != NULL)
			{
				free(a->r[j].c);
				a->r[j].c = NULL;
			}
		}
		free(a->r);
		a->r = NULL;
	}
}

Array3Dstruct *tss_allocate3Darray(unsigned int da, unsigned int dr, unsigned int dc)
{
	int outOfMemory = 0;
	Array3Dstruct *a = (Array3Dstruct*)malloc(sizeof(Array3Dstruct));
	if (a != NULL)
	{
		a->a = (Array2Dstruct *)malloc(da*sizeof(Array2Dstruct));
		if (a->a == NULL)
		{
			free(a);
			a = NULL;
		}
		else
		{
			for (unsigned int i = 0; i < da; i++)
			{
				a->a[i].r = NULL;
			}
			for (unsigned int i = 0; i < da; i++)
			{
				int ret = tss_allocate2Darray(&(a->a[i]), dr, dc);
				if (ret == ERR_OUTOFMEMORY)
				{
					outOfMemory = 1;
					break;
				}
			}
			if (outOfMemory)
			{
				tss_free3Darray(a, da, dr);
				a = NULL;
			}
		}
	}
	return a;
}

void tss_free3Darray(Array3Dstruct *a, unsigned int da, unsigned int dr)
{
	if (a != NULL)
	{
		if (a->a != NULL)
		{
			for (unsigned int i = 0; i < da; i++)
			{
				tss_free2Darray(&(a->a[i]),dr);
			}
			free(a->a);
			a->a = NULL;
		}
	}
}

void copy2Darray(double *W, Array2Dstruct *A, unsigned int M)
{
	unsigned int mi = 0;
	for (unsigned int i = 0; i < M; i++)
	{
		for (unsigned int j = 0; j < M; j++)
		{
			A->r[i].c[j] = W[mi + j];
		}
		mi += M;
	}
}
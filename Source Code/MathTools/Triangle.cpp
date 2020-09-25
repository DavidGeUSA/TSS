/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
//#include "stdafx.h"
#include "MathTools.h"
#include "Triangle.h"
#include <malloc.h>

#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int fill_sin_cos(unsigned n, double *s, double *c, rsize_t size)
{
	int ret = ERR_OK;
	size_t m = 2*(n-1)*(n-1);
	if(size < m)
	{
		ret = ERR_MATH_MEM_SMALL;
	}
	else
	{
		s[0]=0.0;s[1]=sin(2.0*M_PI/(double)n);
		c[0]=1.0;c[1]=cos(2.0*M_PI/(double)n);
		for(size_t i=1,j=2;i<m;i++,j++)
		{
			s[j]=s[i]*c[1]+c[i]*s[1];
			c[j]=c[i]*c[1]-s[i]*s[1];
		}
	}
	return ret;
}

/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
//#include "stdafx.h"

#include "randomTools.h"
#include <malloc.h>

#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double rand_double() 
{
   return rand()/(double)RAND_MAX;
}
double rand_double2(double x, double y)
{
   return (y - x)*rand_double() + x;
}

int rand_int(int a, int b)
{
   return (int)((b - a + 1)*rand_double()) + a;
}

/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/

#include "DivergenceComparer.h"

#include <malloc.h>
#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  


DivergenceComparer::DivergenceComparer(double *divgE, double *divgH, DerivativeEstimatorAsymmetric *owner, FieldPoint3D *fields, double halfSpaceStep)
	:FieldStatisticsByDivergenceAsymmetric(owner, fields, halfSpaceStep),FieldStatisticsByDivergence(fields, halfSpaceStep)
{
	_divE = divgE;
	_divH = divgH;
}

void DivergenceComparer::MakeStatistics(FieldPoint3D *_fields, size_t index, int r)
{
	_divE[r] += abs(_divergE);
	_divH[r] += abs(_divergH);
}

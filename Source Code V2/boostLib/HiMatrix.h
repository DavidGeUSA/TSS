
#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/02/2020
Allrights reserved by David Ge

Modifications
Date            Author      Description
---------------------------------------------

********************************************************************/
class HiMatrixTools
{
private:
	void HIgetCofactor(void *inA, void *intemp, int p, int q, int n);
	void HIdeterminant(void *inA, void *outret, int n);
	void HIadjoint(void *inA, void *inadj, int N);
public:
	HiMatrixTools();
	void HIMatrixMultiply(void *inA, void *inB, void *inC, int N);
	bool HIinverse(void *inA, void *inInverse, int N);
	double HIinverseError(void *inA, void *inInverse, int N, double *errMax);
};

#ifndef __MATRIX_H__
#define __MATRIX_H__
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

double determinant(double *A, int n);
void adjoint(double *A, double *adj, int N);
bool inverse(double *A, double *inverse, int N);
void MatrixMultiply(double *A, double *B, double *C, int N);
bool IsIdentityMatrix(double *A, int N);
int matrixRankOf(double *mat, int R, int C);
int testInverseMatrix(int N);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

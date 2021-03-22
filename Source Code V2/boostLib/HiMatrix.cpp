/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/02/2020
Allrights reserved by David Ge

Modifications
Date            Author      Description
---------------------------------------------

********************************************************************/
#include <malloc.h>

#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  

#include "HiMatrix.h"

#include <boost/multiprecision/cpp_dec_float.hpp>
using namespace boost::multiprecision;
HiMatrixTools::HiMatrixTools()
{

}

// Function to get cofactor of A[p][q] in temp[][]. n is current
// dimension of A[][]
void HiMatrixTools::HIgetCofactor(void *inA, void *intemp, int p, int q, int n)
{
	int i = 0, j = 0;
	int rowx = 0, ix = 0;
	cpp_dec_float_100 *A = (cpp_dec_float_100 *)inA;
	cpp_dec_float_100 *temp = (cpp_dec_float_100*)intemp;
	// Looping for each element of the matrix
	for (int row = 0; row < n; row++)
	{
		if (row != p)
		{
			for (int col = 0; col < n; col++)
			{
				//  Copying into temporary matrix only those element
				//  which are not in given row and column
				if (col != q)
				{
					temp[ix + j] = A[rowx + col];//A[row][col];

					j++;
					// Row is filled, so increase row index and
					// reset col index
					if (j == n - 1)
					{
						j = 0;
						i++;
						ix += (n - 1);
					}
				}
			}
		}
		rowx += n;
	}
}

/* Recursive function for finding determinant of matrix.
n is current dimension of A[][]. */
void HiMatrixTools::HIdeterminant(void *inA,void *outret, int n)
{
	cpp_dec_float_100 D = 0; // Initialize result
	cpp_dec_float_100 *A = (cpp_dec_float_100 *)inA;
	cpp_dec_float_100 *ret = (cpp_dec_float_100 *)outret;
	//  Base case : if matrix contains single element
	if (n == 1)
	{
		ret[0] = A[0];
		return;
	}
	cpp_dec_float_100 det;
	cpp_dec_float_100 *temp = (cpp_dec_float_100 *)malloc((n - 1)*(n - 1)*sizeof(cpp_dec_float_100));// [N][N]; // To store cofactors
	if (temp == NULL)
	{
		throw;
	}
	int sign = 1;  // To store sign multiplier

	// Iterate for each element of first row
	for (int f = 0; f < n; f++)
	{
		// Getting Cofactor of A[0][f]
		HIgetCofactor(A, temp, 0, f, n);
		//D += sign * A[0][f] * determinant(temp, n - 1);
		if (sign > 0)
		{
			HIdeterminant(temp, &det, n - 1);
			D += A[f] * det;
		}
		else
		{
			HIdeterminant(temp, &det, n - 1);
			D -= A[f] * det;
		}
		// terms are to be added with alternate sign
		sign = -sign;
	}
	free(temp);
	ret[0] = D;
	return;
}

// Function to get adjoint of A[N][N] in adj[N][N].
void HiMatrixTools::HIadjoint(void *inA, void *inadj, int N)
{
	cpp_dec_float_100 *A = (cpp_dec_float_100 *)inA;
	cpp_dec_float_100 *adj = (cpp_dec_float_100 *)inadj;
	if (N == 1)
	{
		adj[0] = 1;
		return;
	}

	// temp is used to store cofactors of A[][]
	int sign = 1;
	int ix;
	cpp_dec_float_100 det;
	cpp_dec_float_100 *temp = (cpp_dec_float_100 *)malloc((N - 1)*(N - 1)*sizeof(cpp_dec_float_100));

	for (int i = 0; i<N; i++)
	{
		ix = 0;
		for (int j = 0; j<N; j++)
		{
			// Get cofactor of A[i][j]
			HIgetCofactor(A, temp, i, j, N);

			// sign of adj[j][i] positive if sum of row
			// and column indexes is even.
			sign = ((i + j) % 2 == 0) ? 1 : -1;

			// Interchanging rows and columns to get the
			// transpose of the cofactor matrix
			//adj[j+ix] = (sign)*(determinant(temp, N-1));
			if (sign > 0)
			{
				HIdeterminant(temp, &det, N - 1);
				adj[i + ix] = det;
			}
			else
			{
				HIdeterminant(temp, &det, N - 1);
				adj[i + ix] = -det;
			}
			ix += N;
		}

	}
	free(temp);
}

// Function to calculate and store inverse, returns false if
// matrix is singular
bool HiMatrixTools::HIinverse(void *inA, void *inInverse, int N)
{
	cpp_dec_float_100 *A = (cpp_dec_float_100 *)inA;
	cpp_dec_float_100 *Inverse = (cpp_dec_float_100 *)inInverse;
	// Find determinant of A[][]
	cpp_dec_float_100 det;
	double ddet;
	HIdeterminant(A, &det, N);
	ddet = det.convert_to<double>();
	if (abs(ddet) < 1.1e-10)
	{
		//cout << "Singular matrix, can't find its inverse";
		return false;
	}

	// Find adjoint
	cpp_dec_float_100 *adj = (cpp_dec_float_100 *)malloc(N*N*sizeof(cpp_dec_float_100));
	if (adj == NULL) throw;
	HIadjoint(A, adj, N);

	// Find Inverse using formula "inverse(A) = adj(A)/det(A)"
	int ix = 0;
	for (int i = 0; i<N; i++)
	{
		for (int j = 0; j<N; j++)
		{
			Inverse[ix + j] = adj[ix + j] / det;
		}
		ix += N;
	}
	free(adj);
	return true;
}

void HiMatrixTools::HIMatrixMultiply(void *inA, void *inB, void *inC, int N)
{
	int i, j, ix, k, kx;
	cpp_dec_float_100 *A = (cpp_dec_float_100 *)inA;
	cpp_dec_float_100 *B = (cpp_dec_float_100 *)inB;
	cpp_dec_float_100 *C = (cpp_dec_float_100 *)inC;
	ix = 0;
	for (i = 0; i<N; i++)
	{
		for (j = 0; j<N; j++)
		{
			C[ix + j] = 0.0;
			kx = 0;
			for (k = 0; k<N; k++)
			{
				C[ix + j] += A[ix + k] * B[kx + j];
				kx += N;
			}
		}
		ix += N;
	}
}
double HiMatrixTools::HIinverseError(void *inA, void *inInverse, int N, double *errMax)
{
	double err = 0.0;
	size_t k;
	cpp_dec_float_100 *A = (cpp_dec_float_100 *)inA;
	cpp_dec_float_100 *B = (cpp_dec_float_100 *)inInverse;
	cpp_dec_float_100 *C = (cpp_dec_float_100 *)malloc(N*N*sizeof(cpp_dec_float_100));
	if (C == NULL)
		throw;
	cpp_dec_float_100 e0,em = 0.0;
	HIMatrixMultiply(A, B, C, N);
	k = 0;
	for ( int i = 0; i < N; i++)
	{
		for ( int j = 0; j < N; j++)
		{
			if (i == j)
			{
				e0 = 1.0 - C[k];
			}
			else
			{
				e0 = C[k];
			}
			err += abs(e0.convert_to<double>());
			if (e0 > em)
			{
				em = e0;
			}
			k++;
		}
	}
	free(C);
	*errMax = em.convert_to<double>();
	return err;
}
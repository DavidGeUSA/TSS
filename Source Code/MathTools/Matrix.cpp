/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/
//#include "stdafx.h"

#include "MathTools.h"

#include "Matrix.h"
#include <malloc.h>

#include <math.h>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to get cofactor of A[p][q] in temp[][]. n is current
// dimension of A[][]
void getCofactor(double *A, double *temp, int p, int q, int n)
{
    int i = 0, j = 0;
	int rowx = 0, ix = 0;
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
					temp[ix+j] = A[rowx+col];//A[row][col];
 
					j++;
					// Row is filled, so increase row index and
					// reset col index
					if (j == n - 1)
					{
						j = 0;
						i++; 
						ix += (n-1);
					}
				}
			}
		}
		rowx += n;
    }
}

/* Recursive function for finding determinant of matrix.
   n is current dimension of A[][]. */
double determinant(double *A, int n)
{
    double D = 0; // Initialize result
 
    //  Base case : if matrix contains single element
    if (n == 1)
        return A[0];
 
    double *temp = (double *)malloc((n-1)*(n-1)*sizeof(double));// [N][N]; // To store cofactors
 
    int sign = 1;  // To store sign multiplier
 
     // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of A[0][f]
        getCofactor(A, temp, 0, f, n);
        //D += sign * A[0][f] * determinant(temp, n - 1);
		if(sign > 0)
			D += A[f] * determinant(temp, n - 1);
		else
			D -= A[f] * determinant(temp, n - 1);
        // terms are to be added with alternate sign
        sign = -sign;
    }
    free(temp);
    return D;
}

// Function to get adjoint of A[N][N] in adj[N][N].
void adjoint(double *A, double *adj, int N)
{
    if (N == 1)
    {
        adj[0] = 1;
        return;
    }
 
    // temp is used to store cofactors of A[][]
    int sign = 1;
	int ix;
	double *temp = (double *)malloc((N-1)*(N-1)*sizeof(double));
 
    for (int i=0; i<N; i++)
    {
		ix = 0;
        for (int j=0; j<N; j++)
        {
            // Get cofactor of A[i][j]
            getCofactor(A, temp, i, j, N);
 
            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i+j)%2==0)? 1: -1;
 
            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
			//adj[j+ix] = (sign)*(determinant(temp, N-1));
			if(sign > 0)
				adj[i+ix] = (determinant(temp, N-1));
			else
				adj[i+ix] = -(determinant(temp, N-1));
			ix += N;
        }
		
    }
	free(temp);
}

// Function to calculate and store inverse, returns false if
// matrix is singular
bool inverse(double *A, double *Inverse, int N)
{
    // Find determinant of A[][]
    double det = determinant(A, N);
    if (abs(det) < 1.1e-10)
    {
        //cout << "Singular matrix, can't find its inverse";
        return false;
    }
 
    // Find adjoint
    double *adj = (double *)malloc(N*N*sizeof(double));
    adjoint(A, adj, N);
 
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
	int ix = 0;
    for (int i=0; i<N; i++)
	{
        for (int j=0; j<N; j++)
		{
            Inverse[ix + j] = adj[ix + j]/det;
		}
		ix += N;
	}
    return true;
}

void MatrixMultiply(double *A, double *B, double *C, int N)
{
	int i, j, ix, k,kx;
	ix = 0;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			C[ix+j] = 0.0;
			kx = 0;
			for(k=0;k<N;k++)
			{
				C[ix+j] += A[ix+k] * B[kx+j];
				kx += N;
			}
		}
		ix += N;
	}
}
bool IsIdentityMatrix(double *A, int N)
{
	int ix = 0;
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			if(i == j)
			{
				if(abs(1.0-A[ix+j])>1.1e-8)
				{
					return false;
				}
			}
			else
			{
				if(abs(A[ix+j])>1.1e-8)
				{
					return false;
				}
			}
		}
		ix += N;
	}
	return true;
}
/* function for exchanging two rows of
   a matrix */
void matrixSwapRow(double *mat, int row1, int row2, int col, int R)
{
	int row1x = row1 * col;
	int row2x = row2 * col;
    for (int i = 0; i < col; i++)
    {
        double temp = mat[row1x + i];
        mat[row1x + i] = mat[row2x + i];
        mat[row2x + i] = temp;
    }
}
/* function for displaying the matrix */
void matrixDisplay(double *mat, int row, int col)
{
	int ix = 0;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            printf("  %g", mat[ix + j]);
        printf("\n");
		ix += col;
    }
}
/* function for finding rank of matrix */
int matrixRankOf(double *mat, int R, int C)
{
	double eps = 1.0e-09;
    int rank = C;
	int rowx = 0;
	//
    for (int row = 0; row < rank; row++)
    {
        // Before we visit current row 'row', we make
        // sure that mat[row][0],....mat[row][row-1]
        // are 0.
        // Diagonal element is not zero
        if (abs(mat[rowx + row]) > eps) //note that "row" is less than rank <= C
        {
			int rx = 0;
			for (int r = 0; r < R; r++)
			{
				if (r != row)
				{
					// This makes all entries of current
					// column as 0 except entry 'mat[row][row]'
					double mult = mat[rx + row] / mat[rowx + row];
					for (int i = 0; i < rank; i++)
					{
						mat[rx + i] -= mult * mat[rowx + i];
					}
				}
				rx += C;
			}
		}
		else
		{
			// Diagonal element is already zero. Two cases
			// arise:
			// 1) If there is a row below it with non-zero
			//    entry, then swap this row with that row
			//    and process that row
			// 2) If all elements in current column below
			//    mat[r][row] are 0, then remvoe this column
			//    by swapping it with last column and
			//    reducing number of columns by 1.
			bool reduce = true;
			int ix;
			/* Find the non-zero element in current
				column  */
			for (int i = row + 1, ix=i*C; i < R;  i++, ix += C)
			{
				// Swap the row with non-zero element
				// with this row.
				if (abs(mat[ix + row]) > eps)
				{
					//matrixSwapRow(mat, row, i, rank, R);
					matrixSwapRow(mat, row, i, C, R);
					reduce = false;
					break ;
				}
			}
 
			// If we did not find any row with non-zero
			// element in current columnm, then all
			// values in this column are 0.
			if (reduce)
			{
				// Reduce number of columns
				rank--;
				// Copy the last column here
				ix = 0;
				for (int i = 0; i < R; i ++)
				{
					mat[ix + row] = mat[ix + rank];
					ix += C;
				}
			}
			// Process this row again
			row--;
			rowx -= C;
		}
		// Uncomment these lines to see intermediate results
		matrixDisplay(mat, R, C);
		printf("\n");
		rowx += C;
	}
	return rank;
}
int testInverseMatrix(int N)
{
	int ret = ERR_OK;
	if(N<=0)
	{
		ret = ERR_MATH_INV_DIM;
	}
	else
	{
		int N2 = N*N;
		size_t size=N2*sizeof(double);
		double *A = (double *)malloc(size);
		if(!A)
		{
			ret = ERR_MATH_OUTOFMEMORY;
		}
		else
		{
			double *Inverse = (double *)malloc(size);
			if(!Inverse)
			{
				ret = ERR_MATH_OUTOFMEMORY;
			}
			else
			{
				double *Verify = (double *)malloc(size);
				if(!Verify)
				{
					ret = ERR_MATH_OUTOFMEMORY;
				}
				else
				{
					int i,j,ix;
					ix = 0;
					for(i=0;i<N;i++)
					{
						for(j=0;j<N;j++)
						{
							A[ix+j] = rand()/(double)RAND_MAX;
						}
						ix += N;
					}
					if(inverse(A, Inverse, N))
					{
						MatrixMultiply(A,Inverse,Verify,N);
						if(IsIdentityMatrix(Verify, N))
						{
						}
						else
						{
							ret = ERR_MATH_NOT_INVERSE;
						}
					}
					else
					{
						ret = ERR_MATH_DET_0;
					}
					free(Verify);
				}
				free(Inverse);
			}
			free(A);
		}
	}
	return ret;
}


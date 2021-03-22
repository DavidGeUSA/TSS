/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

********************************************************************/
#include "Space.h"
#include "Curl.h"
#include "../MathTools/Matrix.h"
#include "../FileUtil/fileutil.h"
#include <stdlib.h>

#include "../boostLib/HiMatrix.h"
#include <boost/multiprecision/cpp_dec_float.hpp>

Space::Space()
{
	ds = 0.0;
	nx = ny = nz = 0;
	nx1 = ny1 = nz1 = 0;
	cellCount = 0;
	A = NULL;
	smax = M = 0;
	fieldMemorySize = 0;
	curlH = curlE = NULL;
}


Space::~Space()
{
	cleanup();
}

void Space::cleanup()
{
	tss_free3Darray(A, M + 1, M);
	if (curlH != NULL)
	{
		delete curlH;
		curlH = NULL;
	}
	if (curlE != NULL)
	{
		delete curlE;
		curlE = NULL;
	}
}

void Space::setCalculationMethod(CURLMETHOD method)
{
	curlE->setCalculationMethod(method);
	curlH->setCalculationMethod(method);
}

inline size_t Space::Idx(unsigned i, unsigned j, unsigned k)
{
	return k + nz1 * (j + ny1 * i);
}

/*
	generate space derivative estimation matrixes
*/
int Space::generateMatrix()
{
	int ret = ERR_OK;
	using namespace boost::multiprecision;
	cpp_dec_float_100 *W = (cpp_dec_float_100 *)malloc(M*M*sizeof(cpp_dec_float_100));
	cpp_dec_float_100 *Ai = (cpp_dec_float_100 *)malloc(M*M*sizeof(cpp_dec_float_100));
	double *AI = (double *)malloc(M*M*sizeof(double));
	A = tss_allocate3Darray(M + 1, M, M);
	if (AI == NULL || W == NULL || Ai == NULL || A == NULL)
	{
		ret = ERR_OUTOFMEMORY;
	}
	if (ret == ERR_OK)
	{
		HiMatrixTools HiMatrix;
		unsigned int P = smax;
		unsigned int N = smax;
		unsigned int mi;
		cpp_dec_float_100 X = 1.0;
		cpp_dec_float_100 Dn = 1.0;
		cpp_dec_float_100 Dx;
		unsigned int i;
		unsigned int w = 0;
		size_t k;
		for (w = 0; w <= 2 * smax; w++)
		{
			if (w <= smax)
			{
				P = smax + w;
				N = smax - w;
			}
			else
			{
				P = smax - (w - smax);
				N = smax + (w - smax);
			}
			mi = 0;
			k = 0;
			i = 0;
			while (i < P)
			{
				Dn = ((double)i + 1.0);
				Dx = Dn;
				X = 1.0;
				for (unsigned int j = 0; j < M; j++)
				{
					X = X*((double)j + 1.0);
					W[k] = Dx / X;
					Dx = Dx * Dn;
					k++;
				}
				mi += M;
				i++;
			}
			i = 0;
			while (i < N)
			{
				Dn = -((double)i + 1.0);
				Dx = Dn;
				X = 1.0;
				for (unsigned int j = 0; j < M; j++)
				{
					X = X*((double)j + 1.0);
					W[k] = Dx / X;
					Dx = Dx * Dn;
					k++;
				}
				mi += M;
				i++;
			}
			if (HiMatrix.HIinverse(W, Ai, (int)M))
			{
				k = 0;
				for (i = 0; i < M; i++)
				{
					for (unsigned int j = 0; j < M; j++)
					{
						AI[k] = Ai[k].convert_to<double>();
						k++;
					}
				}
				copy2Darray(AI, &(A->a[w]), M);
			}
			else
			{
				ret = ERR_INVERSEMATRIX;
				break;
			}
		}
	}
	if(AI != NULL) free(AI);
	if (W != NULL) free(W);
	if (Ai != NULL) free(Ai);
	return ret;
}

int Space::initializeSpace(double spaceStep, unsigned int inx, unsigned int iny, unsigned int inz, unsigned int ismax, const char *matrixFile)
{
	int ret = ERR_OK;
	ds = spaceStep;
	nx = inx; ny = iny; nz = inz;
	nx1 = inx+1; ny1 = iny+1; nz1 = inz+1;
	smax = ismax;
	M = 2 * smax;
	if (ds <= 0.0 || nx == 0 || ny == 0 || nz == 0 || smax == 0)
	{
		ret = ERR_INVALID_PARAM;
	}
	else
	{
		if (matrixFile != NULL && fileexists(matrixFile))
		{
			A = LoadEstimationMatrixesFromFile(matrixFile, M, &ret);
		}
		else
		{
			ret = generateMatrix();
		}
	}
	if (ret == ERR_OK)
	{
		cellCount = (nx + 1)*(ny + 1)*(nz + 1);
		fieldMemorySize = cellCount*sizeof(Point3Dstruct);
	}
	if (ret == ERR_OK)
	{
		curlH = new Curl();
		ret = curlH->initialize(this);
		if (ret == ERR_OK)
		{
			curlE = new Curl();
			ret = curlE->initialize(this);
			if (ret == ERR_OK)
			{
				//hard coded here as default
				setCalculationMethod(ByFastMethodFirstOrder);
			}
		}
	}
	return ret;
}

////////3 functions for dFx/dx,dy,dz/////////////////////////////////////
double Space::dx_Fx(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k)
{
	double d = 0.0;
	unsigned int P, N;
	unsigned int m;
	size_t w0 = Idx(i, j, k);
	int w = smax - i;
	if (w > 0) //near or at the lower boundary: w=1,2,...,smax; i=smax-1, smax-2,...,0
	{
		P = smax + w;
		N = smax - w;
	}
	else
	{
		w = i + smax - nx;
		if (w > 0) //near or at the upper boundary: w=1,2,...,smax; i=nx-smax+1, nx-smax+2,...,nx
		{
			P = smax - w;
			N = smax + w;
			w = w + smax; //index into the array
		}
		else
		{
			P = smax;
			N = smax;
			w = 0;
		}
	}
	//
	h = h - 1;
	for (m = 1; m <= P; m++)
	{
		d += A->a[w].r[h].c[m - 1] * (F[Idx(i + m,j,k)].x - F[w0].x);
	}
	for (m = 1; m <= N; m++)
	{
		d += A->a[w].r[h].c[P + m - 1] * (F[Idx(i - m,j,k)].x - F[w0].x);
	}
	return d;
}

double Space::dy_Fx(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k)
{
	double d = 0.0;
	unsigned int P, N;
	unsigned int m;
	size_t w0 = Idx(i, j, k);
	int w = smax - j;
	if (w > 0)
	{
		P = smax + w;
		N = smax - w;
	}
	else
	{
		w = j + smax - ny;
		if (w > 0)
		{
			P = smax - w;
			N = smax + w;
			w = w + smax;
		}
		else
		{
			P = smax;
			N = smax;
			w = 0;
		}
	}
	//
	h = h - 1;
	for (m = 1; m <= P; m++)
	{
		d += A->a[w].r[h].c[m - 1] * (F[Idx(i,j + m,k)].x - F[w0].x);
	}
	for (m = 1; m <= N; m++)
	{
		d += A->a[w].r[h].c[P + m - 1] * (F[Idx(i,j - m,k)].x - F[w0].x);
	}
	return d;
}

double Space::dz_Fx(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k)
{
	double d = 0.0;
	unsigned int P, N;
	unsigned int m;
	size_t w0 = Idx(i, j, k);
	int w = smax - k;
	if (w > 0)
	{
		P = smax + w;
		N = smax - w;
	}
	else
	{
		w = k + smax - nz;
		if (w > 0)
		{
			P = smax - w;
			N = smax + w;
			w = w + smax;
		}
		else
		{
			P = smax;
			N = smax;
			w = 0;
		}
	}
	//
	h = h - 1;
	for (m = 1; m <= P; m++)
	{
		d += A->a[w].r[h].c[m - 1] * (F[Idx(i,j,k + m)].x - F[w0].x);
	}
	for (m = 1; m <= N; m++)
	{
		d += A->a[w].r[h].c[P + m - 1] * (F[Idx(i,j,k - m)].x - F[w0].x);
	}
	return d;
}
/////////--end of dFx /dx.dy.dz/////////////////////////////////////////
/////////--start of dFy /dx.dy.dz/////////////////////////////////////////
double Space::dx_Fy(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k)
{
	double d = 0.0;
	unsigned int P, N;
	unsigned int m;
	size_t w0 = Idx(i, j, k);
	int w = smax - i;
	if (w > 0)
	{
		P = smax + w;
		N = smax - w;
	}
	else
	{
		w = i + smax - nx;
		if (w > 0)
		{
			P = smax - w;
			N = smax + w;
			w = w + smax;
		}
		else
		{
			P = smax;
			N = smax;
			w = 0;
		}
	}
	//
	h = h - 1;
	for (m = 1; m <= P; m++)
	{
		d += A->a[w].r[h].c[m - 1] * (F[Idx(i + m,j,k)].y - F[w0].y);
	}
	for (m = 1; m <= N; m++)
	{
		d += A->a[w].r[h].c[P + m - 1] * (F[Idx(i - m,j,k)].y - F[w0].y);
	}
	return d;
}

double Space::dy_Fy(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k)
{
	double d = 0.0;
	unsigned int P, N;
	unsigned int m;
	size_t w0 = Idx(i, j, k);
	int w = smax - j;
	if (w > 0)
	{
		P = smax + w;
		N = smax - w;
	}
	else
	{
		w = j + smax - ny;
		if (w > 0)
		{
			P = smax - w;
			N = smax + w;
			w = w + smax;
		}
		else
		{
			P = smax;
			N = smax;
			w = 0;
		}
	}
	//
	h = h - 1;
	for (m = 1; m <= P; m++)
	{
		d += A->a[w].r[h].c[m - 1] * (F[Idx(i,j + m,k)].y - F[w0].y);
	}
	for (m = 1; m <= N; m++)
	{
		d += A->a[w].r[h].c[P + m - 1] * (F[Idx(i,j - m,k)].y - F[w0].y);
	}
	return d;
}

double Space::dz_Fy(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k)
{
	double d = 0.0;
	unsigned int P, N;
	unsigned int m;
	size_t w0 = Idx(i, j, k);
	int w = smax - k;
	if (w > 0)
	{
		P = smax + w;
		N = smax - w;
	}
	else
	{
		w = k + smax - nz;
		if (w > 0)
		{
			P = smax - w;
			N = smax + w;
			w = w + smax;
		}
		else
		{
			P = smax;
			N = smax;
			w = 0;
		}
	}
	//
	h = h - 1;
	for (m = 1; m <= P; m++)
	{
		d += A->a[w].r[h].c[m - 1] * (F[Idx(i,j,k + m)].y - F[w0].y);
	}
	for (m = 1; m <= N; m++)
	{
		d += A->a[w].r[h].c[P + m - 1] * (F[Idx(i,j,k - m)].y - F[w0].y);
	}
	return d;
}
/////////--end of dFy /dx.dy.dz/////////////////////////////////////////
/////////--start of dFz /dx.dy.dz/////////////////////////////////////////
double Space::dx_Fz(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k)
{
	double d = 0.0;
	unsigned int P, N;
	unsigned int m;
	size_t w0 = Idx(i, j, k);
	int w = smax - i;
	if (w > 0)
	{
		P = smax + w;
		N = smax - w;
	}
	else
	{
		w = i + smax - nx;
		if (w > 0)
		{
			P = smax - w;
			N = smax + w;
			w = w + smax;
		}
		else
		{
			P = smax;
			N = smax;
			w = 0;
		}
	}
	//
	h = h - 1;
	for (m = 1; m <= P; m++)
	{
		d += A->a[w].r[h].c[m - 1] * (F[Idx(i + m,j,k)].z - F[w0].z);
	}
	for (m = 1; m <= N; m++)
	{
		d += A->a[w].r[h].c[P + m - 1] * (F[Idx(i - m,j,k)].z - F[w0].z);
	}
	return d;
}

double Space::dy_Fz(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k)
{
	double d = 0.0;
	unsigned int P, N;
	unsigned int m;
	size_t w0 = Idx(i, j, k);
	int w = smax - j;
	if (w > 0)
	{
		P = smax + w;
		N = smax - w;
	}
	else
	{
		w = j + smax - ny;
		if (w > 0)
		{
			P = smax - w;
			N = smax + w;
			w = w + smax;
		}
		else
		{
			P = smax;
			N = smax;
			w = 0;
		}
	}
	//
	h = h - 1;
	for (m = 1; m <= P; m++)
	{
		d += A->a[w].r[h].c[m - 1] * (F[Idx(i,j + m,k)].z - F[w0].z);
	}
	for (m = 1; m <= N; m++)
	{
		d += A->a[w].r[h].c[P + m - 1] * (F[Idx(i,j - m,k)].z - F[w0].z);
	}
	return d;
}

double Space::dz_Fz(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k)
{
	double d = 0.0;
	unsigned int P, N;
	unsigned int m;
	size_t w0 = Idx(i, j, k);
	int w = smax - k;
	if (w > 0)
	{
		P = smax + w;
		N = smax - w;
	}
	else
	{
		w = k + smax - nz;
		if (w > 0)
		{
			P = smax - w;
			N = smax + w;
			w = w + smax;
		}
		else
		{
			P = smax;
			N = smax;
			w = 0;
		}
	}
	//
	h = h - 1;
	for (m = 1; m <= P; m++)
	{
		d += A->a[w].r[h].c[m - 1] * (F[Idx(i,j,k + m)].z - F[w0].z);
	}
	for (m = 1; m <= N; m++)
	{
		d += A->a[w].r[h].c[P + m - 1] * (F[Idx(i,j,k - m)].z - F[w0].z);
	}
	return d;
}
/////////--end of dFz /dx.dy.dz/////////////////////////////////////////

void Space::SetFields(Point3Dstruct *fh, Point3Dstruct *fe)
{
	curlH->SetField(fh);
	curlE->SetField(fe);
}

int Space::CalculateNextCurlH(Point3Dstruct *pNext)
{
	return curlH->CalculateCurl(pNext);
}

int Space::CalculateNextCurlE(Point3Dstruct *pNext)
{
	return curlE->CalculateCurl(pNext);
}

Point3Dstruct *Space::GetCurrentCurlH()
{
	return curlH->GetCurrentCurl();
}

Point3Dstruct *Space::GetCurrentCurlE()
{
	return curlE->GetCurrentCurl();
}

int Space::writeMatrixesToFile(char *file)
{
	int ret = ERR_OK;
	FILE *retFileHandle = 0;
	ret = deleteFile(file);
	if (ret == ERR_OK)
	{
		ret = openTextfileWrite(file, &retFileHandle);
		if (ret == ERR_OK)
		{
			if (retFileHandle == 0)
			{
				ret = ERR_FILE_OPEN_WRIT_EINVAL;
			}
			else
			{
				unsigned int size = 1024;
				char buff[1024];
				Array3Dstruct *A = GetMaxtrixes(); //1+2smax,2smax,2smax
				sprintf_1(buff, size, "smax=%u, space estiation order = 2*smax = %u\r\n", smax, 2 * smax);
				writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
				for (unsigned int h = 0; h < 2 * smax + 1; h++)
				{
					if (h == 0)
					{
						sprintf_1(buff, size, "\r\nestimation matrix for spaces inside the boundary [%u x %u]=\n", 2 * smax, 2 * smax);
					}
					else if (h <= smax)
					{
						sprintf_1(buff, size, "\r\nestimation matrix for spaces near or at the lower boundary [%u x %u]=\n", 2 * smax, 2 * smax);
					}
					else
					{
						sprintf_1(buff, size, "\r\nestimation matrix for spaces near or at the upper boundary [%u x %u]=\n", 2 * smax, 2 * smax);
					}
					writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
					//
					for (unsigned int i = 0; i < 2 * smax; i++)
					{
						for (unsigned int j = 0; j < 2 * smax; j++)
						{
							if (j == 2 * smax - 1)
							{
								sprintf_1(buff, size, "%g\n", A->a[h].r[i].c[j]);
							}
							else
							{
								sprintf_1(buff, size, "%g,\t", A->a[h].r[i].c[j]);
							}
							writefile(retFileHandle, buff, (unsigned int)strnlen_0(buff, size));
						}
					}
				}
				closefile(retFileHandle);
			}
		}
	}
	return ret;
}

int Space::saveMaxtrixToFile(char *file)
{
	int ret = ERR_OK;
	FILE *fh = 0;
	size_t w;
	size_t size = (M + 1)*M*M*sizeof(double);
	double *data = (double *)malloc(size);
	if (data == NULL)
	{
		ret = ERR_OUTOFMEMORY;
	}
	else
	{
		w = 0;
		for (unsigned int h = 0; h <= M; h++)
		{
			for (unsigned int i = 0; i < M; i++)
			{
				for (unsigned int j = 0; j < M; j++)
				{
					data[w] = A->a[h].r[i].c[j];
					w++;
				}
			}
		}
		ret = openfileWrite(file, &fh);
		if (ret == ERR_OK)
		{
			ret = writefile(fh, data, (unsigned int)size);
			closefile(fh);
		}
		free(data);
	}
	return ret;
}

//int Space::loadMaxtrixFromFile(const char *file)
Array3Dstruct *Space::LoadEstimationMatrixesFromFile(const char *file, unsigned int M, int *ret)
{
	*ret = ERR_OK;
	FILE *fh = 0;
	Array3Dstruct *A = NULL;
	size_t w;
	size_t size = (M + 1)*M*M*sizeof(double);
	double *data = (double *)malloc(size);
	if (data == NULL)
	{
		*ret = ERR_OUTOFMEMORY;
	}
	else
	{
		size_t fsize = fileSize(file);
		if (fsize != size)
		{
			*ret = ERR_INVALID_SIZE;
		}
	}
	if (*ret == ERR_OK)
	{
		*ret = openfileRead(file, &fh);
		if (*ret == ERR_OK)
		{
			*ret = readfile(fh, data, (unsigned int)size);
			closefile(fh);
		}
	}
	if (*ret == ERR_OK)
	{
		A = tss_allocate3Darray(M + 1, M, M);
		if (A == NULL)
		{
			*ret = ERR_OUTOFMEMORY;
		}
	}
	if (*ret == ERR_OK)
	{
		w = 0;
		for (unsigned int h = 0; h <= M; h++)
		{
			for (unsigned int i = 0; i < M; i++)
			{
				for (unsigned int j = 0; j < M; j++)
				{
					A->a[h].r[i].c[j] = data[w];
					w++;
				}
			}
		}
	}
	if (data != NULL)
		free(data);
	return A;
}

int Space::CreateMatrixFile(unsigned int smax0, char *file)
{
	int ret = ERR_OK;
	smax = smax0;
	M = 2 * smax;
	ret = generateMatrix();
	if (ret == ERR_OK)
	{
		ret = saveMaxtrixToFile(file);
	}
	return ret;
}

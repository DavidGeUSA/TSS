#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

space module for curl estimations

********************************************************************/
#include "Tss.h"

#include "../MemoryMan/MemoryManager.h"

/*
	different ways of calculating curls
*/
typedef enum CURLMETHOD
{
	ByFirstOrder           = 1, //use first order calculated by the first row of the space estimation matrix
	ByHigherOrder          = 2, //calculated by all rows of the space estimation matrix (only up to the second row at this implementation)
	ByFastMethod           = 3, //optimized for speed up to the second order
	ByFastMethodFirstOrder = 4, //optimized for speed, only use the first order
}CURLMETHOD;


class Curl; //curl calculator

typedef struct FieldStatisticStruct
{
	unsigned int timeIndex;
	double maxDivergenceE;
	double averageDivergenceE;
	double maxDivergenceH;
	double averageDivergenceH;
	double energySum;
	double magSumE;
	double magSumH;
}FieldStatisticStruct;

/*
	calculate space derivative and curls
*/
class Space
{
private:
	double ds;                   //space step size
	unsigned int nx, ny, nz;     //computation domain. 0...nx; 0...ny; 0...nz
	unsigned int smax;           //half estimatin order. Estimation order = 2*smax
	unsigned int M;              //estimation order = 2*smax
	size_t cellCount;            //(nx+1)*(ny+1)*(nz+1)
	size_t fieldMemorySize;      //(nx+1)*(ny+1)*(nz+1)*sizeof(Point3Dstruct)
	Array3Dstruct *A;            //derivative estimation matrices dim: 1+2smax,2smax,2smax 
	unsigned int nx1, ny1, nz1;  //nx1=nx+1; ny1=ny+1; nz1=nz+1
	//
	//curl calculators
	Curl *curlH;
	Curl *curlE;
	//
	int generateMatrix();
	int saveMaxtrixToFile(char *file);
	int loadMaxtrixFromFile(const char *file);
public:
	Space();
	~Space();
	void cleanup();
	void setCalculationMethod(CURLMETHOD method);
	//
	static Array3Dstruct *LoadEstimationMatrixesFromFile(const char *file, unsigned int M, int *ret);
	//
	size_t Idx(unsigned i, unsigned j, unsigned k);
	//
	int CreateMatrixFile(unsigned int smax0, char *file);
	//
	int initializeSpace(double spaceStep, unsigned int inx, unsigned int iny, unsigned int inz, unsigned int ismax, const char *matrixFile);
	//
	Array3Dstruct *GetMaxtrixes(){ return A; }
	//
	//derivatives
	double dx_Fx(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k);
	double dy_Fx(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k);
	double dz_Fx(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k);
	double dx_Fy(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k);
	double dy_Fy(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k);
	double dz_Fy(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k);
	double dx_Fz(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k);
	double dy_Fz(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k);
	double dz_Fz(Point3Dstruct *F, unsigned int h, unsigned int i, unsigned int j, unsigned int k);
	//
	unsigned int Nx(){ return nx; }
	unsigned int Ny(){ return ny; }
	unsigned int Nz(){ return nz; }
	double DS(){ return ds; }
	unsigned int SMAX() { return smax; }
	unsigned int GetEstimationOrder(){ return M; }
	size_t GetCellCount(){ return cellCount; }
	size_t GetFieldMemorySize(){ return fieldMemorySize; }
	void SetFields(Point3Dstruct *fh, Point3Dstruct *fe);
	int writeMatrixesToFile(char *file);
	//curls
	Curl *GetCurlE(){ return curlE; } //for testing
	int CalculateNextCurlH(Point3Dstruct *pNext);
	int CalculateNextCurlE(Point3Dstruct *pNext);
	Point3Dstruct *GetCurrentCurlH();
	Point3Dstruct *GetCurrentCurlE();
};


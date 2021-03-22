#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

********************************************************************/
#include "Space.h"
class Space;

/*
	calculating curls
*/
class Curl
{
private:
	unsigned int nx, ny, nz;     //computation domain. 0...nx; 0...ny; 0...nz
	Space *space;                //space derivative estimator
	Point3Dstruct *pCurrent;     //current curl
	//
	//these values are only needed for calculating curls using higher order detivatives
	unsigned int M;              //estimation order
	unsigned int CurlNumber;     //current curl number = 0,1,2,3,...
	unsigned int CurlK;          //CurlNumber=2*CurlK or CurlNumber=2*CurlK+1
	bool isEven;                 //true: CurlNumber=2*CurlK; false: CurlNumber=2*CurlK+1
	bool isKeven;                //indicates if CurlK is even
	Point3Dstruct *Field;        //field: keep it for calculating higher order derivatives 
	//
	unsigned int nxN1, nyN1, nzN1; //nx-1, ny-1, nz-1
	unsigned int nx1, ny1, nz1;    //nx+1, ny+1, nz+1
	//
	Point3Dstruct *work;
	//
	//work variables
	unsigned int i, j, k;
	size_t w;
	//
protected:
	CURLMETHOD _calculationMethod;
	/*
	for sharply changing fields, using higher order derivatives to get
	higher order curls can more accurate, because fewer space points are used.
	this implementation only use the second order derivatives.
	*/
	int _getNextCurlUseHighOrder(Point3Dstruct * pNext);
	/*
	for not sharply changing fields, using 1st derivatives to get higher order curls
	can be more accurate, because more space points are used
	*/
	int _getNextCurlUseFirstOrder(Point3Dstruct *pNext);
	/*
	calculate 1 to 3 curls directly from the field without calculating derivatives first
	*/
	int _getNextCurlUseFastMethod(Point3Dstruct *pNext);
	/*
	calculate next curls optimized for speed
	*/
	int _getNextCurlUseFastFirstOrderMethod(Point3Dstruct *pNext);
public:
	Curl();
	~Curl();
	void cleanup();
	int initialize(Space *owner);
	void SetField(Point3Dstruct *field);
	void SetWorkField(Point3Dstruct *wk){ work = wk; }
	size_t I(unsigned int i, unsigned int j, unsigned int k){ return k + nz1*(j + ny1*i); }
	//
	unsigned int GetCurrentCurlNumber(){ return CurlNumber; }
	Point3Dstruct *GetCurrentCurl(){return pCurrent;}
	//
	void setCalculationMethod(CURLMETHOD method){ _calculationMethod = method; }
	//
	int CalculateCurl(Point3Dstruct *curl);
	//

};
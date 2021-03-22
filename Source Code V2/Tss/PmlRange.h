#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 03/31/2018
Allrights reserved by David Ge

define types of PML ranges: corner cube, edge bar, side board

********************************************************************/
#include "Tss.h"
#include "../MemoryMan/MemoryManager.h"

typedef struct PmlCoefficients
{
	Point3Dstruct u;
	Point3Dstruct f;
	Point3Dstruct g;
	Point3Dstruct w;
	Point3Dstruct e;
	Point3Dstruct h;
}PmlCoefficients;


/*
	defines one range of geometry for applying PML
*/
class PmlRange :public virtual MemoryManUser
{
protected:
	int rangeIndex; //identify a range among the same type of ranges
	SimStruct *pams;
	//update coefficients
	PmlCoefficients **coeff;
	//field integrations in the PML range
	Point3Dstruct *sumH;
	Point3Dstruct *sumE;
	//
public:
	PmlRange();
	~PmlRange();
	virtual const char *name() = 0;
	//
	PmlCoefficients **Coefficients(){ return coeff; }
	Point3Dstruct *SmH(){ return sumH; }
	Point3Dstruct *SmE(){ return sumE; }
	//range definition
	virtual unsigned int iStart() = 0;
	virtual unsigned int iEnd() = 0;
	virtual unsigned int jStart() = 0;
	virtual unsigned int jEnd() = 0;
	virtual unsigned int kStart() = 0;
	virtual unsigned int kEnd() = 0;
	//
	int initializePmlRange(SimStruct *p, int rangeIndex0);
	virtual int onInitializePmlRange() = 0;
};

/*
	at corner
*/
#define COR_LLL 0
#define COR_HLL 1
#define COR_LHL 2
#define COR_LLH 3
#define COR_LHH 4
#define COR_HLH 5
#define COR_HHL 6
#define COR_HHH 7
#define CORNERCOUNT 8
class PmlCorner:public virtual PmlRange
{
public:
	PmlCorner();
	~PmlCorner();
	virtual const char *name();
	//
	//range definition
	virtual unsigned int iStart();
	virtual unsigned int iEnd();
	virtual unsigned int jStart();
	virtual unsigned int jEnd();
	virtual unsigned int kStart();
	virtual unsigned int kEnd();
	//
	virtual int onInitializePmlRange();
};

/*
	at edge
*/
#define EDG_XLL 0
#define EDG_XHL 1
#define EDG_XLH 2
#define EDG_XHH 3
#define EDG_LYL 4
#define EDG_HYL 5
#define EDG_LYH 6
#define EDG_HYH 7
#define EDG_LLZ 8
#define EDG_HLZ 9
#define EDG_LHZ 10
#define EDG_HHZ 11
#define EDGCOUNT 12
class PmlEdgeBar :public virtual PmlRange
{
public:
	PmlEdgeBar();
	~PmlEdgeBar();
	virtual const char *name();
	//
	bool parallelX(){ return rangeIndex >= EDG_XLL && rangeIndex <= EDG_XHH; }
	bool parallelY(){ return rangeIndex >= EDG_LYL && rangeIndex <= EDG_HYH; }
	bool parallelZ(){ return rangeIndex >= EDG_LLZ && rangeIndex <= EDG_HHZ; }
	
	unsigned int edgeLength();
	//
	//range definition
	virtual unsigned int iStart();
	virtual unsigned int iEnd();
	virtual unsigned int jStart();
	virtual unsigned int jEnd();
	virtual unsigned int kStart();
	virtual unsigned int kEnd();
	//
	virtual int onInitializePmlRange();
};

/*
	at side
*/
#define SIDE_XL 0
#define SIDE_XH 1
#define SIDE_YL 2
#define SIDE_YH 3
#define SIDE_ZL 4
#define SIDE_ZH 5
#define SIDECOUNT 6
class PmlSide :public virtual PmlRange
{
public:
	PmlSide();
	~PmlSide();
	virtual const char *name();
	//
	bool sideX(){ return rangeIndex == SIDE_XL || rangeIndex == SIDE_XH; }
	bool sideY(){ return rangeIndex == SIDE_YL || rangeIndex == SIDE_YH; }
	bool sideZ(){ return rangeIndex == SIDE_ZL || rangeIndex == SIDE_ZH; }
	bool isLowEnd(){ return rangeIndex == SIDE_XL || rangeIndex == SIDE_YL || rangeIndex == SIDE_ZL; }
	//
	size_t sideArea();
	//
	//range definition
	virtual unsigned int iStart();
	virtual unsigned int iEnd();
	virtual unsigned int jStart();
	virtual unsigned int jEnd();
	virtual unsigned int kStart();
	virtual unsigned int kEnd();
	//
	virtual int onInitializePmlRange();
};


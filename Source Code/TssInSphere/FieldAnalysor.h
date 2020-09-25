#pragma once
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include "..\EMField\EMField.h"
#include "..\EMField\RadiusIndex.h"
#include "FieldStatistics.h"
#include "DerivativeEstimator.h"
#include "FieldStatisticsByDivergence.h"

/*
	do statistics on a field
*/
class FieldAnalysor:public virtual GoThroughSphereBySpaces, public virtual RadiusIndexCacheUser
{
private:
	FieldPoint3D *_fields;
	int maxRadius;
	double ds;
	int _halfOrder;
	//
	double c0;
	double mu0;
	double eps0;
	//
	DivergenceByRadius *dataByRadius;
	double sx,sy,sz,sa, sm, se;
	//
	DerivativeEstimatorAsymmetric *_derivativeAsymmetric; //estimating derivative, using asymmetric estimation near boundary
	FieldStatisticsByDivergenceAsymmetric *_divergenceEstimator; //check field by divergence=0
	//
public:
	FieldAnalysor(RadiusIndexToSeriesIndex* indexCache);
	~FieldAnalysor(void);
	void cleanup();
	int setFields(FieldPoint3D *fields, int maxR, double spaceStep, int halfOrder);
	int execute();
	int WriteDivegenceToFile(int filehandle);
	void ShowDetails(fnProgressReport reporter);
	void ShowReport(fnProgressReport reporter);
	double getAverageDivergenceE();
	double getAverageDivergenceH();
	//
	virtual void handleData(double x, double y, double z);
};


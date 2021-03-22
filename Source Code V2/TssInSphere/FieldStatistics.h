#pragma once
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "../EMField/EMField.h"
#include "../ProcessMonitor/workProcess.h"

//////////////////////////////////////
/*
	record field magnitude vs divergence
*/
typedef struct DivergenceByRadius
{
	int r;
	double maxDivergenceE;
	double sumDivergenceE;
	double sumFieldStrengthE;
	double maxDivergenceH;
	double sumDivergenceH;
	double sumFieldStrengthH;
	double sumEnergyInwards;
	double sumEnergyOutwards;
	double sumEnergyCircular;
	double sumEnergy;
}DivergenceByRadius;
///////////////////////////////////////////
/*
	field statistics
*/
class FieldStatistics
{
protected:
	int maxRadius;
	double _maxFieldE, _maxFieldH;
	double _maxDivergenceE, _maxDivergenceH;
	double _divergE, _divergH;
	int _rMaxE, _rMaxH;
	int _rMaxDivE, _rMaxDivH;
	double _sumDivgE, _sumDivgH;
	DivergenceByRadius *_dvgByRadius;
	//
public:
	FieldStatistics(void);
	virtual ~FieldStatistics();
	void Reset();
	int AllocateList(int maxR);
	virtual void SetDivergences(double dgE, double dgH){ _divergE = dgE; _divergH = dgH; }
	virtual void MakeStatistics(double fmE, double fmH, double energy, int r);
	virtual void MakeStatistics(FieldPoint3D *_fields, size_t index, int r);
	double GetMaxFieldE();
	double GetMaxFieldH();
	double GetMaxDivergenceE();
	double GetMaxDivergenceH();
	double GetDivergenceE();
	double GetDivergenceH();
	double GetSumDivergenceE();
	double GetSumDivergenceH();
	double GetAverageDivergenceE();
	double GetAverageDivergenceH();
	int GetRadiusMaxE();
	int GetRadiusMaxH();
	int GetRadiusMaxDivE();
	int GetRadiusMaxDivH();
	DivergenceByRadius *GetList();
	//
	int WriteDivegenceToFile(FILE *filehandle);
	//
	void ShowDetails(fnProgressReport reporter);
	void ShowSummary(fnProgressReport reporter);
	void ShowReport(fnProgressReport reporter);
};

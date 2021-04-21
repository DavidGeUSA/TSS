#pragma once
#include "../Tss/FieldSourceTss.h"
#include "../FileUtil/taskFile.h"

#define HEILDER_USE_HIGHORDER "HEILDER.USE.HIGHER.ORDER"

class HeidlerCurrent :
	virtual public FieldSourceTss
{
private:
	double factor;//space factor
	//
	bool useHigherOrder; //apply time derivatives and space curls
	//
	double c0;
	double t11, t12;
	double I1;
	double n1;
	double v;
	double lamdaH;
	double ita;
	//center i, j
	unsigned int i0, j0;
	unsigned int iN, iP, jN, jP;
	//computed values
	double ez;
	double i1ita;
	double n1t11;
	double i1ita_f; //i1ita*factor
	//dynamic values
	double z; //z-axis
	double t; //time
protected:
	double J1();
	double dJ1_dt();
	double d2J1_dt2();
	double J2();
	double dJ2_dt();
	double d2J2_dt2();
	
public:
	HeidlerCurrent();
	virtual ~HeidlerCurrent();
	void setTime(double zCurrent, double tCurrent){ z = zCurrent; t = tCurrent; }
	double I();
	double dI_dt();
	double d2I_dt2();
	/*
		use task file to read parameters if so desired
	*/
	virtual int initialize(TaskFile *configs);
	/*
		initialize based on simulation parameters
	*/
	virtual int initialize(SimStruct *params);
	/*
		test if the point is at the source
	*/
	virtual bool isInSource(unsigned int i, unsigned int j, unsigned int k);
	/*
		apply the Heidler current 
	*/
	virtual int applySources(double tValue, size_t tIndex, Point3Dstruct *efile, Point3Dstruct *hfile);
	virtual int applyToZrotateSymmetry(double tValue, size_t tIndex, RotateSymmetryField *efile, RotateSymmetryField *hfile);
};


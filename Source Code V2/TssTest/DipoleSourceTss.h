#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 04/17/2021
Allrights reserved by David Ge

********************************************************************/

#include "../Tss/FieldSourceTss.h"

#define DIPOLE_X   "DIPOLE.X"
#define DIPOLE_Y   "DIPOLE.Y"
#define DIPOLE_Z10 "DIPOLE.Z1START"
#define DIPOLE_Z11 "DIPOLE.Z1END"
#define DIPOLE_Z20 "DIPOLE.Z2START"
#define DIPOLE_Z21 "DIPOLE.Z2END"

#define SOURCE_MAGNITUDE "SOURCE.MAGNITUDE"
#define GAUSSIAN_FRE "GAUSSIAN.FREQUENCY"

/*
	magnitude * exp(-decay*(t-p)^2)
	decay=2 * pi^2 * f^2
	p = 1 / f;
*/
class GaussianWaveForm :public virtual WaveForm
{
private:
	double _mag; //magnitude
	double _f;   //center frequency
	//
	double _decay; //2*pi^2*f^2
	double _p;    //1/f
public:
	GaussianWaveForm(){}
	virtual ~GaussianWaveForm(){}
	//
	void setParameters(double magnitude, double centerFrequency){
		_mag = magnitude; _f = centerFrequency; 
		_decay = 2.0*M_PI*M_PI*_f*_f;
		_p = 1.0 / _f;
	}
	virtual double value(double t){
		return _mag * exp(-_decay*(t - _p)*(t - _p));
	}
	virtual double dv_dt(double t){
		return -2.0*_mag * _decay * (t - _p) * exp(-_decay*(t - _p)*(t - _p));
	}
	virtual double dv2_dt2(double t){
		double ex = _decay*(t - _p)*(t - _p);
		return 2.0*_mag * _decay*(2.0*ex-1.0) * exp(-ex);
	}
};

/*
	implement dipole source as two pec rods
	
	1. rod positions

	to reduce digitization errors, it is better that the rods are in alignment with axis, so that the rods pass through all the grid nodes,
	i.e. in alignment with one axis and in 0, +-45, +-90, +-135 or 180 degrees on the 2D plane of the other 2 axis

	for simplicity, this implementation assumes the 2 rods are in alignment with z-axis. 
	the parameters are the starting and ending z-positions for each rod.

	2. exciting source

		2.1 source current passing the rods
			assuming the current passing (0,0,z) is I(t), 
			out side of z-axis, the current is 0, that is, 0 current for (+-ds, 0, z), (0, +-ds, z), (+-ds, +-ds,z),
			we can assume that the cross-section of the rod is ((1/2)ds+(1/2)ds)^2=ds^2,
			thus, current density J(t) = I(t) / ds^2
			waveform I(t): Gaussian, parameters: center frequency, magnitude

		2.2 source voltage between the two rods
			not implemented
	
*/
class DipoleSourceTss : virtual public FieldSourceTss
{
private:
	double x00, y00;
	double z10, z11; //rod 1 position (x00,y00,z10) - (x00,y00,z11)
	double z20, z21; //rod 2 position (x00,y00,z20) - (x00,y00,z21)
	double magnitude;       //waveform magnitude
	double centerFrequency; //gaussian center frequency
	//
	//convert to node index
	//x = xmin + i*ds -> i = (x - xmin) / ds
	//y = ymin + j*ds -> j = (y - ymin) / ds
	//z = zmin + k*ds -> k = (z - zmin) / ds
	unsigned int i00, j00;
	unsigned int k10, k11; //rod 1 position (i00, j00, k10) - (i00, j00, k11)
	unsigned int k20, k21; //rod 2 position (i00, j00, k20) - (i00, j00, k21)
	//source area
	unsigned int iN; //i00-smax
	unsigned int iP; //i00+smax
	//
	unsigned int jN; //j00-smax
	unsigned int jP; //j00+smax
	//
	unsigned int k10N; //k10-smax
	//unsigned int k10P; //k10+smax
	//unsigned int k11N; //k11-smax
	unsigned int k11P; //k11+smax
	unsigned int k20N; //k20-smax
	//unsigned int k20P; //k20+smax
	//unsigned int k21N; //k21-smax
	unsigned int k21P; //k21+smax

	//
	WaveForm *wave; //source current
public:
	DipoleSourceTss();
	virtual ~DipoleSourceTss();
	//
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
	apply the field source
	*/
	virtual int applySources(double tValue, size_t tIndex, Point3Dstruct *efile, Point3Dstruct *hfile);
	virtual int applyToZrotateSymmetry(double tValue, size_t tIndex, RotateSymmetryField *efile, RotateSymmetryField *hfile){ return 0; }

};


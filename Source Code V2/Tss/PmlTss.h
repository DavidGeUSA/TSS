#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 03/31/2018
Allrights reserved by David Ge

implementation of perfect match layer

********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "PmlRange.h"


/*
	Perfect Match Layer
*/
class PmlTss
{
private:
	SimStruct *pams; //simulation parameters
	//
	unsigned int nxLn; //nx-Ln
	unsigned int nyLn; //ny-Ln
	unsigned int nzLn; //nz-Ln
	unsigned int nx1;  //nx+1
	unsigned int ny1;  //ny+1
	unsigned int nz1;  //nz+1
	double dt1;// = pams->dt;
	double dt2;// = pams->dt*pams->dt / 2.0;
	double dt3;// = pams->dt*pams->dt*pams->dt / 6.0;
	double eu; // = 1.0 / (pams->eps * pams->mu);

	//PML ranges ============================
	PmlCorner corners[CORNERCOUNT]; //8 corners
	PmlEdgeBar edgbars[EDGCOUNT];   //12 edge bars
	PmlSide sides[SIDECOUNT];       //6 side boards
	//
	bool pmlLLL; //Pxl || Pyl || Pzl
	bool pmlHLL; //Pxh || Pyl || Pzl
	bool pmlLHL; //Pxl || Pyh || Pzl
	bool pmlLLH; //Pxl || Pyl || Pzh
	bool pmlLHH; //Pxl || Pyh || Pzh
	bool pmlHLH; //Pxh || Pyl || Pzh
	bool pmlHHL; //Pxh || Pyh || Pzl
	bool pmlHHH; //Pxh || Pyh || Pzh
	bool bUsePML;
	//==========================================
	void writeCoefficients(FILE *fileHandle, PmlCoefficients **coeff, size_t w);
	//
protected:
	double effectStrength(unsigned int m);
	double alpha(unsigned int m);
	double beta(unsigned int m);
	unsigned int mx(unsigned int i);
	unsigned int my(unsigned int j);
	unsigned int mz(unsigned int k);
	//calculate coefficients
	//////fields coefficients///////////////////////////////////////////////
	//0
	double p_01x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_01y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_01z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_02x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_02y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_02z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_03x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_03y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_03z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	//1
	double p_11x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_11y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_11z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_12x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_12y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_12z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_13x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_13y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_13z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	//2,3
	double p_22x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_22y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_22z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_23x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_23y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_23z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_33x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_33y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double p_33z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	////////integral coefficients///////////////////////////////////////////
	//0
	double q_01x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_01y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_01z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_02x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_02y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_02z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_03x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_03y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_03z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	//1
	double q_11x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_11y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_11z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_12x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_12y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_12z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_13x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_13y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_13z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	//2,3
	double q_22x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_22y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_22z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_23x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_23y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_23z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_33x(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_33y(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	double q_33z(unsigned int m_x, unsigned int m_y, unsigned int m_z);
	////////////////////////////////////////////////////////////////////////
	void calculateCornerCoefficients();
	void calculateEdgeCoefficients();
	void calculateSideCoefficients();
	void calculateCoefficients(PmlCoefficients **coeff, size_t w, unsigned int i, unsigned int j, unsigned int k);
public:
	PmlTss();
	~PmlTss();
	//
	bool usePML(){ return bUsePML; }
	//index into full field
	size_t IdxField(unsigned int i, unsigned int j, unsigned int k) { return ((k)+nz1 * ((j)+ny1 * (i))); }
	virtual int initialize(SimStruct *p);
	//
	void writeCoefficientsToFile(FILE *fileHandle);
	//
	void apply1stCurlCorners(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);
	void apply2ndCurlCorners(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);
	void apply3rdCurlCorners(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);
	//
	void apply1stCurlEdges(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);
	void apply2ndCurlEdges(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);
	void apply3rdCurlEdges(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);
	//
	void apply1stCurlSides(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);
	void apply2ndCurlSides(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);
	void apply3rdCurlSides(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);
	//
	void apply1stCurl(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);
	void apply2ndCurl(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);
	void apply3rdCurl(Point3Dstruct *efield, Point3Dstruct *hfield, Point3Dstruct *eCurl, Point3Dstruct *hCurl);

};


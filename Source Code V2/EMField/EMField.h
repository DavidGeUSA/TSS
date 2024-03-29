#ifndef __EMFIELD_H__
#define __EMFIELD_H__
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility
********************************************************************/

#define M_PI       3.14159265358979323846
#ifdef _WIN32
#define M_E        2.71828182845904523536
#endif

#define ERR_OK                      0
#define ERR_OUTOFMEMORY             1
#define ERR_NOTINITIALIZED          2
#define ERR_INVALID_SIZE            3
#define ERR_INVALID_TIME_ARRAY_SIZE 4
#define ERR_INVALID_INIT            5
#define ERR_INVALID_CALL            6
#define ERR_INVALID_RADIUS_INDEX    7
#define ERR_INVALID_INDEX_RADIUS    8
#define ERR_RADIUS_INDEX_UNUSED     9
#define ERR_RADIUS_INDEX_TOOMANY    10
#define ERR_RADIUS_INDEX_MISMATCH   11
#define ERR_RADIUS_INDEX_TOOLITTLE  12
#define ERR_RADIUS_INDEX_DUPLICATE  13
#define ERR_RADIUS_INDEX_NOTSPHERE  14
#define ERR_RADIUS_INDEX_NEGATIVE   15
#define ERR_SPHERE_COUNT_MISMATCH   16
#define ERR_NO_FIELD_SOURCE         17
#define ERR_GEOMETRY_TOO_BIG        18
#define ERR_RADIUS_3DINDEX_TOO_BIG  19
#define ERR_RADIUS_3DINDEXTOOSMALL  20
#define ERR_RADIUS_OUTOFSPHERE      21
#define ERR_SIMULATION_CANCEL       22
#define ERR_REACHED_TIME_LIMIT      23
#define ERR_INVALID_ESTIMATE_ORDER  24 
#define ERR_RADIUS_INDEX_NOT_END    25
#define ERR_RADIUS_INDEX_CACHE      26
#define ERR_RADIUS_HANDLE_DATA      27
#define ERR_INVALID_DATA_FOLDER     28
#define ERR_STRINGFORM_FAILED       29
#define ERR_INVALID_EPS_MU          30
#define ERR_INVALID_DOMAIN          31
#define ERR_INVALID_SPACE_STEP      32

//it is used for specifying a location or a 3D vector
typedef struct Point3Dstruct{ double x;double y;double z;} Point3Dstruct;

//E and H field values at one space point, a space point is not included; the space point of it is determined by its usage context
typedef struct FieldPoint3D{Point3Dstruct E; Point3Dstruct H;} FieldPoint3D;

//E and H field values at a space point P
typedef struct FieldItem3D{Point3Dstruct P; Point3Dstruct E; Point3Dstruct H;} FieldItem3D;

/*
	space location
*/
class Point3D
{

public:
	double x;
	double y;
	double z;
};



#endif
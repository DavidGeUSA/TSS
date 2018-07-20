#pragma once
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/

#include "EMField.h"

/*
	field memory is allocated using a memory mapped file which is a large size one-dimensional array.
	the array items are arranged in a domain-size-independent manner via "radius indexing":
	the fields at the origin point (0,0,0) is the first item, followed by fields at those 
	space points around (0,0,0) but not more than 1 space interval, followed by fields at
	those space points around (0,0,0) but not more than 2 space intervals, and so on.
*/

//max radius, usually stored in a variable named maxRadius.
//i_N is a number of double space intervals at one side of axis (positive axis and negative axis),
//it starts at one space interval, so, at one side of axis, there are 2*i_N + 1 space intervals and
//there are 2*i_N+1 data points, not counting the data point at 0
#define GRIDRADIUS(i_N) (2 * (i_N) + 1)

//total space points for a whole axis.
//at one side of axis, there are 2*i_N + 1 data points, not counting the data point at 0, 
//for both sides, there are 2*(2*i_N+1)=4*i_N+2 data points,
//adding the data point at 0, totally there are 4*i_N+3 data points
#define GRIDPOINTS(i_N) (4*(i_N)+3)

//space interval size.
//range is a positive value, it is the space location corresponding to the data point at the far end of positive side of axis,
//that is, the (2*i_N+1)-th data point. because it starts at one space interval, not at 0, there are 2*i_N+1 space intervals at
//one side of axis, therefore, the size of space interval is the range divided by 2*i_N+1
#define SPACESTEP(range, i_N) ((range) / (2.0 * (double)(i_N)+1.0))

/*
	Allocate memory for a 3D integer array.
	it can be used for storing series indexes for a 3D radius index.
	After using the array, it should be freed by Free3DIntegerArray()
*/
size_t ***Allocate3DIntegerArray(int n);

/*
	free a 3D integer array which was allocated by Allocate3DIntegerArray
*/
void Free3DIntegerArray(size_t ***a, int n);

/*
	*p is a cubic root of p3, return value is ERR_OK.
	if p3 is not a power of 3 of p then it returns ERR_INVALID_SIZE
*/
int cubicRoot(size_t p3, size_t *p);

/*
	calculate radius from memory size.
	memory size = sizeof(FieldPoint3D) * (2*radius+1)^3
*/
int MemorySizeToRadius(size_t size, unsigned *radius);

/*
	convert radius indexing m,n,p to a series indexing which is one-dimensional memory index.
	r: radius at which to convert the radius indexing; one or more radius indexes must eaqual to r,
		absolute values of m,n, and p are less than or equal to r
	m,n,p: radius indexing
	ret: error code
	return: series indexing corresponding to the radius indexing
*/
size_t IndexesToIndex(unsigned radius, int m, int n, int p, int *ret);

/*
	convert a series indexing to radius indexing
	r: radius at which to convert the series index to radius indexes
	a: the series index to be converted
	m,n,p: the radius indexes corresponding to the series index
	return: error code
*/
int IndexToIndexes(unsigned radius, size_t a, int *m, int *n, int *p);

/*
	get number of data points at a given radius
	a: radius
	return: number of data points at the radius
*/
size_t pointsAt(unsigned r);

/*
	get total data points in a sphere for a given radius
*/
size_t totalPointsInSphere(unsigned r);

/*
	return value of setRadius
*/
typedef enum 
{
	NeedProcess      = 0, //I want to process the radius
	DoNotProcess     = 1, //I do not want to process the radius
	Finish           = 2, //I do not want to process the radius and all other radius
	ProcessAndFinish = 3, //I want to process the radius, but I will not process all other radius
}RadiusHandleType;

/*
	go through space points by radius indexes m,n,p
*/
class GoThroughSphereByIndexes
{
protected:
	int ret;
	int r;
	size_t index;
	int maxRadius;
	virtual RadiusHandleType setRadius(int radius){r = radius; return NeedProcess;}
	virtual void handleData(int m, int n, int p)=0;
	virtual void onFinish(){}
	
public:
	GoThroughSphereByIndexes(void){ret = ERR_OK; index = 0;}
	int GetLastHandlerError(){return ret;}
	size_t getCurrentIndex(){return index;}
	int gothroughSphere(int maxR);
};
/*
	go through space points by point location x,y,z
*/
class GoThroughSphereBySpaces
{
protected:
	int ret;
	int r;
	size_t index;
	//
	virtual RadiusHandleType setRadius(int radius){r=radius; return NeedProcess;}
	virtual void handleData(double x, double y, double z)=0;
	virtual void onFinish(){}

public:
	GoThroughSphereBySpaces(void){ret = ERR_OK; index = 0;}
	int GetLastHandlerError(){return ret;}
	size_t getCurrentIndex(){return index;}
	int gothroughSphere(int maxR, double ds);
};

/*
	use a FieldsInitializer to populate fields,
	its implementation code is in EMField.cpp
*/
class FieldsFiller:public GoThroughSphereBySpaces
{
private:
	FieldsInitializer* _fields0; //field value provider
	FieldPoint3D *_fields;       //fields to be populated
	int index;                   //memory index into _fields
public:
	FieldsFiller(FieldsInitializer* fields0, FieldPoint3D *field);
	virtual void handleData(double x, double y, double z);
};

/*
	getting memory index from sphere radius indexing and from cubic indexing
	radius index (m,n,p) -> series index
	cubic index  (i,j,k) -> series index
*/
class RadiusIndexToSeriesIndex: public GoThroughSphereByIndexes
{
private:
	size_t ***seriesIndex; //3D array to map (m,n,p) to index
protected:
	virtual void handleData(int m, int n, int p);
public:
	RadiusIndexToSeriesIndex(void);
	~RadiusIndexToSeriesIndex();
	int initialize(int maxR);
	size_t Index(int m, int n, int p);
	size_t CubicIndex(int i, int j, int k);
	void cleanup();
};

//convert a radius indexing to a positive sequential indexing
//m=0,+-1,+-2,...,+-maxRadius => 0,1,2,...,maxRadius,maxRadius+1,..., 2 maxRadius; totally 2 maxRadius+1 items
#define CUBICINDEX(m) ((m)+maxRadius)

//index for a row-major 3D array of dimension maxN, array index is (i,j,k), i=CUBICINDEX(m), j=CUBICINDEX(n), k=CUBICINDEX(p), maxN2=maxN*maxN
#define ROWMAJORINDEX(m,n,p) CUBICINDEX(p)+maxN*CUBICINDEX(n)+maxN2*CUBICINDEX(m)

///////////////////////////////////
/*
	an interface for passing the index cache around.
	the purpose is to minimize memory usage by
	sharing one index-cache among all objects
*/
class RadiusIndexCacheUser
{
protected:
	RadiusIndexToSeriesIndex *seriesIndex; //3D array to map (m,n,p) to one-dimensional memory index
public:
	RadiusIndexCacheUser(void){seriesIndex=NULL;}
	void setIndexCache(RadiusIndexToSeriesIndex *cache){seriesIndex=cache;}
	RadiusIndexToSeriesIndex *getIndexCache(){return seriesIndex;}
	void shareIndexCacheTo(RadiusIndexCacheUser *user){user->setIndexCache(seriesIndex);}
};

//check whether the cache is avilable
#define RADIUSINDEXMAPEXIST (seriesIndex==NULL)?ERR_RADIUS_INDEX_CACHE:ERR_OK

/*
	all subclasses of RadiusIndexCacheUser can use following macroes to get field memory index.
	a field memory is a one-dimensional array, its index is 0,1,2,...,(2*maxRadius+1)^3
	radius indexing: m,n,p=0,+-1,+-2,...,+-maxRadius
	cubic indexing:  i,j,k=0,1,2,...,2*maxRadius
*/

//get memory index from radius indexing
#define SINDEX(m,n,p) seriesIndex->Index((m),(n),(p))

//get memory index from cubic indexing
#define CINDEX(i,j,k) seriesIndex->CubicIndex((i),(j),(k))
////////////////////////////////////////////////////////////////////////

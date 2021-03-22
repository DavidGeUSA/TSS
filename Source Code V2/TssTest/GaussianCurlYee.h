#pragma once
#include "GaussianCurl.h"
class GaussianCurlYee : virtual public GaussianCurl
{
private:
	double ds;
public:
	GaussianCurlYee();
	~GaussianCurlYee();
	//
	virtual int initializeByConfig(TaskFile *configs);
	//
	virtual void SetFields(double x, double y, double z, Point3Dstruct *pE, Point3Dstruct *pH);
};


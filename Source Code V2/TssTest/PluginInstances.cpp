/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

create plugin instance by class name
********************************************************************/
#include "../Tss/FieldsSetter.h"
#include "../Tss/FieldSourceTss.h"
#include "GaussianCurl.h"
#include "GaussianCurlYee.h"
#include "PluginInstances.h"
#include "CurlXYZ.h"
#include "CurlXYZ2.h"
#include "GaussianTest.h"
#include "RickerSource.h"
#include "FieldsNull.h"
#include "HeidlerCurrent.h"
#include "../Tss/DivergenceStatistic.h"
//#include "DivergenceStatisticHeilder.h"
#include "BoundaryTssVoid.h"

#ifdef __linux__ 
#include <cstring>
#endif

Simulator* CreateSimulatorInstance(const char *className)
{
	if (className == NULL)
		return NULL;
	if (strcmp(className, "Simulator") == 0)
	{
		Simulator * p = new Simulator();
		p->setClassName(className);
		return dynamic_cast<Simulator *>(p);
	}
	else
	{
		printf("\nInvalid className:%s\n",className);
	}
	return NULL;
}

FieldsSetter* CreateFieldsSetterInstance(const char *className)
{
	if (className == NULL)
		return NULL;
	if (strcmp(className, "GaussianCurl") == 0)
	{
		GaussianCurl *p0 = new GaussianCurl();
		p0->setClassName(className);
		return dynamic_cast<GaussianCurl *>(p0);
	}
	else if (strcmp(className, "GaussianCurlYee") == 0)
	{
		GaussianCurlYee *p0 = new GaussianCurlYee();
		p0->setClassName(className);
		return dynamic_cast<GaussianCurlYee *>(p0);
	}
	else if (strcmp(className, "CurlXYZ") == 0)
	{
		CurlXYZ *p0 = new CurlXYZ();
		p0->setClassName(className);
		return dynamic_cast<CurlXYZ *>(p0);
	}
	else if (strcmp(className, "CurlXYZ2") == 0)
	{
		CurlXYZ2 *p0 = new CurlXYZ2();
		p0->setClassName(className);
		return dynamic_cast<CurlXYZ2 *>(p0);
	}
	else if (strcmp(className, "GaussianTest") == 0)
	{
		GaussianTest *p0 = new GaussianTest();
		p0->setClassName(className);
		return dynamic_cast<GaussianTest *>(p0);
	}
	else if (strcmp(className, "FieldsNull") == 0)
	{
		FieldsNull *p = new FieldsNull();
		p->setClassName(className);
		return dynamic_cast<FieldsNull *>(p);
	}
	return NULL;
}

FieldSourceTss* CreateSourceInstance(const char *className)
{
	if (className == NULL)
		return NULL;
	if (strcmp(className, "FieldSourceTss") == 0)
	{
		FieldSourceTss *p = new FieldSourceTss();
		p->setClassName(className);
		return dynamic_cast<FieldSourceTss *>(p);
	}
	if (strcmp(className, "RickerSource") == 0)
	{
		RickerSource *p = new RickerSource();
		p->setClassName(className);
		return dynamic_cast<RickerSource *>(p);
	}
	if (strcmp(className, "HeidlerCurrent") == 0)
	{
		HeidlerCurrent *p = new HeidlerCurrent();
		p->setClassName(className);
		return dynamic_cast<HeidlerCurrent *>(p);
	}
	return NULL;
}

BoundaryTss* CreateBoundaryInstance(const char *className)
{
	if (className == NULL)
		return NULL;
	if (strcmp(className, "BoundaryTss") == 0)
	{
		BoundaryTss *p = new BoundaryTss();
		p->setClassName(className);
		return dynamic_cast<BoundaryTss *>(p);
	}
	else if (strcmp(className, "BoundaryTssVoid") == 0)
	{
		BoundaryTssVoid *p = new BoundaryTssVoid();
		p->setClassName(className);
		return dynamic_cast<BoundaryTssVoid *>(p);
	}
	return NULL;
}

DivergenceStatistic* CreateDivgStatistics(const char *className)
{
	if (className == NULL)
		return NULL;
	if (strcmp(className, "DivergenceStatistic") == 0)
	{
		DivergenceStatistic *p = new DivergenceStatistic();
		return dynamic_cast<DivergenceStatistic *>(p);
	}
	return NULL;
}

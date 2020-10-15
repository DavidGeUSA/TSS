#include "SourceX.h"


SourceX::SourceX()
{
	r = 0;
}


SourceX::~SourceX()
{
}

int SourceX::initialize(double Courant, double timeStep, double spaceStep, int maximumRadius, TaskFile *taskParameters)
{
	int ret = FieldSource::initialize(Courant,timeStep,spaceStep, maximumRadius, taskParameters);
	if (ret == ERR_OK)
	{
		//other initialization code here
	}
	return ret;
}

RadiusHandleType SourceX::setRadius(int radius)
{
	if (radius > 1)
	{
		//end applying the source
		return Finish;
	}
	return NeedProcess;
}

void SourceX::handleData(int m, int n, int p)
{
	if (_timeIndex == 0)
	{
		if (m == 1 && n == 0 && p == 0)
		{
			size_t s = IndexesToIndex(1, 1, 0, 0, &ret);
			_fields[s].E.x += 1.0;
		}
	}
	else if (_timeIndex == 1)
	{
		if (m == 1 && n == 0 && p == 0)
		{
			size_t s = IndexesToIndex(1, 1, 0, 0, &ret);
			_fields[s].E.x += -1.0;
		}
	}
}

#include "SourceX.h"


SourceX::SourceX()
{
	r = 0;
}


SourceX::~SourceX()
{
}

int SourceX::initialize(double Courant, int maximumRadius, TaskFile *taskParameters)
{
	int ret = FieldSource::initialize(Courant, maximumRadius, taskParameters);
	if (ret == ERR_OK)
	{
		//other initialization code here
	}
	return ret;
}

RadiusHandleType SourceX::setRadius(int radius)
{
	return NeedProcess;
}

void SourceX::handleData(int m, int n, int p)
{

}

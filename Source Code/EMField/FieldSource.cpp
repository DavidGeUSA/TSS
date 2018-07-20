/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by Bob Limnor

********************************************************************/

#include "FieldSource.h"

FieldSource::FieldSource(void)
{
	_fields = NULL;
	_timeIndex = 0;
	Cdtds = 0.0;
}

/*
	a subclass may override this function to do more initializations,
	including reading back configurations values from taskParameters.
*/
int FieldSource::initialize(double Courant, int maximumRadius, TaskFile *taskParameters)
{
	Cdtds = Courant;
	maxRadius = maximumRadius;
	return ERR_OK;
}
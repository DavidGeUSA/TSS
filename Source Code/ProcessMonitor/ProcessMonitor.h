#ifndef __PROCEEMONITOR_H__
#define __PROCEEMONITOR_H__
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/
#include "workProcess.h"

void reportProcess(fnProgressReport reporter, bool oneline, const char *format, ...);

#endif
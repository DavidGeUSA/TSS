#ifndef __PROCEEMONITOR_H__
#define __PROCEEMONITOR_H__
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include "workProcess.h"

void reportProcess(fnProgressReport reporter, bool oneline, const char *format, ...);

#endif
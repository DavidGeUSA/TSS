/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
#include <stdio.h>
#include <cstdarg>
#include "ProcessMonitor.h"

#define SHORTMESSAGESIZE 512

void reportProcess(fnProgressReport reporter, bool oneline, const char *format, ...)
{
	va_list a_list; 
	char msg[SHORTMESSAGESIZE];
	va_start ( a_list, format ); 
	vsprintf_s(msg, SHORTMESSAGESIZE, format, a_list);
	reporter(msg, oneline);

	va_end(a_list);
}

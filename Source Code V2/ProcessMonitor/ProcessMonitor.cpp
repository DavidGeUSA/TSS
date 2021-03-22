/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility

********************************************************************/
#include <stdio.h>
#include <cstdarg>
#include "ProcessMonitor.h"
#include "workProcess.h"
#include "../FileUtil/fileutil.h"

char msg[SHORTMESSAGESIZE];
void reportProcess(fnProgressReport reporter, bool oneline, const char *format, ...)
{
	va_list a_list; 
	
	va_start ( a_list, format ); 
	sprintf_1(msg, SHORTMESSAGESIZE, format, a_list);
	reporter(msg, oneline);

	va_end(a_list);
}
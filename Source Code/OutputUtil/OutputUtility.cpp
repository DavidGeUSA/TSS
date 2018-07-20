/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/
//#include "stdafx.h"
#include "OutputUtility.h"

unsigned long getTimeCount()
{
	return GetTickCount();
}

OutputUtility out;
void movecursorup()
{
	out.MoveCursorUp();
}

OutputUtility::OutputUtility(void)
{
	
}


OutputUtility::~OutputUtility(void)
{
}

void OutputUtility::MoveCursorUp()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &coninfo);
	coninfo.dwCursorPosition.Y -= 1;
	SetConsoleCursorPosition(hConsole, coninfo.dwCursorPosition);
}

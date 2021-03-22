/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility

********************************************************************/

#include "OutputUtility.h"

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
#ifdef _WIN32
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &coninfo);
	coninfo.dwCursorPosition.Y -= 1;
	SetConsoleCursorPosition(hConsole, coninfo.dwCursorPosition);
#endif
}

#pragma once

/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility

********************************************************************/

#ifdef _WIN32
#include <Windows.h>
#endif
class OutputUtility
{
private:
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	HANDLE hConsole;
#endif
public:
	OutputUtility(void);
	~OutputUtility(void);
	//
	void MoveCursorUp();
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
	void movecursorup();
#ifdef __cplusplus
}
#endif /* __cplusplus */


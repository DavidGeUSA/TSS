#pragma once
#include <Windows.h>
/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/
class OutputUtility
{
private:
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	HANDLE hConsole;
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
	unsigned long getTimeCount();
#ifdef __cplusplus
}
#endif /* __cplusplus */


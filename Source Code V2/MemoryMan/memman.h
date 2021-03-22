#ifndef __MEMMAN_H__
#define __MEMMAN_H__
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

	Modifications
	Date            Author      Description
	---------------------------------------------
	2021-03-06      David Ge    Linux compatibility

********************************************************************/

#define ERR_OK                 0
#define ERR_MEM_CREATE_FILE    6001
#define ERR_MEM_CREATE_MAP     6002
#define ERR_MEM_CREATE_VIEW    6003
#define ERR_MEM_ADDR_NOT_FOUND 6004
#define ERR_MEM_EINVAL         6005
#define ERR_MEM_WRITE_FILE     6006
#define ERR_MEM_OPEN_FILE      6007
#define ERR_INVALID_DLLPARAM   6008
#define ERR_INVALID_DLLPATH    6009
#define ERR_DLL_FUNC_NOTFOUND  6010
#define ERR_DLL_FUNC_ERROR     6011
#define ERR_INVALID_PARAMS     6012
#define ERR_MEM_GET_FILE_SIZE  6013
#define ERR_FOLDER_NOTEXIST    6014
#define ERR_MEM_MAN_NULL       6015
#define ERR_MEM_MAN_APP_PATH   6016
#define ERR_MEM_OUTOFMEMORY    6017
#define ERR_MEM_DIR_NOTEXIST   6018
//
#define ERR_MEM_UNKNOWN        6030

#ifdef __linux__ 
#define LODWORD(l)           ((unsigned long)(((size_t)(l)) & 0xffffffff))
#define HIDWORD(l)           ((unsigned long)((((size_t)(l)) >> 32) & 0xffffffff))
#elif _WIN32
#define LODWORD(l)           ((DWORD)(((size_t)(l)) & 0xffffffff))
#define HIDWORD(l)           ((DWORD)((((size_t)(l)) >> 32) & 0xffffffff))
#else

#endif

extern unsigned long osErrorCode;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void RememberOSerror();
void ClearOSerror();
unsigned long GetLastWinError();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
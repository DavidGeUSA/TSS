#ifndef __WORKPROCESS_H__
#define __WORKPROCESS_H__
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#define SHORTMESSAGESIZE 512

typedef void (*fnProgressReport)(const char *message, bool sameline);

typedef bool (*fnOperationCanceld)();

#endif
/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#ifdef __linux__
#include <cstring>
#endif
#include "EMField.h"
#include "Plugin.h"
#include "../FileUtil/fileutil.h"

Plugin::Plugin(void)
{
	_className = NULL;
}
Plugin::~Plugin(void)
{
	if(_className != NULL)
	{
		free(_className);
	}

}

char *Plugin::getClassName()
{
	if(_className != NULL && strlen(_className) > 0)
	{
		return _className;
	}
	return NULL;
}

/*
	class name is used to load/remove plugin module.
	for FDTD module, class name is also used to form data file names.
	when you develop a dynamic link library to implement
	a plugin module, call this function on creating an instance
	of the plugin module. See CreatePluginInstance functions of
	each plugin module.
*/
void Plugin::setClassName(const char *name)
{
	if(_className != NULL)
	{
		free(_className);
		_className = NULL;
	}
	if(name != NULL)
	{
		size_t n = strlen(name);
		_className = (char *)malloc(n+1);
		strcpy_0(_className, n+1, name);
	}
}

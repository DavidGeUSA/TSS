
#include "BoundaryTss.h"

BoundaryTss* BoundaryTss::CreateInstance(char *className)
{
	if (className == NULL)
		return NULL;
	if (strcmp(className, "BoundaryTss") == 0)
	{
		BoundaryTss *p = new BoundaryTss();
		p->setClassName(className);
		return p;
	}
	throw;
}
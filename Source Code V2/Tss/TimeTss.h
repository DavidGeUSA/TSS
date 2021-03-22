#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

time advancement estimation module, single thread
********************************************************************/
#include "TimeTssBase.h"
//

class TimeTss : virtual public TimeTssBase
{
private:
	size_t w;
	unsigned int i, j, k;
protected:
	Point3Dstruct *work;
	//
	virtual void cleanup();
public:
	TimeTss();
	virtual ~TimeTss();
	virtual void startThreads();
	virtual void removeThreads();
	
	virtual int initFields(FieldsSetter* f0);
	virtual int GetFirstCurls();
	virtual int GetNextCurls();
	virtual int AdvanceOneStep();
};


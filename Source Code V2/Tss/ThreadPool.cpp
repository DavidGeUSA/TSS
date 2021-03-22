/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/16/2020
Allrights reserved by David Ge

add a wait function to the ThreadPool
********************************************************************/
#include "ThreadPool.h"

void ThreadPool::wait()
{
	std::unique_lock<std::mutex> lock(this->queue_mutex);
	this->conditionWait.wait(lock,
		[this]{ return this->stop || runningTasks == 0; });
}

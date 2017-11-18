#include "semaphor.h"
#include "KernelSe.h"
#include "schedule.h"
#include "kernel.h"
#include <stdlib.h>
#include <iostream.h>
Semaphore::Semaphore(int init)
{
	lock();
	myImpl = new KernelSem(init);
	unlock();
}

Semaphore::~Semaphore()
{
	delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait)
{
	int val;
	lock();
	val = myImpl->wait(maxTimeToWait);
	unlock();
	return val;
}

void Semaphore::signal()
{
	lock();
	myImpl->signal();
	unlock();
}

int Semaphore::val() const
{
	int val;
	lock();
	val = myImpl->getValue();
	unlock();
	return val;
}
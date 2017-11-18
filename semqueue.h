#ifndef _SEMQUEUE_H_
#define _SEMQUEUE_H_

#include "semaphor.h"

class Semaphore;

class SemaphoreElement 
{
public:
	KernelSem *semaphore;
	SemaphoreElement *next;
	SemaphoreElement(KernelSem *_semaphore);
};

class SemaphoreQueue
{
private:
	SemaphoreElement *first;
	SemaphoreElement *last;
	int size;
public:
	SemaphoreQueue();
	~SemaphoreQueue();
	int getSize() const;
	void put(KernelSem* _semaphore);
	KernelSem* get();
	SemaphoreElement* getFirst();
};


#endif
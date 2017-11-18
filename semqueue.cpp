#include <stdlib.h>
#include "semaphor.h"
#include "semqueue.h"
#include "kernel.h"

SemaphoreElement::SemaphoreElement(KernelSem *_semaphore)
{
	semaphore = _semaphore;
	next = 0;
}

SemaphoreQueue::SemaphoreQueue() 
{
	lock();
	first = 0;
	last = 0;
	size = 0;
	unlock();
}

SemaphoreQueue::~SemaphoreQueue() 
{
	lock();
	SemaphoreElement *temp = first;
	while(temp)
	{
		delete temp;
		first = first->next;
		temp = first;
	}
	first = 0;
	last = 0;
	unlock();
}

int SemaphoreQueue::getSize() const 
{
	return size;
}

void SemaphoreQueue::put(KernelSem *_semaphore)
{
	lock();
	//last = first ? last->next : first = new SemaphoreElement(_semaphore);
	if(first == 0) first = last = new SemaphoreElement(_semaphore);
	else last = last->next = new SemaphoreElement(_semaphore);	
	size++;
	unlock();
}

KernelSem* SemaphoreQueue::get()
{
	lock();
	if(first == 0)
	{
		unlock();
		return 0;
	}
	SemaphoreElement *temp = first;
	KernelSem *semaphore = temp->semaphore;
	first = first->next;
	if(first == 0) last = 0;
	delete temp;
	size--;
	unlock();
	return semaphore;
}

SemaphoreElement* SemaphoreQueue::getFirst()
{
	return first;
}
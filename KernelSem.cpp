#include "KernelSe.h"
#include "kernel.h"
#include "semaphor.h"
#include "schedule.h"
#include "queue.h"
#include "pcb.h"
#include "semqueue.h"
#include <stdlib.h>
#include <iostream.h>

SemaphoreQueue* KernelSem::allSemaphors = new SemaphoreQueue();

KernelSem::KernelSem(int init)
{
	lock();
	value = init;
	waitTimeThreads = new Queue();
	waitAllThreads = new Queue();
	allSemaphors->put(this);
	unlock();
}

KernelSem::~KernelSem()
{
	delete waitTimeThreads;
	delete waitAllThreads;
}

int KernelSem::wait(Time maxTimeToWait)
{
	lock();
	if(--value < 0)
	{
		Kernel::runningThread->signalized = 0;
		Kernel::runningThread->state = PCB::BLOCKED;
		waitAllThreads->put((PCB*)Kernel::runningThread);
		if(maxTimeToWait != 0)
		{
			Kernel::runningThread->semaphoreTimeLeft = maxTimeToWait;
			waitTimeThreads->put((PCB*)Kernel::runningThread);
		}
		unlock();
		Kernel::dispatch();
		
		if(Kernel::runningThread->signalized) return 1;
		else return 0;
	}
	unlock();
	return 0;
}

void KernelSem::signal()
{
	static PCB* temp_pcb;
	static int i = 0;
	i++;
	lock();
	if(value++ < 0)
	{
		temp_pcb = waitAllThreads->get();
		if(temp_pcb->semaphoreTimeLeft != 0) 
			waitTimeThreads->findAndDelete(temp_pcb);
		
		temp_pcb->state = PCB::READY;
		if(temp_pcb->myThread != Kernel::idleThread)
			Scheduler::put(temp_pcb);
		
		temp_pcb->signalized = 1;
	}
	unlock();
}

int KernelSem::getValue() const
{
	return value;
}

void KernelSem::semaphoreTimer()
{
	static SemaphoreElement* semaphors;
	lock();
	semaphors = allSemaphors->getFirst();
	while(semaphors)
	{
		static Element* temp;
		temp = semaphors->semaphore->waitTimeThreads->getFirst();
		while(temp)
		{
			PCB* pom = temp->pcb;
			temp = temp->next;
			pom->semaphoreTimeLeft--;
			if(pom->semaphoreTimeLeft == 0)
			{
				semaphors->semaphore->waitTimeThreads->findAndDelete(pom);
				semaphors->semaphore->waitAllThreads->findAndDelete(pom);

				pom->state = PCB::READY;
				if(pom->myThread != Kernel::idleThread) Scheduler::put(pom);
			}
		}
		semaphors = semaphors->next;
	}
	unlock();
}
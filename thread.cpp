#include "thread.h"
#include "pcb.h"
#include "kernel.h"
#include "schedule.h"
#include <dos.h>
#include <stdlib.h>
#include <iostream.h>

class Kernel;
class Scheduler;

Thread::Thread (StackSize stackSize, Time timeSlice) 
{
	lock();
	myPCB = new PCB(this, stackSize, timeSlice);
	PCB::allThreads->put(myPCB);
	unlock();
}

Thread::~Thread()
{
	lock();
	PCB *temp = myPCB;
	PCB::allThreads->findAndDelete(temp);
	delete myPCB;
	unlock();
}

void Thread::start()
{
	if(myPCB->state != PCB::CREATED) return;
	lock();
	myPCB->state = PCB::READY;
	myPCB->createStack();
	Scheduler::put(myPCB);
	unlock();
}

void Thread::waitToComplete()
{
	lock();
	if(myPCB->state == PCB::CREATED)
	{
		unlock();
		return;
	}
	else if(myPCB->state == PCB::FINISHED)
	{
		unlock();
		return;
	}
	else if(myPCB == Kernel::runningThread)
	{
		unlock();
		return;
	}
	else if(this == Kernel::idleThread)
	{
		unlock();
		return;
	}
	else if(this == Kernel::firstThread)
	{
		unlock();
		return;
	}
	Kernel::runningThread->state = PCB::BLOCKED;
	myPCB->waitThreads->put((PCB*)Kernel::runningThread);
	unlock();
	Kernel::dispatch();
}

ID Thread::getId()
{
	return this->myPCB->myThreadID;
}

ID Thread::getRunningId()
{
	return Kernel::runningThread->myThreadID;
}

Thread* Thread::getThreadById(ID id)
{
	Thread* thread = PCB::allThreads->findThread(id);
	return thread;
}

void dispatch()
{
	Kernel::dispatch();
}
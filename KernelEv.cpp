#include "KernelEv.h"
#include "kernel.h"
#include "IVTE.h"
#include "schedule.h"
#include <iostream.h>
#include <stdlib.h>

KernelEv::KernelEv(IVTNo ivtNo)
{
	value = 0;
	this->ivtNo = ivtNo;
	eventOwner = (PCB*)Kernel::runningThread;
	blockedThread = 0;
	IVTEntry::IVT[ivtNo]->myEvent = this;
}

KernelEv::~KernelEv()
{
	lock();
	IVTEntry::IVT[ivtNo]->myEvent = 0;
	delete IVTEntry::IVT[ivtNo]->myEvent;
	delete blockedThread;
	delete eventOwner;
	unlock();
}

void KernelEv::signal()
{
	lock();
	if(blockedThread == 0)
	{
		value = 1;	
		unlock();
	}
	else
	{
		blockedThread->state = PCB::READY;
		Scheduler::put(blockedThread);
		blockedThread = 0;
		unlock();
		Kernel::dispatch();
	}
}

void KernelEv::wait()
{
	lock();
	if(Kernel::runningThread != eventOwner)
	{
		unlock();
		return;
	}
	if(value == 1)
	{
		value = 0;
		unlock();
	}
	else
	{
		Kernel::runningThread->state = PCB::BLOCKED;
		blockedThread = (PCB*)Kernel::runningThread;
		unlock();
		Kernel::dispatch();
	}
}
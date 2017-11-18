#include "kernel.h"
#include "schedule.h"
#include "KernelSe.h"
#include <dos.h>
#include <stdlib.h>
#include <iostream.h>

Thread* Kernel::firstThread = 0;
IdleThread* Kernel::idleThread = 0;
volatile PCB* Kernel::runningThread = 0;
InterruptRoutine Kernel::oldRoutine = 0;
volatile int Kernel::dispatch_request = 0;

void Kernel::load()
{
	lock();
	oldRoutine = getvect(0x08);
	setvect(0x08, timer);
	setvect(0x60, oldRoutine);
	
	firstThread = new Thread(1024, 2);
	
	firstThread->myPCB->state = PCB::READY;
	
	runningThread = (volatile PCB*)firstThread->myPCB;
	
	idleThread = new IdleThread();
	idleThread->start();
	unlock();
}

void Kernel::unload()
{
	lock();
	
	if((PCB*)runningThread != firstThread->myPCB)
	{
		unlock();
		return ;
	}
	
	setvect(0x08, oldRoutine);
	
	delete firstThread;
	delete idleThread;
	unlock();
}

void Kernel::dispatch()
{
	lock();
	dispatch_request = 1;
	
	timer();
	
	dispatch_request = 0;
	unlock();
}

void Kernel::wrapper()
{
	runningThread->myThread->run();
	lock();
	
	PCB* temp;
	while(runningThread->waitThreads->getSize() > 0 )
	{
		temp = runningThread->waitThreads->get();
		temp->state = PCB::READY;
		Scheduler::put(temp); 
	}
	
	runningThread->state = PCB::FINISHED;
	
	unlock();
	
	dispatch();
}

void interrupt Kernel::timer(...)
{
	static volatile unsigned tsp, tss, tbp;
	if(!dispatch_request) asm int 60h;
	if(!dispatch_request) KernelSem::semaphoreTimer();
	if(!dispatch_request && runningThread->timeSlice != 0) runningThread->timeLeft--;
	if(!dispatch_request && (runningThread->timeLeft > 0 || runningThread->timeSlice == 0)) return;
	
	dispatch_request = 0;
	
	asm {
		mov tsp, sp
		mov tss, ss
		mov tbp, bp
	}
	
	runningThread->sp = tsp;
	runningThread->ss = tss;
	runningThread->bp = tbp;
	
	runningThread->timeLeft = runningThread->timeSlice;
	
	if(runningThread->state == PCB::READY && runningThread->myThread != idleThread)
		Scheduler::put((PCB *)runningThread);
	
	runningThread = Scheduler::get();
	
	if(!runningThread)
		runningThread = idleThread->myPCB;
	
	tsp = runningThread->sp;
	tss = runningThread->ss; 
	tbp = runningThread->bp;
	asm {
		mov sp, tsp
		mov ss, tss
		mov bp, tbp
	}
}
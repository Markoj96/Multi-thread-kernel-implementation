#include "idlethr.h"
#include "pcb.h"
#include "kernel.h"
#include <iostream.h>
IdleThread::IdleThread():Thread(256, 1) { }

void IdleThread::run()
{
	while(1);
}

void IdleThread::start()
{
	if(myPCB->state != PCB::CREATED) return;
	lock();
	myPCB->state = PCB::READY;
	myPCB->createStack();
	unlock();
}
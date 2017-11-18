#include "pcb.h"
#include "kernel.h"
#include <dos.h>
#include <stdlib.h>
#include <iostream.h>
const int PCB::CREATED = 0;

const int PCB::READY = 1; 

const int PCB::BLOCKED = 2; 

const int PCB::FINISHED = 3;

int PCB::auto_id = 0; 

Queue* PCB::allThreads = new Queue();

PCB::PCB(Thread* myThread, StackSize stackSize, Time timeSlice)
{
	lock();
	this->myThread = myThread;
	
	this->myThreadID = ++auto_id;
	
	state = CREATED;
	
	waitThreads = new Queue();
	
	this->stackSize = stackSize;
	
	stack = 0;
	ss = 0;
	bp = 0;
	sp = 0;
	
	this->timeSlice = timeSlice;
	
	timeLeft = timeSlice; 
	
	semaphoreTimeLeft = 0;
	signalized = 0;
	unlock();
}

PCB::~PCB() 
{
	lock();
	delete stack;
	delete waitThreads;
	unlock();
}

void PCB::createStack()
{
	lock();
	static int size;
	size = stackSize/sizeof(unsigned);
	
	stack = new unsigned[size];

	stack[size-1] = 0x200;
	
	stack[size-2] = FP_SEG(Kernel::wrapper);
	
	stack[size-3] = FP_OFF(Kernel::wrapper);
	
	ss = FP_SEG(stack+size-12);
	
	sp = FP_OFF(stack+size-12);
	
	bp = FP_OFF(stack+size-12);
	unlock();
}
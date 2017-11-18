#ifndef _PCB_H
#define _PCB_H

#include "thread.h"
#include "queue.h"

class Thread;
class Queue;

class PCB
{
public:
	PCB(Thread* myThread, StackSize stackSize, Time timeSlice);
	
	~PCB();
	
	StackSize stackSize;
	
	unsigned *stack;
	
	unsigned int ss;

	unsigned int sp;
	
	unsigned int bp;
	
	void createStack();
	
	Thread* myThread;
	
	static const int CREATED, READY, BLOCKED, FINISHED;
	
	volatile int state;
	
	static int auto_id;
	int myThreadID;
	
	Queue* waitThreads;
	
	Time timeSlice;
	
	volatile Time timeLeft;
	
	Time semaphoreTimeLeft;
	int signalized;
	
	static Queue* allThreads;
};

#endif
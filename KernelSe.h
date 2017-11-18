#ifndef _KERNELSE_H_
#define _KERNELSE_H_

#include "semqueue.h"
#include "queue.h"
#include "pcb.h"

class KernelSem
{
private:
	int value;
public:
	KernelSem(int init = 1);
	~KernelSem();
	
	int wait(Time maxTimeToWait);
	void signal();
	
	int getValue() const;
	static void semaphoreTimer();
	
	Queue* waitTimeThreads;
	Queue* waitAllThreads;
	
	static SemaphoreQueue* allSemaphors;	
};

#endif
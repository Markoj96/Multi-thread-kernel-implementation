#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "pcb.h"
#include "thread.h"
#include "idlethr.h"

typedef void interrupt (*InterruptRoutine)(...);

#define lock() { asm { pushf; cli; } }

#define unlock() { asm { popf; sti; } }

class Kernel 
{
private:
	friend class Thread;
	friend class PCB;
	friend class KernelSem;
	friend class KernelEv;
	
	static Thread* firstThread;
	
	static IdleThread* idleThread;
	
	static volatile PCB* runningThread;
	
	static InterruptRoutine oldRoutine;
	
	static volatile int dispatch_request;
	
	
	static void interrupt timer(...);
	
	static void wrapper();
public:
	static void load();
	
	static void unload();
	
	static void dispatch();
};

#endif
#ifndef _KERNELEV_H_
#define _KERNELEV_H_

#include "kernel.h"
#include "event.h"

class KernelEv
{
private:
	int value;
	IVTNo ivtNo;
	PCB* blockedThread;
	PCB* eventOwner;
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
};

#endif
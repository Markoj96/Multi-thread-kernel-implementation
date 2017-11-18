#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

#include "kernel.h"
#include "event.h"
#include "kernelev.h"


class IVTEntry
{
private:
	IVTNo ivtNo;
	KernelEv* myEvent;
	InterruptRoutine oldRoutine;
public:
	friend class KernelEv;

	static IVTEntry* IVT[256];
	IVTEntry(IVTNo ivtNo, InterruptRoutine interruptRoutine);
	~IVTEntry();
	void callOldRoutine();
	void signal();
};

#endif
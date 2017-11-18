#include "IVTE.h"
#include <stdlib.h>
#include <dos.h>

IVTEntry* IVTEntry::IVT[256];

IVTEntry::IVTEntry(IVTNo ivtNo, InterruptRoutine interruptRoutine)
{
	lock();
	this->ivtNo = ivtNo;
	oldRoutine = getvect(ivtNo);
	setvect(ivtNo, interruptRoutine);
	IVT[ivtNo] = this;
	unlock();
}

IVTEntry::~IVTEntry()
{
	(*oldRoutine)();
	setvect(ivtNo, oldRoutine);
	IVT[ivtNo] = 0;
}

void IVTEntry::callOldRoutine()
{
	(*oldRoutine)();
}

void IVTEntry::signal()
{
	if(myEvent == 0) return;
	myEvent->signal();
}
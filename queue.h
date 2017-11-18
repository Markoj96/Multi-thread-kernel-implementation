#ifndef _QUEUE_H
#define _QUEUE_H

#include "pcb.h"

class PCB;

class Element 
{
public:
	PCB* pcb;
	Element* next;
	Element(PCB* _pcb);
};

class Queue
{
private:
	Element *first;
	Element *last;
	int size;
public:
	Queue();
	~Queue();
	int getSize() const;
	void put(PCB* _pcb);
	PCB* get();
	Thread* findThread(int ID);
	void findAndDelete(PCB* _pcb);
	Element* getFirst();
};

#endif
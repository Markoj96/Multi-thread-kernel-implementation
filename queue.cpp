#include <stdlib.h>
#include "pcb.h"
#include "queue.h"
#include "kernel.h"
#include <iostream.h>
Element::Element(PCB *_pcb)
{
	pcb = _pcb;
	next = 0;
}

Queue::Queue() 
{
	lock();
	first = 0;
	last = 0;
	size = 0;
	unlock();
}

Queue::~Queue() 
{
	lock();
	Element* temp = first;
	while(temp)
	{
		delete temp;
		first = first->next;
		temp = first;
	}
	first = 0;
	last = 0;
	unlock();
}

int Queue::getSize() const 
{
	return size;
}

void Queue::put(PCB *_pcb)
{
	lock();
	//last = first ? last->next : first = new Element(_pcb);
	if(first == 0) first = last = new Element(_pcb);
	else last = last->next = new Element(_pcb);
	size++;
	unlock();
}

PCB* Queue::get()
{
	lock();
	if(first == 0)
	{
		unlock();
		return 0;
	}
	Element *temp = first;
	PCB *pcb = temp->pcb;
	first = first->next;
	if(first == 0) last = 0;
	delete temp;
	size--;
	unlock();
	return pcb;
}

Thread* Queue::findThread(int ID)
{
	lock();
	Element *temp = first;
	if(first == 0)
	{
		unlock();
		return 0;
	}
	while(temp->pcb->myThreadID != ID && temp)
	{
		cout << "MyThreadID je " << temp->pcb->myThreadID << " a ID trazeni je " << ID << endl;
		temp = temp->next;
	}
	if(temp == 0)
	{
		unlock();
		return 0;
	}
	else
	{
		unlock();
		return temp->pcb->myThread;
	}
}

void Queue::findAndDelete(PCB* _pcb)
{
	lock();
	Element *temp = first;
	Element *temp_last = 0;
	if(first == 0)
	{
		unlock();
		return;
	}
	while(temp->pcb != _pcb)
	{
		temp_last = temp;
		temp = temp->next;
	}
	
	if(temp_last != 0) temp_last->next = temp->next;
	else first = first->next;
	if(temp == last) last = temp_last;	
	if(first == 0) last = 0;
	size--;
	delete temp;
	unlock();
}

Element* Queue::getFirst()
{
	return first;
}
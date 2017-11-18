#ifndef _semaphor_h_
#define _semaphor_h_

typedef unsigned int Time;

class KernelSem;

class Semaphore 
{
private:
	KernelSem* myImpl;
public:
	Semaphore (int init=1);
	
	virtual ~Semaphore ();
	
	virtual int wait (Time maxTimeToWait);
	
	virtual void signal();
	
	int val () const; // Returns the current value of the semaphore
};

#endif
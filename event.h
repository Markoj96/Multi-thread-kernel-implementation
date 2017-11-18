#ifndef _event_h_
#define _event_h_

typedef unsigned char IVTNo;

class KernelEv;

class Event 
{
private:
	KernelEv* myImpl;
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();

};

#endif
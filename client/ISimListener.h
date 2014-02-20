#pragma once
#include "SimDataEvent.h"

class ISimListener
{


public: 
	virtual ~ISimListener() {}
	virtual void dataReceived(SimDataEvent& e) = 0;
	virtual void simQuitted(HANDLE h) =0 ;
	virtual void simStopped(HANDLE h) = 0;
	virtual void simStarted(HANDLE h) = 0;

};

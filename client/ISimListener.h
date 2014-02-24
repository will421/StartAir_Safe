#pragma once
#include "SimDataEvent.h"
#include "SimReceiver.h"

class ISimListener
{


public: 
	virtual ~ISimListener() {}
	virtual void dataReceived(SimDataEvent& e) = 0;
	virtual void simQuitted(HANDLE h) =0 ;
	virtual void simStopped(HANDLE h) = 0;
	virtual void simStarted(HANDLE h) = 0;
	virtual void latlonaltReceived(HANDLE h, SIMCONNECT_DATA_LATLONALT& d) = 0;
	virtual void PBHReceived(HANDLE h, SIMCONNECT_DATA_PBH& d) = 0;

};

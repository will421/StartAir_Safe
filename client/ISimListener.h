#pragma once
#include "SimDataEvent.h"

class ISimListener
{
public: 
	virtual ~ISimListener() {}
	virtual void dataReceived(SimDataEvent& e) = 0;

};

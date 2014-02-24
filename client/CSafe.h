#pragma once

#include "types.h"
#include "SimReceiver.h"


namespace safe{
	class CSafe : public ISimListener
	{
	public:
		CSafe();
		~CSafe();
		void launch();
		void dataReceived(SimDataEvent& e);
		void simQuitted(HANDLE h);
		void simStopped(HANDLE h);
		void simStarted(HANDLE h);
		void latlonaltReceived(HANDLE h, SIMCONNECT_DATA_LATLONALT& d);
		void PBHReceived(HANDLE h, SIMCONNECT_DATA_PBH& d);
	private :
		int requid;
	};
};

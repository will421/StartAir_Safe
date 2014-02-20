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
	private :
		int requid;
	};
};
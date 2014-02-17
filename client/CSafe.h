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
	private :
		int requid;
	};
};

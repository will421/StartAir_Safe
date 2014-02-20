#pragma once

#include "types.h"
#include "SimReceiver.h"


namespace safe{
	class sendData : public ISimListener
	{
		static enum DATA_DEFINE_ID {
			DEFINITION_THROTTLE,
		};
		struct DATA {
			/*double verticalSpeed;
			float pilotHeat;*/
			double planeLatitude;
			double planeLongitude;
		};
	public:
		sendData();
		~sendData();
		void launch();
		void dataReceived(SimDataEvent& e);
	private :
		SimReceiver forex;
		int requid;
		HANDLE hSimConnect; /* Handle for the connection*/
		DATA dataToSend;	// The data to send
		void simQuitted(HANDLE h);
		void simStopped(HANDLE h);
		void simStarted(HANDLE h);
	};
};

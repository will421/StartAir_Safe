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
			float verticalSpeed;
			float pilotHeat;
			float planeLatitude;
		};
	public:
		sendData();
		~sendData();
		void launch();
		void dataReceived(SimDataEvent& e);
	private :
		int requid;
		HANDLE hSimConnect; /* Handle for the connection*/
		DATA dataToSend;	// The data to send
	};
};

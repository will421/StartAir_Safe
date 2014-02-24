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
		void sendLatLonAlt(SIMCONNECT_DATA_LATLONALT d);
		void sendPBH(SIMCONNECT_DATA_PBH d);
	private :
		SIMCONNECT_DATA_LATLONALT lla;
		SIMCONNECT_DATA_PBH pbh;
		SimReceiver forex;
		int requid;
		HANDLE hSimConnect; /* Handle for the connection*/
		DATA dataToSend;	// The data to send
		void dataReceived(SimDataEvent& e);
		void simQuitted(HANDLE h);
		void simStopped(HANDLE h);
		void simStarted(HANDLE h);
		void latlonaltReceived(HANDLE h, SIMCONNECT_DATA_LATLONALT& d);
		void PBHReceived(HANDLE h, SIMCONNECT_DATA_PBH& d);
	};
};

#pragma once

#include "types.h"
#include "SimReceiver.h"


namespace safe{
	class DataSender : public SimConnection /*: public ISimListener*/
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
		DataSender();
		DataSender(int numCfg );
		~DataSender();
		//void launch();
		void sendLatLonAlt(SIMCONNECT_DATA_LATLONALT &d);
		void sendPBH(SIMCONNECT_DATA_PBH &d);
		//void sendAll(SIMCONNECT_DATA_LATLONALT &d1, SIMCONNECT_DATA_PBH &d2);
		static int boucle;
	private :
		SIMCONNECT_DATA_LATLONALT lla;
		SIMCONNECT_DATA_PBH pbh;
		/*SimReceiver forex;
		int requid;
		HANDLE hSimConnect; // Handle for the connection
		DATA dataToSend;	// The data to send
		void dataReceived(SimDataEvent& e);
		void simQuitted(HANDLE h);
		void simStopped(HANDLE h);
		void simStarted(HANDLE h);
		void latlonaltReceived(HANDLE h, SIMCONNECT_DATA_LATLONALT& d);
		void PBHReceived(HANDLE h, SIMCONNECT_DATA_PBH& d);*/
	};
};

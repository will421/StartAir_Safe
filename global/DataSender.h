#pragma once

#include "types.h"
#include "SimReceiver.h"


namespace safe{
	class DataSender : public SimExManager,public SimConnection
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
		void sendLatLonAlt(SIMCONNECT_DATA_LATLONALT &d);
		void sendPBH(SIMCONNECT_DATA_PBH &d);
		void sendPos(SAFE_DATA_POS&d);
	private :
		SIMCONNECT_DATA_LATLONALT lla;
		SIMCONNECT_DATA_PBH pbh;
	protected :
		//void requestFreezeState();
		void freeze();
		void onRecvSimobjectData(SIMCONNECT_RECV *pData);
		void onRecvException(SIMCONNECT_RECV *pData, DWORD cbData);
	};
};

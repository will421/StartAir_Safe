#include "DataSender.h"
//#include <ctime>

using std::list;
using std::cout;
using std::endl;


namespace safe {

	//clock_t tbegin, tend;
	//double texec = 0.;
	//int DataSender::boucle = 0;

	DataSender::DataSender() : SimConnection(NULL)
	{

	}

	DataSender::DataSender(int numCfg) : SimConnection(NULL)
	{
		
		//Connection au simulateur local
		char * nomclient = "Data Sender";
		if (SUCCEEDED(SimConnect_Open(&hSimConnect, nomclient, NULL, 0, 0, numCfg))) // Local
		{
			cout << "\nDataSender connected" << endl;
			int id;
			try{
				loadConfig();
				const Setting &cfg = cfg_Safe.lookup("datasender");
				if (!cfg.lookupValue("position_definition_id", id))
				{
					id = 99;
				}
			}
			catch (SettingNotFoundException ex)
			{
				std::cerr << "No 'datasender' setting in configuration file." << endl;
				id = 99;
			}
			SimConnect_AddToDataDefinition(hSimConnect, id, "PLANE LATITUDE", "Radians");
			SimConnect_AddToDataDefinition(hSimConnect, id, "PLANE LONGITUDE", "Radians");
			SimConnect_AddToDataDefinition(hSimConnect, id, "PLANE ALTITUDE", "Feet");
			SimConnect_AddToDataDefinition(hSimConnect, id, "PLANE PITCH DEGREES", "degree");
			SimConnect_AddToDataDefinition(hSimConnect, id, "PLANE BANK DEGREES", "degree");
			SimConnect_AddToDataDefinition(hSimConnect, id, "PLANE HEADING DEGREES TRUE", "degree");

			freeze();
			//requestFreezeState();
		}
		else
		{
			cout << "Connection failure. Is the simulator launched ?" << std::endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
	}


	DataSender::~DataSender()
	{
	}

	//void DataSender::requestFreezeState()
	//{
	//	SimConnect_AddToDataDefinition(hSimConnect, 100, "IS LATITUDE LONGITUDE FREEZE ON", "bool");
	//	SimConnect_AddToDataDefinition(hSimConnect, 100, "IS ALTITUDE FREEZE ON", "bool");
	//	SimConnect_AddToDataDefinition(hSimConnect, 100, "IS ATTITUDE FREEZE ON", "bool");
	//	SimConnect_RequestDataOnSimObject(hSimConnect, 100, 100, 0, SIMCONNECT_PERIOD_SECOND);
	//}
	void DataSender::freeze()
	{
		int id1,id2,id3;
		try {
			const Setting &cfg = cfg_Safe.lookup("datasender");
			if (!(cfg.lookupValue("freeze_latitude_event_id", id1) && cfg.lookupValue("freeze_altitude_event_id", id2) && cfg.lookupValue("freeze_attitude_event_id", id3)))
			{
				id1 = 0;
				id2 = 1;
				id3 = 2;
			}
		}
		catch (SettingNotFoundException ex)
		{
			std::cerr << "No 'datasender' setting in configuration file." << endl;
			id1 = 0;
			id2 = 1;
			id3 = 2;
		}

		SimConnect_MapClientEventToSimEvent(hSimConnect, id1, "FREEZE_LATITUDE_LONGITUDE_TOGGLE");
		SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, id1, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_MapClientEventToSimEvent(hSimConnect, id2, "FREEZE_ALTITUDE_TOGGLE");
		SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, id2, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_MapClientEventToSimEvent(hSimConnect, id3, "FREEZE_ATTITUDE_TOGGLE");
		SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, id3, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	}

	//void DataSender::sendLatLonAlt(SIMCONNECT_DATA_LATLONALT &d)
	//{
	//	//SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(dataToSend), &dataToSend);
	//	SimConnect_SetDataOnSimObject(hSimConnect, 100, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);
	//	//forex.addSendRecord("SimConnect_SetDataOnSimObject(hSimConnect, 50, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);");
	//}
	//void DataSender::sendPBH(SIMCONNECT_DATA_PBH &d)
	//{
	//	SimConnect_SetDataOnSimObject(hSimConnect, 61, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);
	//	//forex.addSendRecord("SimConnect_SetDataOnSimObject(hSimConnect, 60, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);");
	//}

	void DataSender::sendPos(SAFE_DATA_POS &d) {
		int id;
		try{
			const Setting &cfg = cfg_Safe.lookup("datasender");
			if (!cfg.lookupValue("position_definition_id", id))
			{
				id = 99;
			}
		}
		catch (SettingNotFoundException ex)
		{
			std::cerr << "No 'datasender' setting in configuration file." << endl;
			id = 99;
		}
		//cout << endl << "PLANE LATITUDE = " << d.Latitude;
		//cout << endl << "PLANE LONGITUDE = " << d.Longitude;
		//cout << endl << "PLANE ALTITUDE = " << d.Altitude;
		//cout << endl << "P = " << d.p;
		//cout << endl << "B = " << d.b;
		//cout << endl << "H = " << d.h;
		SimConnect_SetDataOnSimObject(hSimConnect, id, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);
	}
	//void DataSender::sendAll(SIMCONNECT_DATA_LATLONALT &d1, SIMCONNECT_DATA_PBH &d2) {
	//	struct STRUCT_ALL {
	//		SIMCONNECT_DATA_LATLONALT d1;
	//		SIMCONNECT_DATA_PBH d2;
	//	};
	//	SimConnect_SetDataOnSimObject(hSimConnect, 71, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(pbh), &pbh);
	//}

	void DataSender::onRecvSimobjectData(SIMCONNECT_RECV *pData) {
		

	}

	void DataSender::onRecvException(SIMCONNECT_RECV *pData, DWORD cbData)
	{
		SIMCONNECT_RECV_EXCEPTION *except = (SIMCONNECT_RECV_EXCEPTION*)pData;
		printf("\n\n***** EXCEPTION=%d  SendID=%d  Index=%d  cbData=%d\n", except->dwException, except->dwSendID, except->dwIndex, cbData);

		// Locate the bad call and print it out 
		char* s = findSendRecord(except->dwSendID);
		printf("\n%s", s);
	}
}


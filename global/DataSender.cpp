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

		list<structVarUnit> l;
		l.push_back({ "PLANE LATITUDE", "radians" });
		l.push_back({ "PLANE LONGITUDE", "radians" });


		//Connection au simulateur local
		char * nomclient = "Data Sender";
		if (SUCCEEDED(SimConnect_Open(&hSimConnect, nomclient, NULL, 0, 0, numCfg))) // Local
		{
			cout << "\nDataSender connected" << endl;
			//forex = SimReceiver(hSimConnect);

			//std::for_each(l.begin(), l.end(), [&](structVarUnit s) {SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, s.varName, s.unitName); });



			//SimConnect_AddToDataDefinition(hSimConnect, 100, "PLANE LATITUDE", "Radians");
			////forex.addSendRecord(hSimConnect,"a");
			//SimConnect_AddToDataDefinition(hSimConnect, 100, "PLANE LONGITUDE", "Radians");
			////forex.addSendRecord(hSimConnect,"b");
			//SimConnect_AddToDataDefinition(hSimConnect, 100, "PLANE ALTITUDE", "Feet");
			////forex.addSendRecord(hSimConnect,"c");
			//SimConnect_AddToDataDefinition(hSimConnect, 61, "PLANE PITCH DEGREES", "Radians");
			////forex.addSendRecord(hSimConnect,"2");
			//SimConnect_AddToDataDefinition(hSimConnect, 61, "PLANE BANK DEGREES", "Radians");
			////forex.addSendRecord(hSimConnect,"3");
			//SimConnect_AddToDataDefinition(hSimConnect, 61, "PLANE HEADING DEGREES TRUE", "Radians");
			////forex.addSendRecord(hSimConnect,"4");

			SimConnect_AddToDataDefinition(hSimConnect, 99, "PLANE LATITUDE", "Radians");
			//forex.addSendRecord(hSimConnect,"a");
			SimConnect_AddToDataDefinition(hSimConnect, 99, "PLANE LONGITUDE", "Radians");
			//forex.addSendRecord(hSimConnect,"b");
			SimConnect_AddToDataDefinition(hSimConnect, 99, "PLANE ALTITUDE", "Feet");
			//forex.addSendRecord(hSimConnect,"c");
			SimConnect_AddToDataDefinition(hSimConnect, 99, "PLANE PITCH DEGREES", "Radians");
			//forex.addSendRecord(hSimConnect,"2");
			SimConnect_AddToDataDefinition(hSimConnect, 99, "PLANE BANK DEGREES", "Radians");
			//forex.addSendRecord(hSimConnect,"3");
			SimConnect_AddToDataDefinition(hSimConnect, 99, "PLANE HEADING DEGREES TRUE", "Radians");
			//forex.addSendRecord(hSimConnect,"4");

			freeze();
			//requestFreezeState();
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
		SimConnect_MapClientEventToSimEvent(hSimConnect, 0, "FREEZE_LATITUDE_LONGITUDE_TOGGLE");
		firstSendRecord(hSimConnect, "freeze()");
		SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, 0, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_MapClientEventToSimEvent(hSimConnect, 1, "FREEZE_ALTITUDE_TOGGLE");
		SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, 1, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_MapClientEventToSimEvent(hSimConnect, 2, "FREEZE_ATTITUDE_TOGGLE");
		SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, 2, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		lastSendRecord(hSimConnect);
	}

	void DataSender::sendLatLonAlt(SIMCONNECT_DATA_LATLONALT &d)
	{
		//SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(dataToSend), &dataToSend);
		SimConnect_SetDataOnSimObject(hSimConnect, 100, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);
		//forex.addSendRecord("SimConnect_SetDataOnSimObject(hSimConnect, 50, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);");
	}
	void DataSender::sendPBH(SIMCONNECT_DATA_PBH &d)
	{
		SimConnect_SetDataOnSimObject(hSimConnect, 61, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);
		//forex.addSendRecord("SimConnect_SetDataOnSimObject(hSimConnect, 60, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);");
	}

	void DataSender::sendPos(SAFE_DATA_POS &d) {
		SimConnect_SetDataOnSimObject(hSimConnect, 99, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);
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


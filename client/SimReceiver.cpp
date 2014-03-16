#include "SimReceiver.h"

using std::cout;
using std::endl;
using std::string;
using std::set;
using std::map;
using std::pair;
using std::list;

#include "DataSender.h"

namespace safe{



	SimReceiver::SimReceiver(HANDLE h) : quit(0), SimConnection(h), latLonAltRequested(false), PBHRequested(false), AllRequested(false)
	{

		// Request a simulation start event 
		SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
		SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_STOP, "SimStop");

	}

	SimReceiver::SimReceiver(int numCfg) : quit(0), SimConnection(NULL), latLonAltRequested(false), PBHRequested(false), AllRequested(false)
	{
		HRESULT hr;
		char * nomclient = "Test client";

		if (SUCCEEDED(SimConnect_Open(&hSimConnect, nomclient, NULL, 0, 0, numCfg)))
		{
			cout << "\nConnected to Flight Simulator!" << endl;

			// Request a simulation start event 
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_STOP, "SimStop");
		}
		else
		{
			cout << "Connection failure" << endl;
			exit(0);
		}
	}

	int SimReceiver::request(list<structVarUnit> s)
	{

		Request r = Request(hSimConnect, s);
		requests[r.getRequestId()] = r;
		return r.getRequestId();
	}

	SimReceiver::~SimReceiver()
	{
	}


	void SimReceiver::close()
	{
		if (SUCCEEDED(SimConnect_Close(hSimConnect)))
		{
			cout << "Client disconnected" << endl;
		}
		else
		{
			cout << "\nDisconnection failure\n" << endl;
		}
	}
	boolean SimReceiver::stop(){
		return quit;
	}


	void SimReceiver::onRecvSimobjectData(SIMCONNECT_RECV *pData) {
		DataSender::boucle++;

		SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

		cout << endl << DataSender::boucle << "-> " << pObjData->dwRequestID;

		if (pObjData->dwRequestID == 50)
		{
			SIMCONNECT_DATA_LATLONALT*  d = (SIMCONNECT_DATA_LATLONALT*)&pObjData->dwData;
			fireLatLonAlt(*d);
		}
		else if (pObjData->dwRequestID == 60)
		{
			//SIMCONNECT_DATA_LATLONALT*  d = (SIMCONNECT_DATA_LATLONALT*)&pObjData->dwData;
			SIMCONNECT_DATA_PBH * d = (SIMCONNECT_DATA_PBH*)&pObjData->dwData;
			firePBH(*d);
		}
		else if (pObjData->dwRequestID == 70)
		{
			struct STRUCT_ALL {
				SIMCONNECT_DATA_LATLONALT d1;
				SIMCONNECT_DATA_PBH d2;
			};
			//SIMCONNECT_DATA_LATLONALT*  d = (SIMCONNECT_DATA_LATLONALT*)&pObjData->dwData;
			STRUCT_ALL * d = (STRUCT_ALL*)&pObjData->dwData;

			fireLatLonAlt(d->d1);
			//firePBH(d->d2);
		}
		else
		{
			fireDataReceived(pObjData);
		}

		//	StructDatum *pS = (StructDatum*)&pObjData->dwData;
	}

	void SimReceiver::onRecvEvent(SIMCONNECT_RECV *pData) {
		
			//cout << "SIMCONNECT_RECV_ID_EVENT" << endl;
			SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;
			switch (evt->uEventID)
			{
				case EVENT_SIM_START:
					fireSimStart();
					//cout << "EVENT_SIM_START" << endl;
					// Make the call for data every sim frame, but only when it changes and 
					// only that data that has changed 
					for (std::map<int, Request>::iterator it = requests.begin(); it != requests.end(); it++)
					{
						SimConnect_RequestDataOnSimObject(hSimConnect, it->first, it->first, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED | SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);
						//hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_PDR, DEFINITION_PDR, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED | SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);
					}
					if (latLonAltRequested){ SimConnect_RequestDataOnSimObject(hSimConnect, 50, 50, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED); }
					if (PBHRequested){ SimConnect_RequestDataOnSimObject(hSimConnect, 60, 60, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED); }
					if (AllRequested){ SimConnect_RequestDataOnSimObject(hSimConnect, 70, 70, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED); }
					break;
				case EVENT_SIM_STOP:
					fireSimStop();
					break;
				default:
					//cout << "EVENT UNKNOW" << endl;
					break;
			}
	}

	void SimReceiver::onRecvException(SIMCONNECT_RECV *pData, DWORD cbData)
	{
		SIMCONNECT_RECV_EXCEPTION *except = (SIMCONNECT_RECV_EXCEPTION*)pData;
		printf("\n\n***** EXCEPTION=%d  SendID=%d  Index=%d  cbData=%d\n", except->dwException, except->dwSendID, except->dwIndex, cbData);

		// Locate the bad call and print it out 
		char* s = findSendRecord(except->dwSendID);
		printf("\n%s", s);
	}
	void SimReceiver::onRecvQuit(SIMCONNECT_RECV *pData)
	{
		fireSimQuit();
		quit = 1;
	}

	void SimReceiver::requestLatLonAlt() {

		//SimConnect_AddToDataDefinition(hSimConnect, 50, "PLANE LATITUDE", "Radians");
		//SimConnect_AddToDataDefinition(hSimConnect, 50, "PLANE LONGITUDE", "Radians");
		//SimConnect_AddToDataDefinition(hSimConnect, 50, "PLANE ALTITUDE", "Feet");
		list<structVarUnit> l;
		l.push_back({ "PLANE LATITUDE", "Radians" });
		l.push_back({ "PLANE LONGITUDE", "Radians" });
		l.push_back({ "PLANE ALTITUDE", "Feet" });
		this->addToDataDefinition(50, l);
		latLonAltRequested = true;

	}
	void SimReceiver::requestPBH(){
		//SimConnect_AddToDataDefinition(hSimConnect, 60, "struct LATLONALTPBH", NULL, SIMCONNECT_DATATYPE_LATLONALT, 0, 0);
		//SimConnect_AddToDataDefinition(hSimConnect, 60, "PLANE PITCH DEGREES", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
		//SimConnect_AddToDataDefinition(hSimConnect, 60, "PLANE BANK DEGREES", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
		//SimConnect_AddToDataDefinition(hSimConnect, 60, "PLANE HEADING DEGREES TRUE", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
		list<structVarUnit> l;
		l.push_back({ "PLANE PITCH DEGREES", "Radians" });
		l.push_back({ "PLANE BANK DEGREES", "Radians" });
		l.push_back({ "PLANE HEADING DEGREES TRUE", "Radians" });
		this->addToDataDefinition(60, l);
		PBHRequested = true;
	}
	void SimReceiver::requestAll(){
		//SimConnect_AddToDataDefinition(hSimConnect, 70, "PLANE LATITUDE", "Radians");
		//SimConnect_AddToDataDefinition(hSimConnect, 70, "PLANE LONGITUDE", "Radians");
		//SimConnect_AddToDataDefinition(hSimConnect, 70, "PLANE ALTITUDE", "Feet");
		//SimConnect_AddToDataDefinition(hSimConnect, 70, "PLANE PITCH DEGREES", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
		//SimConnect_AddToDataDefinition(hSimConnect, 70, "PLANE BANK DEGREES", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
		//SimConnect_AddToDataDefinition(hSimConnect, 70, "PLANE HEADING DEGREES TRUE", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
		list<structVarUnit> l;
		l.push_back({ "PLANE LATITUDE", "Radians" });
		l.push_back({ "PLANE LONGITUDE", "Radians" });
		l.push_back({ "PLANE ALTITUDE", "Feet" });
		l.push_back({ "PLANE PITCH DEGREES", "Radians" });
		l.push_back({ "PLANE BANK DEGREES", "Radians" });
		l.push_back({ "PLANE HEADING DEGREES TRUE", "Radians" });
		this->addToDataDefinition(70, l);
		AllRequested = true;
	}

	void SimReceiver::addToDataDefinition(int defId, std::list<structVarUnit> s)
	{
		for each (structVarUnit var in s)
		{
			SimConnect_AddToDataDefinition(hSimConnect, defId, var.varName, var.unitName);
		}
	}

	void SimReceiver::addListener(ISimListener* l) {
		simListeners.push_back(l);
	}
	void SimReceiver::removeListener(ISimListener* l) {
		simListeners.remove(l);
		/*	else {
				// handle the case
				cout << "Could not unregister the specified listener object as it is not registered." << std::endl;
				}*/
	}
	HANDLE SimReceiver::getHandle()
	{
		return hSimConnect;
	}
	void SimReceiver::fireDataReceived(SIMCONNECT_RECV_SIMOBJECT_DATA * pObjData)
	{
		SimDataEvent e = SimDataEvent(&hSimConnect, &requests[pObjData->dwRequestID], pObjData);

		std::for_each(simListeners.begin(), simListeners.end(), [&](ISimListener *l) {l->dataReceived(e); });
	}
	void SimReceiver::fireSimStart()
	{
		std::for_each(simListeners.begin(), simListeners.end(), [&](ISimListener *l) {l->simStarted(hSimConnect); });
	}
	void SimReceiver::fireSimStop()
	{
		std::for_each(simListeners.begin(), simListeners.end(), [&](ISimListener *l) {l->simStopped(hSimConnect); });
	}
	void SimReceiver::fireSimQuit()
	{
		std::for_each(simListeners.begin(), simListeners.end(), [&](ISimListener *l) {l->simQuitted(hSimConnect); });
	}

	void SimReceiver::fireLatLonAlt(SIMCONNECT_DATA_LATLONALT& d)
	{
		std::for_each(simListeners.begin(), simListeners.end(), [&](ISimListener *l) {l->latlonaltReceived(hSimConnect, d); });
	}
	void SimReceiver::firePBH(SIMCONNECT_DATA_PBH& d)
	{
		std::for_each(simListeners.begin(), simListeners.end(), [&](ISimListener *l) {l->PBHReceived(hSimConnect, d); });
	}
};
#include "SimReceiver.h"

using std::cout;
using std::endl;
using std::string;
using std::set;
using std::map;
using std::pair;
using std::list;

namespace safe{


	// Given the ID of an erroneous packet, find the identification string of the call 

	char* SimReceiver::findSendRecord(DWORD id)
	{
		bool found = false;
		int count = 0;
		while (!found && count < record_count)
		{
			if (id == send_record[count].sendid)
				return send_record[count].call;
			++count;
		}
		return "Send Record not found";
	}

	void SimReceiver::addSendRecord(char* c)
	{
		DWORD id;

		if (record_count < max_send_records)
		{
			int hr = SimConnect_GetLastSentPacketID(hSimConnect, &id);

			strncpy_s(send_record[record_count].call, 255, c, 255);
			send_record[record_count].sendid = id;
			++record_count;
		}
	}


	SimReceiver::SimReceiver(HANDLE h) : quit(0), hSimConnect(h), latLonAltRequested(false), PBHRequested(false)
	{

		// Request a simulation start event 
		SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
		SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_STOP, "SimStop");

	}

	SimReceiver::SimReceiver(int numCfg) : quit(0), hSimConnect(NULL), latLonAltRequested(false), PBHRequested(false)
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

	void SimReceiver::dispatch()
	{
		SimConnect_CallDispatch(hSimConnect, SimReceiver::dispatchCallback, this);
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
	void SimReceiver::dispatchCallback(SIMCONNECT_RECV *pData, DWORD cbData, void *pContext)
	{
		SimReceiver *pThis = reinterpret_cast<SimReceiver*>(pContext);
		pThis->process(pData, cbData);
	}
	void SimReceiver::process(SIMCONNECT_RECV *pData, DWORD cbData)
	{
		HRESULT hr;

		switch (pData->dwID)
		{
			case SIMCONNECT_RECV_ID_EVENT:
				;
				{
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
								hr = SimConnect_RequestDataOnSimObject(hSimConnect, it->first, it->first, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED | SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);
								//hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_PDR, DEFINITION_PDR, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED | SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);
							}
							if (latLonAltRequested){ SimConnect_RequestDataOnSimObject(hSimConnect, 50, 50, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED); }
							if (PBHRequested){ SimConnect_RequestDataOnSimObject(hSimConnect, 60, 60, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED); }
							break;
						case EVENT_SIM_STOP:
							fireSimStop();
							break;
						default:
							//cout << "EVENT UNKNOW" << endl;
							break;
					}
					break;
				}

			case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
				;
				{/*
						#define maxReturnedItems    20
					// A basic structure for a single item of returned data 
					struct StructOneDatum {
						int        id;
						SIMCONNECT_DATA_LATLONALT value;
					};

					// A structure that can be used to receive Tagged data 
					struct StructDatum {
						StructOneDatum  datum[maxReturnedItems];
					};
					*/
					//cout << "SIMCONNECT_RECV_ID_SIMOBJECT_DATA" << endl;
					SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
					if (pObjData->dwRequestID == 50)
					{
						SIMCONNECT_DATA_LATLONALT*  d = (SIMCONNECT_DATA_LATLONALT*) &pObjData->dwData;
						fireLatLonAlt(*d);
					} else if (pObjData->dwRequestID == 60)
					{
						//SIMCONNECT_DATA_LATLONALT*  d = (SIMCONNECT_DATA_LATLONALT*)&pObjData->dwData;
						SIMCONNECT_DATA_PBH * d = (SIMCONNECT_DATA_PBH*)&pObjData->dwData;
						firePBH(*d);
					}
					else
					{
						fireDataReceived(pObjData);
					}
					
				//	StructDatum *pS = (StructDatum*)&pObjData->dwData;
					


					break;
				}

			case SIMCONNECT_RECV_ID_EXCEPTION:
				;
				{
					SIMCONNECT_RECV_EXCEPTION *except = (SIMCONNECT_RECV_EXCEPTION*)pData;
					printf("\n\n***** EXCEPTION=%d  SendID=%d  Index=%d  cbData=%d\n", except->dwException, except->dwSendID, except->dwIndex, cbData);

					// Locate the bad call and print it out 
					char* s = findSendRecord(except->dwSendID);
					printf("\n%s", s);
					break;
				}
			case SIMCONNECT_RECV_ID_QUIT:
				;
				{
					fireSimQuit();
					quit = 1;
					break;
				}

			default:
				printf("\nUnknown dwID: %d", pData->dwID);
				break;
		}
	}

	void SimReceiver::requestLatLonAlt() {

		SimConnect_AddToDataDefinition(hSimConnect, 50, "PLANE LATITUDE", "Radians");
		SimConnect_AddToDataDefinition(hSimConnect, 50, "PLANE LONGITUDE", "Radians");
		SimConnect_AddToDataDefinition(hSimConnect, 50, "PLANE ALTITUDE", "Feet");


		//SimConnect_AddToDataDefinition(hSimConnect, 50, "struct LATLONALT", NULL, SIMCONNECT_DATATYPE_LATLONALT, 0, 0);
		latLonAltRequested = true;

	}
	void SimReceiver::requestPBH(){
		//SimConnect_AddToDataDefinition(hSimConnect, 60, "struct LATLONALTPBH", NULL, SIMCONNECT_DATATYPE_LATLONALT, 0, 0);
		SimConnect_AddToDataDefinition(hSimConnect, 60, "PLANE PITCH DEGREES", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
		SimConnect_AddToDataDefinition(hSimConnect, 60, "PLANE BANK DEGREES", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
		SimConnect_AddToDataDefinition(hSimConnect, 60, "PLANE HEADING DEGREES TRUE", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
		PBHRequested = true;
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
	HANDLE SimReceiver :: getHandle()
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
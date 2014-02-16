#include "SimReceiver.h"

using std::cout;
using std::endl;
using std::string;
using std::set;
using std::map;
using std::pair;
using std::list;

namespace safe{

	SimReceiver::SimReceiver(HANDLE h) : quit(0), hSimConnect(h)
	{
		HRESULT hr;
		char * nomclient = "Test client";
		int numCfg = 0;

		if (SUCCEEDED(SimConnect_Open(&hSimConnect, nomclient, NULL, 0, 0, numCfg)))
		{
			cout << "\nConnected to Flight Simulator!" << endl;

			// Request a simulation start event 
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
		}
		else
		{
			cout << "Connexion failure" << endl;
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
			cout << "\nDisconnexion failure\n" << endl;
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
							// Make the call for data every second, but only when it changes and 
							// only that data that has changed 
							for (std::map<int, Request>::iterator it = requests.begin(); it != requests.end(); it++)
							{
								hr = SimConnect_RequestDataOnSimObject(hSimConnect, it->first, it->first, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED | SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);
								//hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_PDR, DEFINITION_PDR, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED | SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);
							}

							break;
						default:
							//cout << "EVENT UNKNOW" << endl;
							break;
					}
					break;
				}

			case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
				;
				{
					//cout << "SIMCONNECT_RECV_ID_SIMOBJECT_DATA" << endl;
					SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
					fireDataReceived(pObjData);
					/*switch (pObjData->dwRequestID)
					{
						case REQUEST_PDR:
							;
							{
								int count = 0;
								StructDatum *pS = (StructDatum*)&pObjData->dwData;
								// There can be a minimum of 1 and a maximum of maxReturnedItems 
								// in the StructDatum structure. The actual number returned will 
								// be held in the dwDefineCount parameter. 

								while (count < (int)pObjData->dwDefineCount)
								{
									switch (pS->datum[count].id)
									{
										case DATA_VERTICAL_SPEED:
											printf("\nVertical speed = %f", pS->datum[count].value);
											break;

										case DATA_PITOT_HEAT:
											printf("\nPitot heat = %f", pS->datum[count].value);
											break;

										default:
											printf("\nUnknown datum ID: %d", pS->datum[count].id);
											break;
									}
									++count;
								}
								break;
							}

						default:
							cout << endl << "REQUEST UNKNOW";
							break;
					}*/
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
	void SimReceiver::fireDataReceived(SIMCONNECT_RECV_SIMOBJECT_DATA * pObjData)
	{
		SimDataEvent e = SimDataEvent(&hSimConnect, &requests[pObjData->dwRequestID],pObjData);
		for (list<ISimListener*>::iterator it = simListeners.begin(); it != simListeners.end(); it++)
		{
			(*it)->dataReceived(e);
		}
		//std::for_each(simListeners.begin(), simListeners.end(), [&](ISimListener *l) {l->dataReceived(e); });
	}
	void SimReceiver::fireSimStart()
	{
		cout << "\nSIM START" << endl;
	}
	void SimReceiver::fireSimStop()
	{
		cout << "\nSIM STOP" << endl;
	}
	void SimReceiver::fireSimQuit()
	{

	}

};
#include "sendData.h"

using std::list;
using std::cout;
using std::endl;





namespace safe {



	sendData::sendData() : hSimConnect(NULL)
	{
	}


	sendData::~sendData()
	{
	}

	void sendData::launch()
	{
		//Connection au simulateur local
		char * nomclient = "Test client";
		if (SUCCEEDED(SimConnect_Open(&hSimConnect, nomclient, NULL, 0, 0, 0))) // Local
		{
			cout << "\nConnected to Local FSX" << endl;
			forex = SimReceiver(hSimConnect);

			// Initialisation pour l'envoi des données sur le simulateur local
		/*	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, "Vertical Speed", "Feet per second");
			forex.addSendRecord("SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, \"Vertical Speed\", \"Feet per second\");");
			SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, "Pitot Heat", "Bool");
			forex.addSendRecord("SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, \"Pitot Heat\", \"Bool\");");*/
			SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, "PLANE LATITUDE", "radians");
			forex.addSendRecord("SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, \"PLANE LATITUDE\", \"radians\");");
			SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, "PLANE LONGITUDE", "radians");
			forex.addSendRecord("SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, \"PLANE LONGITUDE\", \"radians\");");
		}
		else
		{
			cout << "Connexion to Local FSX Fail" << endl;
			exit(0);
		}

		// Initialise le receiver pour la reception des données depuis une autre machine
		SimReceiver sr = SimReceiver(1);

		// Inscription au type de données reçue
		list<structVarUnit> l;
		l.push_back({ "Vertical Speed", "Feet per second" });
		l.push_back({ "Pitot Heat", "Bool" });
		l.push_back({ "PLANE LATITUDE", "radians" });
		l.push_back({ "PLANE LONGITUDE", "radians" });
		requid = sr.request(l);
		HANDLE a = sr.getHandle();
		sr.addListener(this);


		// Boucle de reception pour le traitement des données recues
		while (!sr.stop())
		{
			sr.dispatch();
			forex.dispatch();
			Sleep(1);
		}
	}
	// Fonstion lors du traitement de la reception des données
	void sendData::dataReceived(SimDataEvent& e)
	{
		TListDatum d = e.data;		// La structure reçue
		
		// Test si l'on recoit la réponse de la bonne request
		if (e.requ->getRequestId() == requid)
		{
			// On parcour tous les elements de la structure
			for (TListDatum::iterator it = d.begin(); it != d.end(); it++)
			{
				//hr = SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(tc), &tc;);
		/*		if (it->first == "Vertical Speed")
				{
					printf("\nVertical speed = %f", it->second);
					dataToSend.verticalSpeed = it->second;
				}
			else if (it->first == "Pitot Heat")
				{
					printf("\nPitot heat = %f", it->second);
					dataToSend.pilotHeat = it->second;
				}
				else */if (it->first == "PLANE LATITUDE")
				{
					printf("\nPLANE LATITUDE = %f", it->second);
					dataToSend.planeLatitude = it->second;
				}
				else if (it->first == "PLANE LONGITUDE")
				{
					printf("\nPLANE LONGITUDE = %f", it->second);
					dataToSend.planeLongitude = it->second;
				}
				else
				{
					cout << endl << "Unknown datum :" << it->first;
				}

				// Envoi des données
				SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(dataToSend), &dataToSend);
				forex.addSendRecord("SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(dataToSend), &dataToSend);");
			}
		}
		else {
			cout << endl << "Unknown request :" << e.requ->getRequestId();
		}
	}

	void sendData::simQuitted(HANDLE h) { cout << "\nSIM QUIT" << endl; }
	void sendData::simStopped(HANDLE h) { cout << "\nSIM STOP" << endl; }
	void sendData::simStarted(HANDLE h) { cout << "\nSIM START" << endl; }
};

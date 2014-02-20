#include "sendData.h"

using std::list;
using std::cout;
using std::endl;

namespace safe {
	sendData::sendData()
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

			// Initialisation pour l'envoi des données sur le simulateur local
			SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, "Vertical Speed", "Feet per second");
			SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, "Pitot Heat", "Bool");
			SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, "PLANE LATITUDE", "radians");
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
		requid = sr.request(l);
		sr.addListener(this);

		// Boucle de reception pour le traitement des données recues
		while (!sr.stop())
		{
			sr.dispatch();
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
				if (it->first == "Vertical Speed")
				{
					printf("\nVertical speed = %f", it->second);
					dataToSend.verticalSpeed = it->second;
				}
				else if (it->first == "Pitot Heat")
				{
					printf("\nPitot heat = %f", it->second);
					dataToSend.pilotHeat = it->second;
				}
				else if (it->first == "PLANE LATITUDE")
				{
					printf("\nPLANE LATITUDE = %f", it->second);
					dataToSend.planeLatitude = it->second;
				}
				else
				{
					cout << endl << "Unknown datum :" << it->first;
				}

				// Envoi des données
				SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(dataToSend), &dataToSend);
			}
		}
		else {
			cout << endl << "Unknown request :" << e.requ->getRequestId();
		}
	}
};

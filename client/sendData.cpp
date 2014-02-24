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

		list<structVarUnit> l;
		l.push_back({ "PLANE LATITUDE", "radians" });
		l.push_back({ "PLANE LONGITUDE", "radians" });


		//Connection au simulateur local
		char * nomclient = "Test client";
		if (SUCCEEDED(SimConnect_Open(&hSimConnect, nomclient, NULL, 0, 0, 0))) // Local
		{
			cout << "\nConnected to Local FSX" << endl;
			forex = SimReceiver(hSimConnect);

			std::for_each(l.begin(), l.end(), [&](structVarUnit s) {SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, s.varName, s.unitName); });



			SimConnect_AddToDataDefinition(hSimConnect, 100, "PLANE LATITUDE", "Radians");
			forex.addSendRecord("a");
			SimConnect_AddToDataDefinition(hSimConnect, 100, "PLANE LONGITUDE", "Radians");
			forex.addSendRecord("b");
			SimConnect_AddToDataDefinition(hSimConnect, 100, "PLANE ALTITUDE", "Feet");
			forex.addSendRecord("c");
			//SimConnect_AddToDataDefinition(hSimConnect, 100, "struct LATLONALT", NULL, SIMCONNECT_DATATYPE_LATLONALT/*, 0, 0*/);
			//forex.addSendRecord("1");
			SimConnect_AddToDataDefinition(hSimConnect, 61, "PLANE PITCH DEGREES", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
			forex.addSendRecord("2");
			SimConnect_AddToDataDefinition(hSimConnect, 61, "PLANE BANK DEGREES", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
			forex.addSendRecord("3");
			SimConnect_AddToDataDefinition(hSimConnect, 61, "PLANE HEADING DEGREES TRUE", "Radians"/*, SIMCONNECT_DATATYPE_LATLONALT, 0, 0*/);
			forex.addSendRecord("4");



			/*SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, "PLANE LATITUDE", "radians");
			forex.addSendRecord("SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, \"PLANE LATITUDE\", \"radians\");");
			SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, "PLANE LONGITUDE", "radians");
			forex.addSendRecord("SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE, \"PLANE LONGITUDE\", \"radians\");");*/
		}
		else
		{
			cout << "Connexion to Local FSX Fail" << endl;
			exit(0);
		}

		// Initialise le receiver pour la reception des données depuis une autre machine
		SimReceiver sr = SimReceiver(2);

		// Inscription au type de données reçue

		//requid = sr.request(l);
		sr.requestLatLonAlt();
		sr.requestPBH();
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
					cout << endl << "PLANE LATITUDE = " << it->second;
					dataToSend.planeLatitude = it->second;
				}
				else if (it->first == "PLANE LONGITUDE")
				{
					cout  << endl << "PLANE LONGITUDE = "<< it->second;
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

	void sendData::latlonaltReceived(HANDLE h, SIMCONNECT_DATA_LATLONALT& d) {
		cout << endl << "PLANE LATITUDE = " << d.Latitude;
		cout << endl << "PLANE LONGITUDE = " << d.Longitude;
		cout << endl << "PLANE ALTITUDE = " << d.Altitude;
		lla = d;
		sendLatLonAlt(d);
	}
	void sendData::PBHReceived(HANDLE h, SIMCONNECT_DATA_PBH& d) {
		cout << endl << "P = " << d.p;
		cout << endl << "B = " << d.b;
		cout << endl << "H = " << d.h;
		pbh = d;
		sendPBH(d);
	}


	void sendData::simQuitted(HANDLE h) { cout << "\nSIM QUIT" << endl; }
	void sendData::simStopped(HANDLE h) { cout << "\nSIM STOP" << endl; }
	void sendData::simStarted(HANDLE h) { cout << "\nSIM START" << endl; }


	void sendData::sendLatLonAlt(SIMCONNECT_DATA_LATLONALT d)
	{
		struct temp {
			double altitude;
		};
		temp t;
		double l = d.Longitude;
		t.altitude = d.Longitude;
		//SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(dataToSend), &dataToSend);
		SimConnect_SetDataOnSimObject(hSimConnect, 100, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);
		forex.addSendRecord("SimConnect_SetDataOnSimObject(hSimConnect, 50, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);");
	}
	void sendData::sendPBH(SIMCONNECT_DATA_PBH d)
	{
		SimConnect_SetDataOnSimObject(hSimConnect, 61, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(pbh), &pbh);
		forex.addSendRecord("SimConnect_SetDataOnSimObject(hSimConnect, 60, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(d), &d);");
	}
};

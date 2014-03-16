#include "CSafe.h"


using std::list;
using std::cout;
using std::endl;

namespace safe {
	CSafe::CSafe()
	{
	}


	CSafe::~CSafe()
	{
	}

	void CSafe::launch()
	{
		/*
		SimReceiver sr = SimReceiver(0);

		list<structVarUnit> l;
		l.push_back({ "PLANE LATITUDE", "radians" });
		l.push_back({ "PLANE LONGITUDE", "radians" });
		//l.push_back({ "struct LATLONALT", "SIMCONNECT_DATA_LATLONALT" });
		//requid = sr.request(l);
		sr.requestLatLonAlt();
		sr.requestPBH();
		sr.addListener(this);

		while (!sr.stop())
		{
			sr.dispatch();
			Sleep(1);
		}*/

		list<structVarUnit> l;
		l.push_back({ "PLANE LATITUDE", "radians" });
		l.push_back({ "PLANE LONGITUDE", "radians" });



		// Initialise le receiver pour la reception des données depuis une autre machine
		sr = SimReceiver(2);

		// Inscription au type de données reçue

		//requid = sr.request(l);
		//sr.requestLatLonAlt();
		//sr.requestPBH();
		sr.requestAll();
		HANDLE a = sr.getHandle();
		sr.addListener(this);

		ds = DataSender(0);
		



		//tbegin = clock();

		// Boucle de reception pour le traitement des données recues
		while (!sr.stop())
		{
			sr.dispatch();
			//forex.dispatch();
			Sleep(1);
			//if (boucle > 100)
			//{
				//tend = clock();
			//	texec = (float)(tend - tbegin) / CLOCKS_PER_SEC;
			//	std::cout << endl << "Execution time : " << texec;
			//	boucle = 0;
			//	tbegin = clock();
			//}
		}
	}

	/*
	void CSafe::dataReceived(SimDataEvent& e)
	{
		//Voir cet exemple pour l'envoi de donnée : http://msdn.microsoft.com/en-us/library/cc730359.aspx
		TListDatum d = e.data;
		if (e.requ->getRequestId() == requid)
		{
			for (TListDatum::iterator it = d.begin(); it != d.end(); it++)
			{
				if (it->first == "PLANE LATITUDE")
				{
					cout << endl << "PLANE LATITUDE = " << it->second;
				}
				else if (it->first == "PLANE LONGITUDE")
				{
					cout << endl << "PLANE LONGITUDE = " << it->second;
				}
				else
				{
					cout << endl << "Unknown datum :" << it->first;
				}
			}
		}
		else {
			cout << endl << "Unknown request :" << e.requ->getRequestId();
		}
	}

	void CSafe::latlonaltReceived(HANDLE h, SIMCONNECT_DATA_LATLONALT& d) {
		cout << endl << "PLANE LATITUDE = " << d.Latitude;
		cout << endl << "PLANE LONGITUDE = " << d.Longitude;
		cout << endl << "PLANE ALTITUDE = " << d.Altitude;
	}
	void CSafe::PBHReceived(HANDLE h, SIMCONNECT_DATA_PBH& d) {
		cout << endl << "P = " << d.p;
		cout << endl << "B = " << d.b;
		cout << endl << "H = " << d.h;
	}

	*/
	void CSafe::simQuitted(HANDLE h) { cout << "\nSIM QUIT" << endl; }
	void CSafe::simStopped(HANDLE h) { cout << "\nSIM STOP" << endl; }
	void CSafe::simStarted(HANDLE h) { cout << "\nSIM START" << endl; }

	void CSafe::dataReceived(SimDataEvent& e)
	{
		//TListDatum d = e.data;		// La structure reçue

		//// Test si l'on recoit la réponse de la bonne request
		//if (e.requ->getRequestId() == requid)
		//{
		//	// On parcour tous les elements de la structure
		//	for (TListDatum::iterator it = d.begin(); it != d.end(); it++)
		//	{
		//		if (it->first == "PLANE LATITUDE")
		//		{
		//			cout << endl << "PLANE LATITUDE = " << it->second;
		//			dataToSend.planeLatitude = it->second;
		//		}
		//		else if (it->first == "PLANE LONGITUDE")
		//		{
		//			cout << endl << "PLANE LONGITUDE = " << it->second;
		//			dataToSend.planeLongitude = it->second;
		//		}
		//		else
		//		{
		//			cout << endl << "Unknown datum :" << it->first;
		//		}

		//		// Envoi des données
		//		SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(dataToSend), &dataToSend);
		//		//forex.addSendRecord("SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(dataToSend), &dataToSend);");
		//	}
		//}
		//else {
		//	cout << endl << "Unknown request :" << e.requ->getRequestId();
		//}
	}

	void CSafe::latlonaltReceived(HANDLE h, SIMCONNECT_DATA_LATLONALT& d) {
		//cout << endl << "PLANE LATITUDE = " << d.Latitude;
		//cout << endl << "PLANE LONGITUDE = " << d.Longitude;
		//cout << endl << "PLANE ALTITUDE = " << d.Altitude;
		ds.sendLatLonAlt(d);
	}
	void CSafe::PBHReceived(HANDLE h, SIMCONNECT_DATA_PBH& d) {
		//cout << endl << "P = " << d.p;
		//cout << endl << "B = " << d.b;
		//cout << endl << "H = " << d.h;
		ds.sendPBH(d);
	}
};

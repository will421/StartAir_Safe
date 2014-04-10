#include "CSafe.h"


#include <ctime>

using std::list;
using std::cout;
using std::endl;

namespace safe {

	


	CSafe::CSafe(int port) : UDPReceiver(port), ds(0)
	{
	}


	CSafe::~CSafe()
	{
	}

	void CSafe::launch()
	{


		/*
		list<structVarUnit> l;
		l.push_back({ "PLANE LATITUDE", "radians" });
		l.push_back({ "PLANE LONGITUDE", "radians" });

		// Initialise le receiver pour la reception des donn�es depuis une autre machine
		sr = SimReceiver(0);

		// Inscription au type de donn�es re�ue

		//requid = sr.request(l);
		sr.requestLatLonAlt();
		sr.requestPBH();
		//sr.requestAll();

		sr.addListener(this);

		ds = DataSender(0);
		
		// Boucle de reception pour le traitement des donn�es recues
		while (!sr.stop())
		{
			clock_t tbegin, tend;
			tbegin = clock();
			sr.dispatch();
			//ds.dispatch();
			tend = clock();
			if (tend-tbegin !=0) cout << endl << "Temp dispatch :" << (tend - tbegin) << "-------------------------------------- "/ CLOCKS_PER_SEC * 1000;

			Sleep(1);
		}
		*/

		boucle();

	}


	void CSafe::simQuitted(HANDLE h) { cout << "\nSIM QUIT" << endl; exit(EXIT_SUCCESS); }
	void CSafe::simStopped(HANDLE h) { cout << "\nSIM STOP" << endl; }
	void CSafe::simStarted(HANDLE h) { cout << "\nSIM START" << endl; }

	void CSafe::dataReceived(SimDataEvent& e)
	{
		//TListDatum d = e.data;		// La structure re�ue

		//// Test si l'on recoit la r�ponse de la bonne request
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

		//		// Envoi des donn�es
		//		SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(dataToSend), &dataToSend);
		//		//forex.addSendRecord("SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(dataToSend), &dataToSend);");
		//	}
		//}
		//else {
		//	cout << endl << "Unknown request :" << e.requ->getRequestId();
		//}
	}

	void CSafe::latlonaltReceived(HANDLE h, SIMCONNECT_DATA_LATLONALT& d) {
		cout << endl << "PLANE LATITUDE = " << d.Latitude;
		cout << endl << "PLANE LONGITUDE = " << d.Longitude;
		cout << endl << "PLANE ALTITUDE = " << d.Altitude;
		//ds.sendLatLonAlt(d);
	}
	void CSafe::PBHReceived(HANDLE h, SIMCONNECT_DATA_PBH& d) {
		cout << endl << "P = " << d.p;
		cout << endl << "B = " << d.b;
		cout << endl << "H = " << d.h;
		
		//ds.sendPBH(d);
	}
	void CSafe::latLonAltPBHReceived(HANDLE h, SAFE_DATA_POS& d)
	{

	}

	void CSafe::datagramReceived(SOCKADDR & from, SAFE_RECV & datagram)
	{


		ds.sendPos(datagram.d);
	}
};

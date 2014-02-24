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
		}
	}
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


	void CSafe::simQuitted(HANDLE h) { cout << "\nSIM QUIT" << endl; }
	void CSafe::simStopped(HANDLE h) { cout << "\nSIM STOP" << endl; }
	void CSafe::simStarted(HANDLE h) { cout << "\nSIM START" << endl; }
};

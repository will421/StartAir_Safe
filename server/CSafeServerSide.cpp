#include "stdafx.h"
#include "CSafeServerSide.h"

#include "config.h"
#include <string>

using std::cout;
using std::endl;

namespace safe
{


	CSafeSS::CSafeSS() : udps()
	{
	}


	CSafeSS::~CSafeSS()
	{
	}

	void CSafeSS::launch()
	{
		loadConfig();
		try {
			const Setting& root = cfg_Safe.getRoot();

			const Setting &clients = root["csafess_clients"];
			int count = clients.getLength();
			for (int i = 0; i < count; ++i)
			{
				const Setting &client = clients[i];
				std::string addr;
				int port;
				if (!(client.lookupValue("address", addr)
					&& client.lookupValue("port", port)))
					continue;

				udps.addTarget(addr, port);
			}
		}
		catch(SettingNotFoundException ex)
		{
			std::cerr << "csafess_clients not found" << std::endl;
			system("pause");
			exit(EXIT_FAILURE);
		}

		SimReceiver sr = SimReceiver(0);
		sr.requestPos();
		//sr.requestLatLonAlt();
		//sr.requestPBH();

		sr.addListener(this);

		while (!sr.stop())
		{
			//clock_t tbegin, tend;
			//tbegin = clock();
			sr.dispatch();
			//ds.dispatch();
			//tend = clock();
			//	if (tend-tbegin !=0) cout << endl << "Temp dispatch :" << (tend - tbegin) << "-------------------------------------- "/*/ CLOCKS_PER_SEC * 1000*/;

			Sleep(1);
		}
	}

	void CSafeSS::simQuitted(HANDLE h) { cout << "\nSIM QUIT" << endl; }
	void CSafeSS::simStopped(HANDLE h) { cout << "\nSIM STOP" << endl; }
	void CSafeSS::simStarted(HANDLE h) { cout << "\nSIM START" << endl; }

	void CSafeSS::dataReceived(SimDataEvent& e)
	{
	}

	void CSafeSS::latlonaltReceived(HANDLE h, SIMCONNECT_DATA_LATLONALT& d) {
		cout << endl << "PLANE LATITUDE = " << d.Latitude;
		cout << endl << "PLANE LONGITUDE = " << d.Longitude;
		cout << endl << "PLANE ALTITUDE = " << d.Altitude;
		//udps.send((char*)&d, sizeof(d));
	}
	void CSafeSS::PBHReceived(HANDLE h, SIMCONNECT_DATA_PBH& d) {
		cout << endl << "P = " << d.p;
		cout << endl << "B = " << d.b;
		cout << endl << "H = " << d.h;

		//udps.send((char*)&d, sizeof(d));
	}
	void CSafeSS::latLonAltPBHReceived(HANDLE h, SAFE_DATA_POS& d)
	{
		cout << endl << "PLANE LATITUDE = " << d.Latitude;
		cout << endl << "PLANE LONGITUDE = " << d.Longitude;
		cout << endl << "PLANE ALTITUDE = " << d.Altitude;
		cout << endl << "P = " << d.p;
		cout << endl << "B = " << d.b;
		cout << endl << "H = " << d.h;

		udps.send((char*)&d, sizeof(d));
	}
};
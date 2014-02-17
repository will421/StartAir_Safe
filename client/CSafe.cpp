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

		SimReceiver sr = SimReceiver();

		list<structVarUnit> l;
		l.push_back({ "Vertical Speed", "Feet per second" });
		l.push_back({ "Pitot Heat", "Bool" });
		l.push_back({ "PLANE LATITUDE", "radians" });
		int requid = sr.request(l);
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
				if (it->first == "Vertical Speed")
				{
					printf("\nVertical speed = %f", it->second);
				}
				else if (it->first == "Pitot Heat")
				{
					printf("\nPitot heat = %f", it->second);
				}
				else if (it->first == "PLANE LATITUDE")
				{
					printf("\nPLANE LATITUDE = %f", it->second);
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
};

#include "CSafe.h"


using std::list;

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
		TListDatum d = e.data;
		for (TListDatum::iterator it = d.begin(); it != d.end(); it++)
		{
			if (it->first == "Vertical Speed")
			{
				printf("\nVertical speed = %f",it->second);
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
				printf("\nUnknown datum : %s", it->first);
			}
		}
	}
};

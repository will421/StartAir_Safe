#pragma once
#include "SimDataEvent.h"
#include "SimReceiver.h"


namespace safe {
	class ISimListener
	{


	public:
		virtual ~ISimListener() {}
		/* /brief deprecated
		*/
		virtual void dataReceived(SimDataEvent& e) = 0;
		/* /brief called when the simulator quit
		*/
		virtual void simQuitted(HANDLE h) = 0;
		/* /brief called when the simulation stop
		*/
		virtual void simStopped(HANDLE h) = 0;
		/* /bried called when the simulation start
		*/
		virtual void simStarted(HANDLE h) = 0;
		/* /bried called when a latitude, longitude and altitude are received
		*  /param d struct which contain 3 latitude, longitude and altitude
		*/
		virtual void latlonaltReceived(HANDLE h, SIMCONNECT_DATA_LATLONALT& d) = 0;
		/* /bried called when a p, b and h are received
		*  /param d struct which contain 3  p, b and h
		*/
		virtual void PBHReceived(HANDLE h, SIMCONNECT_DATA_PBH& d) = 0;

		virtual void latLonAltPBHReceived(HANDLE h, SAFE_DATA_POS& d) = 0;

	};

};


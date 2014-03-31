#pragma once
#include "SimDataEvent.h"
#include "SimReceiver.h"


namespace safe {
	class IDataPool
	{


	public:
		virtual ~IDataPool() {}

		/* /brief called when a p, b and h are received
		*  /param d struct which contain 3  p, b and h
		*/
		virtual SIMCONNECT_DATA_PBH nextPBH() = 0;
		virtual SIMCONNECT_DATA_LATLONALT nextLatLonAlt() = 0;

	};

};
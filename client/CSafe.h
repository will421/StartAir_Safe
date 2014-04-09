#pragma once

#include "types.h"
#include "SimReceiver.h"
#include "DataSender.h"
#include "UDPReceiver.h"
#include "libconfig.h++"
using namespace libconfig;

namespace safe{
	class CSafe : public ISimListener ,public UDPReceiver
	{
	public:
		CSafe(int port);
		~CSafe();
		void launch();
		void dataReceived(SimDataEvent& e);
		void simQuitted(HANDLE h);
		void simStopped(HANDLE h);
		void simStarted(HANDLE h);
		void latlonaltReceived(HANDLE h, SIMCONNECT_DATA_LATLONALT& d);
		void PBHReceived(HANDLE h, SIMCONNECT_DATA_PBH& d);
		void latLonAltPBHReceived(HANDLE h, SAFE_DATA_POS& d);
		void datagramReceived(SOCKADDR & from, SAFE_RECV & message);
	private :
		int requid;
		DataSender ds;
	};
};

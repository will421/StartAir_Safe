#pragma once
#include "IDataPool.h"


namespace safe {
	class Extrapolation : public IDataPool
	{
	public:
		Extrapolation();
		~Extrapolation();

		SIMCONNECT_DATA_PBH nextPBH();
		SIMCONNECT_DATA_LATLONALT nextLatLonAlt();
	};
};


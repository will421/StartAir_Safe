#include "Extrapolation.h"

namespace safe{
	Extrapolation::Extrapolation()
	{
	}


	Extrapolation::~Extrapolation()
	{
	}

	SIMCONNECT_DATA_PBH& Extrapolation::nextPBH()
	{
		SIMCONNECT_DATA_PBH d;
		d.b = 0.1;
		d.h = 0.2;
		d.p = 0.3;
		return d;

	}
	SIMCONNECT_DATA_LATLONALT& Extrapolation::nextLatLonAlt()
	{
		SIMCONNECT_DATA_LATLONALT d;
		d.Altitude = 120;
		d.Latitude = 120;
		d.Longitude = 120;
		return d;
	}

};
#pragma once
#include "SimConnect.h"
#include "types.h"
#include "Request.h"
#include <utility>
#include <string>
#include <list>
#include <iostream>

typedef std::list<std::pair<std::string, double>> TListDatum;

class SimDataEvent
{

	// maxReturnedItems is 2 in this case, as the sample only requests 
	// vertical speed and pitot heat switch data 
	#define maxReturnedItems    20
	// A basic structure for a single item of returned data 
	struct StructOneDatum {
		int        id;
		float    value;
	};

	// A structure that can be used to receive Tagged data 
	struct StructDatum {
		StructOneDatum  datum[maxReturnedItems];
	};


public:
	SimDataEvent(HANDLE* hSim, Request * r, SIMCONNECT_RECV_SIMOBJECT_DATA * pObjData);
	SimDataEvent(HANDLE* hSim,Request * r,TListDatum d);
	~SimDataEvent();
	HANDLE* handleSim; /*To identify from which simulator this event come from*/
	Request* requ;		/*To identify from which request are datas*/
	TListDatum data;

};


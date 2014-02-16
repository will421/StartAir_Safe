#include "Request.h"


using std::map;
using std::pair;
using std::list;

int Request::next_id = 0;

Request::Request(){}
Request::Request(HANDLE hand, std::list<structVarUnit> s)
{
	varUnits = s;
	h = hand;
	id = next_id;
	next_id++;
	int i = 0;
	for (std::list<structVarUnit>::iterator it = s.begin(); it != s.end(); ++it)
	{
		addToSubscribedData(i, *it);
		i++;
	}
	addToDataDef();

}


Request::~Request()
{

}


void Request::addToDataDef()
{

	for (std::map<char *, structIdType>::iterator it = data.begin(); it != data.end(); ++it)
	{
		SimConnect_AddToDataDefinition(h, id, it->first, it->second.unitName, SIMCONNECT_DATATYPE_FLOAT32, 0, it->second.id);
		//SimConnect_AddToDataDefinition(h, id, it->first, it->second.unitName,SIMCONNECT_DATATYPE_FLOAT32, 0,it->second.id);
	}
}
void Request::addToSubscribedData(int id, structVarUnit vu){
	structIdType s;
	s.id = id;
	s.unitName = vu.unitName;
	data[vu.varName] = s;
}
int Request::getDataId(char * varName){
	return data[varName].id;
}
int Request::getRequestId(){
	return id;
}
std::list<structVarUnit> Request::getvarUnitList()
{
	return varUnits;
}
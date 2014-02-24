#include "SimDataEvent.h"

using std::cout;
using std::endl;
using std::string;
using std::pair;

SimDataEvent::SimDataEvent(HANDLE* hSim, Request* r, SIMCONNECT_RECV_SIMOBJECT_DATA * pObjData) : handleSim(hSim), requ(r)
{
	TListDatum ld;
	
	StructDatum *pS = (StructDatum*)&pObjData->dwData;
	int count = 0;
	while (count < (int)pObjData->dwDefineCount)
	{
		std::list<structVarUnit> l = r->getvarUnitList();
		for (std::list<structVarUnit>::iterator it = l.begin(); it != l.end();it++)
		{
			if (pS->datum[count].id == r->getDataId(it->varName))
			{
				ld.push_back(pair< string, double>(it->varName,pS->datum[count].value));
				break;
			}
		}
		count++;
	}
	data = ld;
}
SimDataEvent::SimDataEvent(HANDLE* hSim, Request* r, TListDatum d) : handleSim(hSim), requ(r), data(d)
{
}


SimDataEvent::~SimDataEvent()
{
}

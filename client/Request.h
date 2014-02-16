#pragma once
#include <map>
#include <utility>
#include <set>
#include <list>
#include "types.h"
#include "SimConnect.h"

/*
struct structVarType{
char* varName;
char* unitName;
};
struct structIdType{
int id;
char * unitName;

}; */

class Request
{
	struct structIdType{
		int id;
		char * unitName;
	};
public:
	Request();
	Request(HANDLE hand, std::list<structVarUnit> s);
	~Request();
	int getDataId(char * varName);
	int getRequestId();
	std::list<structVarUnit> getvarUnitList();
private:
	HANDLE h;
	static int next_id;
	int id;
	std::map<char *, structIdType> data;
	std::list<structVarUnit> varUnits;
protected:
	void addToDataDef();
	void requestDataOnSimObject();
	void addToSubscribedData(int id, structVarUnit vu);
};


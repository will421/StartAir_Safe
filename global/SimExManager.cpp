#include "SimExManager.h"


SimExManager::SimExManager()
{
	firstSet = -1;
}


SimExManager::~SimExManager()
{
}

// Given the ID of an erroneous packet, find the identification string of the call 

char* SimExManager::findSendRecord(DWORD id)
{
	bool found = false;
	int count = 0;
	while (!found && count < record_count)
	{
		if (id == send_record[count].sendid)
			return send_record[count].call;
		++count;
	}
	return "Send Record not found";
}

void SimExManager::addSendRecord(HANDLE hSimConnect,char* c)
{
	DWORD id;

	if (record_count < max_send_records)
	{
		int hr = SimConnect_GetLastSentPacketID(hSimConnect, &id);

		add(id,c);

	}
}

void SimExManager::add(int id,char *c)
{
	strncpy_s(send_record[record_count].call, 255, c, 255);
	send_record[record_count].sendid = id;
	++record_count;
}

void SimExManager::firstSendRecord(HANDLE h, char* rn) {
	DWORD id;
	if (firstSet != -1 )
	{
		std::cout << std::endl << "SimExManager, illegal firstSendRecord";
	}
	else
	{
		recordname = rn;
		int hr = SimConnect_GetLastSentPacketID(h, &id);
		firstSet = id;
	}
	
}
void SimExManager::lastSendRecord(HANDLE h){
	DWORD id;
	if (firstSet == -1)
	{
		std::cout << std::endl << "SimExManager, illegal lastSendRecord";
	}
	else
	{
		SimConnect_GetLastSentPacketID(h, &id);
		int lastset = id;
		for (int i = firstSet; i <= lastset; i++)
		{
			add(i, recordname);
		}
		firstSet = -1;
	}
	
}

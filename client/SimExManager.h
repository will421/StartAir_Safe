#pragma once

#include "SimConnect.h"
#include <iostream>
#include <string>

class SimExManager
{
#define max_send_records 20 

	struct  record_struct {
		char  call[256];
		DWORD   sendid;
	};

public:
	SimExManager();
	~SimExManager();

private :
	int     record_count = 0;
	struct  record_struct send_record[max_send_records];
	int firstSet;
	char* recordname;
	void add(int id,char * c);
protected :
	void addSendRecord(HANDLE hSimConnect, char* c);
	char* findSendRecord(DWORD id);
	void firstSendRecord(HANDLE h, char* rn);
	void lastSendRecord(HANDLE h);
};


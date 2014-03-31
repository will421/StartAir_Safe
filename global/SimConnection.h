#pragma once

#include "SimConnect.h"
#include <iostream>

class SimConnection
{
public:
	SimConnection();
	SimConnection(HANDLE h);
	~SimConnection();
	/*
	* \brief Process the next SimConnect message received through the process function.
	*/
	void dispatch();
private:
protected:
	HANDLE hSimConnect; /* Handle for the connection*/
	static void CALLBACK dispatchCallback(
		SIMCONNECT_RECV *pData,
		DWORD cbData,
		void *pContext
		);
	void process(SIMCONNECT_RECV *pData, DWORD cbData);
	virtual void onRecvSimobjectData(SIMCONNECT_RECV *pData);
	virtual void onRecvEvent(SIMCONNECT_RECV *pData) ;
	virtual void onRecvException(SIMCONNECT_RECV *pData, DWORD cbData) ;
	virtual void onRecvQuit(SIMCONNECT_RECV *pData);
};


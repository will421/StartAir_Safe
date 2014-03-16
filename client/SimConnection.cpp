#include "SimConnection.h"


SimConnection::SimConnection()
{
}

SimConnection::SimConnection(HANDLE h) : hSimConnect(h)
{
}

SimConnection::~SimConnection()
{
}

void SimConnection::dispatch()
{
	SimConnect_CallDispatch(hSimConnect, SimConnection::dispatchCallback, this);
}

void SimConnection::dispatchCallback(SIMCONNECT_RECV *pData, DWORD cbData, void *pContext)
{
	SimConnection *pThis = reinterpret_cast<SimConnection*>(pContext);
	pThis->process(pData, cbData);
}

void SimConnection::onRecvSimobjectData(SIMCONNECT_RECV *pData){
	printf("onRecvSimobjectData");
}
void SimConnection::onRecvEvent(SIMCONNECT_RECV *pData){
	printf("onRecvSimobjectData");
}
void SimConnection::onRecvException(SIMCONNECT_RECV *pData, DWORD cbData){
	printf("onRecvSimobjectData");
}
void SimConnection::onRecvQuit(SIMCONNECT_RECV *pData){
	printf("onRecvSimobjectData");
}

void SimConnection::process(SIMCONNECT_RECV *pData, DWORD cbData)
{
	//HRESULT hr;

	switch (pData->dwID)
	{
		case SIMCONNECT_RECV_ID_EVENT:
			onRecvEvent(pData);
			break;

		case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
			onRecvSimobjectData(pData);
			break;


		case SIMCONNECT_RECV_ID_EXCEPTION:
			onRecvException(pData, cbData);
			break;

		case SIMCONNECT_RECV_ID_QUIT:
			onRecvQuit(pData);
			break;

		default:
			printf("\nUnknown dwID: %d", pData->dwID);
			break;
	}
}
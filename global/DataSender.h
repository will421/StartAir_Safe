#pragma once

#include "types.h"
#include "SimReceiver.h"



/*!
* \file DataSender.h
* \brief Header for the class DataSender
* \author RICOU & BOBO
* \version 0.1
* \date 08/04/2014
*/


/*! \namespace safe
* namespace which contain class for the project StartAir Safe
*/
namespace safe{
	class DataSender : public SimExManager,public SimConnection
	{

	public:
		DataSender();
		DataSender(int numCfg );
		~DataSender();
		void sendLatLonAlt(SIMCONNECT_DATA_LATLONALT &d);
		void sendPBH(SIMCONNECT_DATA_PBH &d);
		void sendPos(SAFE_DATA_POS&d);
	private :
		SIMCONNECT_DATA_LATLONALT lla;
		SIMCONNECT_DATA_PBH pbh;
	protected :
		//void requestFreezeState();
		void freeze();
		void onRecvSimobjectData(SIMCONNECT_RECV *pData);
		void onRecvException(SIMCONNECT_RECV *pData, DWORD cbData);
	};
};

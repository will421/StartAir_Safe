#pragma once
#include "SimConnect.h"
#include "types.h"
#include "ISimListener.h"
#include "Request.h"
#include "SimDataEvent.h"
#include "SimExManager.h"
#include "SimConnection.h"
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <list>
#include <algorithm>



/*!
* \file SimReceiver.h
* \brief Header for the class SimReceiver
* \author RICOU & BOBO
* \version 0.1
* \date 29 janvier 2014
*/


/*! \namespace safe
* namespace which contain class for simulator data manipulation
*/
namespace safe{

	static enum SYM_EVENT {
		EVENT_SIM_START,
		EVENT_SIM_STOP
	};


	/*!
	* \class SimReceiver
	* \brief Allow to retrieve data
	*/
	class SimReceiver : public SimExManager, public SimConnection
	{
	private:
		
		boolean quit;			/*!<True if the simulator quit*/
		boolean latLonAltRequested;
		boolean PBHRequested;
		boolean AllRequested;
		std::list<ISimListener*> simListeners; /*listener list*/
		std::map<int, Request> requests; /*request list*/
	public:
		/* \brief Create a SimReceiver and a new connection
		*  \param numCfg number of the configuration from SimConnect.cfg file
		*/
		SimReceiver(int numCfg = 0);
		/* \brief Create a SimReceiver with an existing connection
		*  \param h the handle of the connection
		*/
		SimReceiver(HANDLE h);

		~SimReceiver();
		/*
		* \brief Subcribre to a list of data
		* \param s set datas data that you want to retrieve for this request
		* \return l'id de la requete
		*/
		int request(std::list<structVarUnit> s);

		void requestLatLonAlt();
		void requestPBH();
		void requestAll();

		/*
		*\brief Close the connection
		*/
		void close();
		/* \brief
		*	\return true if the simulator has quit
		*/
		boolean stop();
		/* \brief add l to the listener of this receiver
		*/
		void addListener(ISimListener* l);
		/* \brief remove l from the listener list
		*/
		void removeListener(ISimListener* l);
		/* \brief Get the handle of this receiver
		*  \return The handle
		*/
		HANDLE getHandle();


	protected:
		void onRecvSimobjectData(SIMCONNECT_RECV *pData);
		void onRecvEvent(SIMCONNECT_RECV *pData);
		void onRecvException(SIMCONNECT_RECV *pData, DWORD cbData);
		void onRecvQuit(SIMCONNECT_RECV *pData);
		void addToDataDefinition(int defId, std::list<structVarUnit> s);
		void fireDataReceived(SIMCONNECT_RECV_SIMOBJECT_DATA * pObjData);
		void fireSimStart();
		void fireSimStop();
		void fireSimQuit();
		void fireLatLonAlt(SIMCONNECT_DATA_LATLONALT& d);
		void firePBH(SIMCONNECT_DATA_PBH& d);
	};

};
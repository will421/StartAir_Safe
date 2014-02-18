#pragma once
#include "SimConnect.h"
#include "types.h"
#include "ISimListener.h"
#include "Request.h"
#include "SimDataEvent.h"
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

	/*!
	* \class SimReceiver
	* \brief Allow to retrieve data
	*/
	class SimReceiver
	{
	private:
		HANDLE hSimConnect; /* Handle for the connection*/
		boolean quit;			/*!<True if the simulator quit*/
		std::list<ISimListener*> simListeners; /*listener list*/
		std::map<int,Request> requests; /*request list*/
	public:
		/* \brief Create a SimReceiver and a new connection
		*  \param numCfg number of the configuration from SimConnect.cfg file
		*/
		SimReceiver(int numCfg=0);
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
		/*
		*\brief Close the connection
		*/
		void close();
		/*
		* \brief Process the next SimConnect message received through the process function.
		*/
		void dispatch();
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
		static void CALLBACK dispatchCallback(
			SIMCONNECT_RECV *pData,
			DWORD cbData,
			void *pContext
			);
		void process(SIMCONNECT_RECV *pData, DWORD cbData);
		void fireDataReceived(SIMCONNECT_RECV_SIMOBJECT_DATA * pObjData);
		void fireSimStart();
		void fireSimStop();
		void fireSimQuit();
	};

};
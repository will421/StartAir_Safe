#pragma once



#define _WINSOCKAPI_ //To avoid redefinition from #include <windows.h> in "SimConnect.h
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
#include "config.h"



/*!
* \file SimReceiver.h
* \brief Header for the class SimReceiver
* \author RICOU & BOBO
* \version 0.1
* \date 08/04/2014
*/


/*! \namespace safe
* namespace which contain class for the project StartAir Safe
*/
namespace safe{


	/**
	* \enum SIM_EVENT
	* \brief simulator event constant
	*
	* SIM_EVENT are used to subscribing to system event
	* 
	*/
	static enum SIM_EVENT {
		EVENT_SIM_START,
		EVENT_SIM_STOP
	};


	/*!
	* \class SimReceiver
	* \brief Allow to retrieve data from the simulator
	* You have to implement ISimListener in order to receive data
	*/
	class SimReceiver : public SimExManager, public SimConnection
	{
	private:
		Config cfg; /*!<The configuration from config.h for SimReceiver */
		boolean quit;			/*!<True if the simulator quit*/
		boolean latLonAltRequested; /*!<True if requestLatLonAlt() has been called */
		boolean PBHRequested; /*!<True if requestPBH() has been called */
		boolean posRequested; /*!<True if requestPos() has been called */
		std::list<ISimListener*> simListeners; /*!<Listener list */
		std::map<int, Request> requests; /*!<The id of the request ans the Request created when request() is called */
	public:
		/*! \brief Create a SimReceiver and a new connection
		*  \param numCfg number of the configuration from SimConnect.cfg file
		*/
		SimReceiver(int numCfg = 0);
		///* \brief Create a SimReceiver with an existing connection
		//*  \param h the handle of the connection
		//*/
		//SimReceiver(HANDLE h);

		/*! \brief Destructor
		*/
		~SimReceiver();


		/*!
		* \brief Subscribe to a list of data
		* \param s set datas data that you want to retrieve for this request
		* Data will be received by using dataReceived() in ISimListener
		* \return l'id de la requete
		*/
		int request(std::list<structVarUnit> s);

		/*! \brief Subscribe to latitude, longitude and latitude of user plane
		* Data will be received by using latlonaltReceived() in ISimListener
		*/
		void requestLatLonAlt();

		/*! \brief Subscribe to pitch, bank and heading of user plane
		* Data will be received by using PBHReceived() in ISimListener
		*/
		void requestPBH();

		/*! \brief Subscribe to both, latLonAlt and PBH.
		* Data will be received by using latLonAltPBH() in ISimListener
		*/
		void requestPos();

		/*!
		*\brief Close the connection with the simulator
		*/
		void close();

		/*! \brief 
		*	\return true if the simulator has quit
		*/
		boolean stop();

		/*! \brief add l to the listener of this receiver
		*/
		void addListener(ISimListener* l);

		/*! \brief remove l from the listener list
		*/
		void removeListener(ISimListener* l);

		/*! \brief Get the handle of this receiver
		*  \return The handle
		*/
		HANDLE getHandle();


	protected:
		/*! \brief See class SimConnection
		*/
		void onRecvSimobjectData(SIMCONNECT_RECV *pData);

		/*! \brief See class SimConnection
		*/
		void onRecvEvent(SIMCONNECT_RECV *pData);

		/*! \brief See class SimConnection
		*/
		void onRecvException(SIMCONNECT_RECV *pData, DWORD cbData);

		/*! \brief See class SimConnection
		*/
		void onRecvQuit(SIMCONNECT_RECV *pData);

		/*! \brief Create a data definition on the simulator
		*/
		void addToDataDefinition(int defId, std::list<structVarUnit> s);

		/*! \brief Tell all listeners that data is received
		*/
		void fireDataReceived(SIMCONNECT_RECV_SIMOBJECT_DATA * pObjData);

		/*! \brief Tell all listeners that the simulation start
		*/
		void fireSimStart();

		/*! \brief Tell all listeners that the simulation stop
		*/
		void fireSimStop();

		/*! \brief Tell all listeners that the simulation quit
		*/
		void fireSimQuit();

		/*! \brief Tell all listeners that data is received
		*/
		void fireLatLonAlt(SIMCONNECT_DATA_LATLONALT& d);

		/*! \brief Tell all listeners that data is received
		*/
		void firePBH(SIMCONNECT_DATA_PBH& d);

		/*! \brief Tell all listeners that data is received
		*/
		void firePos(SAFE_DATA_POS& d);
	};

};
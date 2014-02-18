#ifndef DEF_CLIENT
#define DEF_CLIENT

/*!
 * \file Client.h
 * \brief Le Client
 * \author RICOU & BOBO
 * \version 0.1
 */

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <string>;

/*! \namespace std
 *
 * espace de nommage regroupant les Inputs/Output
 */
using namespace std;

/*! 
 * \class Client
 * \brief classe representant un Client
 */
class Client {

private:
	
	int id;						/*!< L'id du Client*/

	SOCKET sock;				/*!< La socket de connexion au server*/

	SOCKADDR_IN sin;			/*!< Les parametres de connexion*/

public:

	/*!
	 *	\brief Create and Initialise the Client
	 *	\param int port The port on wich the client will send his message
	 *	\param string adresse The adresse of the server
	 *
	 */
	Client(int port, string adresse);

	/*
	 *	\brief Start the main loop of the client
	 */
	void start();

	/*
	 *	\brief
	 *	\return The id of the Client
	 */
	int getID();

};

#endif;
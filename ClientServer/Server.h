#ifndef DEF_SERVER
#define DEF_SERVER

/*!
 * \file Server.h
 * \brief The server 
 * \author RICOU & BOBO
 * \version 0.1
 */

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")


/*!
 * \class Client
 * \brief classe representant un Client
 */
class Server {

private:

	int port;					/*!< L'id du Client*/

	SOCKET sock;				/*!< L'id du Client*/

	SOCKADDR_IN sin;			/*!< L'id du Client*/

public:

	/*!
	 *	\brief Create and Initialise the Server
	 *	\param int port The port on wich the client will send his message
	 */
	Server(int port);

	/*!
	 *	\brief Start the Server and run the main loop
	 *
	 */
	void start();

};

#endif;
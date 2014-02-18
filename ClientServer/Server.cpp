#include "Server.h";

#include <stdio.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

Server::Server(int port){

	sin.sin_addr.s_addr = htonl(INADDR_ANY);					// no need of inet_adresse	
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
};

void Server::start(){
	int a;
	SOCKET csock;		// the socket of the client
	SOCKADDR_IN csin;	// the connection information of the client
	sock = socket(AF_INET, SOCK_STREAM, 0);					// creation of the socket
	bind(sock, (SOCKADDR *)&sin, sizeof(sin));		// Bind the socket on the specified port

	// Notice the OS that we'll use socket
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);

	// The main loop to wait for the client to request a connection
	listen(sock, 0);
	while (1)
	{
		int sizeof_csin = sizeof(csin);
		csock = accept(sock, (SOCKADDR *)&csin, &sizeof_csin);
		if (csock > 0)
		{
			cout << "Connection Failed" << endl;
		}
		else{
			cout << "Connection succed" << endl;
			send(csock, "CONNEXION_OK", 12, 0);
			cout << "Message send" << endl;
			// Creation of an ID for the new client

			// Add the client and his socket inside an Array

			// Creation and start of a thread for the specified client
		}

		cin >> a;

	}

	// Clean the WSA to remove the use of socket
	WSACleanup();

}
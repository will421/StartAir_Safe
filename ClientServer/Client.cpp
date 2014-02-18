#include "Client.h"

#include <stdio.h>
#include <iostream>

using namespace std;

Client::Client(int port, string adresse){

	sin.sin_addr.s_addr = inet_addr(adresse.c_str());					// no need of inet_adresse	
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

};

void Client::start(){

	WSADATA WSAData;

	char buffer[255];
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	/* Tout est configuré pour se connecter sur IRC, haarlem, Undernet. */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(sock, (SOCKADDR *)&sin, sizeof(sin));
	recv(sock, buffer, sizeof(buffer), 0);

	cout << "Message Client: ";
	cout << buffer << endl;

	cin >> buffer;

	WSACleanup();

};

int Client::getID(){
	return id;
};
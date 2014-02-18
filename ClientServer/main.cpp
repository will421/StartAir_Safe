#include <iostream>
#include <stdio.h>

#include "Client.h";
#include "Server.h";

using namespace std;



int main()
{
	char r = 's';
	cout << "Client ou Server ? (c/s):" << endl;
	cin >> r;
	if (r == 'c'){
		//The Client
		int port = 60000;
		char adresse[15] = "127.0.0.1";
		cout << "Adresse: " << endl;
		cin >> adresse;
		cout << adresse << endl;
		cout << "Port: " << endl;
		cin >> port;
		//Client client = Client(port, string(adresse));
		//client.start();
	}
	else{
		// The Server
		int port = 60000;
		cout << "Port: " << endl;
		cin >> port;
		cout << "Port Server : " + port << endl;
		//Server server = Server(port);
		//server.start();
	}


}
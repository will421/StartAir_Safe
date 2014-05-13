#include "stdafx.h"
#include "UDPReceiver.h"
#include "config.h"


UDPReceiver::UDPReceiver(int port)
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	bind(sock, (SOCKADDR *)&sin, sizeof(sin));
	listen(sock, 0);

	safe::loadConfig();
	try {
		if (!safe::cfg_Safe.lookupValue("udpreceiver_buffer_size", size_buff))
		{
			std::cerr << "warning : udpreceiver_buffer_size missing in " << configFile << "the listening port will be set to 255" << std::endl;
			size_buff = 255;
		}
	}
	catch (const SettingNotFoundException &nfex)
	{
		std::cerr << "warning : udpreceiver_buffer_size missing in " << configFile << "the listening port will be set to 255" << std::endl;
		size_buff = 255;
	}

}


UDPReceiver::~UDPReceiver()
{
	closesocket(sock);
	WSACleanup();
	//delete[] buffer;
}

//void UDPReceiver::datagramReceived(SOCKADDR & from, SAFE_RECV & message)
//{
//	//std::cout << "NOOOOOO" << std::endl;
//}


void UDPReceiver::boucle()
{
	
	char * buffer = NULL;
	buffer = new char[size_buff];

	SOCKADDR_IN csin;
	int boucle = 0;
	while (1) /* Boucle infinie. Exercice : améliorez ce code. */
	{
	    
		int sizeof_csin = sizeof(csin);
		int res = recvfrom(sock, buffer, /*sizeof(buffer)*/size_buff, 0, (SOCKADDR *)&csin, &sizeof_csin);
		int error = WSAGetLastError();
		SAFE_RECV * recv = (SAFE_RECV *)buffer;
		
		datagramReceived(*(SOCKADDR *)&csin,*recv);

	}
	delete[] buffer;
	closesocket(sock);
}

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
}


UDPReceiver::~UDPReceiver()
{
	closesocket(sock);
	WSACleanup();
}

//void UDPReceiver::datagramReceived(SOCKADDR & from, SAFE_RECV & message)
//{
//	//std::cout << "NOOOOOO" << std::endl;
//}


void UDPReceiver::boucle()
{
	int size;
	safe::loadConfig();
	try {
		if (!safe::cfg_Safe.lookupValue("udpreceiver_buffer_size", size))
		{
			std::cerr << "warning : udpreceiver_buffer_size missing in " << configFile << "the listening port will be set to 255" << std::endl;
			size = 255;
		}
	}
	catch (const SettingNotFoundException &nfex)
	{
		std::cerr << "warning : udpreceiver_buffer_size missing in " << configFile << "the listening port will be set to 255" << std::endl;
		size = 255;
	}

	char buffer[255];

	SOCKADDR_IN csin;
	int boucle = 0;
	while (1) /* Boucle infinie. Exercice : améliorez ce code. */
	{
	    
		int sizeof_csin = sizeof(csin);
		int res = recvfrom(sock, buffer, sizeof(buffer), 0, (SOCKADDR *)&csin, &sizeof_csin);
		int error = WSAGetLastError();
		SAFE_RECV * recv = (SAFE_RECV *)buffer;
		
		datagramReceived(*(SOCKADDR *)&csin,*recv);
		//datagramReceived(*(SOCKADDR *)&csin, SAFE_RECV_ID_POS, buffer);
		//std::cout << res << " " << errno  << " " << error  << std::endl;
	


		//boucle++;
		//if (boucle > 10)
		//system("pause");
		//	exit(0);
	}
	closesocket(sock);
}
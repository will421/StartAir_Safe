#include "stdafx.h"
#include "UDPSender.h"

using std::string;
using std::pair;

UDPSender::UDPSender()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
}


UDPSender::~UDPSender()
{
	closesocket(sock);
	WSACleanup();
}

void UDPSender::addTarget(string adresse, int port)
{
	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = inet_addr(adresse.c_str());
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	targets.push_back(sin);
}

void UDPSender::send(char * data, int size)
{
	int error;
	for each (SOCKADDR_IN sin in targets)
	{
		if (sendto(sock, data, size, 0, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
			error = WSAGetLastError();
		}
	}
	
}


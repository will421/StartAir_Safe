#pragma once

#include <string>
#include <list>
#include <utility>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")


class UDPSender
{
public:
	UDPSender();
	~UDPSender();
	void addTarget(std::string adresse, int port);
	void send(char * data, int size);
private:
	std::list<SOCKADDR_IN> targets;
	SOCKET sock;
};


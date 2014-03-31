#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include "types.h"

class UDPReceiver
{
public:
	UDPReceiver(int port);
	virtual ~UDPReceiver();
	void boucle();

protected:
	virtual void datagramReceived(SOCKADDR & from, SAFE_RECV & message) = 0;
private:
	SOCKET sock;

};


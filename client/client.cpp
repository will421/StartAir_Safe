// client.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "SimConnect.h"
#include "CSafe.h"
#include "sendData.h"

using namespace safe;

int _tmain(int argc, _TCHAR* argv[])
{
	//CSafe s = CSafe();
	//s.launch();

	sendData d = sendData();
	d.launch();
	d.~sendData();

	return 0;
}


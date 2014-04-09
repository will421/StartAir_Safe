// server.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "CSafeServerSide.h"
#include "config.h"

using namespace safe;

int _tmain(int argc, _TCHAR* argv[])
{
	loadConfig();

	safe::CSafeSS c;
	c.launch();

	return 0;
}


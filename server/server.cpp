// server.cpp�: d�finit le point d'entr�e pour l'application console.
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


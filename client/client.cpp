// client.cpp�: d�finit le point d'entr�e pour l'application console.
//

#include "stdafx.h"
#include "SimConnect.h"
#include "CSafe.h"

using namespace safe;

int _tmain(int argc, _TCHAR* argv[])
{
	CSafe s = CSafe();
	s.launch();

	return 0;
}


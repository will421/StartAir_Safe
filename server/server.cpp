// server.cpp�: d�finit le point d'entr�e pour l'application console.
//

#include "stdafx.h"
#include "CSafeServerSide.h"


int _tmain(int argc, _TCHAR* argv[])
{
	safe::CSafeSS c;
	c.launch(15020);

	return 0;
}


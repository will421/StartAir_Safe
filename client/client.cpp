// client.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

#include "CSafe.h"
#include "DataSender.h"

using namespace safe;

int _tmain(int argc, _TCHAR* argv[])
{
	CSafe s = CSafe(15020); 
	s.launch();

	//DataSender d = DataSender(); d.launch(); d.~DataSender();
	
	return 0;
}


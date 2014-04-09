// client.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

#include "CSafe.h"
#include "DataSender.h"
#include "config.h"
#include <string>


using namespace safe;

int _tmain(int argc, _TCHAR* argv[])
{	
	loadConfig();
	int port;
	try {
		bool res = cfg_Safe.exists("datasender_position_definition_id");
		if (!cfg_Safe.lookupValue("client_listening_port", port))
		{
			std::cerr << "warning : client_listening_port missing in " << configFile << "the listening port will be set to 15020" << std::endl;
			port = 15020;
		}
	}
	catch (const SettingNotFoundException &nfex)
	{
		std::cerr << "warning : client_listening_port missing in " << configFile << "the listening port will be set to 15020" << std::endl;
		port = 15020;
	}

	CSafe s = CSafe(port);
	s.launch();
	
	return 0;
}


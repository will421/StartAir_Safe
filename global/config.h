#ifndef CONFIG_H
#define CONFIG_H


#pragma once

#include <libconfig.h++>

using namespace libconfig;

#define configFile "safe.cfg"

namespace safe{

	static Config cfg_Safe;

	static void loadConfig(){
		try
		{
			cfg_Safe.readFile(configFile);
		}
		catch (const FileIOException &fioex)
		{
			std::cerr << "I/O error while reading file. " << "Is " << "\"" <<configFile <<  "\""  << " existing ?" << std::endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
		catch (const ParseException &pex)
		{
			std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
				<< " - " << pex.getError() << std::endl;
			exit(EXIT_FAILURE);
		}
	}


};

#endif
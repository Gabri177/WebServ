#include "../include/Config.hpp"
#include "../include/Server.hpp"


t_config  g_config;

int	main(int argc, char **argv){

	std::string config_file;
	if (argc == 1)
		config_file = "./config/default.ini";
	else if (argc == 2)
		config_file = argv[1];
	else{

		std::cout << "Error: Wrong arguments!!!" << std::endl;
		return 1;
	}
	std::cout << "Using configuration file: " << config_file << std::endl;
	Config	_config;
	g_config = _config.parseConfig(config_file);
	Server		_serv(g_config);
	//std::cout << _config["server"]["port"] << std::endl;
	// for (t_config_it it = g_config.begin(); it != g_config.end(); it ++){

	// 	for (t_location_it itl = (*it)._location.begin(); itl != (*it)._location.end(); itl ++){

	// 		if ((*itl).second._autoindex)
	// 			std::cout << "the   " << (*itl).second._root << "    HAAAAAAASSSSSS auto index" << std::endl;
	// 		else
	// 			std::cout << "the   " << (*itl).second._root << "    do nos has auto index" << std::endl;
	// 	}
	// }
	_serv.start();

	return 0;
}

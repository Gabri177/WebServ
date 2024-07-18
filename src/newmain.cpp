#include "../include/Config.hpp"
#include "../include/Server.hpp"

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
	Config	_config;
	_config.parseConfig(config_file);
	Server		_serv(_config.servers);

	//std::cout << _config["server"]["port"] << std::endl;
	_serv.start();

	return 0;
}
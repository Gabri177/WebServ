#include "../include/ConfigInfo.hpp"
#include "../include/Server.hpp"

int	main(int argc, char **argv){

	if (argc == 1)
		ConfigInfo	_config("./config/default.ini");
	else if (argc == 2)
		ConfigInfo 	_config(arv[1]);
	else{

		std::cout << "Error: Wrong arguments!!!" << std::endl;
	}
	Server		_serv(_config);

	//std::cout << _config["server"]["port"] << std::endl;
	_serv.start();

	return 0;
}
#include "../include/ConfigInfo.hpp"
#include "../include/Server.hpp"

int	main(){

	ConfigInfo	_config("./config/default.ini");
	Server		_serv(_config);

	std::cout << _config["server"]["port"] << std::endl;
	_serv.start();

	return 0;
}
#include "../include/parse_config.hpp"

int	main(){

	ConfigInfo	_config("/Users/gabrielgao/Desktop/42/WebServ/config/default.ini");

	std::cout << _config["server"]["port"] << std::endl;

	return 0;
}
#include "../include/ConfigInfo.hpp"

int	main(){

	ConfigInfo	_config("./config/default.ini");

	std::cout << _config["server"]["port"] << std::endl;

	return 0;
}
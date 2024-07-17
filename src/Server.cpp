#include "../include/Server.hpp"



Server::Server(const std::string & config_file){

	// read config information if it can not read the file, it will throw an error.
	try{

		ConfigInfo	set_info(config_file);
	}catch (std::exception & e){

		std::cout << e.what() << std::endl;
	}
}

Server::~Server(){

}

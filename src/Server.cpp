#include "../include/Server.hpp"



Server::Server(const ConfigInfo & info){

	_info = info.getInfo();
}

Server::~Server(){

}


void								Server::start(){

	int						host_sock;
	struct sockaddr_in		host_addr;

	if ((host_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Server start error!!!");
	
	memset(&host_addr, 0, sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	char	*end;
	int		host_port = std::strtol(_info["server"]["port"].c_str(), &end, 10);
	if (*end != '\0'){

		close(host_sock);
		throw std::runtime_error("Port format error!!!");
	}
	host_addr.sin_port = htons(host_port);

	if (bind(host_sock, (struct sockaddr *)&host_addr, sizeof(host_addr)) == -1){

		close(host_sock);
		throw std::runtime_error("Server start error!!!");
	}
	
	if (listen(host_sock, 1024) == -1){

		close(host_sock);
		throw std::runtime_error("Server start error!!!");
	}
	
	std::cout << "Server started successfully on port " << host_port << std::endl;
}
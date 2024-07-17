#include "../include/Server.hpp"



void								Server::set_nonblockint(int fd){

	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("fcntl F_GETFL error!!!");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl F_SETFL error!!!");
}

Server::Server(const ConfigInfo & info): _info(info.getInfo()), epoll_fd(-1), host_sock(-1){

}

Server::~Server(){

	if (epoll_fd != -1)
		close(epoll_fd);
	if (host_sock != -1)
		close(host_sock);
}


void								Server::start(){

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

	set_nonblockint(host_sock);

	if ((epoll_fd = epoll_create1(0)) == -1){
		
		close(host_sock);
		throw std::runtime_error("Server start error!!!");
	}

	struct epoll_event event;
	event.data.fd = host_sock;
	event.events = EPOLLIN | EPOLLET;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, host_sock, &event) == -1){

		close(host_sock);
		throw std::runtime_error("Server start error!!!");
	}
	
	std::cout << "Server started successfully on port " << host_port << std::endl;
}
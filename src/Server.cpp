#include "../include/Server.hpp"



void								Server::set_nonblocking(int fd){

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

	set_nonblocking(host_sock);

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


	const int MAX_EVENTS = 10;
	struct epoll_event events[MAX_EVENTS];

	while (true) {
		int num_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (num_fds == -1) {
			close(host_sock);
			throw std::runtime_error("Server error: epoll_wait failed");
		}

		for (int i = 0; i < num_fds; ++i) {
			if (events[i].data.fd == host_sock) {
				while (true) {
					struct sockaddr_in client_addr;
					socklen_t client_len = sizeof(client_addr);
					int client_fd = accept(host_sock, (struct sockaddr *)&client_addr, &client_len);
					if (client_fd == -1) {
						if (errno == EAGAIN || errno == EWOULDBLOCK) {
							break; // No more incoming connections
						} else {
							close(host_sock);
							throw std::runtime_error("Server error: accept failed");
						}
					}

					set_nonblocking(client_fd);

					struct epoll_event client_event;
					client_event.data.fd = client_fd;
					client_event.events = EPOLLIN | EPOLLET;

					if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) == -1) {
						close(client_fd);
						close(host_sock);
						throw std::runtime_error("Server error: epoll_ctl failed to add client");
					}
				}
			} else {
				// Handle client requests
				int client_fd = events[i].data.fd;
				char buffer[512];
				ssize_t count;
				while ((count = read(client_fd, buffer, sizeof(buffer))) > 0) {
					write(client_fd, buffer, count); // Echo back the data
					printf ("Receive data: %s\n", buffer);
					memset(&buffer, 0, sizeof(buffer));
				}
				if (count == -1 && errno != EAGAIN) {
					close(client_fd);
					close(host_sock);
					throw std::runtime_error("Server error: read failed");
				}
				if (count == 0) {
					close(client_fd); // Client disconnected
				}
			}
		}
	}
}
#include "../include/Server.hpp"


// Set file descriptor to non-blocking state
void								Server::set_nonblocking(int fd){

	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("fcntl F_GETFL error!!!");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl F_SETFL error!!!");
}

// Alternative Functions for Reducing Code
static void							err_close_throw(int sock, const std::string & info){

	close(sock);
	throw std::runtime_error(info);
}

static std::vector<int>				parse_port(const std::string & port_set){

	std::vector<int>	temp;
	std::string			token;
	std::stringstream	ss(port_set);

	while(std::getline(ss, token, ',')){

		int port;
		std::stringstream	convert(token);
		convert >> port;
		temp.push_back(port);
	}
	return temp;
}

// Load configuration file information, set up epoll event sheet and host sock
Server::Server(const ConfigInfo & info): _info(info.getInfo()), epoll_fd(-1){

	ports = parse_port(_info["server"]["port"]);
}

Server::~Server(){

	if (epoll_fd != -1)
		close(epoll_fd);
	for (std::vector<int>::iterator it = host_socks.begin(); it != host_socks.end(); it ++){

		if (*it != -1)
			close (*it);
	}
}


void								Server::start(){

	struct sockaddr_in		host_addr;

	if ((epoll_fd = epoll_create1(0)) == -1)
		std::runtime_error("Epoll event establish error!!!");

	std::vector<int>::iterator	it = ports.begin();
	while (it != ports.end()){

		int host_sock = -1;
		//host buzon set
		if ((host_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw std::runtime_error("Server start error!!!");
		memset(&host_addr, 0, sizeof(host_addr));
		host_addr.sin_family = AF_INET; //IPv4
		host_addr.sin_addr.s_addr = htonl(INADDR_ANY); // listen all the interface of the Internet (IP) host to net long
		host_addr.sin_port = htons(*it); // port. host to net short

		if (bind(host_sock, (struct sockaddr *)&host_addr, sizeof(host_addr)) == -1)
			err_close_throw(host_sock, "Server start error!!!");
		
		if (listen(host_sock, 4096) == -1)
			err_close_throw(host_sock, "Server start error!!!");

		set_nonblocking(host_sock);

		struct epoll_event event;
		event.data.fd = host_sock;
		event.events = EPOLLIN | EPOLLET; // see in man epoll_ctl

		//add the host_sock in the epoll event to listen
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, host_sock, &event) == -1)
			err_close_throw(host_sock, "Server start error!!!");
		host_socks.push_back(host_sock);
		it ++;
	}
	
	std::cout << "Server started successfully on port "  << std::endl;


	const int MAX_EVENTS = 10; // the maximium event can be solved at the same time
	struct epoll_event events[MAX_EVENTS];

	while (true) {
		int num_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (num_fds == -1)
			throw std::runtime_error("Server error: epoll_wait failed");


		for (int i = 0; i < num_fds; ++i) {
			if (std::find(host_socks.begin(), host_socks.end(), events[i].data.fd) != host_socks.end()) {
				while (true) {
					struct sockaddr_in	client_addr;
					socklen_t 			client_len = sizeof(client_addr);
					int 				client_fd = accept(events[i].data.fd, (struct sockaddr *)&client_addr, &client_len);

					if (client_fd == -1) {

						// No more incoming connections
						if (errno == EAGAIN || errno == EWOULDBLOCK)
							break;
						else
							throw std::runtime_error("Server error: accept failed");
					}

					set_nonblocking(client_fd);

					struct epoll_event client_event;
					client_event.data.fd = client_fd;
					client_event.events = EPOLLIN | EPOLLET;

					if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) == -1) {

						close(client_fd);
						err_close_throw(events[i].data.fd, "Server error: epoll_ctl failed to add client");
					}
				}
			} else
				handle_client_request(events[i].data.fd);
		}
	}
}

// Handle client requests
void								Server::handle_client_request(int client_fd){

	char	buffer[1024];
	ssize_t count;

	count = 0;
	memset(&buffer, 0, sizeof(buffer));
	while ((count = read(client_fd, buffer, sizeof(buffer))) > 0) {

		// Echo back the data
		write(client_fd, buffer, count);
		printf ("Receive data: %s\n", buffer);
		memset(&buffer, 0, sizeof(buffer));
	}
	if (count == -1 && errno != EAGAIN) {

		close(client_fd);
		throw std::runtime_error("Server error: read failed");
	}
	if (count == 0)
		close(client_fd);
}
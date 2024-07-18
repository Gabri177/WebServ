#ifndef	SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <stdexcept>
# include <arpa/inet.h>
# include <algorithm>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <netinet/in.h>
# include <sys/epoll.h>
# include <string>
# include <cstdlib>
# include <cstdio>
# include <fcntl.h>
# include "./Config.hpp"
# include <cerrno>
# include <map>
# include <cstring>
# include <vector>
# include <sstream>
# include "./RequestHandler.hpp"

class ConfigInfo;

class Server {

	private:
		
		//std::map<std::string, std::map<std::string, std::string> >		_info;
		std::vector<ServerConfig>										_info;
		int																epoll_fd;
		std::vector<int>												host_socks;
		std::vector<int>												ports;

		void															set_nonblocking(int fd);

		
	public:
																		Server(const std::vector<ServerConfig> & info);
																		~Server();
		void															start();
};

#endif
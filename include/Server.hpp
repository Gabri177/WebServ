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
# include "./ConfigInfo.hpp"
# include <cerrno>
# include <map>
# include <cstring>

class ConfigInfo;

class Server {

	private:
		
		std::map<std::string, std::map<std::string, std::string> >		_info;
		int																epoll_fd;
		int																host_sock;

		void															set_nonblockint(int fd);

		
	public:
																		Server(const ConfigInfo & info);
																		~Server();
		void															start();
};

#endif
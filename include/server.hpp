#ifndef	SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <algorithm>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <netinet/in.h>
# include <string>
# include <cstdlib>
# include <cstdio>
# include "./ConfigInfo.hpp"
# include <cerrno>
# include <map>

class ConfigInfo;

class Server {

	private:
		
		std::map<std::string, std::map<std::string, std::string> >		_info;

		
	public:
																		Server(const ConfigInfo & info);
																		~Server();
		void															start();
};

#endif
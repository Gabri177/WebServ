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

class ConfigInfo;

class Server {

	public:
												Server(const std::string & config_file);
												~Server();
};

#endif
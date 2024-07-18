#include "../include/Config.hpp"

static std::string										str_no_space(const std::string & ori){

	size_t	start = ori.find_first_not_of(' ');
	if (start == std::string::npos)
		return	ori;
	size_t	end	  = ori.find_last_not_of(' ');
	return ori.substr(start, end - start + 1);

}

static std::pair<std::string, std::string>				str_split(const std::string & line, char deliminator){


	size_t	p_deliminator = line.find(deliminator);
	if (p_deliminator == std::string::npos)
		throw std::invalid_argument("Error: Invalid format in config file!!!");
	return std::make_pair(str_no_space(line.substr(0, p_deliminator)), str_no_space(line.substr(p_deliminator + 1)));
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

LocationConfig								parseLocation(const std::string & content){

	LocationConfig				loc;
	std::string					line;
	std::istringstream	iss(content);
	while(std::getline(iss, line)){

		std::istringstream		iss_line(line);
		std::string				FWord;

		iss_line >> FWord;
		if (FWord == "client_size"){

			long	len;
			iss_line >> len;
			loc._client_size = len;
		}else if (FWord == "methods"){

			std::string	 meth;
			while(iss_line >> meth)
				loc._methods.push_back(meth);
		}else if (FWord == "cgi_pass"){

			std::string	 path;
			iss_line >> path;
			loc._cgi = path;
		}else if (FWord == "error_page"){

			std::string	 i;
			std::string	 path;

			iss_line >> i >> path;

			if (i == "400")
				loc._err_page[400] = path;
			else if (i == "405")
				loc._err_page[405] = path;
			else if (i == "404")
				loc._err_page[404] = path;
		}else if (FWord == "return"){

			std::string  path;
			iss_line >> path;
			loc._return_url = path;
		}else if (FWord == "autoindex"){

			std::string	 stats;
			iss_line >> stats;

			if (stats == "on")
				loc._autoindex = true;
			else if (stats == "off")
				loc._autoindex = false;
		}else if (FWord == "index"){

			std::string  path;
			iss_line >> path;
			loc._index = path;
		}
	}
	return loc;
}

void                                        Config::parseConfig(const std::string & filename){

    std::ifstream               file(filename);
    if (!file.is_open())
        throw std::runtime_error("Error: coould not open the file!!!");
    
    std::string  				line;
    ServerConfig 				serverConfig;

    while (std::getline(file, line)){

        std::istringstream  	iss(line);
        std::string 			FWord;
		std::string				SWord;
        iss >> FWord >> SWord;
		// std::cout << "line: " << line << std::endl;
		// std::cout << std::endl << "             FWord: " << FWord << std::endl << std::endl;

        if (FWord == "server"){

            if (!serverConfig._ip.empty()){

				// for (std::vector<std::string>::iterator it = serverConfig._methods.begin(); it != serverConfig._methods.end(); it ++)
				// 	std::cout << *it << " " ;
				// std::cout << std::endl;
                servers.push_back(serverConfig);
                serverConfig = ServerConfig();
            }
			
        }else if (FWord == "listen"){

			size_t				start = SWord.find_first_of(':');
			if (start == std::string::npos)
				throw std::runtime_error("Error: wrong format in ip and port set!!!");
			serverConfig._ip = str_no_space(SWord.substr(0, start));
			serverConfig._port = parse_port(SWord.substr(start + 1));
			// for (std::vector<int>::iterator it = serverConfig._port.begin(); it != serverConfig._port.end(); it ++)
			// 	std::cout << "  " << *it;
			// std::cout << std::endl;
		}else if (FWord == "server_name")
			serverConfig._name = SWord;		
		else if (FWord == "root")
			serverConfig._root = SWord;
		else if (FWord == "index")
			serverConfig._index = SWord;
		else if (FWord == "methods"){

			serverConfig._methods.push_back(SWord);
			while(iss >> SWord)
				serverConfig._methods.push_back(SWord);
		}else if (FWord == "error_page"){
			
			std::string			temp;
			if (!(iss >> temp))
				throw std::runtime_error ("Error: wrong format of err_page set!!!");
			//std::cout << temp << "==" << std::endl;
			if (SWord == "400")
				serverConfig._err_page[400] = temp;
			else if (SWord == "405")
				serverConfig._err_page[405] = temp;
			else if (SWord == "404")
				serverConfig._err_page[404] = temp;
		}else if (FWord == "client_size"){

			char *end;
			size_t len = std::strtol(SWord.c_str(), &end, 10);
			if (*end != '\0')
				throw std::runtime_error("Error: wrong format for the client_size!!!");
			serverConfig._client_size = len;
			//std::cout << "size_client: " << len << std::endl;
		}else if (FWord == "location"){

			if (serverConfig._ip.empty())
				throw std::runtime_error("Error: wrong format en the server setting, missing info!!!");

            std::getline(file, line, '}');
			//std::cout << "read content: \"" << line << "\"" << std::endl;
            serverConfig._location[SWord] = parseLocation(line);
        }else {
           //parseServer(iss, serverConfig);
        }

    }

    if (!serverConfig._ip.empty()){
		// for (std::vector<std::string>::iterator it = serverConfig._methods.begin(); it != serverConfig._methods.end(); it ++)
		// 	std::cout << *it << " " ;
		// std::cout << std::endl;
        servers.push_back(serverConfig);
	}

}
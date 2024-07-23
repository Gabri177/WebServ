#include "../include/Config.hpp"
#include "../include/Server.hpp"
#include "../include/Cgi.hpp"

t_config  g_config;
char **g_envp = NULL;

int	main(int argc, char **argv, char **envp)
{
    g_envp = envp;

	/*std::string config_file;
	if (argc == 1)
		config_file = "./config/mac_server.conf";
	else if (argc == 2)
		config_file = argv[1];
	else{

		std::cout << "Error: Wrong arguments!!!" << std::endl;
		return 1;
	}
	std::cout << "Using configuration file: " << config_file << std::endl;
	Config	_config;
	g_config = _config.parseConfig(config_file);
	Server		_serv(g_config);
	_serv.start();

	return 0;*/

	process_script("/home/javier/Desktop/executable.py");
	(void)argc;
	(void)argv;
}

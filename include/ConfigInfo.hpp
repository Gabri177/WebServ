#ifndef PARSER_CONFIG_HPP
# define PARSER_CONFIG_HPP
# include <string>
# include <iostream>
# include <fstream>
# include <iostream>
# include <map>

class	ConfigInfo {

	private:
		std::map<std::string, std::map<std::string, std::string> >		_data;

	public:
																		ConfigInfo(const std::string & file_name);
																		~ConfigInfo();
		std::map<std::string, std::string>								operator[](const std::string & key_name);
		std::map<std::string, std::map<std::string, std::string> >		getInfo() const;
};


#endif
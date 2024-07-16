#include "../include/ConfigInfo.hpp"


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

ConfigInfo::ConfigInfo(const std::string & file_name){

	std::ifstream	file(file_name.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: Could not open the config file!!!");

	std::string									new_line;
	std::string									temp_key;
	while(std::getline(file, new_line)){

		if (new_line == "\n" || new_line.empty())
			continue ;
		if (new_line.find('[') != std::string::npos && new_line.find(']') != std::string::npos){

			size_t	start = new_line.find('[');
			size_t	end   = new_line.find(']');
			temp_key = new_line.substr(start + 1, end - start - 1);
		}else if (new_line.find('=') != std::string::npos && !temp_key.empty()){

			std::pair<std::string, std::string>		content = str_split(new_line, '=');
			_data[temp_key][content.first] = content.second;
		}else
			throw	std::runtime_error("Error: Invalid format in config file!!!");
	}
}

ConfigInfo::~ConfigInfo(){}

std::map<std::string, std::string>						ConfigInfo::operator[](const std::string & key_name){

	std::map<std::string, std::map<std::string, std::string> >::iterator	it = _data.find(key_name);
	if (it != _data.end())
		return _data[key_name];
	else
		throw std::runtime_error("The key to search does nos exist!!!");
}	


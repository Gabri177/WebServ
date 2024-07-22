#include "./include/ParserURL.hpp"


int main(void){


	std::map<std::string, std::string>  key_val;

	key_val = ParserURL<std::string, std::string>::get_url_key_val("www.baidu.com/index.html");

	if (!key_val.empty())
		for (std::map<std::string, std::string>::iterator it = key_val.begin(); it != key_val.end(); it ++)
			std::cout << "key: " << it->first << "\t" << "Val: " << it->second << std::endl;
	else
		std::cout << "Nooooooooooooooooooooooooooooooo" << std::endl;
	



	return 0;
}
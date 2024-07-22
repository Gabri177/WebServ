#ifndef PARSERURL_HPP
# define PARSERURL_HPP
# include <map>
# include <vector>
# include <algorithm>
# include <iostream>
# include <sstream>

template <typename T, typename B>
class ParserURL {

	public:

		static std::map<T, B>				get_url_key_val(const std::string & oriurl){

			std::map<T, B>	temp;
			size_t			p_question_mark = oriurl.find('?');
			if (p_question_mark == std::string::npos)
				return temp;
			std::string		cur_url = oriurl.substr(p_question_mark + 1);
			if (cur_url.empty())
				return temp;
			
			std::istringstream	iss(cur_url);
			std::string			newline;
			std::string			key;
			std::string			val;
			while(std::getline(iss, newline, '&')){

				std::istringstream key_val(newline);
				if (std::getline(key_val, key, '=') && std::getline(key_val, val))
					temp[key] = val;
			}
			return temp;
		}
};

#endif
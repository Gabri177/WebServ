#ifndef PARSERURL_HPP
# define PARSERURL_HPP
# include <map>
# include <vector>
# include <algorithm>
# include <iostream>
# include <sstream>


class ParserURL {

	public:

		typedef std::map<std::string, std::string>				t_str_keyval_map;
		typedef t_str_keyval_map::iterator						t_str_keyval_map_it;

		static t_str_keyval_map									get_url_key_val(const std::string & oriurl){

			t_str_keyval_map	temp;
			size_t				p_question_mark = oriurl.find('?');
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
#include "../include/ParserURL.hpp"


static void												replace_path(std::string& str, const std::string& from, const std::string& to) {

	size_t 				start_pos = str.find(from);

	if (start_pos != std::string::npos && start_pos == 0)
		str.replace(start_pos, from.length(), to);
}

static int 												count_occurrences(const std::string& str, char ch) {

	size_t				count = 0;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
		if (*it == ch)
			++ count;
	return count;
}

t_str_keyval_map										ParserURL::get_url_key_val(const std::string & oriurl){

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

const std::string										ParserURL::get_abs_url(std::string oriurl, const ServerConfig & curserv, const std::string & meth){

	//std::cout << "PARSERURL: oriurl==>\"" << oriurl << "\"" << std::endl;
	t_str_vec			methods;
	bool				is_access = false;
	size_t				p_question_mark = oriurl.find('?');
	if(p_question_mark != std::string::npos)
		oriurl = oriurl.substr(0, p_question_mark);

	bool				is_file = false;
	size_t				p_file_mark = oriurl.find('.');
	std::string			path;
	std::string			filename;
		

	if (p_file_mark != std::string::npos){

		is_file = true;
		//if (oriurl == "/")
			path = oriurl.substr(0, oriurl.find_last_of('/') + 1);
		// else
		// 	path = oriurl.substr(0, oriurl.find_last_of('/'));
		filename = oriurl.substr(oriurl.find_last_of('/') + 1);
		//std::cout << "has fileeeeeeeeeeeeeeeeeeeeeeeeeeeee\n\n";
	}else
		path = oriurl;

	std::string			match_location_path = path;
	int					path_grade = 0;
	LocationConfig		temp_loc;
	// std::cout << "PARSERURL: path=>>\"" << path << "\"" << std::endl;
	// std::cout << "PARSERURL: filename=>>\"" << filename << "\"" << std::endl;
	for(t_location_it_const it = curserv._location.begin(); it != curserv._location.end(); it ++){

		if (path.find(it->first) == 0 && count_occurrences(it->first, '/') >= path_grade){

			match_location_path = path;
			if (it->first != "/"){

				path_grade = count_occurrences(it->first, '/');
				// std::cout << "\n\n				PARSERURL: current_match_path ==>\"" << it->second._root << "\"" << std::endl;
				// std::cout << "				PARSERURL: current_match_first ==>\"" << it->first << "\"" << std::endl;
				// std::cout << "				PARSERURL: current_match_path ==>\"" << match_location_path << "\"\n\n" << std::endl;
				replace_path(match_location_path, it->first, it->second._root);
				//std::cout << "				PARSERURL: switch_path results ==>\"" << match_location_path << "\"" << std::endl;
				temp_loc = it->second;
				//std::cout << "				AUTOINDEX::::::::" << temp_loc._autoindex << std::endl;
				methods = it->second._methods;
				break ;
			}else{

				//match_location_path = it->second._root;
				match_location_path = it->second._root + path;
				//std::cout << "							PARSERURL: switch_path results ==>\"" << match_location_path << "\"" << std::endl;
				methods = it->second._methods;
				temp_loc = it->second;
			}
		}
	}

	// std::cout << "PARSERURL: match_location_path =>>\"" << match_location_path << "\"" << std::endl;
	// std::cout << "PARSERURL: filename =>>\"" << filename << "\"" << std::endl; 

	for (t_str_vec_it it = methods.begin(); it != methods.end(); it ++)
		if (*it == meth){
			
			is_access = true;
			break;
		}
	if (is_access){

		if(!is_file){

			if (temp_loc._autoindex == true && !temp_loc._index.empty()){

				//std::cout << "------------------------------------\n";
				return match_location_path + "/" + temp_loc._index;
			}
			else{

				//std::cout << "++++++++++++++++++++++++++++++++++++\n";
				return match_location_path;
			}
		} else
			return match_location_path + "/" + filename;
	}else
		return "";

	/// methods test
}
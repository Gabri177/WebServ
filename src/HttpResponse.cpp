/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabpalma <pabpalma>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:49:20 by pabpalma          #+#    #+#             */
/*   Updated: 2024/07/25 14:36:29 by pabpalma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HttpResponse.hpp"

std::string 				getContentType(const std::string& file_extension){

    static std::map<std::string, std::string> content_types;
	if (content_types.empty()){

		content_types[".html"] = "text/html";
		content_types[".htm"] = "text/html";
		content_types[".txt"] = "text/plain";
		content_types[".css"] = "text/css";
		content_types[".js"] = "text/javascript";
		content_types[".json"] = "application/json";
		content_types[".xml"] = "application/xml";
		content_types[".jpg"] = "image/jpeg";
		content_types[".jpeg"] = "image/jpeg";
		content_types[".png"] = "image/png";
		content_types[".gif"] = "image/gif";
		content_types[".webp"] = "image/webp";
		content_types[".svg"] = "image/svg+xml";
		content_types[".mp3"] = "audio/mpeg";
		content_types[".ogg"] = "audio/ogg";
		content_types[".mp4"] = "video/mp4";
		content_types[".webm"] = "video/webm";
		content_types[".pdf"] = "application/pdf";
		content_types[".zip"] = "application/zip";
    }

    std::map<std::string, std::string>::iterator it = content_types.find(file_extension);
    if (it != content_types.end())
        return it->second;
    return "application/octet-stream";
}

std::string 				determineContentType(const std::string& filepath){
	
    size_t dot_pos = filepath.find_last_of(".");
    if (dot_pos == std::string::npos)
        return "application/octet-stream";

    std::string file_extension = filepath.substr(dot_pos);
    return getContentType(file_extension);
}

std::string					list_directory(const std::string &path){

	std::cout << "Start List Directory ..." << std::endl;
    std::ostringstream body;

    body << "<html><head><title>Directory listing for current path ...</title></head><body>";
    body << "<h1>Directory listing for current path ...</h1><ul>";
    DIR *dir = opendir(path.c_str());

    if (dir){

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL){

            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){

                body << "<li>" << entry->d_name << "</li>";
            }
        }
        closedir(dir);
    }else{

        body << "<li>Could not open directory</li>";
    }
    body << "</ul></body></html>";
    return body.str();
}

bool						HttpResponse::is_exist_err_page(t_status_respond page_num){

	bool	is_exist = false;
	if (!CurrentServerConfig._err_page.empty() && CurrentServerConfig._err_page.find(page_num) != CurrentServerConfig._err_page.end())
		is_exist = true;
	return is_exist;
}

static std::string			getHttpDate(){

	char		buffer[100];
	time_t		now = time(0);
	struct tm	tm = *gmtime(&now);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &tm);
	return std::string(buffer);
}

std::string					HttpResponse::loadFileContent(const std::string & url, const std::string & meth){

	std::cout << "Loading file content ..." << std::endl;
	std::ifstream	file(ParserURL::get_abs_url(url, CurrentServerConfig, meth).c_str());
	//std::cout << "loadFileContent: absolute path: \"" << ParserURL::get_abs_url(url, CurrentServerConfig, meth) << "\"" << std::endl;
	if (file.is_open()){

		//std::cout << "loadFileContent: file opend successfully..." << std::endl;
		std::stringstream	buffer;
		buffer << file.rdbuf();
		//std::cout << "loadFileContent: file content: \"" << buffer.str() << "\"" << std::endl;
		return buffer.str();
	}else{
		return "";
	}
}

void						HttpResponse::handleGet(const HttpRequest & request){

	//try{
		//std::string cur_url = urlToFilePath(request.url, "GET");
		std::string cur_url = ParserURL::get_abs_url(request.url, CurrentServerConfig, "GET");
		std::string red_url = ParserURL::get_redireccion_url(request.url, CurrentServerConfig, "GET", CurrentPort);

		std::cout << "GET: redireccion url => \"" << red_url << "\"" << std::endl;
		if (!red_url.empty()) {

			status_code = FOUND;
			status_text = RES_STATUS_FOUND;
			headers[CONTENT_LOCATION] = red_url;
			headers[CONTENT_SERVER] = "MyServer/1.0";
			headers[CONTENT_DATE] = getHttpDate();
			headers[CONTENT_CONNECTION] = "keep-alive";

			// Optionally, set the body to explain the redirection
			body = "<html><body><h1>302 Found</h1><p>Resource has moved to <a href=\"" + red_url + "\">" + red_url + "</a></p></body></html>";
			std::stringstream ss;
			ss << body.size();
			headers[CONTENT_LENGTH] = ss.str();
			headers[CONTENT_TYPE] = getContentType(".html");
			return;
    	}

		//std::cout << "Cur_path == > " << cur_url << std::endl;
		std::cout << "handleGet: start do the GET request..." << std::endl;

		if (cur_url.find('.') == std::string::npos) {

			body = list_directory(cur_url);
			headers[CONTENT_TYPE] = getContentType(".html");
		} else {

			if (cur_url.find(".py") != std::string::npos) {
				
				headers[CONTENT_TYPE] = getContentType(".html");
				body = "<html><body><h1>Result ==></h1><h2>" + run_cgi_script(cur_url) +"</h2></body></html>";
			} else {

				headers[CONTENT_TYPE] = getContentType(cur_url.substr(cur_url.find_last_of(".")));
				body = loadFileContent(request.url, "GET");
			}
		}
			
		//std::cout << "GET:: BODY :" << body << std::endl;
		if (body == "") {
			defaultErrPageSet(request, NOT_FOUND);
			return;
		}
		std::cout << "\n\nBODY CONTENT:\"" << body << "\"" << std::endl;
		status_code = OK;
		status_text = RES_STATUS_OK;

		std::stringstream	ss;
		ss << body.size();
		headers[CONTENT_LENGTH] = ss.str();
		headers[CONTENT_SERVER] = "MyServer/1.0";
		headers[CONTENT_DATE] = getHttpDate();
		headers[CONTENT_CONNECTION] = "keep-alive";
	// } catch (const std::exception & e) {

	// 	std::cout << "ERROR : " << e.what() << std::endl;
	// }
	
}


void						HttpResponse::handlePost(const HttpRequest & request){

	std::cout << "start do the POST request..." << std::endl;
	std::string  path = ParserURL::get_abs_url(request.url, CurrentServerConfig, "POST");
	std::cout << "POST: path -> " << path << std::endl;
	if (path == "" || request.headers.find("Content-Type") == request.headers.end()) {

		std::cout << "POST: could not find Content-Type..." << std::endl;
		defaultErrPageSet(request, NOT_FOUND);
		return;
	}
	std::map<std::string, std::string>::const_iterator it = request.headers.find("Content-Type");
   
	std::string content_type = it->second;
	if (content_type.find("multipart/form-data") == std::string::npos) {

        std::cout << "POST: Content-Type is not multipart/form-data..." << std::endl;
		std::cout << "POST: Content-Type: " << it->second << std::endl;
		std::cout << "POST: The body size of request is => " << request.body.size() << std::endl;
        //defaultErrPageSet(request, NOT_FOUND);
        return;
    }
	std::string boundary = "--" + content_type.substr(content_type.find("boundary=") + 9);
    std::cout << "Boundary: " << boundary << std::endl;

	std::string req_body = request.body;
    size_t boundaryPos = req_body.find(boundary);
    while (boundaryPos != std::string::npos) {
        size_t nextBoundaryPos = req_body.find(boundary, boundaryPos + boundary.size());
        std::string part = req_body.substr(boundaryPos + boundary.size(), nextBoundaryPos - (boundaryPos + boundary.size()));

        size_t contentDispPos = part.find("Content-Disposition: form-data; ");
        if (contentDispPos != std::string::npos) {
            size_t namePos = part.find("name=\"", contentDispPos);
            size_t nameEnd = part.find("\"", namePos + 6);
            std::string name = part.substr(namePos + 6, nameEnd - (namePos + 6));

            size_t filenamePos = part.find("filename=\"", contentDispPos);
            if (filenamePos != std::string::npos) {
                size_t filenameEnd = part.find("\"", filenamePos + 10);
                std::string filename = part.substr(filenamePos + 10, filenameEnd - (filenamePos + 10));

                size_t fileContentStart = part.find("\r\n\r\n", filenameEnd) + 4;
                std::string fileContent = part.substr(fileContentStart, part.find("\r\n--", fileContentStart) - fileContentStart);

                std::cout << "POST: filename: " << filename << std::endl;

                std::string filepath = path + "/" + filename;
				std::cout << "load path: " << filepath << std::endl;

                std::ofstream file(filepath.c_str(), std::ios::binary);
                if (file.is_open()) {

					std::cout << "POST:  File opened." << std::endl;
                    file.write(fileContent.c_str(), fileContent.size());
                    file.close();
					std::cout << "Upload sucess !!!" << std::endl;
					
					if (filename.find(".py") != std::string::npos){
						
						body = "<html><body><h1>Result ==></h1><h2>" + run_cgi_script(filepath) +"</h2></body></html>";
						status_code = OK;
						status_text = RES_STATUS_CREATED;
						headers[CONTENT_SERVER] = "MyServer/1.0";
						headers[CONTENT_DATE] = getHttpDate();
						headers[CONTENT_CONNECTION] = "keep-alive";
						
					}else{

						defaultErrPageSet(request, OK);
					}
						
                    return;
                } else {

					std::cout << "POST:  File can not open." << std::endl;
					defaultErrPageSet(request, INTERNAL_SERVER_ERROR);
                    return;
                }
            }
        }

        boundaryPos = req_body.find(boundary, nextBoundaryPos);
	}
}

void						HttpResponse::handleDelete(const HttpRequest & request){

	std::cout << "DELETE: start to solve the DELETE request..." << std::endl;

	std::string	cur_url = ParserURL::get_abs_url(request.url, CurrentServerConfig, "DELETE");

	std::cout << "DELETE: absolute path -> \"" << cur_url << "\"" << std::endl; 

	struct stat file_info;

	if (stat(cur_url.c_str(), &file_info) != 0){

		std::cout << "DELETE: file does not exist..." << std::endl;
		defaultErrPageSet(request, NOT_FOUND);
		return ;
	}

	if (cur_url == ""){
		
		std::cout << "DELETE: refuse to delete the file, do not have right..." << std::endl;
		defaultErrPageSet(request, FORBIDDEN);
		return ;
	}

	if (remove(cur_url.c_str()) == 0){

		defaultErrPageSet(request, OK);
	}else {

        defaultErrPageSet(request, INTERNAL_SERVER_ERROR);
    }

	
}


HttpResponse::HttpResponse(const HttpRequest & request, int clt_fd): http_version(request.http_version){

	// find the server config to see if it comfort those limits
	struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    if (getsockname(clt_fd, (struct sockaddr*)&server_addr, &server_addr_len) == -1) {
        
		std::cout << "Could not find the current server ip for the request!!!!" << std::endl;
        defaultErrPageSet(request, NOT_FOUND);
        return;
    }
    std::string server_ip = inet_ntoa(server_addr.sin_addr);
	int			server_port = ntohs(server_addr.sin_port);

	std::cout << "                          port:" << server_port << std::endl;
	std::cout << "                          ip  :" << server_ip << std::endl;

	CurrentPort = server_port;
	bool is_find = false;
	for (t_config_it it = g_config.begin(); it != g_config.end(); it ++){
		
		if ((*it)._ip == server_ip && std::find((*it)._port.begin(), (*it)._port.end(), server_port) != (*it)._port.end()){
			this->CurrentServerConfig = *it;
			//std::cout << "GET: 1index_page: " << it->_index << std::endl;
			is_find = true;
			break ;
		}

		if ((*it)._ip == "0.0.0.0" && std::find((*it)._port.begin(), (*it)._port.end(), server_port) != (*it)._port.end()){
			this->CurrentServerConfig = *it;
			//std::cout << "GET: 2index_page: " << it->_index << std::endl;
			is_find = true;
			break ;
		}
	}

	if (is_find && request.headers.find("Host") != request.headers.end()){

		std::string		name = request.headers.find("Host")->second;
		if (name.find(':') == std::string::npos){
			
			for (t_config_it it = g_config.begin(); it != g_config.end(); it ++){

				if ((*it)._name == name && (*it)._ip == server_ip && std::find((*it)._port.begin(), (*it)._port.end(), server_port) != (*it)._port.end()){
					this->CurrentServerConfig = *it;
					//std::cout << "GET: 3index_page: " << it->_index << std::endl;
					break ;
				}
			}
			
		}
	}

	if (!is_find){

		std::cout << "Could not find the current server ip for the request!!!!" << std::endl;
        defaultErrPageSet(request, NOT_FOUND);
        return;
	}



	if (request.body.size() > CurrentServerConfig._client_size){

		std::cout << "Client body size is too large!!!" << std::endl;
        defaultErrPageSet(request, TOO_LARGE);
        return;
	}


	std::string     test_path = ParserURL::get_abs_url(request.url, CurrentServerConfig, "GET");
	std::cout << "\n\nPARSERURL:   \"" << test_path << "\"\n" << std::endl;

	// To see if exist the url in the configfile if no it will find the url (root + filename)
	if (test_path == ""){

		std::cout << "No pass the url test, do not exist!!!" << std::endl;
        defaultErrPageSet(request, NOT_FOUND);
        return;
    }

	if(request.method == "GET")
		handleGet(request);
	else if (request.method == "POST")
		handlePost(request);
	else if (request.method == "DELETE")
		//std::cout << "Un finish DELETE method!!!" << std::endl;
		handleDelete(request);
	else
		defaultErrPageSet(request, NOT_FOUND);
}

void								HttpResponse::defaultErrPageSet(const HttpRequest & request, t_status_respond page_code){

	http_version = request.http_version;
	status_code = page_code;
	headers[CONTENT_DATE] = getHttpDate();
	headers[CONTENT_SERVER] = "MyServer/1.0";
	headers[CONTENT_TYPE] = "text/html; charset=UTF-8";
	switch (page_code)
	{
		case OK:
			status_text = RES_STATUS_OK;
			break;
		case NOT_FOUND:
			status_text = RES_STATUS_NOT_FOUND;
			break;
		case INTERNAL_SERVER_ERROR:
			status_text = RES_STATUS_CREATED;
			break;
		default:
			break;
	}

	if (is_exist_err_page(page_code)){


		std::cout << "DEFAULT ERR PAGE: page exist..." << std::endl;
		std::string path;

		if (CurrentServerConfig._root != "/")
			path = CurrentServerConfig._root + "/" + CurrentServerConfig._err_page[page_code];
		else
			path = "/" + CurrentServerConfig._err_page[page_code];
		std::ifstream	file(path.c_str());
		if (file.is_open()){
			
			std::stringstream	buffer;
			buffer << file.rdbuf();
			body = buffer.str();
		}else{

			std::stringstream ss;
			ss << page_code;
			body = "<html><body><h1> Default page. Status code:" + ss.str() + "</h1></body></html>";
		}
		std::ostringstream ss_size;
		ss_size << body.size();
		headers[CONTENT_LENGTH] = ss_size.str();
	}else{

		std::stringstream ss;
		ss << page_code;
		body = "<html><body><h1> Default page. Status code:" + ss.str() + "</h1></body></html>";
        std::ostringstream ss_size;
		ss_size << body.size();
		headers[CONTENT_LENGTH] = ss_size.str();
	}
}

std::string							HttpResponse::CreateResponse() {

    std::ostringstream response;

    response << http_version << " " << status_code << " " << status_text << "\r\n";

    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
        response << it->first << ": " << it->second << "\r\n";

    response << "\r\n";
    response << body;
    return response.str();
}

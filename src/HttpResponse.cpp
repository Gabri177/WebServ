#include "../include/HttpResponse.hpp"

// static std::string										str_no_space(const std::string & ori){

// 	size_t	start = ori.find_first_not_of(' ');
// 	if (start == std::string::npos)
// 		return	ori;
// 	size_t	end	  = ori.find_last_not_of(' ');
// 	return ori.substr(start, end - start + 1);

// }

std::string getContentType(const std::string& file_extension){

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

std::string determineContentType(const std::string& filepath){
	
    size_t dot_pos = filepath.find_last_of(".");
    if (dot_pos == std::string::npos)
        return "application/octet-stream";

    std::string file_extension = filepath.substr(dot_pos);
    return getContentType(file_extension);
}

static bool					is_exist_url(const std::string & url){

	
	std::string url_path;
	if (url.find('.') != std::string::npos)
		url_path = url.substr(0, url.find_last_of('/'));
	else
		url_path = url;
	if (url_path == "")
		url_path = "/";
	std::cout << "url pathhhh: " << url_path << std::endl;
	bool	is_exist = false;
	for (t_config_it it = g_config.begin(); it != g_config.end(); it ++){

		if (!(*it)._location.empty() && (*it)._location.find(url_path) != (*it)._location.end()){
			is_exist = true;
			std::cout << "the path of url : " << url_path << "  exist!!!" << std::endl;
			break;
		}
	}
	return is_exist;
}

static bool					is_exist_404(){

	bool	is_exist = false;
	for (t_config_it it = g_config.begin(); it != g_config.end(); it ++){

		if (!(*it)._err_page.empty() && (*it)._err_page.find(404) != (*it)._err_page.end()){
			is_exist = true;
			break;
		}
	}
	return is_exist;
}

static std::string			getHttpDate(){

	char		buffer[100];
	time_t		now = time(0);
	struct tm	tm = *gmtime(&now);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &tm);
	return std::string(buffer);
}

std::string					HttpResponse::urlToFilePath(const std::string & url, const std::string & meth){

	//std::cout << "find url" << url << std::endl;
	std::string url_path;
	if (url.find('.') != std::string::npos)
		url_path = url.substr(0, url.find_last_of('/'));
	else
		url_path = url;
	if (url_path == "")
		url_path = "/";
	//for (t_config_it it = g_config.begin(); it != g_config.end(); it ++){
		if((CurrentServerConfig)._location.find(url_path) != (CurrentServerConfig)._location.end()){
			if (!(CurrentServerConfig)._location[url_path]._root.empty() && std::find((CurrentServerConfig)._location[url_path]._methods.begin(), (CurrentServerConfig)._location[url_path]._methods.end(), meth) != (CurrentServerConfig)._location[url_path]._methods.end()){
				if ((CurrentServerConfig)._location[url_path]._root != "/")
					return (CurrentServerConfig)._location[url_path]._root + url;
				else
					return url;
			}
		}
	//}

	//for (t_config_it it = g_config.begin(); it != g_config.end(); it ++){
		if (!(CurrentServerConfig)._root.empty() && std::find((CurrentServerConfig)._methods.begin(), (CurrentServerConfig)._methods.end(), meth) != (CurrentServerConfig)._methods.end()){
			if ((CurrentServerConfig)._root != "/"){
				std::ifstream	file(url);
				if (file.is_open())
					return url;
			} else {
				std::ifstream	file(((CurrentServerConfig)._root + url));
				if (file.is_open())
					return (CurrentServerConfig)._root + url;
			}
		}
	//}
	//std::cout << "no exist" << std::endl;
	return url;
}

std::string					HttpResponse::loadFileContent(const std::string & url, const std::string & meth){

	std::ifstream	file(urlToFilePath(url, meth));
	std::cout << "absolute path : " << urlToFilePath(url, meth) << std::endl;
	if (file.is_open()){

		std::cout << "file opend..." << std::endl;
		std::stringstream	buffer;
		buffer << file.rdbuf();
		std::cout << "file content: " << buffer.str() << std::endl;
		return buffer.str();
	}else{
		return "";
	}
}

void					HttpResponse::handleGet(const HttpRequest & request){

	std::cout << "start do the GET request..." << std::endl;
	body = loadFileContent(request.url, "GET");
	//std::cout << "GET:: BODY :" << body << std::endl;
	if (body == "") {
		Default404Set(request);
		return;
	}
	status_code = OK;
	status_text = RES_STATUS_OK;
	headers[CONTENT_TYPE] = getContentType(request.url.substr(request.url.find_last_of(".")));
	std::stringstream	ss;
	ss << body.size();
	headers[CONTENT_LENGTH] = ss.str();
	headers[CONTENT_SERVER] = "MyServer/1.0";
	headers[CONTENT_DATE] = getHttpDate();
	headers[CONTENT_CONNECTION] = "keep-alive";
}

// static std::string 		parseContentDisposition(const std::string &content_disposition) {

//     std::stringstream ss(content_disposition);
//     std::string token;
//     std::string filename;

//     while (std::getline(ss, token, ';')) {
//         size_t pos = token.find("filename=");
//         if (pos != std::string::npos) {
//             filename = token.substr(pos + 9);
//             filename = str_no_space(filename);
//             if (filename.front() == '"' && filename.back() == '"')
//                 filename = filename.substr(1, filename.size() - 2);
//             break;
//         }
//     }

//     return filename;
// }


void					HttpResponse::handlePost(const HttpRequest & request){

	std::cout << "start do the POST request..." << std::endl;
	std::string  path = urlToFilePath(request.url, "POST");
	std::cout << "POST: path -> " << path << std::endl;
	if (path == "" || request.headers.find("Content-Type") == request.headers.end()) {

		std::cout << "POST: could not find Content-Type..." << std::endl;
		Default404Set(request);
		return;
	}
	std::map<std::string, std::string>::const_iterator it = request.headers.find("Content-Type");
   
	std::string content_type = it->second;
	if (content_type.find("multipart/form-data") == std::string::npos) {

        std::cout << "POST: Content-Type is not multipart/form-data..." << std::endl;
        Default404Set(request);
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
                    file.write(fileContent.c_str(), fileContent.size());
                    file.close();
                    body = "<html><body><h1>Upload Sucess!</h1></body></html>";
                    status_code = OK;
                    status_text = RES_STATUS_CREATED;
                    headers[CONTENT_SERVER] = "MyServer/1.0";
                    headers[CONTENT_DATE] = getHttpDate();
                    headers[CONTENT_CONNECTION] = "keep-alive";
					std::cout << "Upload sucess !!!" << std::endl;
                    return;
                } else {
                    status_code = INTERNAL_SERVER_ERROR;
                    status_text = "Internal Server Error";
                    body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
                    headers[CONTENT_TYPE] = "text/html; charset=UTF-8";
                    std::stringstream ss;
                    ss << body.size();
                    headers[CONTENT_LENGTH] = ss.str();
                    headers[CONTENT_SERVER] = "MyServer/1.0";
                    headers[CONTENT_DATE] = getHttpDate();
                    headers[CONTENT_CONNECTION] = "keep-alive";
                    return;
                }
            }
        }

        boundaryPos = body.find(boundary, nextBoundaryPos);
	}
}

// void					HttpResponse::handleDelete(const HttpRequest & request){

// 	// if (loadFileContent(request.url, "DELETE") == "") {
// 	// 	Default404Set(request);
// 	// 	return;
// 	// }
// }


HttpResponse::HttpResponse(const HttpRequest & request, int clt_fd): http_version(request.http_version){

	// find the server config to see if it comfort those limits
	struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    if (getsockname(clt_fd, (struct sockaddr*)&server_addr, &server_addr_len) == -1) {
        
		std::cout << "Could not find the current server ip for the request!!!!" << std::endl;
        Default404Set(request);
        return;
    }
    std::string server_ip = inet_ntoa(server_addr.sin_addr);
	int			server_port = ntohs(server_addr.sin_port);

	std::cout << "                          port:" << server_port << std::endl;
	std::cout << "                          ip  :" << server_ip << std::endl;
	bool is_find = false;
	for (t_config_it it = g_config.begin(); it != g_config.end(); it ++){
		
		if ((*it)._ip == server_ip && std::find((*it)._port.begin(), (*it)._port.end(), server_port) != (*it)._port.end()){
			this->CurrentServerConfig = *it;
			is_find = true;
			break ;
		}

		if ((*it)._ip == "0.0.0.0" && std::find((*it)._port.begin(), (*it)._port.end(), server_port) != (*it)._port.end()){
			this->CurrentServerConfig = *it;
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
					break ;
				}
			}
			
		}
	}

	if (!is_find){

		std::cout << "Could not find the current server ip for the request!!!!" << std::endl;
        Default404Set(request);
        return;
	}



	if (sizeof(request.body) > CurrentServerConfig._client_size){

		std::cout << "Client body size is too large!!!" << std::endl;
        Default404Set(request);
        return;
	}

	// To see if exist the url in the configfile if no it will find the url (root + filename)
	if (!is_exist_url(request.url)){

		std::cout << "No pass the url test, do not exist!!!" << std::endl;
        Default404Set(request);
        return;
    }

	if(request.method == "GET")
		handleGet(request);
	else if (request.method == "POST")
		handlePost(request);
	else if (request.method == "DELETE")
		std::cout << "Un finish DELETE method!!!" << std::endl;
		//handleDelete(request);
	else
		Default404Set(request);
}

void								HttpResponse::Default404Set(const HttpRequest & request){

	http_version = request.http_version;
	status_code = NOT_FOUND;
	status_text = RES_STATUS_NOT_FOUND;
	headers[CONTENT_DATE] = getHttpDate();
	headers[CONTENT_SERVER] = "MyServer/1.0";
	headers[CONTENT_TYPE] = "text/html; charset=UTF-8";
	
	bool is_err_page = is_exist_404();
	//std::cout << "here1" << std::endl;
	if (is_err_page){

		//std::cout << "here2" << std::endl;
        for (t_config_it it = g_config.begin(); it != g_config.end(); it++) {

            if ((*it)._err_page.find(404) != (*it)._err_page.end()){

				std::string path;
				if ((*it)._root != "/")
					path = (*it)._root + (*it)._err_page[404];
				else
					path = (*it)._err_page[404];
				//std::cout << "ERR PAGE PATH:\"" << path << "\"" << std::endl;
				std::ifstream	file(path);
				if (file.is_open()){
					
					//std::cout << "OPENED DDDDDDDDDDDDDDDDDDDDDDDDDD" << std::endl;
					std::stringstream	buffer;
					buffer << file.rdbuf();
					body = buffer.str();
				}else
                	body = "Default Error!";
				std::ostringstream ss;
                ss << body.size();
                headers[CONTENT_LENGTH] = ss.str();
                return ;
			}
		}
    }else{

		//std::cout << "here3" << std::endl;
        body = "<html><body><h1>404 Not Found</h1></body></html>";
        headers["Content-Length"] = std::to_string(body.size());
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

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP
# include <string>
# include <iostream>
# include <algorithm>
# include <map>
# include "Server.hpp"
# include "Config.hpp"
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include "ParserURL.hpp"
# define CONTENT_TYPE "Content-Type"
# define CONTENT_LENGTH "Content-Length"
# define CONTENT_SERVER "Server"
# define CONTENT_DATE "Date"
# define CONTENT_CONNECTION "Connection"
# define RES_STATUS_OK "OK"
# define RES_STATUS_NOT_FOUND "Not Found"
# define RES_STATUS_CREATED "Created"

class HttpRequest;
typedef enum  status_respond{

	OK = 200,
	NOT_FOUND = 404,
	INTERNAL_SERVER_ERROR = 500
}status_respond;

struct HttpResponse{

	std::string							http_version;
	status_respond						status_code;
	std::string							status_text;
	std::map<std::string, std::string>	headers;
	std::string							body;

										HttpResponse(const HttpRequest & request, int clt_fd);
	std::string							CreateResponse();

	private:

		ServerConfig					CurrentServerConfig;
		void							Default404Set(const HttpRequest & request);
		void							handleGet(const HttpRequest & request);
		void							handlePost(const HttpRequest & request);
		//std::string						urlToFilePath(const std::string & url, const std::string & meth);
		std::string						loadFileContent(const std::string & url, const std::string & meth);
		void							handleDelete(const HttpRequest & request);
};


#endif
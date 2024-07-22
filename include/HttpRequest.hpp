/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabpalma <pabpalma>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:23:50 by pabpalma          #+#    #+#             */
/*   Updated: 2024/07/22 08:49:50 by pabpalma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>
# include <map>
# include "Server.hpp"

class HttpRequest {
	public:
		std::string method;
		std::string url;
		std::string http_version;
		std::map<std::string, std::string> headers;
		std::string body;
		size_t content_length;
};

class HttpRequestParser {
	private:
		enum State {
			REQUEST_LINE_METHOD,
			REQUEST_LINE_URL,
			REQUEST_LINE_VERSION,
			HEADER_NAME,
			HEADER_VALUE,
			HEADER_LF,
			BODY,
			ERROR
		};

		State state;
		std::string current_token;
		std::string current_header_name;
		bool error_flag;

		void parseRequestChar(char byte, HttpRequest &httpRequest);
		void parseHeaderChar(char byte, HttpRequest &httpRequest);
		void parseBodyChar(char byte, HttpRequest &httpRequest);

	public:
		HttpRequestParser();
		void feed(const char* data, size_t len, HttpRequest& httpRequest);
		bool hasError() const;
};

#endif

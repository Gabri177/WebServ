/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabpalma <pabpalma>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:37:02 by pabpalma          #+#    #+#             */
/*   Updated: 2024/07/22 18:49:19 by pabpalma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include <sstream>
#include <stdexcept>
#include <iostream>

HttpRequestParser::HttpRequestParser() : state(REQUEST_LINE_METHOD), error_flag(false){}

void HttpRequestParser::feed(const char *data, size_t len, HttpRequest &httpRequest) {
	std::cout << "request len: " << len << std::endl;
	for (size_t i = 0; i < len; ++i) {
		char byte = data[i];
		switch (state) {
			case REQUEST_LINE_METHOD:
			case REQUEST_LINE_URL:
			case REQUEST_LINE_VERSION:
				parseRequestChar(byte, httpRequest);
				break;
			case HEADER_NAME:
			case HEADER_VALUE:
			case HEADER_LF:
				parseHeaderChar(byte, httpRequest);
				break;
			case BODY:
				parseBodyChar(byte, httpRequest);
				break;
			case ERROR:
				error_flag = true;
				return;
		}
	}
}

bool HttpRequestParser::hasError() const {
	return error_flag;
}

void HttpRequestParser::parseRequestChar(char byte, HttpRequest &httpRequest) {
	switch (state) {
		case REQUEST_LINE_METHOD:
			if (byte == ' ') {
				httpRequest.method = current_token;
				current_token.clear();
				state = REQUEST_LINE_URL;
			} else {
				current_token += byte;
			}
			break;
		case REQUEST_LINE_URL:
			if (byte == ' ') {
				httpRequest.url = current_token;
				current_token.clear();
				state = REQUEST_LINE_VERSION;
			}
			else {
				current_token += byte;
			}
			break;
		case REQUEST_LINE_VERSION:
			if (byte == '\r') {
				httpRequest.http_version = current_token;
				current_token.clear();
				state = HEADER_LF;
			} else {
				current_token += byte;
			}
			break;
		default:
			std::cerr << "parseRequestChar error: unexpected state" << std::endl; // Log para debugging

			error_flag = true;
			state = ERROR;
			break;
	}
}

void HttpRequestParser::parseHeaderChar(char byte, HttpRequest &httpRequest) {
	switch (state) {
		case HEADER_LF:
			if (byte == '\n') {
				state = HEADER_NAME;
			} else {
				std::cerr << "parseHeaderChar error: expected LF" << std::endl; // Log para debugging
				error_flag = true;
				state = ERROR;
			}
			break;
		case HEADER_NAME:
			if (byte == ':') {
				current_header_name = current_token;
				current_token.clear();
				state = HEADER_VALUE;
			} else if (byte == '\r') {
				if (current_token.empty()) {
					state = BODY; 
				} else {
					std::cerr << "parseHeaderChar error: unexpected character in HEADER_NAME" << std::endl; // Log para debugging
					error_flag = true;
					state = ERROR;
				}
			} else if (byte == '\n') {
				if (current_token.empty()) {
					state = BODY;
				} else {
					std::cerr << "parseHeaderChar error: unexpected LF in HEADER_NAME" << std::endl; // Log para debugging
					error_flag = true;
					state = ERROR;
				}
			} else {
				current_token += byte;
			}
			break;
		case HEADER_VALUE:
			if (byte == '\r') {
				httpRequest.headers[current_header_name] = current_token;
				if (current_header_name == "Content-Length") {
					char *end;
					httpRequest.content_length = std::strtol(current_token.c_str(), &end, 10);
					if (*end != '\0') {
						std::cerr << "parseHeaderChar error: invalid Content-Length" << std::endl; // Log para debugging
                        error_flag = true;
                        state = ERROR;
                        return;
					}
				}
				current_token.clear();
				state = HEADER_LF;
			} else {
				current_token += byte;
			}
			break;
		default:
			 std::cerr << "parseHeaderChar error: unexpected state" << std::endl; // Log para debugging
			error_flag = true;
			state = ERROR;
			break;
	}
}

void HttpRequestParser::parseBodyChar(char byte, HttpRequest &httpRequest) {
	httpRequest.body += byte;
}

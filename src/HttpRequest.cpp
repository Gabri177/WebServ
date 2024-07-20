/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabpalma <pabpalma>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:37:02 by pabpalma          #+#    #+#             */
/*   Updated: 2024/07/20 12:37:10 by pabpalma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include <sstream>
#include <stdexcept>

HttpRequestParser::HttpRequestParser() : state(REQUEST_LINE) {}

HttpRequest HttpRequestParser::parse(const std::string &request) {
    HttpRequest httpRequest;
    std::istringstream request_stream(request);
    std::string line;

    while (std::getline(request_stream, line)) {
        if (line[line.size() - 1] == '\r') {
            line = line.substr(0, line.size() - 1);
        }

        switch (state) {
            case REQUEST_LINE:
                parseRequestLine(line, httpRequest);
                state = HEADERS;
                break;
            case HEADERS:
                if (line.empty()) {
                    state = BODY;
                } else {
                    parseHeaderLine(line, httpRequest);
                }
                break;
            case BODY:
                httpRequest.body += line + "\n";
                break;
        }
    }
    return httpRequest;
}

void HttpRequestParser::parseRequestLine(const std::string &line, HttpRequest &httpRequest) {
    std::istringstream request_line_stream(line);
    if (!(request_line_stream >> httpRequest.method >> httpRequest.url >> httpRequest.http_version)) {
        throw std::runtime_error("Invalid HTTP request line");
    }
}

void HttpRequestParser::parseHeaderLine(const std::string &line, HttpRequest &httpRequest) {
    std::string::size_type colon_pos = line.find(':');
    if (colon_pos != std::string::npos) {
        std::string header_name = line.substr(0, colon_pos);
        std::string header_value = line.substr(colon_pos + 1);
        header_value = header_value.substr(header_value.find_first_not_of(" \t"));
        httpRequest.headers[header_name] = header_value;
    }
}


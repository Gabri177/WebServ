/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabpalma <pabpalma>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:38:18 by pabpalma          #+#    #+#             */
/*   Updated: 2024/07/22 18:45:14 by pabpalma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/RequestHandler.hpp"
#include "../include/HttpRequest.hpp"
#include "../include/HttpResponse.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <vector>

void handle_client_request(int client_fd) {
    size_t buffer_size = 1024;
    char buffer[buffer_size];
    HttpRequestParser parser;
    HttpRequest request;
    ssize_t bytes_read;
    bool headers_complete = false;

    while ((bytes_read = recv(client_fd, buffer, buffer_size, 0)) > 0) {
        parser.feed(buffer, bytes_read, request);
        if (parser.hasError()) {
            std::cerr << "Error parsing the request." << std::endl;
            close(client_fd);
            return;
        }
        if (request.headers.find("Content-Length") != request.headers.end()) {
            size_t content_length = request.content_length;
            if (request.body.size() >= content_length) {
                headers_complete = true;
                break;
            }
        }
    }

    if (headers_complete || !request.headers.empty()) {
        // Debug output for the parsed request
        std::cout << "Method: " << request.method << std::endl;
        std::cout << "URL: " << request.url << std::endl;
        std::cout << "HTTP Version: " << request.http_version << std::endl;
        for (std::map<std::string, std::string>::iterator it = request.headers.begin(); it != request.headers.end(); ++it) {
            std::cout << "Header: " << it->first << " = " << it->second << std::endl;
        }
        std::cout << "Body: " << request.body << std::endl;

        // Handle the request and generate a response
        HttpResponse res_msg_obj(request, client_fd);
        std::string msg = res_msg_obj.CreateResponse();

        send(client_fd, msg.c_str(), msg.size(), 0);

        std::cout << "Sent response: " << msg << std::endl;
    }

    close(client_fd);
}


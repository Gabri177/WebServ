/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabpalma <pabpalma>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 12:38:18 by pabpalma          #+#    #+#             */
/*   Updated: 2024/07/20 12:39:48 by pabpalma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/RequestHandler.hpp"

#include "RequestHandler.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <vector>

void handle_client_request(int client_fd) {

    size_t buffer_size = 1024;
    char buffer[buffer_size];
    std::vector<char> request_buffer;
    size_t bytes_read;

    while ((bytes_read = recv(client_fd, buffer, buffer_size, 0)) > 0) {
        request_buffer.insert(request_buffer.end(), buffer, buffer + bytes_read);
        if (std::string(buffer, buffer + bytes_read).find("\r\n\r\n") != std::string::npos) {
            break;
        }
    }

    if (!request_buffer.empty()) {
        std::string request_str(request_buffer.begin(), request_buffer.end());
        HttpRequestParser parser;
        HttpRequest request = parser.parse(request_str);

        // Read the request body if there is one
        if (request.content_length > 0) {
            size_t header_length = request_str.find("\r\n\r\n") + 4;

            while (request_buffer.size() - header_length < request.content_length) {
                bytes_read = recv(client_fd, buffer, buffer_size, 0);
                if (bytes_read <= 0) {
                    break;
                }
                request_buffer.insert(request_buffer.end(), buffer, buffer + bytes_read);
            }

            if (request_buffer.size() > header_length) {
                request.body = std::string(request_buffer.begin() + header_length, request_buffer.end());
            }
            std::cout << "Request body:\n|||||||||||||||||||||||||||||\"" << request.body << "\"|||||||||||||||||||||||||||||"<< std::endl;
        }

        HttpResponse res_msg_obj(request, client_fd);
        std::string msg = res_msg_obj.CreateResponse();
        send(client_fd, msg.c_str(), msg.size(), 0);
    }
    close(client_fd);
}


//void handle_client_request(int client_fd) {
//    char buffer[1024];
//    int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
//    if (bytes_read > 0) {
//        buffer[bytes_read] = '\0';
//        std::cout << "Receive data: " << buffer << std::endl;
//        
//        // Simple HTTP response
//        const char* response_body = "Hello World";
//        const char* response_template = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s";
//        char response[1024];
//        int response_length = snprintf(response, sizeof(response), response_template, strlen(response_body), response_body);
//
//        send(client_fd, response, response_length, 0);
//
//        std::cout << "Sent response: " << response << std::endl;
//    }
//    close(client_fd);
//}



// Handle client requests
//void	handle_client_request(int client_fd){
//
//	char	buffer[1024];
//	ssize_t count;
//
//	count = 0;
//	memset(&buffer, 0, sizeof(buffer));
//	while ((count = read(client_fd, buffer, sizeof(buffer))) > 0) {
//
//		// Echo back the data
//		write(client_fd, buffer, count);
//		printf ("Receive data: %s\n", buffer);
//	        const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello World";
//        send(client_fd, response, strlen(response), 0);
//
//        std::cout << "Sent response: " << response << std::endl;
//
//		//memset(&buffer, 0, sizeof(buffer));
//	}
//	if (count == -1 && errno != EAGAIN) {
//
//		close(client_fd);
//		throw std::runtime_error("Server error: read failed");
//	}
//	if (count == 0)
//		close(client_fd);
//}
//

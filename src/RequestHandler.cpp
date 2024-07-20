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
    char buffer[1024];
    std::vector<char> request_buffer;
    size_t bytes_read;
    while ((bytes_read = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
        request_buffer.insert(request_buffer.end(), buffer, buffer + bytes_read);
        if (bytes_read < sizeof(buffer)) {
            break;
        }
    }

    if (!request_buffer.empty()) {
        std::string request_str(request_buffer.begin(), request_buffer.end());
        HttpRequestParser parser;
        HttpRequest request = parser.parse(request_str);
        
        // Debug output for the parsed request
        std::cout << "Method: " << request.method << std::endl;
        std::cout << "URL: " << request.url << std::endl;
        std::cout << "HTTP Version: " << request.http_version << std::endl;
        for (std::map<std::string, std::string>::iterator it = request.headers.begin(); it != request.headers.end(); ++it) {
            std::cout << "Header: " << it->first << " = " << it->second << std::endl;
        }
        std::cout << "Body: " << request.body << std::endl;

        // Simple HTTP response
        const char* response_body = "Hello World";
        const char* response_template = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s";
        char response[1024];
        int response_length = snprintf(response, sizeof(response), response_template, strlen(response_body), response_body);

        send(client_fd, response, response_length, 0);

        std::cout << "Sent response: " << response << std::endl;
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

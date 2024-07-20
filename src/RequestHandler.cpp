#include "../include/RequestHandler.hpp"

void handle_client_request(int client_fd) {
    char buffer[1024];
    int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        std::cout << "Receive data: " << buffer << std::endl;
        
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

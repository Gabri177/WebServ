#include "../include/RequestHandler.hpp"




// Handle client requests
void								handle_client_request(int client_fd){

	char	buffer[1024];
	ssize_t count;

	count = 0;
	memset(&buffer, 0, sizeof(buffer));
	while ((count = read(client_fd, buffer, sizeof(buffer))) > 0) {

		// Echo back the data
		write(client_fd, buffer, count);
		//std::string test = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length:100\n\n<html><title>dsfsdfsf</title><body>" + std::string(buffer) +"<ln>testtesttest</ln></body></html>";
		//write(client_fd, test.c_str(), test.size());
		printf ("Receive data: %s\n", buffer);
		memset(&buffer, 0, sizeof(buffer));
	}
	if (count == -1 && errno != EAGAIN) {

		close(client_fd);
		throw std::runtime_error("Server error: read failed");
	}
	if (count == 0)
		close(client_fd);
}
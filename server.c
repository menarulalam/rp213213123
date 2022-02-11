#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 5984
#define BUFF_SIZE 4096

int main(int argc, const char *argv[])
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[BUFF_SIZE] = {0};
	char *hello = "Hello from server";

	/* [S1]
	 * Explaint the following here.
	 * If socket set up fails, 
	 * exit otherwise set server_fd to file descriptor of the socket for client streams
	 */
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	/* [S2]
	 * Explaint the following here.
	 * Sets socket options to reuse address and port on subsequent connections
	 * Errors on failure
	 */
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		       &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	/* [S3]
	 * Explaint the following here.
	 * Initialization of the socket address
	 * sin_family is set to the family of addresses the socket can communicate with
	 * 2nd part means it is not bound to a specific ip address
	 * 3rd part is just setting the port
	 */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	/* [S4]
	 * Explaint the following here.
	 * Binds the file descriptor to the particular address we set
	 * Errors on failure
	 */
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	/* [S5]
	 * Starts listening on the socket. The 3 indicates that the 
	 * queue of incoming connections has a limit of 3 full connections
	 * Errors on failure.
	 */
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	/* [S6]
	 * Explaint the following here.
	 * Accepts a connection on the socket (the first connection on the queue)
	 * Errors on failure
	 */
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
				 (socklen_t*)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	/* [S7]
	 * It waits for a key press to continue.
	 */
	printf("Press any key to continue...\n");
	getchar();

	/* [S8]
	 * Explaint the following here.
	 * It reads up to 1024 bytes from the client connection into the buffer
	 * Errors on failure
	 * Then prints the message
	 */
	if (read( new_socket , buffer, 1024) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("Message from a client: %s\n",buffer );

	/* [S9]
	 * Sends a hello message to the client through the client connection.
	 */
	send(new_socket , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
	return 0;
}

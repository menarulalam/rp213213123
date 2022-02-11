#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 5984
#define BUFF_SIZE 4096

int main(int argc, const char *argv[])
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[BUFF_SIZE] = {0};

	/* [C1]
	 * Creates a socket for connecting with the server
	 * AF_INET specifies family of addresses
	 * SOCK_STREAM implies a 2 way connection
	 * 0 implies the default protocol
	 * Errors on failure
	 */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	/* [C2]
	 * Explaint the following here.
	 * Initialization of the socket address
	 * sin_family is set to the family of addresses the socket can communicate with
	 * 2nd part means it is not bound to a specific ip address
	 * 3rd part is just setting the port
	 */
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	/* [C3]
	 * Concerts the ip address into binary and sets it to the sin_addr of the server address
	 * Errors on failure
	 */
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	/* [C4]
	 * Connects to the given server address
	 * Errors on failure
	 */
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}


	/* [C5]
	 * Waits for keypress to conitnue
	 */
	printf("Press any key to continue...\n");
	getchar();

	/* [C6]
	 * Sends the message through the socket
	 */
	send(sock , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");

	/* [C7]
	 * Reads a message of up to length 1024 from the socket into the buffer
	 * Errors on failure
	 */
	if (read( sock , buffer, 1024) < 0) {
		printf("\nRead Failed \n");
		return -1;
    }
	printf("Message from a server: %s\n",buffer );
	return 0;
}

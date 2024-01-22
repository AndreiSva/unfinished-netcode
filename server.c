#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

typedef struct BoardInstance {
	int userCount;
	char* motd;
} BoardInstance;

inline void write_message(int clientfd, char* message) {
	write(clientfd, message, strlen(message));
}

void greet(int clientfd) {
	write_message(clientfd, "Welcome to the bboard");
}

int main(int argc, char** argv) {
	int port = 7979;
	if (argc > 1) {
		port = atoi(argv[1]);
	}

	puts("Starting Server...");
	
	// create the server socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	
	// bind the socket to the address
	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		fprintf(stderr, "Unable to bind socket: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	
	// set the socket to non-blocking mode
	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	// listen for connections, 3 is the maximum number of connections in the queue
	listen(sockfd, 3);

	puts("Listening for connections...");

	fd_set readfds; // set of file descriptors to read from
	int maxfd = sockfd;
	
	// this is a list of the file descriptors of all the clients
	int client_sockets[1024];
	
	// initialize everything to 0
	memset(client_sockets, 0, sizeof(client_sockets));

	while (true) {
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);
		
		maxfd = sockfd;

		// we find the maximum file descriptor
		for (int i = 0; i < 1024; i++) {
			int fd = client_sockets[i];
			if (fd > 0) {
				FD_SET(fd, &readfds);
			}
			if (fd > maxfd) {
				maxfd = fd;
			}
		}
		
		// select needs the maximum file descriptor + 1 for some reason
		int ready = select(maxfd + 1, &readfds, NULL, NULL, NULL);

		// ready now contains the amount of sockets that are ready to be read from
		if (ready > 0) {
			// we have a connection i guess	
			
		}

		int clientfd = accept(sockfd, NULL, NULL);
		fprintf(stderr, "Client connected!\n");
	
	}

	close(sockfd);
	return EXIT_SUCCESS;
}

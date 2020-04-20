/**
 * A simple server in IPv4 internet domain using TCP to 
 * receive messages and reply to clients with the messages converted. 
 * The port number is passed as an argument
 * @author Tong (Debby) Ding
 * @version 1.0
 * @see CPSC 5510 Spring 2020, Seattle University
 */
// #define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

using namespace std;

/**
 * Print an error message and exit the program
 * @param msg customized error message
 */ 
void error(const char *msg) {
    perror(msg);
    exit(1);
}

/**
 * Receive messages from clients and convert the messages to reply
 * @param argc number of arguments entered by user
 * @param argv[] the content of arguments
 */ 
int main(int argc, char *argv[]) {

	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	if (argc != 2) {
		cerr << "Usage: ./warmup_svr PORT" << endl;
		return EXIT_FAILURE;
	}

	// create a socket
	// socket(int domain, int type, int protocol)
	sockfd =  socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	error("ERROR opening socket");

	// clear address structure
	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = atoi(argv[1]);

	// setup the addr for server to bind the socket
	serv_addr.sin_family = AF_INET;  
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
	serv_addr.sin_port = htons(portno);

	// bind the socket to the addr of server
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
			error("ERROR on binding");

	//// The listen() function places all incoming connection into a backlog queue until accept() call accepts the connection.
	// let socket listen to incoming connection requests
	// FIX-ME: error checking on the listen() call
	listen(sockfd, 5); // set max size for backlog queue to 5.

	clilen = sizeof(cli_addr); // size of client addr info structure

	// accepts a connection request and open a new socket to free the original socket
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) 
		error("ERROR on accept");

	printf("server: got connection from %s port %d\n",
		inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

	while (true) {
		// This send() function sends the 13 bytes of the string to the new socket
		send(newsockfd, "Hello, world!\n", 13, 0);

		bzero(buffer,256);

		n = read(newsockfd,buffer,255);
		if (n < 0) {
			error("ERROR reading from socket");
		} else {
			printf("Here is the message: %s\n",buffer);
		}
	}
	
	close(newsockfd);
	close(sockfd); 
	
    return EXIT_SUCCESS;
}
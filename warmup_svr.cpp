/**
 * A simple server in IPv4 internet domain using TCP to receive 
 * messages from clients and reply with the messages converted. 
 * The port number is passed as an argument.
 * @author Tong (Debby) Ding
 * @version 1.0
 * @see CPSC 5510 Spring 2020, Seattle University
 */

// #include <stdio.h>
// #include <stdlib.h>
// #include <strings.h>
// #include <sys/types.h> 
// #include <sys/socket.h>
// #include <netinet/in.h>

#include <iostream>
#include <unistd.h> // close()
#include <arpa/inet.h> // inet_ntoa()
#include <cstring> // strlen(), bzero(), ...
#include <algorithm> // reverse()

#define BUF_SIZE 131072
#define EXIT_WORD "quit"
#define PORT_MAX 12000
#define PORT_MIN 11000
#define LISTEN_BKLOG 5

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
 * Print a note message and exit the program
 * @param msg customized note message
 */ 
void note(const char *msg) {
    printf("Note: %s\n", msg);
    exit(1);
}

/**
 * Convert user input port number from string to int,
 * or print the error and exit if validation fails
 * @param str the string of port number to convert
 * @return the int of port number
 */
int convertPort(const string& str) {
	char *end;
    long val = strtol(str.c_str(), &end, 10);

	// no digits or incomplete conversion
    if (end == str || *end != '\0')
		note("port number must all be digits");
		
	// number out of range
	if (val < PORT_MIN || val > PORT_MAX)
		note("port number out of range");

	return (int) val;
}

/**
 * Receive messages from clients and convert the messages to reply
 * @param argc number of arguments entered by user
 * @param argv[] the content of arguments
 */ 
int main(int argc, char *argv[]) {

	int sockfd, newsockfd, port, err;
	socklen_t clilen;
	char buffer[BUF_SIZE];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	// check user input port number
    if (argc != 2)
		note("Usage example \"./warmup_cli HOSTPORT\"");

	// get the port number with validation
	port = convertPort(argv[1]);

	// create a socket
	sockfd =  socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");

	// clear address structure
	bzero((char *) &serv_addr, sizeof(serv_addr));



	// setup the addr for server to bind the socket
	serv_addr.sin_family = AF_INET;  
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
	serv_addr.sin_port = htons(port);

	// bind the socket to the addr of server
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
			error("ERROR on binding");


	// start the loop to get connected from clients
	while (true) {

		// let socket listen to incoming connection requests
		err = listen(sockfd, LISTEN_BKLOG);
		if (err < 0) 
			error("ERROR on listen");

		clilen = sizeof(cli_addr); // size of client addr info structure

		// accepts a connection request and open a new socket to free the original socket
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");

		printf("Accepted connection from %s port %d\n\n",
			inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));	

		while (true) {
			bzero(buffer, BUF_SIZE);

			n = recv(newsockfd,buffer, BUF_SIZE, 0);
			if (n < 0)
				error("ERROR reading from socket");

			// prepare to disconnect if user send "quit"
			if (strncmp(buffer, EXIT_WORD, strlen(EXIT_WORD)) == 0) {
				printf("Connection terminated by %s port %d\n\n",
					inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

				send(newsockfd, buffer, strlen(buffer), 0);
				break;
			}

			printf("Received message: %s\n", buffer);

			// reverse received message
			reverse(buffer, buffer + strlen(buffer));

			// send the revised message
			send(newsockfd, buffer, strlen(buffer), 0);

		}
		close(newsockfd);
	}	
	close(sockfd); 
	
    return EXIT_SUCCESS;
}
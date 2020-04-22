/**
 * A simple client in IPv4 internet domain using TCP to 
 * send messages to a server and receive the responses. 
 * The hostname and port number are passed as arguments.
 * @author Tong (Debby) Ding
 * @version 1.0
 * @see CPSC 5510 Spring 2020, Seattle University
 */

#include <iostream>
#include <unistd.h> // close
#include <netdb.h>
#include <cstring>  // strlen, etc.

#define BUF_SIZE 131073 
#define EXIT_WORD "quit"  
#define PORT_MAX 12000
#define PORT_MIN 11000

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
 * Send messages to server through TCP socket and print the response
 * @param argc number of arguments entered by user
 * @param argv[] the content of arguments
 */ 
int main(int argc, char *argv[]) {

	int sock, err;                    // socket and error number
	struct addrinfo hints, *ser_addr; // server address
    char buffer[BUF_SIZE];            // message buffer size

	// check if user entered hostname and port number
    if (argc != 3)
		error("Error usage i.e. \"./warmup_cli HOSTNAME HOSTPORT\"");

	// create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) 
        error("ERROR create socket");

	// specify criteria for selecting socket address
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;        // IPv4
	hints.ai_socktype = SOCK_STREAM;  // TCP
	hints.ai_flags = 0;
	hints.ai_protocol = 0;       
	
	// get server address information
	err = getaddrinfo(argv[1], argv[2], &hints, &ser_addr);
	if (err < 0)
		error("ERROR host not found");

	// connect to the server
	err = connect(sock, ser_addr->ai_addr, ser_addr->ai_addrlen); 
	if (err < 0) 
		error("ERROR connect server");	

	// instruction to users
	printf("\nWelcome! Enter a message to send or \"quit\" to exit.");
	
	// start user-input loop
	while (true) {

		// get user input
		printf("\n\nSend: ");
		bzero(buffer, BUF_SIZE);
		if (fgets(buffer, BUF_SIZE, stdin) == NULL)
			error("ERROR input message");

		// send message to server
		err = send(sock, buffer, strlen(buffer), 0);
		if (err < 0) 
			error("ERROR send message");

		// receive message
		bzero(buffer,BUF_SIZE);
		err = recv(sock, buffer, BUF_SIZE, 0);
		if (err < 0) 
			error("ERROR receive message");

		// exit if input is "quit"
		if (strncmp(buffer, EXIT_WORD, strlen(EXIT_WORD)) == 0) 
			break;
		
		// print out received message
		printf(buffer);
	}

	// close socket when done
    close(sock);
 
    return EXIT_SUCCESS;
}
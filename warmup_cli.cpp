/**
 * A simple client in IPv4 internet domain using TCP to 
 * send messages to a server and receive the responses. 
 * The port number is passed as an argument
 * @author Tong (Debby) Ding
 * @version 1.0
 * @see CPSC 5510 Spring 2020, Seattle University
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> //
#include <arpa/inet.h>
#include <strings.h> // bzero
#include <cstring> // strlen

#define BUF_SIZE 256
#define EXIT_WORD "quit"

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
 * Convert user input port number from string to int
 * Call error() if an error exists 
 * @param str the string of port number to convert
 * @return the int of port number
 */
int convertPort(const string& str) {
	char *end;
	errno = 0;	
	// note: use strtol(), as atoi() doesn't detect errors in the input
    long val = strtol(str.c_str(), &end, 10);

	// no digits or incomplete conversion
    if (end == str || *end != '\0') 
		error("Error: port number must all be digits");
	// number out of range
	if (val < 11000 || val > 12000)
		error("Error: port number out of range");

	return (int) val;
}

/**
 * Send a message to server and print the response
 * @param argc number of arguments entered by user
 * @param argv[] the content of arguments
 */ 
int main(int argc, char *argv[]) {

	int sock, port, err, cliPort;
    struct sockaddr_in serv_addr;
	struct sockaddr_in clitAddr;
    struct hostent *server;

    char buffer[BUF_SIZE];

	// check user input hostname & port number
    if (argc != 4)
		error("Usage: ./warmup_cli CLIENTPORT HOSTNAME PORT");

	// get the port number with validation
	port = convertPort(argv[3]);

	cliPort = convertPort(argv[1]);

	// create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) 
        error("ERROR: fail to open socket");


	// setup addr for client to bind to a desired port
	clitAddr.sin_family = AF_INET;  
	clitAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // current IP address
	clitAddr.sin_port = htons(cliPort);
	if (bind(sock, (struct sockaddr *) &clitAddr, sizeof(clitAddr)) < 0) 
			error("ERROR on binding");


	// FIX-ME
    server = gethostbyname(argv[2]);
	if (server == NULL) 
		error("ERROR: host not found");
	
	// setup addr for server
    bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);


	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR connecting");	

	printf("\nWelcome! Enter a message to send or \"quit\" to exit.");
	
	// user-input loop
	while (true) {

		printf("\n\nSend: ");

		bzero(buffer,BUF_SIZE);
		fgets(buffer,BUF_SIZE,stdin);

		// send message and check for error
		err = send(sock, buffer, strlen(buffer), 0);
		if (err < 0) 
			error("ERROR writing to socket");

		// clear buffer
		bzero(buffer,BUF_SIZE);

		// receive message
		err = recv(sock, buffer, BUF_SIZE, 0);
		if (err < 0) 
			error("ERROR reading from socket");

		// exit if user enter "quit"
		if (strncmp(buffer, EXIT_WORD, strlen(EXIT_WORD)) == 0) 
			break;
		
		printf(buffer);
		
	}

    close(sock);
 
    return EXIT_SUCCESS;
}
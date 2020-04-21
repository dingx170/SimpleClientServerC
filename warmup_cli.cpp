/**
 * A simple client in IPv4 internet domain using TCP to 
 * send messages to a server and receive the responses. 
 * The hostname and port numbers are passed as arguments.
 * @author Tong (Debby) Ding
 * @version 1.0
 * @see CPSC 5510 Spring 2020, Seattle University
 */

// #include <stdio.h>
// #include <stdlib.h>
// #include <arpa/inet.h>
// #include <strings.h>
// #include <limits.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>

#include <iostream>
#include <unistd.h> // close()
#include <netdb.h>
#include <cstring> // strlen(), bcopy(), ...

#define BUF_SIZE 131072
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
 * Send a message to server and print the response
 * @param argc number of arguments entered by user
 * @param argv[] the content of arguments
 */ 
int main(int argc, char *argv[]) {

	int sock, port, err;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[BUF_SIZE];

	// check user input hostname & port number
    if (argc != 3)
		note("Usage example \"./warmup_cli HOSTNAME HOSTPORT\"");

	// get the port number with validation
	port = convertPort(argv[2]);

	// create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) 
        error("ERROR: fail to open socket");



	// FIX-ME
    server = gethostbyname(argv[1]);
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
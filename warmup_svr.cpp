/**
 * A simple server in IPv4 internet domain using TCP to receive 
 * messages from clients and reply with the messages inverted. 
 * The port number is passed as an argument.
 * @author Tong (Debby) Ding
 * @version 1.0
 * @see CPSC 5510 Spring 2020, Seattle University
 */

#include <iostream>
#include <unistd.h>    // close
#include <arpa/inet.h> // inet_ntoa
#include <cstring>     // strlen, stc.
#include <algorithm>   // reverse

#define BUF_SIZE 131073
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
		error("Error port number (not all digits)");
		
	// number out of range
	if (val < PORT_MIN || val > PORT_MAX)
		error("Error port number (out of range)");

	return (int) val;
}

/**
 * Receive messages from clients and convert the messages to reply
 * @param argc number of arguments entered by user
 * @param argv[] the content of arguments
 */ 
int main(int argc, char *argv[]) {

	int sock, tmp_sock;                     // sockets for listening and message transfer
	int port, err;                          // port and error number
	char buffer[BUF_SIZE];                  // message buffer size
	struct sockaddr_in ser_addr, cli_addr;  // server and client addresses
	socklen_t cli_size;                     // client address size

	// check if user entered port number
    if (argc != 2)
		error("Error usage i.e. \"./warmup_svr HOSTPORT\"");

	// get the port number with validation
	port = convertPort(argv[1]);

	// create a socket
	sock =  socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) 
		error("ERROR create socket");

	// get server address information
	bzero((char *) &ser_addr, sizeof(ser_addr));	
	ser_addr.sin_family = AF_INET;  
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
	ser_addr.sin_port = htons(port);

	// bind socket to server address
	err = bind(sock, (struct sockaddr *) &ser_addr, sizeof(ser_addr));
	if (err < 0) 
		error("ERROR bind socket");

	// start listening loop
	while (true) {

		// listen to incoming connection requests
		err = listen(sock, LISTEN_BKLOG);
		if (err < 0) 
			error("ERROR socket listen");

		cli_size = sizeof(cli_addr);

		// accept connection and open new socket for data transfer
		tmp_sock = accept(sock, (struct sockaddr *) &cli_addr, &cli_size);
		if (tmp_sock < 0) 
			error("ERROR accept client");

		printf("Accepted connection from %s port %d\n\n",
			inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));	

		// start message exchange loop
		while (true) {

			// receive message
			bzero(buffer, BUF_SIZE);
			err = recv(tmp_sock, buffer, BUF_SIZE, 0);
			if (err < 0)
				error("ERROR receive message");

			// prepare to close socket if get "quit" from client
			if (strncmp(buffer, EXIT_WORD, strlen(EXIT_WORD)) == 0) {
				printf("Connection terminated by %s port %d\n\n",
					inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

				send(tmp_sock, buffer, strlen(buffer), 0);
				break;
			}

			printf("Received message: %s\n", buffer);

			try {
				// reverse received message
				reverse(buffer, buffer + strlen(buffer));

				// send revised message
				err = send(tmp_sock, buffer, strlen(buffer), 0);
				if (err < 0) 
					error("ERROR send message");

			} catch (const char* msg) {
				// send error message
				err = send(tmp_sock, msg, strlen(msg), 0);
				if (err < 0) 
					error("ERROR send message");
			}
		}
		// close socket when client disconnected
		close(tmp_sock);
	}
	// close socket when listening stopped	
	close(sock); 
	
    return EXIT_SUCCESS;
}
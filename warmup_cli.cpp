/**
 * A simple client in IPv4 internet domain using TCP to 
 * send messages to a server and receive the responses. 
 * The port number is passed as an argument
 * @author Tong (Debby) Ding
 * @version 1.0
 * @see CPSC 5510 Spring 2020, Seattle University
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * Send a message to server and print the response
 * @param argc number of arguments entered by user
 * @param argv[] the content of arguments
 */ 
int main(int argc, char *argv[]) {

	if (argc != 3) {
		cerr << "Usage: ./warmup_cli ADDR PORT" << endl;
		return EXIT_FAILURE;
	}



    return EXIT_SUCCESS;
}
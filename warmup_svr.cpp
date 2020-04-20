/**
 * A simple server in IPv4 internet domain using TCP to 
 * receive messages and reply to clients with the messages converted. 
 * The port number is passed as an argument
 * @author Tong (Debby) Ding
 * @version 1.0
 * @see CPSC 5510 Spring 2020, Seattle University
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

/**
 * Receive messages from clients and convert the messages to reply
 * @param argc number of arguments entered by user
 * @param argv[] the content of arguments
 */ 
int main(int argc, char *argv[]) {

	if (argc != 2) {
		cerr << "Usage: ./warmup_svr PORT" << endl;
		return EXIT_FAILURE;
	}

	// htonl(INADDR_ANY)
	// error checking on the listen() call
    return EXIT_SUCCESS;
}
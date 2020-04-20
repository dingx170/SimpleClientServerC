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

	int port;
	
	if (argc != 3) {
		cerr << "Usage: ./warmup_cli HOSTNAME PORT" << endl;
		return EXIT_FAILURE;
	}

	string hosthame = argv[1];

	// get the port number with validation
	port = convertPort(argv[2]); 
	if (port == -1)
		return EXIT_FAILURE;

	cout << port << endl;

	cout << "\nWelcome! Enter a message to send or \"quit\" to exit.\n" << endl;
	
	// user-input loop
	while (true) {
		cout << "Send: ";
		string msg;
		getline(cin, msg);

		// exit
		if (msg == "quit")
			break;
		if (msg.length() == 0)
			continue;
		
		cout << msg << endl;
		// send() and recv() 
	}
	// close()
 
    return EXIT_SUCCESS;
}
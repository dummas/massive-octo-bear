/* Network definitions */
#include "network.h"
#include "thread.c"

int set_listening_socket() {
	int sock_fd;
	int optval;

	/* Define the socket */
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	/* Set the option value */
	optval = 1;

	/* Set socket to reuse mode */
	setsockopt(sock_fd, 
		SOL_SOCKET, 
		SO_REUSEADDR, 
		&optval,
		sizeof(optval));

	/* Check for errors */

	if (sock_fd < 0) {
		error("Error opening the socket");
	}

	/* Return the created socket */
	return sock_fd;
}

int bind_socket() {
	int bind_fd;

	bind_fd = bind(sock_fd, 
		(struct sockaddr*) & serv_addr,
		sizeof(serv_addr));

	if ( bind_fd < 0) {
		error("Error binding the socket");
	}

	return bind_fd;
}

bool initiate_mother() {
	/* Open the socket */
	sock_fd = set_listening_socket();
	/* Null the server address */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	/* Define the socket */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT_NO);

	/* Bind the socket */
	bind_fd = bind_socket();

	/* Initiate thread management */

	initiate_thread();

	return true;
}

void listen_to_connections() {
	/* Print status message */
	pstatus("Start of the listen");

	/* Start listen for connections */
	listen(sock_fd, 5);	

	/* Launch the main loop */
	while(1) {

		/* Get the address length */
		addr_size = sizeof(cli_addr);
		/* Accept connection */
		client_fd = accept(sock_fd, (struct sockaddr*) &cli_addr, &addr_size);

		pstatus("New connection arrived");

		/* Check if everything is correct */
		if (client_fd < 0) {
			error("Error on socket accept");
		}

		create_new_thread(client_fd);

	}

	terminate_connection();
	
}



void terminate_handler(int client) {
	pstatus("Killing current client connection");
	/* Terminate the connection */
	close(client);
}

void terminate_connection() {
	pstatus("Killing main socket");
	/* Terminate the socket */
	close(sock_fd);
}

bool check_recv(int n) {

	if (n < 0) {
		
		/* Error */
		return true;

	} else {

		/* Everything is good */
		return false;

	}

}

/**
* MESSAGE MODE
*/

char* recv_input(int client_fd) {
	int n;

	char* b;

	b = calloc(1, sizeof(buffer));

	memset(b, 0, BUFFER_LENGTH);

	n = recv(client_fd, b, BUFFER_LENGTH, 0);

	// printf("Original: %s\n\n", b);

	b = clean_input(b);

	return b;
}

char* clean_input(char *input) {
	int k;
	int j;
	char* p;

	for (k = 0; k < BUFFER_LENGTH; k++) {
		// LF or CR -- cut the char by its
		if (input[k] == 10 || input[k] == 13) {
			break;
		}
	}

	recv_package_length = k;

	/* Form new character */
	p = calloc(1, k*sizeof(char));

	for (j = 0; j < k; j++) {
		p[j] = input[j];

	}

	return p;

}

/**
* BINARY MODE
*/

char* recv_bin_input(int client_fd) {

	int n;

	char* b;

	b = calloc(1, sizeof(buffer));

	memset(b, 0, BUFFER_LENGTH);

	n = recv(client_fd, b, BUFFER_LENGTH, 0);

	b = clean_bin_input(b);

	return b;

}

char* clean_bin_input(char *input) {
	int k;
	int j;
	char* p;

	for (k = 0; k < BUFFER_LENGTH; k++) {		
		// End of message NULL NULL NULL
		if (k > 3) {
			// printf("%x %x %x\n", input[k], input[k-1], input[k-2]);
			if (input[k] == 0 && input[k-1] == 0 && input[k-2] == 0) {
				printf("Found the end\n");
				break;
			}
		}
	}

	recv_package_length = k-2;

	printf("Package length: %d\n", k);

	/* Form new character */
	p = calloc(1, k*sizeof(char));

	for (j = 0; j < k; j++) {
		p[j] = input[j];

	}

	return p;
}

void print_bits(unsigned char v) {
  int i; // for C89 compatability
  for(i = 7; i >= 0; i--) putchar('0' + ((v >> i) & 1));
}

int get_package_length() {
	return recv_package_length;
}
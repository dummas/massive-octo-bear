#include "mcp.h"
#include "database.c"

/**
* Mother Client Protocol
*/

char* get_message(int message_code) {

	switch(message_code) {
		/* OK message code */
		case 100:
			return (char *)"OK";
			break;
		/* READY to login message code */
		case 101:
			return (char *)"READY";
			break;
		/* Unknown command */
		case 102:
			return (char *)"UNKN";
			break;
		/* File transfer ready */
		case 103:
			return (char *)"TTF READY";
			break;
		/* Bussy message code */
		case 300:
			return (char *)"BUSY";
			break;
		/* Bad login/password message code */
		case 301:
			return (char *)"BADL";
			break;
		/* Login received */
		case 302:
			return (char *)"LOGR";
			break;
		/* Login is good */
		case 303:
			return (char *)"LOGG";
			break;
		/* Default return is the NULL message */
		default:
			return (char *)"NULL";
	};

}

int process_connection(int client_fd, int thread_id) {

	/* Received number of bytes */
	int n;

	/* Loop variable */
	int j;

	/* Input char buffer pointer */
	char* p;

	/* Message processing */
	printf("Waiting for the input on %d thread\n", thread_id);

	p = recv_input(client_fd);

	/* Print out the message */
	printf("> %s\n", p);

	/* Request for identification */
	if ( strncmp(p, "IDN", 4) == 0 ) {

		/* Print the status message */
		printf("Request for identification\n");
		/* Send READY message */
		send_message(client_fd, 101);
		/* NULL the buffer */
		username = recv_input(client_fd);

		printf("Received username: %s\n", username);

		password = recv_input(client_fd);

		printf("Received password: %s\n", password);

		/* Send login received message */
		send_message(client_fd, 302);

		/* Check the login */
		if ( check_login(username, password) ) {
			send_message(client_fd, 303);
			return 1;
		} else {
			send_message(client_fd, 301);
			return 0;
		}

		/* Return active connection, listen for further client input */
		return 1;

	}

	/* Request to transfer the file */
	if ( strncmp(p, "TTF", 4) == 0 ) {

		/* Print the status message */
		printf("Request to transfer the file\n");
		/* Send READY message */
		send_message(client_fd, 103);
		/* Null the incoming buffer */
		//bzero(buffer, BUFFER_LENGTH+1);

		/* Read the length of the file */
		p = recv_input(client_fd);

		/* Cascade the char* to int */
		packages_length = atoi(p);

		printf("Receiving size %d B\n", packages_length);

		/* Check the length of the packages */
		if ( packages_length > 0 ) {

			packages_received_length = 0;

			/* Reserve the memory stack */
			input_buffer = calloc(1, packages_length*8);

			/* Read the incomming data */
			while ( packages_length > packages_received_length ) {

				p = recv_bin_input(client_fd);

				memcpy(&work_buffer, &p, sizeof(p));

				/* Read the package number */
				package_number = atoi(&work_buffer[0]);

				package_length = get_package_length();

				printf("Package length: %d\n", get_package_length());

				printf("Received package number: %d\n", package_number);

				/* Scroll and copy received package */
				for (j=1; j < get_package_length(); j++) {

					input_buffer[j+packages_received_length-1] = work_buffer[j];

				}

				/* Increase the received packages length */
				packages_received_length = packages_received_length + package_length - 1;

				/* Send a confirmation */
				send_message(client_fd, 104);

			}

			/* @TODO: store the received data */
			fo_file = fopen("recv_output.dat", "w");
			for (j = 0; j < packages_received_length; j++ ) {
				fprintf(fo_file, "%c", input_buffer[j]);
			}
			
			fclose(fo_file);

			printf("Returning to monitor...\n");

			/* Free the memory */
			free(input_buffer);

		} else {
			perror("Error on packages length");
		}

		/* @TODO: check for furthe protocol related messages */

	}

	printf("Unknown protocol command, kill the connection");

	/* If no protocol command is recognized -- send unknown message */
	send_message(client_fd, 102);

	/* Quit the protocol process step and free the connection for other users */
	return 0;

}

void send_message(int client_fd, int message_code) {
	/* Local number of send bits */
	int n;

	/* Debug loop variable */
	int i;

	char *message;

	int message_length = sizeof(int) + sizeof(char);

	//bzero(message, sizeof(message));
	message = calloc(1, message_length);

	sprintf(message, "%d\n", message_code);

	// for (i = 0; i < message_length; i++) {
	// 	printf("%c - %d\n", message[i]);
	// }

	/* Message to send */
	n = send(client_fd, message, message_length, 0);

	if ( check_recv(n) ) {
		perror("Error no send message");
	}

}
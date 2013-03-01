#include <stdlib.h>
#include <stdio.h>
#include "database.h"
#include "config.h"

/**
* Mother Client Protocol
*/

// /* MCP data structure */
// struct mcp_data {
// 	int thread_id;
// 	int client_fd;
// };

//  The number of protocol connections is equal to threads connections 
// struct mcp_data mcp_data_array[NUM_THREADS];

/* Message buffer */
char buffer[BUFFER_LENGTH];

/* Length of the incoming file */
long packages_length;

/* Length of received packages */
long packages_received_length;

/* Length of received package */
int package_length;

/* Number of bytes received */
int bytes_number;

/* Package numbering */
int package_number;

/* Buffer to work on */
char* work_buffer;

/* Input char buffer */
int* input_buffer;

/* Username */
char* username;

/* Password */
char* password;

/* Multi-use pointer */
char *p = NULL;

/* Receiving end of the file, just for debug */
FILE *fo_file;

/**
* Function for messages initialization
*/
void initialize_mcp_messages();

/**
* Function to get the message by message code
*/
char* get_message(int);

/** 
* Function for processing incomming messages 
*/
int process_connection(int, int);

/**
* Function to send the message to the active connection
*/
void send_message(int, int);
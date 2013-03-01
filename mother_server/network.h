#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

/* SSL related */
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// typedef struct {
//     int socket;
//     SSL *sslHandle;
//     SSL_CTX *sslContext;
// } connection;

/* Listening socket identifier */
int sock_fd;
/* New socket descriptor */
int client_fd;
/* binned descriptor */
int bind_fd;
/*  For returns */
int n;
/* Client connection length */
socklen_t addr_size;
/* Input buffer */
// char buffer[255];
/* Socket decriptor */
struct sockaddr_in serv_addr, cli_addr;

/* Received package length (before 10 & 13 char) */
int recv_package_length;

/**
* Function to define the socket connection
*/
int set_listening_socket();

/**
* Function to bind the socket connection
*/
int bind_socket();

/**
* Function to initialize the socket connection
*/
bool initiate_mother();

/**
* Function to start listen for connections
*/
void listen_to_connections();

/**
* Function to handle the incoming messages
*/
// void* messages_handler(void *);

/**
* Function to terminate the active socket listener
*/
void terminate_connection();

/**
* Function to terminate the current clietn connection
*/
void terminate_handler(int);

/**
*
*/
bool check_recv(int);

/**
* Clearner for the messages data
*/
char* clean_input(char *);

/**
* Cleaner for binary data
*/
char* clean_bin_input(char *);

/**
* Received for messages data
*/
char* recv_input(int);

/**
* Received for binary data
*/
char* recv_bin_input(int);

/**
* Debug function to display received bits
*/
void print_bits(unsigned char);

/**
* Function return the package length
*/
int get_package_length();
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

/* Passing thread data */
struct thread_data {
	int thread_id;
	int client_fd;
};

/* Array of all thread data */
struct thread_data thread_data_array[NUM_THREADS+1];

/* Thread identification */
int t = 0;

/* Thread pointer */
int p_fd;

/* Array of threads */
pthread_t threads[NUM_THREADS+1];

pthread_t emergency_t;

/* Array of thread statuses */
int threads_state[NUM_THREADS+1];
/* 0 - thread is free */
/* 1 - thread is busy */

/* Condition variables for threads to work */
pthread_cond_t threads_condition[NUM_THREADS+1];

/* Shared data locker */
pthread_mutex_t l_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Thread attribute */
pthread_attr_t attr;

/* Status message from the MCP */
int mcp_status = 0;

/* Status message from threads */
void* status;

/*
* Function to create new thread
*/
bool create_new_thread(int);

/**
* Function to initiate thread management
*/
void initiate_thread();

/**
* Thread for main client connections
*/
void* construct_thread(void *);

/**
* Function to construct emergency thread
*/
void construct_emergency(int);

/**
* Thread for emergency connections
*/
void* emergency_thread(void *);

/**
* Function to synchronize threads
*/
void synchronize_thread();

/**
* Function to print the threads states for debugging purposes
*/
void print_threads_states();

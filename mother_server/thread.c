#include "config.h"
#include "thread.h"
#include "network.h"
#include "mcp.c"

/**
* Function for initialization of threads
*/
void initiate_thread() {
	/* Local loop variable */
	int i = 0;
	/* Pointer for the thread identification memory */
	int *p_i;
	/* Pass the status message */
	pstatus("Initiating thread management");
	/* Register the lock */
	pthread_mutex_init(&l_mutex, NULL);
	/* Initiate the attribute variable */
	pthread_attr_init(&attr);
	/* Set the threads attribute */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Assign thread id's */
	for ( i = 0; i < NUM_THREADS; i++ ) {
		/* Set all threads as free */
		threads_state[i] = 0;
	}

	/* Launch all the threads */
	for (i = 0; i < NUM_THREADS; i++) {		
		p_i = calloc(1, sizeof(int));
		memcpy(&p_i, &i, sizeof(int));
		p_fd = pthread_create(&threads[i], &attr, construct_thread, (void *)p_i);
	}
}

/**
* Function to create a new thread
* @client_fd -- client socket descriptor
*/
bool create_new_thread(int client_fd) {

	/* Local loop variable */
	int i = 0;

	pstatus("Searching for a free thread");

	for (i = 0; i < NUM_THREADS; i++ ) {
		if (threads_state[i] == 0) {
			break;
		}
	}

	if ( i == NUM_THREADS ) {

		printf("No free threads found -- rising up the emergency thread.\n");
		/* If all threads are busy -- construct emergency thread for quit status reply */
		construct_emergency( client_fd );
	}

	/* Lock the data update */
	pthread_mutex_lock(&l_mutex);

	/* Set the state of the current thread as busy */
	threads_state[i] = 1;
	pthread_cond_signal(&threads_condition[i]);

	/* Assign the data for thread */
	thread_data_array[i].thread_id = i;
	thread_data_array[i].client_fd = client_fd;

	/* Create the thread */
	//p_fd = pthread_create(&threads[t], NULL,  messages_handler, (void *)&thread_data_array[t]);

	pthread_mutex_unlock(&l_mutex);

	/* Check the threading */
	if (p_fd < 0) {
		error("Error on thread create");
	} else {
		/*pthread_detach(threads[t]);*/
		return true;
	}

	return false;

}

/**
* Function for thread synchronization
*/
void synchronize_thread() {
	pstatus("Synchronizing the threads");
	/* Local loop variable */
	int j;
	/* Free attribute */
	//pthread_attr_destroy(&attr);

	print_threads_states();

	for (j = 0; j < NUM_THREADS; j++) {
		printf("Scrolling on %d thread\n", j);
		if (threads_state[j] == 1) {
			p_fd = pthread_join(threads[j], &status);

			printf("Thread status: %ld\n", (long)status);

			if (p_fd < 0) {
				perror("Error on thread join");
			}
		}
	}
}

/**
* Function for constructing client connection threads
*/
void* construct_thread(void* arg) {

	pstatus("Constructing thread");

	int thread_id;

	int *p_id;

	p_id = (int *) arg;

	memcpy(&thread_id, &p_id, sizeof(int));

	printf("Thread id: %d\n", thread_id);

	for(;;) {
		/* Lock the mutex */
		pthread_mutex_lock(&l_mutex);

		/* Check the signal condition */
		pthread_cond_wait(&threads_condition[thread_id], &l_mutex);

		/* Unlock the mutex */
		pthread_mutex_unlock(&l_mutex);

		pstatus("Lock unleashed");

		/*int client_fd = thread_data_array[thread_id].client_fd;*/

		char buffer[BUFFER_LENGTH];

		do {

			mcp_status = process_connection( thread_data_array[thread_id].client_fd, thread_id );

		} while (mcp_status != 0);

		/* Lock the data upate */
		pthread_mutex_lock(&l_mutex);

		/* Set the state of the current thread as busy */
		threads_state[thread_id] = 0;
		pthread_cond_signal(&threads_condition[thread_id]);

		/* Kill the connection with the client */
		close(thread_data_array[thread_id].client_fd);

		/* Assign the data for thread */
		thread_data_array[thread_id].client_fd = 0;

		/* Destroy the condition */
		pthread_cond_destroy(&threads_condition[thread_id]);

		/* Create the condition */
		pthread_cond_init(&threads_condition[thread_id], NULL);

		/* Unlock the mutex */
		pthread_mutex_unlock(&l_mutex);

	}

	/* Kill the thread */
	pthread_exit(NULL);
}

void construct_emergency( int client_fd ) {
	// TODO: check the emergency track

	/* Create a pointer */
	int* p_client_fd;

	/* Reserve the memory block for the connection description */
	p_client_fd = calloc(1, sizeof(int));

	/* Copy the address of the client descriptor */
	memcpy(&p_client_fd, &client_fd, sizeof(int));

	/* Launch the thread, using the pointer */
	p_fd = pthread_create(&emergency_t, &attr, emergency_thread, (void *)p_client_fd);

	if ( p_fd < 0 ) {
		perror("Error on emergency thread");
	}

}

void* emergency_thread( void *arg ) {

	int* p_client_fd;

	int client_fd;

	/* Receive the information */
	p_client_fd = (int *) arg;

	memcpy(&client_fd, &p_client_fd, sizeof(int));

	printf("Client: %d\n", client_fd);

	/* Send a message */
	send_message(client_fd, 300);

	close(client_fd);

	//free(p_client_fd);

	/* Kill the thread */
	//pthread_cancel(NUM_THREADS+1);
	pthread_exit(NULL);
	//pthread_cancel(emergency_t);
}

// void* messages_handler(void* thread_arg) {

// 	struct thread_data *recv_data;

// 	recv_data = (struct thread_data *) thread_arg;

// 	int client_fd = recv_data->client_fd;

// 	/* Input buffer */
// 	char buffer[BUFFER_LENGTH];

// 	/* Start reading the input from the client */
// 	do {
// 		/* Fill the buffer with zeros */
// 		bzero(buffer, BUFFER_LENGTH + 1);

// 		/* Read the message from the client */
// 		n = read(recv_data->client_fd, buffer, BUFFER_LENGTH);

// 		/* Check for errors */
// 		if (n < 0) {
// 			error("Error on socket recv");
// 		}

// 		/* Print the results */
// 		printf("Here is the message: %s\n", buffer);

// 		/* Sync all the threads */
// 		synchronize_thread();

// 	} while (strncmp(buffer, "bye\r", 4) != 0);

// 	/* Terminate the connection */
// 	terminate_handler(client_fd);

// 	pthread_exit(NULL);
// }

/* DEBUG */
void print_threads_states() {
	int j = 0;
	for (j = 0; j < NUM_THREADS; j++) {
		printf("Thread %d status %d\n", j, threads_state[j]);
	}

}

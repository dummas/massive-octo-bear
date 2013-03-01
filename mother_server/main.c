#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
/* The control functions */
#include "config.h"
#include "errors.c"
#include "network.c"

/**
* The main function
*/

int main(int argc, char *argv[]) {

	/* Try to initiate the mother server */
	if ( initiate_mother() == true ) {

		/* Launch the listening connections process */
		listen_to_connections();

	} else {

		perror("Error on mother initialization");
		
	}
	
	return 0;
}
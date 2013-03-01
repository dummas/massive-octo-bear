/* Function to report the error */
void error(const char *msg) {
	if (DEBUG) {
		perror(msg);
	}
	exit(1);
}

/* Function to print the status messages */
void pstatus(const char *msg) {
	if (DEBUG) {
		printf("%s\n", msg);
	}
}

void pint(int tmp) {
	if (DEBUG) {
		printf("%d\n", tmp);
	}
}
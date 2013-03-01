#include <string.h>
#include "database.h"
#include "config.h"

/**
* Function connects to the database
*/
bool db_connect() {

	printf("Connecting to the database\n");

	mysql_connection = mysql_init(NULL);

	if (mysql_connection == NULL) {

		printf("Error %u: %s\n", mysql_errno(mysql_connection), mysql_error(mysql_connection));

		return false;

	}

	if (mysql_real_connect(mysql_connection, "MYSQL_HOST", "MYSQL_USER",
		"MYSQL_PASS", "MYSQL_DATB", 0, NULL, 0)) {

		printf("Error %u: %s\n", mysql_errno(mysql_connection), mysql_error(mysql_connection));

		return false;

	}

	printf("Connected to the database\n");

	a_connection = true;

	return true;


}

/**
* Function disconnects from the database
*/
void db_disconnect() {

	mysql_close(mysql_connection);

	a_connection = 0;
}

/**
* Function checks the connection state of the database
*/
bool connected() {

	if ( a_connection == 1 ) {

		return false;

	} else {

		return true;

	}

}

int check_login(char *username, char *password) {

	/* Database configuration */
	database_host = "localhost";
	database_user = "root";
	database_pass = "classic";
	database_datb = "grass";

	int j;

	/* Initialize the connection */
	mysql_connection = mysql_init(NULL);
	
	if (mysql_real_connect(mysql_connection, "localhost", "root", "classic", "grass", 0, NULL, 0) == NULL) {

		printf("Connect Error %u: %s\n", mysql_errno(mysql_connection), mysql_error(mysql_connection));

		return false;

	}

	//char a_query[1024];
	char* a_query = calloc(1, sizeof(char)*1024);

	sprintf(a_query, 
		"SELECT * FROM `users` WHERE `username`='%s' AND `password`='%s';",
		username, password);

	printf("%s\n", a_query);

	if ( mysql_query(mysql_connection, a_query) != 0 ) {
		printf("Query Error %u: %s\n", mysql_errno(mysql_connection), mysql_error(mysql_connection));

		/* Mysql gone away */
		if (mysql_errno(mysql_connection) == 2006) {
			/* TODO: Reconnect to the database */
		}

		return 0;
	}

	result = mysql_store_result(mysql_connection);

	num_of_fields = mysql_num_fields(result);

	num_of_rows = mysql_num_rows(result);

	mysql_fields = mysql_fetch_fields(result);

	while (mysql_row = mysql_fetch_row(result)) {
		printf("%s %s\n", mysql_row[1], mysql_row[2]);
	}

	/* Free the results */
	mysql_free_result(result);

	/* Free the query */
	free(a_query);

	mysql_close(mysql_connection);

	if ( num_of_rows > 0 ) {
		return 1;
	} else {
		return 0;
	}

	return 0;
}
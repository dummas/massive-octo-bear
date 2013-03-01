#include <my_global.h>
#include <mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

/* Configuration */
char* database_host;
char* database_user;
char* database_pass;
char* database_datb;

/* Variable, defined for current database connection state */
int a_connection;

/* MySQL connection pointer */
MYSQL *mysql_connection;

/* Result from the database */
MYSQL_RES *result;

MYSQL_FIELD *mysql_fields;

MYSQL_ROW mysql_row;

/* Number of fields retrieved */
int num_of_fields;

/* Number of rows received */
int num_of_rows;

/*
* Function to perform the connection to the database
*/
bool db_connect();

/**
* Function to perform the disconnection from the database
*/
void db_disconnect();

/*
* Function to check if the connection to the database is active 
*/
bool connected();

/**
* Function to perform the user login to the database
*/
int check_login(char *, char *);
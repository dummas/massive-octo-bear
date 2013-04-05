#include <stdio.h>
#include <string.h>

/* Definition of functions */
void print_buffer(void);
void add_to_buffer(char);
void clear_buffer();

/* The reserved buffer */
char buffer[255];
int i = 0;

int main(int argc, char const *argv[]) {
    add_to_buffer(0x42);
    add_to_buffer(0x0d);
    add_to_buffer(0x0a);
    print_buffer();
    clear_buffer();
    add_to_buffer(0x48);
    add_to_buffer(0x0d);
    add_to_buffer(0x0a);
    print_buffer();
    return 0;
}

/**
* Print the buffer
**/
void print_buffer() {
    printf("%s\n", buffer);
}

/**
* Adding char to buffer
**/
void add_to_buffer(char what_to_add) {
    buffer[i] = what_to_add;
    i = i + 1;
}

/**
* Cleaning the buffer
**/
void clear_buffer() {
    memset(buffer, 0, 255);
    i = 0;
}

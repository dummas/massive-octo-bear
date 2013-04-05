#include <stdio.h>

void print_the_message(char*);

int main(int argc, char const *argv[]) {
    /* The message */
    char *message;
    /* Define the message */
    message = "AT+VER";
    /* Print the char length */
    printf("Size of message %d\n", sizeof(message));
    /* The looper */
    print_the_message(message);
    
    return 0;
}

void print_the_message(char *message) {
    int i = 0;
    while(message[i] != '\0') {
        printf("%c\n", message[i]);
        i = i + 1;
    }
    printf("%c\n", 0x0d);
    printf("%s\n", message);
}
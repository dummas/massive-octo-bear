#include <stdio.h>
#include <string.h>

void print_message(char option);
char *append(const char*, char);

int main(int argc, char const *argv[]) {
    print_message('+');
    return 0;
}

void print_message(char option) {
    char *message;
    message = "AT+DCOV";
    printf("%d\n", strlen(message));
    message = append(message, option);
    printf("%s\n", message);
    free(message);
}

char *append(const char *o, char c) {
    int len = strlen(o);
    char buf[len+2];
    strcpy(buf, o);
    buf[len] = c;
    buf[len+1] = 0;
    return strdup(buf);
}
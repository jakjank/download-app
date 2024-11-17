#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAX_LENGTH 99

typedef struct {
    char user[MAX_LENGTH];
    char password[MAX_LENGTH];
    char host[MAX_LENGTH];
    char url_path[MAX_LENGTH];
} args;

int parse_arguments(char *url, args *args);


#endif
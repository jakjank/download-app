#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAX_LENGTH 99
#define PORT 21 // default for FTP


typedef struct {
    char user[MAX_LENGTH];
    char password[MAX_LENGTH];
    char host[MAX_LENGTH];
    char url_path[MAX_LENGTH];
} args;

// saves arguments into args structure
int parse_arguments(char *url, args *args);

// returns socket descriptor
int connect_to_server(const char *address, int port);

#endif
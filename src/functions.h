#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAX_LENGTH 99
#define PORT 21 // default for FTP


typedef struct {
    char user[MAX_LENGTH];
    char password[MAX_LENGTH];
    char host[MAX_LENGTH];
    char path[MAX_LENGTH];
} args;

typedef struct {
    char *ip;
    int port;
} ip_addr;

// saves arguments into args structure
// FIX IF USER/PASSWORD EMPTY
int parse_arguments(char *url, args *args);

// returns socket descriptor
int connect_to_server(const char *address, int port);

// tries to log-in with given credentials
// returns -1 on error
// ADD SENSITIVITY FOR RESPONSES CODES
int log_in(int socket_fd, char *user, char *passwd);

int go_passive(int socket_fd, char *ip, int *port);
#endif
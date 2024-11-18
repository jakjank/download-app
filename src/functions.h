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

// saves arguments into args structure
// FIX IF USER/PASSWORD EMPTY
int parse_arguments(char *url, args *args);

// returns socket descriptor
// if expecting response set resp to 1
// otherwise response wont be read
int get_socket(const char *address, int port, int resp);

// tries to log-in with given credentials
// returns -1 on error
// ADD SENSITIVITY FOR RESPONSES CODES
int log_in(int socket_fd, char *user, char *passwd);

// sends pasv to the remote machine
// sets port and ip
int go_passive(int socket_fd, char *ip, int *port);

int download(int contr_socket, int data_socket, char *path);

#endif
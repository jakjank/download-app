#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "functions.h"

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("usage: %s %s \n", argv[0], "ftp://[<user>:<password>@]<host>/<url-path>");
        exit(-1);
    }
    
    // Parse arguments
    args args;
    if(parse_arguments(argv[1], &args) < 0){
        printf("too litle arguments. \n");
        exit(-1);
    }

    printf("Given arguments:\n user: %s\n password: %s\n host: %s\n path: %s\n" ,
            args.user, args.password, args.host, args.path);

    // Get host IP address (inspired by given  getip.c)
    struct hostent *h;
    if((h = gethostbyname(args.host)) == NULL){
        printf("no such host: %s\n", args.host);
        exit(-1);
    }
    const char *address = inet_ntoa(*((struct in_addr *)h->h_addr_list[0]));

    //printf("%s\n", address);

    // Open a connection
    int socket_fd = connect_to_server(address, PORT);
    if(socket_fd == -1){
        printf("unable to open connection\n");
        exit(-1);
    }

    // Log in
    if(log_in(socket_fd, args.user, args.password) == -1){
        printf("unable to log-in\n");
        exit(-1);
    }

    char buffer[1024];
    // Go into passive mode
    send(socket_fd, "PASV\r\n", strlen("PASV\r\n"), 0);
    recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
    printf("Server Response: %s\n", buffer);

    return 0;
}
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
    int socket_contr = get_socket(address, PORT, 1);
    if(socket_contr == -1){
        printf("unable to open connection\n");
        exit(-1);
    }

    // Log in
    if(log_in(socket_contr, args.user, args.password) == -1){
        printf("unable to log-in\n");
        exit(-1);
    }

    // Enter passive mode and get IP and port for data
    char ip[16];
    int port;
    if(go_passive(socket_contr, ip, &port) == -1){
        printf("unable to enter pasv mode\n");
        exit(-1);
    }
    printf("%s, %d \n", ip, port);

    // get data socket
    int data_socket = get_socket(ip, port, 0);
     if(data_socket == -1){
        printf("unable to get data socket\n");
        exit(-1);
    }

    //change remote directory
    if(download(socket_contr, data_socket, args.path) == -1){
        printf("unable to download\n");
        exit(-1);
    }

    return 0;
}
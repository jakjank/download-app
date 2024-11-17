#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "functions.h"

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("usage: %s %s \n", argv[0], "ftp://[<user>:<password>@]<host>/<url-path>");
        exit(-1);
    }
    
    // Parse arguments
    args args;
    if(parse_arguments(argv[1], &args) != 4){
        printf("expected 4 arguments. \n");
        exit(-1);
    }

    // Get host IP address (inspired by given  getip.c)
    struct hostent *h;
    if((h = gethostbyname(args.host)) == NULL){
        printf("no such host: %s\n", args.host);
        exit(-1);
    }
    const char *address = inet_ntoa(*((struct in_addr *)h->h_addr_list[0]));

    // Open a connection
    int socket_fd = connect_to_server(address, PORT);
    printf("%d\n", socket_fd);
    return 0;
}
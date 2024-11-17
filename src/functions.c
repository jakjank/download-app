#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "functions.h"


int parse_arguments(char *url, args *args){   
    // 99 like MAX_LENGTH
    return sscanf(url, "ftp://%99[^:]:%99[^@]@%99[^/]/%99[^\n]",
                  args->user, args->password, args->host, args->url_path);
}

int connect_to_server(const char *address, int port){
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error creating socket\n");
        return(-1);
    }

    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);
    server_addr.sin_port = htons(port); 

    if(connect(sockfd, (struct sockaddr *) &server_addr,sizeof(server_addr)) < 0) {
        printf("unable to connect\n");
        return -1;
    }

    printf("succesful connection\n");
    return sockfd;
}

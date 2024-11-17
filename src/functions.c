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
                  args->user, args->password, args->host, args->path);
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

    //printf("succesful connection\n");
    
    char r_buffer[1024];
    recv(sockfd, r_buffer, sizeof(r_buffer) - 1, 0);
    printf("Server Response: %s\n", r_buffer);

    return sockfd;
}

int log_in(int socket_fd, char *user, char *passwd){
    char buffer[128];
    char r_buffer[1024];

    snprintf(buffer, sizeof(buffer), "USER %s\r\n\n", user);
    printf("sending: %s", buffer);
    int size = send(socket_fd, buffer, strlen(buffer), 0);
    recv(socket_fd, r_buffer, sizeof(r_buffer) - 1, 0);
    printf("Server Response: %s", r_buffer);
    
    snprintf(buffer, sizeof(buffer), "PASS %s\r\n", passwd);
    printf("sending: %s", buffer);
    send(socket_fd, buffer, strlen(buffer), 0);
    recv(socket_fd, r_buffer, sizeof(r_buffer) - 1, 0);
    printf("Server Response: %s", r_buffer);

    return 0;
}

int go_passive(int socket_fd, char *ip, int *port){
    int h1, h2, h3, h4, p1, p2;
    
    char buffer[1024];
    send(socket_fd, "PASV\r\n", strlen("PASV\r\n"), 0);
    recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
    printf("Server Response: %s\n", buffer);

    if (sscanf(buffer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &h1, &h2, &h3, &h4, &p1, &p2) == 6) {
        snprintf(ip, 16, "%d.%d.%d.%d", h1, h2, h3, h4);
        
        *port = p1 * 256 + p2;
        return 1;
    }
    return 0;
}
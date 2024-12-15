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

int get_socket(const char *address, int port, int resp){
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

    if(resp == 1){
        char r_buffer[1024] = {0};
        int bytes;
        bytes = recv(sockfd, r_buffer, sizeof(r_buffer) - 1, 0);
        if(bytes > 0){
            printf("Server Response: %s\n", r_buffer);
        }
    }
    return sockfd;
}

int log_in(int socket_fd, char *user, char *passwd){
    char buffer[128];
    char r_buffer[1024] = {0};

    snprintf(buffer, sizeof(buffer), "USER %s\r\n", user);
    printf("sending: %s", buffer);
    int size = send(socket_fd, buffer, strlen(buffer), 0);
    recv(socket_fd, r_buffer, sizeof(r_buffer) - 1, 0);
    printf("Server Response: %s", r_buffer);
    
    for(int i=0;i<1024;i++){
        r_buffer[i] = '\0';
    }
    snprintf(buffer, sizeof(buffer), "PASS %s\r\n", passwd);
    printf("sending: %s", buffer);
    size = send(socket_fd, buffer, strlen(buffer), 0);
    //printf("sent %d bytes\n", size);
    recv(socket_fd, r_buffer, sizeof(r_buffer) - 1, 0);
    printf("Server Response: %s", r_buffer);

    return 0;
}

int go_passive(int socket_fd, char *ip, int *port) {
    int h1, h2, h3, h4, p1, p2;
    char buffer[1024] = {0};
    int bytes;

    // Send PASV command
    if (send(socket_fd, "PASV\r\n", strlen("PASV\r\n"), 0) < 0) {
        perror("send failed");
        return -1;
    }

    // Receive response
    bytes = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) {
        perror("recv failed or no data received");
        return -1;
    }
    buffer[bytes] = '\0';  // Null-terminate

    printf("Server Response: %s\n", buffer);

    // Parse server response
    int parsed = sscanf(buffer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)",
                        &h1, &h2, &h3, &h4, &p1, &p2);

    if (parsed == 6) {
        snprintf(ip, 16, "%d.%d.%d.%d", h1, h2, h3, h4);
        *port = p1 * 256 + p2;
        return 1;
    }

    printf("Parsing failed. Response: [%s]\n", buffer);
    return -1;
}

int go_to(int contr_socket, char *path){   
    char buffer[1024] = {0};
    char *last_slash = strrchr(path, '/');
    char *filename;
    char tmp_path[128] = {0};

    //get path
    if (last_slash != NULL) {
        size_t length = last_slash - path + 1;
        strncpy(tmp_path, path, length);
        tmp_path[length] = '\0';
    }
    
    printf("path: %s\n", tmp_path);

    if(tmp_path[0] != 0){
        snprintf(buffer, sizeof(buffer), "CD %s\r\n", tmp_path);
        if(send(contr_socket, buffer, strlen(buffer), 0) < 0){
            printf("unable send CD ...\n");
            return -1;
        }
    }
    
    return 0;
}

int download(int contr_socket, int data_socket, char *path){
    char buffer[1024] = {0};
    char *last_slash = strrchr(path, '/');
    char filename[128]= {0};

    memset(buffer, '0', sizeof(buffer));
    if(last_slash != 0){
        strcpy(filename, last_slash + 1); 
    }
    else{
        strcpy(filename, path);
    }
    printf("filename: %s\n", filename);

    snprintf(buffer, sizeof(buffer), "RETR %s\r\n", filename);
    if(send(contr_socket, buffer, sizeof(buffer), 0) < 0){
        printf("unable to send RETR ...\n");
        return -1;
    }

    FILE *file = fopen(path, "wb");

    int bytes_received;
    while ((bytes_received = recv(data_socket, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
    }
    fclose(file);

    return 0;
}
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    //create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket reation failed\n");
        exit(0);
    } else {
        printf("socket successfully created\n");
    }

    //servaddr initialisieren
    bzero(&servaddr, sizeof(servaddr));

    //assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //own IP
    servaddr.sin_port = htons(PORT);

    //connect
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed\n");
        exit(0);
    } else {
        printf("connected to the server\n");
    }


    //get single message from server
    char buff[MAX];
    int n;

    bzero(buff, sizeof(buff));
    printf("to server: ");
    n = 0;
    while ((buff[n++] = getchar()) != '\n');
    ;
    strtok(buff, "\n");

    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("from server: %s\n", buff);

    //close
    close(sockfd);
}

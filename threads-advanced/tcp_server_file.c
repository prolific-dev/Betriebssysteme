





#include <netdb.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 80
#define BUFFSIZE 1024
#define PORT 8080
#define SA struct sockaddr


// https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/


// Function designed for chat between client and server.

int main()
{
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1)
  {
    printf("socket creation failed...\n");
    exit(0);
  }
  else
  {
    printf("socket succesfully created\n");
  }

  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);
  
  int flag = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));

  // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
    {
        printf("Socket successfully binded..\n");
    }

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
    {
        printf("Server listening..\n");
    }


    // Accept the data packet from client and verification
   //connfd = accept(sockfd, (SA*)&cli, &len);
   //if (connfd < 0) {
   //    printf("server acccept failed...\n");
   //    exit(0);
   //}
   //else
   //    printf("server acccept the client...\n");


  // Function for chatting between client and server
  //func(connfd);

  char buff[BUFFSIZE];
  int n;
  //int testReqs, numReqs = 0, openReqs = 0;

  // initialize sets
  fd_set afds, rfds;
  FD_ZERO(&afds);
  FD_SET(sockfd, &afds);

  //infinite loop for chat
  //infinte loop
    for(;;) {
        rfds = afds;

        // select befehl
        if (select(FD_SETSIZE, &rfds, NULL, NULL, NULL) == -1) {
            printf("select error\n");
            exit(0);
        }

        //check if we have data with fd_isset / fd set loop
        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &rfds)) {
                if (i == sockfd) {
                    int cfd;
                    cfd = accept(sockfd, (SA*)&cli, &len);
                    if (connfd < 0) {
                        printf("server accepting failed\n");
                        exit(0);
                    } else {
                        printf("server accepting client\n");
                    }
                    FD_SET(cfd, &afds);
                } else {

                    //bzero(buff, MAX);

                    memset(buff, 0, BUFFSIZE);

                    //read message from client
                    //read(i, buff, sizeof(buff));

                    if (recv(i, buff, BUFFSIZE, 0) == -1) {
                        printf("error receiving\n");
                    }

                    //printf buffer
                    printf("from client: %s\n", buff);

                    int fd;
                    const char* file = buff;
                    if ((fd = open(file, O_RDONLY)) == -1) {
                        printf("error opening");
                        exit(0);
                    }

                    if (read(fd, buff, BUFFSIZE) == -1) {
                        printf("error reading");
                        exit(0);
                    }

                    printf("file contains %s\n", buff);

                    //answer client
                    write(i, buff, sizeof(buff));

                    close(i);
                    FD_CLR(i, &afds);

                }
            }
        }



    }

  // After chatting close the socket
  close(sockfd);
}

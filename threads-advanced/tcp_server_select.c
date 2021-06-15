#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <sys/time.h>
#include <unistd.h>

#define MAX 80
#define PORT 8081
#define SA struct sockaddr


// https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

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


  int fd;
  int minFD = 0;
  int maxFD = FD_SETSIZE;

  // while (1) {
  //   fd_set readFDs;
  //   FD_ZERO(&readFDs);
  //
  //   int fd;
  //   for(fd = minFD; fd < maxFD; fd++)
  //   {
  //     FD_SET(fd, &readFDs);
  //   }
  //
  //   int rc = select(maxFD+1, &readFDs, NULL, NULL, NULL);
  //
  //
  //   for(fd = minFD; fd < maxFD; fd++)
  //   {
  //     if (FD_ISSET(fd, &readFDs)) {
  //       FD_SET(fd, &readFDs);
  //     }
  //   }
  // }
  //
  char buff[MAX];
  int n;

  // initialize sets
  fd_set afds, rfds;
  FD_ZERO(&afds);
  FD_SET(sockfd, &afds);

  //infinite loop for chat
  for (;;) {
    rfds = afds;

      // if (select(FD_SETSIZE+1, &rfds, NULL, NULL, NULL) == -1)
      // {
      //   printf("select error\n");
      //   exit(0);
      // }


      int rc = select(FD_SETSIZE+1, &rfds, NULL, NULL, NULL);

      if (rc == -1) {
        printf("select error\n");
        exit(0);
      } else if (rc == 0) {
        printf("select timeout\n");
        exit(0);
      }

      // check if we have data with fd_isset / fd set loop
      for(int i = 0; i < FD_SETSIZE; i++)
      {
        if (FD_ISSET(i, &rfds))
        {
          if (i == sockfd)
          {
            int cfd;

            // Accept the data packet from client and verification
            connfd = accept(sockfd, (SA*)&cli, &len);
            if (connfd < 0)
            {
              printf("server accepting failed\n");
              exit(0);
            }
            else
            {
              printf("server accepting client\n");
            }
            cfd = connfd;
            FD_SET(cfd, &afds);
          }
          else
          {
            bzero(buff, MAX);

            // read the message from client and copy it in buffer
            read(i, buff, sizeof(buff));
            // print buffer which contains the client contents
            printf("From client: %s\t To client : ", buff);

            // and send that buffer to client
            write(sockfd, buff, sizeof(buff));

            // if msg contains "Exit" then server exit and chat ended.
            if (strncmp("exit", buff, 4) == 0) {
                printf("Server Exit...\n");
                break;
            }
          }
        }
      }
    }

  // After chatting close the socket
  close(sockfd);
}

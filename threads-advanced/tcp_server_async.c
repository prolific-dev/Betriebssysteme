

#include <netdb.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h> //memset()
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>  //read(), close()
#include <fcntl.h> //open
#include <errno.h> //EINPROGRESS
#include <aio.h> //Aiocb

#define MAX 80
#define BUFFSIZE 1024
#define PORT 8080
#define SA struct sockaddr

//iorequest structure
struct ioRequest {
    int cfd;
    int status;
    struct aiocb *aiocbp;
};

int main() {

    int sockfd, connfd;
    socklen_t cli_size;
    struct sockaddr_in servaddr, cli;

    struct ioRequest *ios;
    struct aiocb *aiocbs;

    int waitingReqs = 0;
    int openReqs = 0;

    //allocate array for IOs
    if ((ios = calloc(80, sizeof(struct ioRequest))) == NULL) {
        printf("error allocating ios\n");
        exit(0);
    }
    if ((aiocbs = calloc(80, sizeof(struct aiocb))) == NULL) {
        printf("error allocating aiocbs\n");
        exit(0);
    }

    // create socket and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed\n");
        exit(0);
    } else {
        printf("socket successfully created - number %i\n", sockfd);
    }

    bzero(&servaddr, sizeof(servaddr));

    // assign IP , PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    //prevent binding errors (open sockets?)
    int flag = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));

    // binding socket to IP
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed\n");
        exit(0);
    } else {
        printf("socket successfully binded\n");
    }

    // now listen to clien
    if ((listen(sockfd, 5)) != 0) {
        printf("listen failed\n");
        exit(0);
    } else {
        printf("server is listening\n");
    }

    char buff[BUFFSIZE];

    //initialize sets
    fd_set afds, rfds;
    FD_ZERO(&afds);
    FD_SET(sockfd, &afds);

    int n;

    cli_size = sizeof(struct sockaddr_in);

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

            printf("FD_SETSIZE %i ---- i: %i\n", FD_SETSIZE, i);
            if (FD_ISSET(i, &rfds)) {
                if (i == sockfd) {
                    int cfd;
                    cfd = accept(sockfd, (struct sockaddr *)&cli, &cli_size);
                    if (cfd < 0) {
                        printf("server accepting failed\n");
                        exit(0);
                    } else {
                        printf("new open connection\n");
                    }

                    FD_SET(cfd, &afds);

                    openReqs++;
                    waitingReqs++;
                } else {

                    //bzero(buff, MAX);

                    memset(buff, 0, BUFFSIZE);

                    waitingReqs++;

                    //read message from client
                    //read(i, buff, sizeof(buff));

                    if (recv(i, buff, BUFFSIZE, 0) == -1) {
                        printf("error receiving\n");
                    }

                    //printf buffer
                    printf("from client: %s!\n", buff);

                    //initizalize IO[waitingReq]
                    ios[waitingReqs].cfd =  i;
                    ios[waitingReqs].status = EINPROGRESS;
                    ios[waitingReqs].aiocbp = &aiocbs[waitingReqs];

                    //inizitalize File descriptor
                    if ((ios[waitingReqs].aiocbp->aio_fildes = open(buff, O_RDONLY)) == -1) {
                        printf("open file error\n");
                        exit(0);
                    }

                    //initialize buffer for read call
                    if ((ios[waitingReqs].aiocbp->aio_buf = malloc(BUFFSIZE)) == NULL) {
                        printf("error allocating buffer\n");
                        exit(0);
                    }

                    //
                    ios[waitingReqs].aiocbp->aio_nbytes = BUFFSIZE;
                    ios[waitingReqs].aiocbp->aio_offset = 0;
                    ios[waitingReqs].aiocbp->aio_reqprio = 0;

                    //call asynchron read
                    if (aio_read(ios[waitingReqs].aiocbp) == -1) {
                       printf("error read\n");
                       exit(0);
                    }



                }
            }
        }

        // check if one of the waiting IOs is ready
        if (openReqs > 0) {

            printf("checking for finished IO\n");
            for(int i = 0; i < waitingReqs; i++) {

                printf("checking for finished IO %i\n", i);

                if (ios[i].status == EINPROGRESS) {

                    //call error to check if ready
                    ios[i].status = aio_error(ios[i].aiocbp);
                    if (ios[i].status == 0) {
                      //IO is finished, send back to client
                      if (send(ios[i].cfd, (char *) ios[i].aiocbp->aio_buf, BUFFSIZE, 0) == -1) {
                          printf("sending error\n");
                      } else {
                          printf("sent file to client\n");
                      }
                      close(ios[i].aiocbp->aio_fildes);
                      close(ios[i].cfd);
                      FD_CLR(ios[i].cfd, &afds);
                    }
                    if (ios[i].status != EINPROGRESS) {
                        //delete one openReq, because its done
                        printf("reduce openReqs\n");
                        openReqs--;
                    }
                }
            }
        }


    }

    return 0;

}

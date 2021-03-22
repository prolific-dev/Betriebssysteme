#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
        printf("hello world (pid:%d)\n", (int) getpid());
        close(STDOUT_FILENO);
	open("./fork_open.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        int rc = fork();

        if(rc < 0) // fork failed; exit
        {
                fprintf(stderr, "fork failed\n");
                exit(1);
        }
        else if (rc == 0) // child (new process)
        {
                printf("hello, I am child (pid:%d)\n", (int) getpid());
        }
        else // parent goes down this path (main)
        {
                printf("hello, I am parent of %d (pid:%d)\n", rc, (int) getpid());
        }
        return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int rc = fork();
	if(rc < 0) // fork failed, exit
	{
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
	else if(rc == 0) // child: redirect standard output to a file
	{
		close(STDOUT_FILENO);
		printf("hello, i am child %d and standard output is closed", rc);
	}
	else // parent goes down with this path (main)
	{
		int rc_wait = wait(NULL);
	}
	return 0;
}

/*
 * Print im Kind-Prozess wird nicht mehr ausgefuehrt.
 */

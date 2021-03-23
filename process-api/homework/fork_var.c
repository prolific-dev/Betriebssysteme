#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int x = 100;
	printf("hello world (pid:%d), x = %d\n", (int) getpid(), x);
	int rc = fork();
	if(rc < 0) // fork failed; exit
	{
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
	else if (rc == 0) // child (new process)
	{	
		printf("hello, I am child (pid:%d) and x = %d\n", (int) getpid(), x);
		x = 101;
		printf("Changed x = %d inside child process\n", x);
	}
	else // parent goes down this path (main)
	{
		printf("hello, I am parent of %d (pid:%d) and x = %d\n", rc, (int) getpid(), x);
		int x = 102;
		printf("Changed x = %d inside parent process\n", x); 
	}
	return 0;

	/*
	 * 1. Wert ist 100
	 * 2. Wert bleibt bei Anfang des Aufruf eines Prozess 100,
	 *    wird im jeweiligen Prozess dann aber geaendert.
	 */
}

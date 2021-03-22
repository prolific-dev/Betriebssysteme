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
		// now exec "/bin/ls"...
		char *myargs[2];
		myargs[0] = strdup("/bin/ls"); // program: "wc" (word count)
		myargs[2] = NULL; // marks end of array
		execvp(myargs[0], myargs); // runs word count
	}
	else // parent goes down with this path (main)
	{
		int rc_wait = wait(NULL);
	}
	return 0;
}

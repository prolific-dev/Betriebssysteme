#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argv, char* argc[])
{

	int r1 = fork();
	int status;	
	int status2;
	int io[2];
	char* msg = "Hello i am Child 1";
	char buf[sizeof(msg+1)];
	if(pipe(io) == -1)
	{
		printf("PIPE ERROR");
	}

	if(r1 < 0)
	{
		fprintf(stderr, "Fork failed\n");
		exit(1);
	}
	else if(r1 == 0)
	{
		//CHILDREN 1
		
		close(io[0]);
      		write(io[1], msg, sizeof(msg)+1);
		close(io[1]);
		exit(0);
	}
	else 
	{
		int r2 = fork();
		if(r2 < 0)
		{
			fprintf(stderr, "Fork failed\n");
			exit(1);
		}
		else if(r2 == 0)
		{
			//CHILDREN 2

			waitpid(r1, &status, WUNTRACED);
			close(io[1]);
			read(io[0], &buf, sizeof(msg)+1);
			close(io[0]);


        		printf("%s\n",buf);

			exit(0);
		}
		exit(0);
	}


}

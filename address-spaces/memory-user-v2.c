#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

long diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if(end.tv_nsec < start.tv_nsec)
	{
		temp.tv_sec = end.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	}
	else
	{
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp.tv_nsec + temp.tv_nsec * 1000000000;
}

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr, "Missing argument\n");
		exit(-1);
	}
	size_t mem = (size_t)((unsigned long)atol(argv[1])*1000000);
	int time = atoi(argv[2]);

	if(mem <= 0 || time <= 0)
	{
		fprintf(stderr, "Invalid Argument. Try again.");
		exit(1);
	}

	char *p = (char*)calloc(mem, sizeof(char));
	printf("Memory(MB): %d		time(seconds): %d		PID: %d\n", (int)round(mem/1000000), time, getpid());
	if(p == NULL)
	{
		fprintf(stderr, "MALLOC ERROR\n");
		exit(1);
	}
	sleep(time);
	free(p);
}

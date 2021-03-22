#include <stdio.h>
#include <unistd.h>
#include <time.h>
#define BILLION 1000000000L

int main(int argc, char *argv[])
{
	struct timespec start, end;
	long sum;
	double avg;
	int i;

	for(i = 0; i < 1000000; i++)
	{
		clock_gettime(CLOCK_BOOTTIME, &start);
		read(0, NULL, 0);
		clock_gettime(CLOCK_BOOTTIME, &end);

		long difference = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
		sum += difference;
	}

	avg = sum / (float)1000000;
	printf("Durschn. Zeit für Syscall betraegt %f Nanosekunden.\n", avg);
	return 0;
}

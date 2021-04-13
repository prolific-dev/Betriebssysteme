#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Put in an argument.");
		exit(1);
	}

	printf("Running PID: %d\n", getpid());

	int mb;
	int *array;
	int array_size;

	mb = atoi(argv[1]);
	array = (int*) malloc(1024 * 1024 * mb * sizeof(int)); // 1024 * 1024 for MB
	array_size = sizeof(array) / sizeof(int);

	int i;

	for(i = 0; i < array_size; ++i)
	{
		array[i] = 1;
		if(i == array_size - 1)
		{
			i = 0;
		}
	}

	free(array);
}

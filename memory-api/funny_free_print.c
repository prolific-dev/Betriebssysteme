#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int size = 10;
	int *data = (int*) malloc(size * sizeof(int));
	data[0] = 1;
	free((int*) data[5]);
	printf("%d\n", data[0]);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int size = 1;
	int *data = (int*) malloc(size * sizeof(int));
	data[0] = 1;
	free(data);
	printf("%d\n", data[0]);
	return 0;
}

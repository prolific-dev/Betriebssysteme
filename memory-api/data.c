#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int size = 100;
	int *data = (int*) malloc(size * sizeof(int));
	data[100] = 0;
	free(data);
	return 0;
}

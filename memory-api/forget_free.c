#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int size = 10;
	int *array;
	array = (int*) malloc(size*sizeof(int));
	printf("Allocated memory but forgot to free it. Oh no!\n");
	return 0;
}

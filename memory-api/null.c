#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int *x = (int*) malloc(sizeof(int));
	printf("%d before NULL initialization\n", *x);
	x = NULL;
	printf("%d after NULL initialization\n", *x);
	free(x);
	return 0;
}

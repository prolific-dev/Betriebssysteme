#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#define ROLLS 10000
#define ERRORROOM 0.3


int compare( const void* a, const void* b)
{
     long a_t = ((long) a);
     long b_t = ((long) b);

     if ( a_t == b_t ) return 0;
     else if ( a_t < b_t ) return -1;
     else return 1;
}
long diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec - start.tv_nsec) < 0)
	{
            temp.tv_sec = end.tv_sec - start.tv_sec - 1;
            temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }else
	{
            temp.tv_sec = end.tv_sec - start.tv_sec;
            temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
	return temp.tv_nsec;
}

int main(int argv, char *argc[])
{
	int anzahlwerte = ROLLS - (ROLLS*ERRORROOM + ROLLS*ERRORROOM);
	long time, nsec, err;
	long messwerte[ROLLS], time_f[anzahlwerte];
	struct timespec start, stop, err_t;
	int io[2];

	for(int i = 0; i < ROLLS; i++)
	{
		if(pipe(io) < 0)
			exit(1);

		int pid = fork();
		if(pid < 0)
			exit(1);	
		if(pid == 0)
		{	
			close(io[0]);		
			clock_gettime(CLOCK_REALTIME, &start);
			write(io[1], &start, sizeof(struct timespec));
			exit(0);
		}
		wait(NULL);		
		clock_gettime(CLOCK_REALTIME, &stop);
		close(io[1]);
		
		read(io[0], &start, sizeof(struct timespec));
		close(io[0]);		
		messwerte[i] = diff(start, stop);	
	}

	//ERASE ERROR TIMES
	if(pipe(io) < 0)
			exit(1);
	clock_gettime(CLOCK_REALTIME, &err_t);
	for(int k = 0; k < anzahlwerte; k++)
	{
		clock_gettime(CLOCK_REALTIME, &start);
		write(io[1], &err_t, sizeof(struct timespec));
		clock_gettime(CLOCK_REALTIME, &stop);
		read(io[0], &err_t, sizeof(struct timespec));
		err += diff(start,stop);
	}
	printf("%ld\n",err);

	//SORT
	qsort(messwerte, ROLLS, sizeof(long), compare);
	memcpy(time_f, &messwerte[(int) round(ROLLS*ERRORROOM)], sizeof(time_f)); 

	for(int j = 0; j < anzahlwerte; j++)
	{
		nsec += time_f[j];
	}

	time = (nsec-err) / anzahlwerte;
	printf("Measured Time at %d ROLLS : %ld nsec\n",ROLLS, time);
}

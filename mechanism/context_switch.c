#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sched.h>
#include <math.h>
#define ROLLS 100000
#define ERRORROOM 0.2


int compare( const void* a, const void* b)
{
     long a_t = *((long*) a);
     long b_t = *((long*) b);

     if ( a_t == b_t ) return 0;
     else if ( a_t < b_t ) return -1;
     else return 1;
}
long diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if (end.tv_nsec < start.tv_nsec)
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


int main(int argv, char* argc[])
{
	int anzahlwerte = ROLLS - (ROLLS*ERRORROOM + ROLLS*ERRORROOM);
	long time, nsec, err;
	long messwerte[ROLLS], time_f[anzahlwerte];
	struct timespec start, stop, err_t;
	int io[2], io2[2];

	if(pipe(io) < 0 || pipe(io2) < 0)
		exit(1);
	
	cpu_set_t set;
  	CPU_ZERO(&set);
 	CPU_SET(0, &set);

	
	int pid = fork();
	if(pid < 0)
		exit(1);	
	if(pid == 0)
	{	
		if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1)
		{
			printf("SCHED_ERROR exit process\n");        		
			exit(1);		
		}
      		for (int i = 0; i < ROLLS; i++) 
		{
          		read(io[0], NULL, 0);
          		write(io2[1], NULL, 0);
      		}
		exit(0);
	}
	if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1)
	{
		printf("SCHED_ERROR exit process\n");        		
		exit(1);		
	}

      	for (size_t i = 0; i < ROLLS; i++) {
		clock_gettime(CLOCK_REALTIME,&start);
        	write(io[1], NULL, 0);
        	read(io2[0], NULL, 0);
		clock_gettime(CLOCK_REALTIME,&stop);
		messwerte[i] = diff(start, stop)/2;
      	}
	//OVERHEAD
	clock_gettime(CLOCK_REALTIME,&start);
	for (size_t i = 0; i < ROLLS; i++) {
      	}
	for (size_t i = 0; i < ROLLS; i++) {
      	}
  	clock_gettime(CLOCK_REALTIME,&stop);
	err = diff(start, stop);
	//SORT
	qsort(messwerte, ROLLS, sizeof(long), compare);
	memcpy(time_f, &messwerte[(int) round(ROLLS*ERRORROOM)], sizeof(time_f)); 

	for(int j = 0; j < anzahlwerte; j++)
	{
		nsec += time_f[j];
	}

	time = (nsec - err) / anzahlwerte;
	printf("Measured Time at %d ROLLS : %ld nsec\n",ROLLS, time);
}

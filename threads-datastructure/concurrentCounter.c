//
//  concurrentCounter.c
//
//
//  Created by Dennis Da Silva, Julian Sabo, Gruppe 5
//
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <sched.h>
#include <stdint.h>
#include <pthread.h>
//#include <sys/sysinfo.h>

int numtrials = 10;
int numthreads = 10;

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
    return temp.tv_nsec+temp.tv_sec*1000000000;
}
long errortime(int trials){
    struct timespec start,stop;
    long errtime = 0;
    for(int k = 0; k < trials; k++)
    {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        clock_gettime(CLOCK_MONOTONIC_RAW, &stop);
        errtime += diff(start,stop);
    }
    return errtime/trials;
}

typedef struct __counter_t {
    int value;
    pthread_mutex_t lock;
} counter_t;
counter_t counter;

void init(counter_t *c) {
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}
void increment(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}
void decrement(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value--;
    pthread_mutex_unlock(&c->lock);
}
int get(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    int rc = c->value;
    pthread_mutex_unlock(&c->lock);
    return rc;
}

void* timemeasure() {
    long loopTime;
    struct timespec start, stop;
    long *returnValue = (long *) malloc(sizeof(long));

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (int i = 0; i < numtrials; ++i){}
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);

    loopTime = diff(start, stop);

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (int i = 0; i < numtrials; ++i)
    {
        increment(&counter);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);

    *returnValue = diff(start, stop) - loopTime / numtrials;

    return returnValue;

}

int main(int argc, char* argv[])
{


    if(argc != 3){
        printf("invalid arguments\n");
        exit(1);
    }

    numtrials = atoi(argv[1]);
    numthreads = atoi(argv[2]);

    long* retVal[numtrials];
    counter_t counter;
    init(&counter);

    pthread_t threads[numthreads];

    for (int i = 0; i < numthreads; i++) {
        pthread_create(&threads[i], NULL, timemeasure, NULL);
    }

    for (int i = 0; i < numthreads; ++i)
    {
        pthread_join(threads[i], (void**) &retVal[i]);
    }

    long sum = 0, finalTime = 0;

    for (int i = 0; i < numtrials; ++i)
    {
        sum += *retVal[i];
    }

    finalTime = sum / numtrials;

    printf("%d threads used. Time %lu ns.\n", numthreads, finalTime);
    //printf("This system has %d processors configured and "     "%d processors available.\n", get_nprocs_conf(), get_nprocs());
}

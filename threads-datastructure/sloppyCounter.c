//
//  sloppyCounter.c
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

#define NUMCPUS 4

int numtrials = 0;
int numthreads = 0;
int threshold = 0;

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
    int global; //global count
    pthread_mutex_t glock; //global lock
    int local [NUMCPUS]; // per-CPU count
    pthread_mutex_t llock[NUMCPUS]; //... and locks
    int threshold; //update frequency
} counter_t;



typedef struct __myarg_t
{
    counter_t *counter;
    int amt;
    int numtrials;
}   myarg_t;

//init: record threshold, init locks, init values of all local counts and global count
void init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);
    int i;
    for (i=0;i<NUMCPUS;i++){
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}

// update: usually, just grab local lock and update local amount; once local count has risen ’threshold’, grab global lock and transfer local values to it
void update(counter_t *c, int threadID, int amt) {
    int cpu = threadID % NUMCPUS;
    pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amt;
    if (c->local[cpu] >= c->threshold) {
        // transfer to global (assumes amt>0)
        pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];
        pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;
    }
    pthread_mutex_unlock(&c->llock[cpu]);
}

// get: just return global amount (approximate)
int get(counter_t *c) {
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val; // only approximate!
}

void* timemeasure(void* args) {
    long loopTime;
    struct timespec start, stop;
    long *returnValue = (long *) malloc(sizeof(long));

    myarg_t *measureArgs = (myarg_t *) args;
    pthread_t tid = pthread_self();

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (int i = 0; i < measureArgs->numtrials; ++i){}
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);

    loopTime = diff(start, stop);

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (int i = 0; i < measureArgs->numtrials; ++i)
    {
        update(measureArgs->counter, tid, measureArgs->amt);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);

    returnValue = (diff(start, stop) - loopTime) / measureArgs->numtrials;

    return returnValue;

}

int main(int argc, char* argv[])
{


    if(argc != 4){
        printf("invalid arguments\n");
        exit(1);
    }

    numtrials = atoi(argv[1]);
    numthreads = atoi(argv[2]);
    threshold = atoi(argv[3]);



    long retVal[numthreads];

    counter_t *counter;

    counter = (counter_t*) calloc(1, sizeof(counter_t));

    init(counter, threshold);

    pthread_t threads[numthreads];

    for (int i = 0; i < numthreads; i++) {
            myarg_t args;
            args.counter = counter;
            args.amt = 1;
            args.numtrials = numtrials;
            pthread_create(&threads[i], NULL, &timemeasure, &args);
    }

    for (int i = 0; i < numthreads; ++i)
    {
        pthread_join(threads[i], (void**) &retVal[i]);
    }

    long sum = 0, finalTime;

    for (int i = 0; i < numthreads; ++i)
    {
        sum += retVal[i];
    }

    finalTime = (sum / numthreads) / NUMCPUS;

    printf("%d threads used. Time %lu ns.\n", numthreads, finalTime);
    //printf("This system has %d processors configured and "     "%d processors available.\n", get_nprocs_conf(), get_nprocs());
}

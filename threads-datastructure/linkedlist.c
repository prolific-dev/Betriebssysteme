//
//  linkedList.c
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
#include <sys/sysinfo.h>


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

// basic node structure
typedef struct __node_t {
    int key;
    struct __node_t *next;
} node_t;

// basic list structure (one used per list)
typedef struct __list_t {
    node_t *head;
    pthread_mutex_t lock;
} list_t;

typedef struct __myarg_t
{
    list_t *list;
    int numtrials;
}   myarg_t;


void List_Init(list_t *L) {
    L->head = NULL;
    pthread_mutex_init(&L->lock, NULL);
}

void List_Insert(list_t *L, int key) {
    // synchronization not needed
    node_t *new = malloc(sizeof(node_t));
    if (new == NULL) {
        perror("malloc");
        return;
    }
    new->key = key;
    // just lock critical section
    pthread_mutex_lock(&L->lock);
    new->next = L->head;
    L->head   = new;
    pthread_mutex_unlock(&L->lock);
}



int List_Lookup(list_t *L, int key) {
    int rv = -1;
    pthread_mutex_lock(&L->lock);
    node_t *curr = L->head;
    while (curr) {
        if (curr->key == key) {
            rv = 0;
            break;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&L->lock);
    return rv; // now both success and failure
}

void* timemeasure(void* args) {
    long loopTime;
    struct timespec start, stop;
    long *returnValue = (long *) malloc(sizeof(long));

    myarg_t *measureArgs = (myarg_t *) args;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (int i = 0; i < measureArgs->numtrials; ++i){}
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);

    loopTime = diff(start, stop);

    //long sum = 0;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (int i = 0; i < measureArgs->numtrials; ++i)
    {
        List_Insert(measureArgs->list, i);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);


    returnValue = (diff(start, stop) - loopTime) / measureArgs->numtrials;

    return returnValue;

}


int main(int argc, char* argv[])
{
    if(argc != 3){
        printf("invalid arguments\n");
        exit(1);
    }
    int numtrials = atoi(argv[1]);
    int numthreads = atoi(argv[2]);

    long retVal[numthreads];

    list_t *list = (list_t*) calloc(1, sizeof(list_t));

    List_Init(list);

    pthread_t threads[numthreads];

    myarg_t args;

    args.list = list;
    args.numtrials = numtrials;

    for (int i = 0; i < numthreads; i++) {

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

    finalTime = sum / numthreads;


    printf("%d threads used. Time %lu ns.\n", numthreads, finalTime);
}

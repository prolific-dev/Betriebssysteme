#include <stdio.h>

#include "common_threads.h"
#include <pthread.h>

int balance = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* worker(void* arg) {
    
    //pthread_mutex_lock(&lock);
    balance++; // unprotected access 
    //pthread_mutex_unlock(&lock);
    
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    Pthread_create(&p, NULL, worker, NULL);
    //pthread_mutex_lock(&lock);
    balance++; // unprotected access 
    //pthread_mutex_unlock(&lock);
    Pthread_join(p, NULL);
    return 0;
}

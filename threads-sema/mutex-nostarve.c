#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

// Solution on https://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf

typedef struct __ns_mutex_t {
  int room1;
  int room2;
  sem_t mutex;
  sem_t t1;
  sem_t t2;

} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
  m->room1 = 0;
  m->room2 = 0;
  sem_init(&m->t1, 0, 1);
  sem_init(&m->t2, 0, 0);
  sem_init(&m->mutex, 0, 1);
}

void ns_mutex_acquire(ns_mutex_t *m) {
  sem_wait(&m->mutex);
  m->room1++;
  sem_post(&m->mutex);

  sem_wait(&m->t1);
  m->room2++;
  sem_wait(&m->mutex);
  m->room1--;
  if (m->room1 == 0) {
    sem_post(&m->mutex);
    sem_post(&m->t1);
  }
}

void ns_mutex_release(ns_mutex_t *m) {
  sem_wait(&m->t2);
  m->room2--;

  if (m->room2 == 0) {
    sem_post(&m->t1);
  } else {
    sem_post(&m->t2);
  }
}


//
// XXX: don't change below here (just run it!)
//

ns_mutex_t lock;

void *worker(void *arg) {
  printf("Before\n");
  ns_mutex_acquire(&lock);
  printf("After\n");
  ns_mutex_release(&lock);
  return NULL;
}

// similar to barrier.c
int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    printf("parent: begin\n");
    ns_mutex_init(&lock);

    int i;
    for (i = 0; i < num_threads; i++) {
      pthread_create(&p[i], NULL, worker, NULL);
    }
    for (i = 0; i < num_threads; i++) {
      pthread_join(p[i], NULL);
    }


    printf("parent: end\n");
    return 0;
}

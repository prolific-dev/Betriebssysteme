#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common_threads.h"

//
// Your code goes in the structure and functions below
//

// Nach dem alle Reader gelaufen sind, laufen alle Writer.
// Und umgekehrt. Es soll sichergestellt werden dass nicht alle Reader/Writer
// erneut laufen.
// Eine zweite Möglichkeit wäre einen threshold einzubauen, der
// sicherstellt dass nur eine gewisse Anzahl jeweiliger Reader durchläuft,
// anschließend zwingend aber den Writer aufruft. Oder andersrum.
// Die dritte Möglichkeit ist es gewisse Flags einzubauen, welche markieren
// sollen ob ein Writer oder Reader zuvor gelaufen ist, damit man anschließend
// das jeweilige Gegenstück aufrufen kann.
// Eine vierte Variante wäre beim Aufruf der jeweiligen acquire-Methode das
// writelock zu halten und nach jeder release-Methode wieder freizugeben,
// so dass es nicht wie bei der Standard-RW-Implementierung bei den Readern
// für eine bestimmte Zeit gehalten wird.

typedef struct __rwlock_t {
  sem_t lock;
  sem_t writelock;
  int readers;
} rwlock_t;


void rwlock_init(rwlock_t *rw) {
  rw->readers = 0;
  sem_init(&rw->lock, 0, 1);
  sem_init(&rw->writelock, 0, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
  sem_wait(&rw->lock);
  rw->readers++;


  if (rw->readers == 1)
  {
    sem_wait(&rw->writelock);
  }
  sem_post(&rw->lock);
}

void rwlock_release_readlock(rwlock_t *rw) {
  sem_wait(&rw->lock);
  rw->readers--;
  if (rw->readers == 0)
  {
    sem_post(&rw->writelock);

  }
  sem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
  sem_wait(&rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw) {
  sem_post(&rw->writelock);
}

//
// Don't change the code below (just use it!)
//

int loops;
int value = 0;

rwlock_t lock;

void *reader(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_readlock(&lock);
	printf("read %d\n", value);
	rwlock_release_readlock(&lock);
    }
    return NULL;
}

void *writer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_writelock(&lock);
	value++;
	printf("write %d\n", value);
	rwlock_release_writelock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 4);
    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);
    loops = atoi(argv[3]);

    pthread_t pr[num_readers], pw[num_writers];

    rwlock_init(&lock);

    printf("begin\n");

    int i;
    for (i = 0; i < num_readers; i++)
	Pthread_create(&pr[i], NULL, reader, NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_create(&pw[i], NULL, writer, NULL);

    for (i = 0; i < num_readers; i++)
	Pthread_join(pr[i], NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_join(pw[i], NULL);

    printf("end: value %d\n", value);

    return 0;
}

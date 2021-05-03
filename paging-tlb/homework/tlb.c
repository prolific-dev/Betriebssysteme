#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define BILLION 1000000000L

int main (int argc, char * argv[])
{
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) < 0) {
       fprintf(stderr, "Fehler bei der CPU-Setzung\n");
       exit(1);
    }
    
    int opt;
    int nr_of_pages = 0, setpages;
    int trials = 0, settrials;
    setpages = settrials = 0;
    
    while ((opt = getopt(argc, argv, "hp:t:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf("Options:\n-p NUMBER PAGES\n  number of pages to touch\n");
                printf("-t TRIALS\n   number of trials\n");
                break;
            case 'p':
                nr_of_pages = atoi(optarg);
                setpages = 1;
                break;
            case 't':
                trials = atoi(optarg);
                settrials = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-p <number of pages>] [-t <number of trials>]\n", argv[0]);
                exit(1);
        }
    }
    if ((1 != setpages) || (1 != settrials))
    {
        fprintf(stderr, "Usage: %s [-p <number of pages>] [-t <number of trials>]\n", argv[0]);
        exit(1);
    }
    
    if (nr_of_pages <= 0 || trials <= 0) {
        fprintf(stderr, "Amount of pages and trials need to be greater zero\n");
        exit(1);
    }
    int pagesize = getpagesize();
    // Seite 212 im Buch
    int jump = pagesize / sizeof(int);
    
    int * a = (int *) calloc((nr_of_pages * pagesize), sizeof(int));
    //int * a = (int *) malloc(nr_of_pages * pagesize);
    int i;
    
    struct timespec start, stop;

    double elapsed = 0;

    for (int t = 0; t < trials; t++) { // loop through the loop as often as number of trials
        for (i = 0; i < nr_of_pages * jump; i += jump) { // loop through the array once
            clock_gettime(CLOCK_REALTIME, &start);
            a[i] += 1;
            clock_gettime(CLOCK_REALTIME, &stop);
            elapsed += (BILLION * (stop.tv_sec - start.tv_sec) + stop.tv_nsec - start.tv_nsec);
         }
    }
    
    elapsed /= (trials * nr_of_pages);

    printf("%.0f\n", elapsed); // This is nanoseconds
    
    free(a);
    return 0;
}

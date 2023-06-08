#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define NUM_THREADS 8
#define MAXIMUM_NUMBER 1000


pthread_mutex_t lock;
int number = 1;

void *count_to_max(void *threadid)
{

    long tid;
    int countNumber;
    tid = ((long)threadid);

    while (number <= MAXIMUM_NUMBER)
    {
        pthread_mutex_lock(&lock);
        countNumber = number;
        number++;
        printf ("Thread %d finished counting %d\n ", tid, countNumber);
        pthread_mutex_unlock(&lock);

        for (int i = 1; i < countNumber; i++)
        {
            for (int j = 1; j < countNumber; j++)
            {
                for (int k = 1; k < countNumber; k++)
                {

                }
            }
        }
    }
}


int main (int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    // int rc;
    clock_t t;
    double time_taken;
    int threadArray[NUM_THREADS];
    // time_t start, end;

    pthread_mutex_init(&lock, NULL);

    // time(&start);
    // t = clock();

    for(int i=0; i<NUM_THREADS; i++) {
        threadArray[i] = i;
        int rc = pthread_create(&threads[i], NULL, count_to_max, (void *)&threadArray[i]);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    t = clock()-t;
    // time(&end);
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    // time_taken = (double) (end - start);

    printf("This took %f seconds to execute \n", time_taken);
    pthread_mutex_destroy(&lock);

    pthread_exit(NULL);
}
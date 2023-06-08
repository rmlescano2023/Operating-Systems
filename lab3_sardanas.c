// by Reah Mae Sardañas 

#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 2
#define COUNT_TARGET 1000

pthread_mutex_t mutex;                                                      // Mutex varible for the thread synchronization
int count = 1;                                                              // global count variable initialized to 1

void* countNumbers(void* thread_id);                                        //function prototype

int main() {
    pthread_t threads[NUM_THREADS];                                         // array for thread ID

    pthread_mutex_init(&mutex, NULL);                                       // initiallizing mutex to NULL

    clock_t start = clock();                                                // Start the timer

    for (long t = 0; t < NUM_THREADS; t++) {                                // loop for creating multiple threads
        pthread_create(&threads[t], NULL, countNumbers, (void*)t);
    }

    for (long t = 0; t < NUM_THREADS; t++) {                                // loop for waiting for each threads to finish to ensure sychronization
        pthread_join(threads[t], NULL);
    }

    clock_t end = clock();                                                  // Stop the timer

    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;        // calculate the utilized CPU time
    printf("\nEXECUTION TIME: %f seconds\n\n", cpu_time_used);

    pthread_mutex_destroy(&mutex);                                          // destroy the mutex

    return 0;
}

void* countNumbers(void* thread_id) {                                       // function for counting operation
    long tid = (long)thread_id;                                             // casting thread id to and store to tid

    while (1) {
        pthread_mutex_lock(&mutex);                                         // ensure mutex lock
        int num = count++;                                                  // incrementing count and store as num
        pthread_mutex_unlock(&mutex);                                       // releasing or unlocking mutex

        if (num > COUNT_TARGET) {                                           // exit once counting exceeds COUNt_TARGET
            break;
        }

        printf("Thread %ld finished counting %d\n", tid, num);              // printing output

        struct timespec delay;                                              // specify delay using nanoseconds
        delay.tv_sec = 0;                                                   // no delay in seconds
        delay.tv_nsec = num * 1000000;                                      // Delay in nanoseconds 

        nanosleep(&delay, NULL);                                            // pause thread execution for the specified duration.
    }

    pthread_exit(NULL);                                                     // exits the thread
}

// Reah Mae Sardañas


#include <stdio.h>                                                               // header files
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>


#define BUFFER_SIZE 5                                                            // size of the buffer

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;                               // mutex to protect the shared count variable; for mutual exclusion
pthread_cond_t buffer_not_full = PTHREAD_COND_INITIALIZER;                       // when buffer is not full
pthread_cond_t buffer_not_empty = PTHREAD_COND_INITIALIZER;                      // when buffer is not empty

int buffer[BUFFER_SIZE];
int in = 0;                                                                     // insertion indices
int out = 0;                                                                    // removal indices
int count = 0;                                                                  // current number of units in the buffer

const int max_units = BUFFER_SIZE * 10;                                         // initialize max_units with BUFFER_SIZE *10

void *producer(void *param);                                                    // function prototypes for producer
void *consumer(void *param);                                                    // function prototypes for consumer

void printBuffer();                                                             // function prototypes for buffer printing

// make main accept two args which is num of consumers and producers
int main(int argc, char *argv[]){

    if (argc != 3){                                                             // if there are no provided number of argument
        printf("Usage: ./lab4 <num of consumers> <num of producers>\n");
        return 1;                                                               // error
    }
    
    int num_producers = atoi(argv[1]);                                          // number of producer
    int num_consumers = atoi(argv[2]);                                          // number of consumer
    pthread_t consumers[num_consumers];                                         // array for holding the thread ID's of consumers
    pthread_t producers[num_producers];                                         // array for holding the thread ID's of producers

    for (int i = 0; i < num_producers; i++){                                    // loop for creating new thread for each producer
        pthread_create(&producers[i], NULL, producer, (void *)(intptr_t)i);
    }

    for (int i = 0; i < num_consumers; i++){                                    // loop for creating new thread for each producer
        pthread_create(&consumers[i], NULL, consumer, (void *)(intptr_t)i);
    }

    for (int i = 0; i < num_producers; i++){                                    // loop for waiting for all producer to complete
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < num_consumers; i++){                                    // loop for waiting for all consumer to complete
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);                                              // destroy the mutex
    pthread_cond_destroy(&buffer_not_full);                                     // destroy conditional variables 
    pthread_cond_destroy(&buffer_not_empty);                                    // destroy conditional variables 
    pthread_exit(NULL);                                                         // terminate the main thread
    return 0;                                                                   // successful excecution
}

void printBuffer(){                                                             // print buffer
    printf("Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++){
        if (buffer[i] == 0){
            printf("- ");
            continue;
        }
        printf("%d ", buffer[i]);
    }
    printf("] \tsize: %d\n\n", count);
}

void *consumer(void *param){                                                    // excecuted by consumer threads
    int id = (intptr_t)param;
    srand(id * (unsigned)time(NULL));                                           // seed random number generator used by the consumer thread
    int item;                                                                   // consumed items
    int units_removed;                                                          // number of uits removed
    while (1){                                                                  // infinite loop for consumer's behavoir
        pthread_mutex_lock(&mutex);                                             // ensure exclusive access to the shared variibles and buffer

        while (count == 0){                                                     // check if the buffer is empty
            printf("Buffer is empty, waiting for a slot to be produced\n");
            pthread_cond_wait(&buffer_not_empty, &mutex);                       // wait for buffer to be not empty; put to sleep
        }

        while (buffer[out] == 0){                                               // skips any empty slots
            out = (out + 1) % BUFFER_SIZE;                                      // retrieves item value
        }

        item = buffer[out];
        units_removed = (rand() % item + 1);                                    // random number between 1 and the value of 'item'
        buffer[out] = item - units_removed;                                     // updates buffer 

        count -= units_removed;                                                 // updates the count

        printf("Consumer %d \tconsumed %d \tat %d\n", id, units_removed, out);  // print output
        out = (out + 1) % BUFFER_SIZE;                                          // points to the next position in the buffer

        pthread_cond_signal(&buffer_not_full);                                  // signal that buffer is not full

        printBuffer();                                                          // print current state of the buffer

        pthread_mutex_unlock(&mutex);                                           // releasing the mutex lock

        sleep(rand() % 2 + 1);                                                  // Sleep for 1-2 seconds
    }
    return NULL;
}

void *producer(void *param){                                                    // excecuted by producer threads

    int id = (intptr_t)param;                                                   // seed random number generator used by the producer thread
    srand(id * (unsigned)time(NULL));                           
    int units;
    while (1){                                                                  // infinite loop for consumer's behavoir
        pthread_mutex_lock(&mutex);                                             // ensure exclusive access to the shared variibles and buffer

        while (count == max_units){                                             // continue to loop as long as the count == max_units
            printf("Buffer is full, waiting for a slot to be consumed\n");
            pthread_cond_wait(&buffer_not_full, &mutex);                        // wait for buffer to be not full
        }

        while ((buffer[in] == 10)){                                             // search for next available slot in the buffer; skips filled to capacity
            in = (in + 1) % BUFFER_SIZE;
        }

        units = rand() % (10 - buffer[in]) + 1;                                 // Generate a random n units from 1-10

        count += units;

        buffer[in] += units;

        printf("Producer %d \tproduced %d \tat %d\n", id, units, in);           // prints indication message
        in = (in + 1) % BUFFER_SIZE;                                            // point to the next position in the buffer
        pthread_cond_signal(&buffer_not_empty);                                 // signal that buffer is not empty

        printBuffer();                                                          // print current state of the buffer

        pthread_mutex_unlock(&mutex);                                           // releasing the mutex lock

        sleep(rand() % 2 + 1);                                                  // Sleep for 1-2 seconds
    }
    return NULL;
}

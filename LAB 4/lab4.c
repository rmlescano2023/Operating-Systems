// to compile: gcc -pthread -o lab4 lab4.c
// to run: lab4 [producer] [consumer]

// empty means 0 items
// full means 5 items

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5 // set buffer size to 5

int buffer[BUFFER_SIZE];

int count = 0;

// initialize mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_empty = PTHREAD_COND_INITIALIZER;      // condition variable
pthread_cond_t buffer_full = PTHREAD_COND_INITIALIZER;       // condition variable

void *producer(void* arg) {
    int producer_id = *(int*)arg;

    while (1) {
    
        pthread_mutex_lock(&mutex); // lock mutex

        // if buffer full, producer waits until a slot becomes empty
        while (count == BUFFER_SIZE) { 
            printf("Producer %d: Buffer full, waiting for a slot to be emptied.\n", producer_id);
            pthread_cond_wait(&buffer_empty, &mutex);   // wait until empty > 0; if full == 5, then empty == 0
        }

        int item = rand() % 10 + 1; // random number produce
        buffer[count++] = item; // produce item in the buffer

        printf("Producer %d: produced %d at %d\n", producer_id, item, count-1);

        // print buffer
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d ", buffer[i]);
        }
        printf("\n");

        pthread_cond_signal(&buffer_full);      // after producing, producer signals to consumer that there is an item in the buffer
        pthread_mutex_unlock(&mutex);           // release lock

        sleep(rand() % 2 + 1);                  // producer sleeps 1-2 seconds
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int consumer_id = *(int*)arg;
    free(arg);

    while (1) {
        
        pthread_mutex_lock(&mutex); // lock mutex

        // if buffer empty, consumer waits until a slot has been filled
        while (count == 0) {
            printf("Consumer %d: Buffer empty, waiting for a slot to be filled.\n", consumer_id);
            pthread_cond_wait(&buffer_full, &mutex);    // wait for full > 0; if empty == 5, then full == 0
        }

        int slot_index = rand() % count;    // generate a random slot to consume
        int units = buffer[slot_index] ;    // store value of the slot

        // consume a random number between 1 to n
        int item_consumed = rand() % units + 1;
        
        printf("Consumer %d consumed %d at %d\n", consumer_id, item_consumed, slot_index);

        // update consumed buffer
        buffer[slot_index] -= item_consumed;

        //print buffer
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d ", buffer[i]);
        }
        printf("\n");

        // if a slot is empty, move items to the left
        if(buffer[slot_index]==0){
            for(int i = slot_index; i < count-1; i++){
                buffer[i]=buffer[i+1];
            }
            count--; // counter
        }

        pthread_cond_signal(&buffer_empty);     // after consuming, consumer signals to producer that there is an empty slot in the buffer
        pthread_mutex_unlock(&mutex);           // Release lock

        sleep(rand() % 2 + 1);                  // consumer sleeps 1-2 seconds
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./program <num_producers> <num_consumers>\n");
        return 1;
    }

    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);

    srand(time(NULL)); // initialize random generator

    // initialize arrays for threads
    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

    // create threads
    for (int i = 0; i < num_producers; i++) {
        int* producer_id = malloc(sizeof(int));
        *producer_id = i;
        pthread_create(&producers[i], NULL, producer, producer_id);
    }

    for (int i = 0; i < num_consumers; i++) {
        int* consumer_id = malloc(sizeof(int));
        *consumer_id = i;
        pthread_create(&consumers[i], NULL, consumer, consumer_id);
    }

    // join threads
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    return 0;
}

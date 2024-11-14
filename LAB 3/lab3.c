#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#define NUM_THREADS 2
#define MAXIMUM_NUMBER 1000

// Mutex -  allows only one thread at a time to access a shared resource or critical section, preventing data races and ensuring data consistency.
pthread_mutex_t count_mutex; // create a mutex

int counter = 0;  // global variable

void *count_to_1000(void *t)
{
   int number = 0;
   long tid;
   tid = (long)t;

   while (counter <= MAXIMUM_NUMBER){ 

      pthread_mutex_lock(&count_mutex);      // acquire the mutex

      number = counter;    // assign the global variable "counter" to the local variable to get its current value before it is incremented
      counter++;           // increment the counter

      pthread_mutex_unlock(&count_mutex);    // release the mutex lock, giving way for other threads

      for(int i=0; i < number; i++){
         for(int j=0; j < number; j++){
            for(int k=0; k < number; k++){
                
            }
         }
      }

      printf("Thread %ld finished counting %d. \n", tid, number);

   }
   
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   pthread_t thread[NUM_THREADS];
   pthread_attr_t attr;
   int rc;
   long t;
   void *status;
   clock_t time;
   double time_taken;
   pthread_mutex_init(&count_mutex, NULL); // initialize mutex

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   time = clock();

   for(t=0; t<NUM_THREADS; t++) {
      //printf("Main: creating thread %ld\n", t);
      rc = pthread_create(&thread[t], &attr, count_to_1000, (void *)t); 
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   /* Free attribute and wait for the other threads */
   pthread_attr_destroy(&attr);
   for(t=0; t<NUM_THREADS; t++) {
      rc = pthread_join(thread[t], &status);
      if (rc) {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
      }
      printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
   }
   
   printf("Main: program completed. Exiting.\n");
   time = clock() -time;
   time_taken = ((double)time)/CLOCKS_PER_SEC;
   printf("This took %f seconds to execute.\n", time_taken);

   pthread_mutex_destroy(&count_mutex);
   pthread_exit(NULL);
}
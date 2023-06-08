#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXIMUM_NUMBER 1000

int main (int argc, char *argv[]) {

    int number = 0;

    clock_t t;
    double time_taken;

    t = clock_t;
    while (number <= MAXIMUM_NUMBER) {

        for (int i = 0; i < number; i ++) {
            for (int j = 0; j < number; j++) {
                for (int k = 0; k < number; k++) {
                    // insert code here
                }
            }
        }

        printf("Finished counting %d.\n", number);
        number++;

    }

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;    // in seconds

    printf("This took %f seconds to execute \n", time_taken);


}
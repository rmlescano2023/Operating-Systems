#include <stdio.h>
#include <stdlib.h>

void main(){

    printf("\nHellow World\n\n");

    int processes = 11;
    int numArray[11] = {1, 2, 3, 0, 5, 6, 0, 8, 9, 10, 11};

    for (int i =  0; i < processes; i++) {
        
        printf("i = %d\n", i);
        if (i == 5) {
            continue;
        }

    }

    printf("\n\n");


}
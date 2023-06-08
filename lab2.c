#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_COUNT 1000

// TO DO:
// 1. Finish Round Robin Scheduling
// 2. Fix SJF Scheduling
// 3. Add Idle Times in SJF
// 4. Do Priority Based Scheduling
// 5. Add Idle Times in Priority Based

//---------------------------------------------------------------------------------------------------------------------------------------------- FUNCTION DECLARATIONS
typedef struct {
    int processNum, priorityNum, arrivalTime, burstTime, remainingTime, idleTime, executeTime, processDone, terminatedTime, toRun, addedToQueue, finalTimer;
    char processName[100];
} PROCESS;

typedef struct {
    int pNum, pTimer;
} GANTT;

void FCFS(int processes);
void SJF(int processes);
void priorityBased(int processes);
void roundRobin(int processes);

int printReadyQueue(PROCESS readyQueue[MAX_COUNT], int readyQueueSize, PROCESS value[MAX_COUNT], int processes);
int roundRobinGanttChart(GANTT ganttChart[MAX_COUNT]);

int totalTime(PROCESS value[MAX_COUNT], int processes);
void bubbleSort(PROCESS value[MAX_COUNT], int processes);


// ---------------------------------------------------------------------------------------------------------------------------------------------- MAIN
int main() {

    //system("clear");

    int decision, processes;

    while(decision != 5) {

        decision = 0;

        printf("\n\t---- SCHEDULING ALGORITHMS ----\n");
        printf("\t1. FCFS\n\t2. SJF\n\t3. Priority Based\n\t4. Round Robin\n\t5. Exit\n\n\tEnter your choice: ");
        scanf("%d", &decision);

        // Error Handler
        if (decision < 1 || decision > 5) {      
            printf("\n+----------------------------------------------------+\
            \n| WARNING: Please choose a number among the choices. |\
            \n+----------------------------------------------------+\n");
            continue;
        }

        switch (decision) {
            case 1:
                //system("clear");

                printf("\n\t>> FCFS Scheduling <<\n\tSpecify the number of processes: ");
                scanf("%d", &processes);

                FCFS(processes);
                decision = 5;
                break;

            case 2:
                //system("clear");

                printf("\n\t>> SJF Scheduling <<\n\tSpecify the number of processes: ");
                scanf("%d", &processes);

                SJF(processes);
                decision = 5;
                break;

            case 3:
                //system("clear");

                printf("\n\t>> Priority Based Scheduling <<\n\tSpecify the number of processes: ");
                scanf("%d", &processes);

                priorityBased(processes);
                decision = 5;
                break;

            case 4:
                //system("clear");

                printf("\n\t>> Round Robin  Scheduling <<\n\tSpecify the number of processes: ");
                scanf("%d", &processes);

                roundRobin(processes);
                decision = 5;
                break;

            case 5:
                break;
        }

    }

    printf("\n");
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------- SCHEDULING ALGORITHMS
void FCFS(int processes) {      // OK

    PROCESS value[MAX_COUNT];    // created an array "value" of type "struct values" with a capacity of 10

    // SAMPLE DATA
    value[0].processNum = 1, value[0].arrivalTime = 0, value[0].burstTime = 30, value[0].processDone = 0;
    value[1].processNum = 2, value[1].arrivalTime = 28, value[1].burstTime = 154, value[1].processDone = 0;
    value[2].processNum = 3, value[2].arrivalTime = 26, value[2].burstTime = 127, value[2].processDone = 0;
    value[3].processNum = 4, value[3].arrivalTime = 200, value[3].burstTime = 300, value[3].processDone = 0;
    // SAMPLE DATA

    printf("\n\tEnter the Arrival Time and Burst Time for every process.\n\tFORMAT: Process #: [Arrival Time] [Burst Time]\n\n");
    for (int i = 0; i < processes; i++) {
        printf("\tProcess %d: %d %d\n", i + 1, value[i].arrivalTime, value[i].burstTime);
    }

    // Counter represents the clock in the OS
    // CurrentProcess represents the current process being executed
    int timer = 0;                      // the timer represents the clock cycle of the process executions
    bool currentProcess = false;

    int readyQueueSize = 0;
    PROCESS readyQueue[MAX_COUNT];

    // Imagine this as a click in the clock of OS
    while (true) {

        // Check if all processes are done
        bool allDone = true;
        for (int i = 0; i < processes; i++) {
            if (value[i].processDone == 0) {        // if there is a single process that is not yet done, set allDone to false then break the loop
                allDone = false;
                break;
            }
        }

        if (allDone == true) {      // if all the processes are done, break the while-loop
            value[0].finalTimer = timer;
            printf("Final timer is %d", value[0].finalTimer);
            break;
        }

        // Consume a process
        // LOGIC: Find a process that can be processed that has the lowest Arrival time, set it to be processed first
        int toProcessIndex = -1, smallestArrivalTime = 0;
        for (int i = 0; i < processes; i++) {
            if (value[i].arrivalTime <= timer && value[i].processDone == 0) {   // if the arrival time of the current process is less than or equal to the current tick of the timer
                
                if (smallestArrivalTime == 0) {                          
                    smallestArrivalTime = value[i].arrivalTime;         // Assigns the first smallest process
                    toProcessIndex = i;                                 // fetches the index of the current process to be processed
                }
                else if (value[i].arrivalTime < smallestArrivalTime) {  // for the succeeding processes, fetch the process that is smaller than the current smallest arrival time
                    smallestArrivalTime = value[i].arrivalTime;
                    toProcessIndex = i;
                }

            }
        }

        // LOGIC: Solve for the idle time of a process
        if (toProcessIndex != -1) {     // if a process arrives at a time when a different process is still executing, it is idle for a time being
            readyQueue[readyQueueSize].processNum = value[toProcessIndex].processNum;
            readyQueue[readyQueueSize].burstTime = value[toProcessIndex].burstTime;
            readyQueue[readyQueueSize].executeTime = timer;
            readyQueue[readyQueueSize].idleTime = timer - value[toProcessIndex].arrivalTime;
            readyQueueSize++;

            printf("\nP%d is being executed at timer %d - at index %d.\n", value[toProcessIndex].processNum, timer, toProcessIndex);
            timer += value[toProcessIndex].burstTime;           // here, we add the burst time to the timer to skip the entire execution time of that process
            
            value[toProcessIndex].processDone = 1;              // then, we set the state of that process to DONE
            value[toProcessIndex].terminatedTime = timer - value[toProcessIndex].arrivalTime;  // here, we set the time that the process finished executing
            
            printf("P%d finished executing at clock cycle %d\n", value[toProcessIndex].processNum, timer);
            printf("Turnaround time of P%d is %d\n\n", value[toProcessIndex].processNum, value[toProcessIndex].terminatedTime);
            continue;
        }

        timer++;    // timer increments for the whole clock cycle
    }

    printReadyQueue(readyQueue, readyQueueSize, value, processes);

}

void SJF(int processes) {

    PROCESS value[MAX_COUNT];    // created an array "value" of type "struct values" with a capacity of 10

    // SAMPLE DATA
    value[0].processNum = 1, value[0].arrivalTime = 15, value[0].burstTime = 100, value[0].processDone = 0;
    value[1].processNum = 2, value[1].arrivalTime = 13, value[1].burstTime = 27, value[1].processDone = 0;
    value[2].processNum = 3, value[2].arrivalTime = 22, value[2].burstTime = 57, value[2].processDone = 0;
    value[3].processNum = 4, value[3].arrivalTime = 90, value[3].burstTime = 39, value[3].processDone = 0;
    // SAMPLE DATA

    printf("\n\tEnter the Arrival Time and Burst Time for every process.\n\tFORMAT: Process #: [Arrival Time] [Burst Time]\n\n");
    for (int i = 0; i < processes; i++) {
        printf("\tProcess %d: %d %d\n", i + 1, value[i].arrivalTime, value[i].burstTime);
    }

    // Counter represents the clock in the OS
    // CurrentProcess represents the current process being executed
    int timer = 0;                      // the timer represents the clock cycle of the process executions
    bool currentProcess = false;

    int readyQueueSize = 0;
    PROCESS readyQueue[MAX_COUNT];

    // Imagine this as a click in the clock of OS
    while (true) {

        // Check if all processes are done
        bool allDone = true;
        for (int i = 0; i < processes; i++) {
            if (value[i].processDone == 0) {        // if there is a single process that is not yet done, set allDone to false then break the loop
                allDone = false;
                break;
            }
        }

        if (allDone == true) {      // if all the processes are done, break the while-loop
            value[0].finalTimer = timer;
            printf("Final timer is %d", value[0].finalTimer);
            break;
        }

        // Consume a process
        // LOGIC: Find a process that can be processed that has the lowest Burst time, set it to be processed first
        int toProcessIndex = -1, smallestBurstTime = 0;
        for (int i = 0; i < processes; i++) {
            if (value[i].arrivalTime <= timer && value[i].processDone == 0) {

                if (smallestBurstTime == 0) {                          
                    smallestBurstTime = value[i].burstTime;             // Assigns the first smallest process
                    toProcessIndex = i;                                 // fetches the index of the current process to be processed
                }
                else if (value[i].burstTime < smallestBurstTime) {      // for the succeeding processes, fetch the process that is smaller than the current smallest arrival time
                    smallestBurstTime = value[i].burstTime;
                    toProcessIndex = i;
                }

            }
        }


        // LOGIC: Solve for the idle time of a process
        if (toProcessIndex != -1) {
            readyQueue[readyQueueSize].processNum = value[toProcessIndex].processNum;
            readyQueue[readyQueueSize].burstTime = value[toProcessIndex].burstTime;
            readyQueue[readyQueueSize].executeTime = timer;
            readyQueue[readyQueueSize].idleTime = timer - value[toProcessIndex].arrivalTime;
            readyQueueSize++;

            printf("\nP%d is being executed at timer %d - at index %d.\n", value[toProcessIndex].processNum, timer, toProcessIndex);
            timer += value[toProcessIndex].burstTime;           // here, we add the burst time to the timer to skip the entire execution time of that process

            value[toProcessIndex].processDone = 1;              // then, we set the state of that process to DONE
            value[toProcessIndex].terminatedTime = timer - value[toProcessIndex].arrivalTime;  // here, we set the time that the process finished executing
            
            printf("P%d finished executing at clock cycle %d\n", value[toProcessIndex].processNum, timer);
            printf("Turnaround time of P%d is %d\n\n", value[toProcessIndex].processNum, value[toProcessIndex].terminatedTime);
            continue;
        }

        timer++;    // timer increments for the whole clock cycle
    }

    printReadyQueue(readyQueue, readyQueueSize, value, processes);

}

void priorityBased(int processes) {

    PROCESS value[MAX_COUNT];    // created an array "value" of type "struct values" with a capacity of 10

    // SAMPLE DATA
    value[0].processNum = 1, value[0].arrivalTime = 15, value[0].burstTime = 100, value[0].priorityNum = 3, value[0].processDone = 0;
    value[1].processNum = 2, value[1].arrivalTime = 13, value[1].burstTime = 27, value[1].priorityNum = 2, value[1].processDone = 0;
    value[2].processNum = 3, value[2].arrivalTime = 22, value[2].burstTime = 57, value[2].priorityNum = 4, value[2].processDone = 0;
    value[3].processNum = 4, value[3].arrivalTime = 90, value[3].burstTime = 39, value[3].priorityNum = 1, value[3].processDone = 0;
    // SAMPLE DATA

    printf("\n\tEnter the Arrival Time, Burst Time, and Priority Number for every process.\n\tFORMAT: Process #: [Arrival Time] [Burst Time] [Priority Number]\nNOTE: Priority Numbers should be set in the range 1 - (number of processes)\n\n");
    for (int i = 0; i < processes; i++) {

        printf("\tProcess %d: %d %d %d\n", i + 1, value[i].arrivalTime, value[i].burstTime, value[i].priorityNum);

        // ADD AN ERROR HANDLER HERE IF A PRIORITY NUMBER INPUT IS NOT WITHIN THE RANGE 1 - NUMBER OF PROCESSESS
    }

    // Counter represents the clock in the OS
    // CurrentProcess represents the current process being executed
    int timer = 0;                      // the timer represents the clock cycle of the process executions
    bool currentProcess = false;

    int readyQueueSize = 0;
    PROCESS readyQueue[MAX_COUNT];

    // Imagine this as a click in the clock of OS
    while (true) {

        // Check if all processes are done
        bool allDone = true;
        for (int i = 0; i < processes; i++) {
            if (value[i].processDone == 0) {        // if there is a single process that is not yet done, set allDone to false then break the loop
                allDone = false;
                break;
            }
        }

        if (allDone == true) {      // if all the processes are done, break the while-loop
            value[0].finalTimer = timer;
            printf("Final timer is %d", value[0].finalTimer);
            break;
        }

        // Consume a process
        // LOGIC: Find a process that can be processed that has the lowest Arrival Time and has Higher Priority, set it to be processed first
        int toProcessIndex = -1, smallestArrivalTime = 0, highestPriorityProcess = -1;
        for (int i = 0; i < processes; i++) {
            if (value[i].arrivalTime <= timer && value[i].processDone == 0) {

                if (highestPriorityProcess == -1) {                           // If there aren't any assigned process with the lowest arrival time yet
                    highestPriorityProcess = value[i].priorityNum;           // Assigns the first smallest process
                    toProcessIndex = i;                                   // fetches the index of the current process to be processed
                    //printf("Prioritize P%d with priority number %d", value[toProcessIndex].processNum, value[toProcessIndex].priorityNum);
                }
                else if (value[i].priorityNum < highestPriorityProcess) {    // for the succeeding processes, fetch the process that is smaller than the current smallest arrival time
                    highestPriorityProcess = value[i].priorityNum;
                    toProcessIndex = i;
                    //printf("Prioritize P%d with priority number %d", value[toProcessIndex].processNum, value[toProcessIndex].priorityNum);
                }

            }
        }

        // LOGIC: Solve for the idle time of a process
        if (toProcessIndex != -1) {
            readyQueue[readyQueueSize].processNum = value[toProcessIndex].processNum;
            readyQueue[readyQueueSize].burstTime = value[toProcessIndex].burstTime;
            readyQueue[readyQueueSize].executeTime = timer;
            readyQueue[readyQueueSize].idleTime = timer - value[toProcessIndex].arrivalTime;
            readyQueueSize++;

            printf("\nP%d is being executed at timer %d - at index %d with priority number %d.\n", value[toProcessIndex].processNum, timer, toProcessIndex, value[toProcessIndex].priorityNum);
            timer += value[toProcessIndex].burstTime;           // here, we add the burst time to the timer to skip the entire execution time of that process

            value[toProcessIndex].processDone = 1;              // then, we set the state of that process to DONE
            value[toProcessIndex].terminatedTime = timer - value[toProcessIndex].arrivalTime;  // here, we set the time that the process finished executing
            
            printf("P%d finished executing at clock cycle %d\n", value[toProcessIndex].processNum, timer);
            printf("Turnaround time of P%d is %d\n\n", value[toProcessIndex].processNum, value[toProcessIndex].terminatedTime);
            continue;
        }

        timer++;    // timer increments for the whole clock cycle
    }

    printReadyQueue(readyQueue, readyQueueSize, value, processes);
}

void roundRobin(int processes) {


    PROCESS value[MAX_COUNT];    // created an array "value" of type "struct values" with a capacity of 10
    int quantum = 5;

    // SAMPLE DATA
    value[0].processNum = 1, value[0].arrivalTime = 5, value[0].burstTime = 10, value[0].processDone = 0, value[0].remainingTime = value[0].burstTime;
    value[1].processNum = 2, value[1].arrivalTime = 13, value[1].burstTime = 27, value[1].processDone = 0, value[1].remainingTime = value[1].burstTime;
    value[2].processNum = 3, value[2].arrivalTime = 22, value[2].burstTime = 57, value[2].processDone = 0, value[2].remainingTime = value[2].burstTime;
    value[3].processNum = 4, value[3].arrivalTime = 90, value[3].burstTime = 39, value[3].processDone = 0, value[3].remainingTime = value[3].burstTime;

    // value is an array
    // SAMPLE DATA

    printf("\n\tEnter the Arrival Time and Burst Time for every process.\n\tFORMAT: Process #: [Arrival Time] [Burst Time]\n\n");
    for (int i = 0; i < processes; i++) {
        printf("\tProcess %d: %d %d\n", i + 1, value[i].arrivalTime, value[i].burstTime);
    }


    // Counter represents the clock in the OS
    // CurrentProcess represents the current process being executed
    int timer = 0;
    bool currentProcess = false;

    // initialize the queue
    int readyQueueSize = 0;
    PROCESS readyQueue[MAX_COUNT];
    // int front = -1, rear = -1;

    GANTT ganttChart[MAX_COUNT];
    int ganttIndex = 0;

    printf("\n\n\tProcessing...");

    printf("\n\n\tGANTT CHART\n");

    // Image this as a click in the clock of OS

    int beingProcessed = -1;
    int remainingQuantum = 5;

    while (true) {

        
        // Check if all processes are done
        bool allDone = true;
        for (int i = 0; i < readyQueueSize; i++) {
            if (readyQueue[i].processDone == 0) {    // traverses through all processes, checks if there is a single process with processDone = 0, allDone value becomes false
                allDone = false;
                break;
            }
        }


        if (allDone == true && readyQueueSize == processes) {      // if the condition above was not satisfied, meaning all processes are done. If not, proceed to next code block.
            value[0].finalTimer = timer;
            printf("\n\n\tProgram terminated at clock time %d.\n", value[0].finalTimer);
            break;
        }


        // Place to check if we can put some process to ready queue
        while (true) {
            int toAddProccess = -1, smallestArrivalTime = 0;

            // Pick the process with the smallest arrival time
            for (int i = 0; i < processes; i++) {
                if (value[i].arrivalTime <= timer &&  value[i].addedToQueue == 0) {
                    if (smallestArrivalTime == 0) {                          // For the first smallest process
                        smallestArrivalTime = value[i].arrivalTime;
                        toAddProccess = i;
                    }
                    else if (value[i].arrivalTime < smallestArrivalTime) {  
                        smallestArrivalTime = value[i].arrivalTime;
                        toAddProccess = i;
                    }

                }
            }

            if (toAddProccess != -1) {          // here, we added the process that arrived, to the readyQueue to prepare to be processed
                readyQueue[readyQueueSize] = value[toAddProccess];
                readyQueueSize++;
                // printf("\n\n\t>> Process %d was added to readyQueue at %d.\t", value[toAddProccess].processNum, timer);
                value[toAddProccess].addedToQueue = 1;

                //printf("\n\n\t>> Process %d was added to readyQueue with arrival time at %d.\t", value[toAddProccess].processNum, value[toAddProccess].arrivalTime);
            }
            else {
                break;
            }

        }

        // Check if you can end the program
        if (beingProcessed == -1) {    // if there is no process being processed
            for (int i = 0; i < readyQueueSize; i++) {
                if (readyQueue[i].processDone == 1) {   // proceed to the next process that is in the readyQueue
                    continue;
                }

                beingProcessed = i;
                remainingQuantum = quantum;
                // printf("trying to process %d\n", beingProcessed);
                break;
            }
        }

        // Actual running of process
        if (beingProcessed != -1) { // if there is a process being processed
            // printf("Processing %d with remaining time %d at timer %d\n", beingProcessed, readyQueue[beingProcessed].remainingTime, timer);
            readyQueue[beingProcessed].remainingTime = readyQueue[beingProcessed].remainingTime - 1;
            remainingQuantum--;

            if (readyQueue[beingProcessed].remainingTime == 0 || remainingQuantum == 0) {
                // printf("\t done processing %d\n", readyQueue[beingProcessed].processNum);
                printf("\tP%d", readyQueue[beingProcessed].processNum);
                if (readyQueue[beingProcessed].remainingTime == 0) {
                    readyQueue[beingProcessed].processDone = 1;
                    

                }
                remainingQuantum = quantum;

                bool foundNextToProcess = false;
                for (int i = beingProcessed + 1; i < readyQueueSize; i++) {
                    if (readyQueue[i].processDone == 1) {   // proceed to the next process that is in the readyQueue
                        continue;
                    }

                    beingProcessed = i;
                    remainingQuantum = quantum;
                    foundNextToProcess = true;
                    // printf("trying to proceed to next process %d\n", beingProcessed);
                    break;
                }

                if (foundNextToProcess == false) {
                    beingProcessed = -1;
                }
            }
            
        }
 
        timer++;

        
    }

    roundRobinGanttChart(ganttChart);

    //printReadyQueue(readyQueue, readyQueueSize, value, processes);

    //printSummary(value, readyQueue);  // process results





/* 
    PROCESS value[MAX_COUNT];    // created an array "value" of type "struct values" with a capacity of 100
    Gantt chart[MAX_COUNT];
    
    int quantum = 20;  // Time Quantum

    // SAMPLE DATA
    value[0].processNum = 1, value[0].arrivalTime = 0, value[0].burstTime = 53, value[0].idleTime = 0;
    value[1].processNum = 2, value[1].arrivalTime = 0, value[1].burstTime = 8, value[1].idleTime = 0;
    value[2].processNum = 3, value[2].arrivalTime = 0, value[2].burstTime = 68, value[2].idleTime = 0;
    value[3].processNum = 4, value[3].arrivalTime = 0, value[3].burstTime = 24, value[3].idleTime = 0;
    // SAMPLE DATA

    printf("\n\tEnter the Arrival Time and Burst Time for every process.\n\tFORMAT: Process #: [Arrival Time] [Burst Time]\n\n");
    for (int i = 0; i < processes; i++) {
        printf("\tProcess %d: %d %d\n", i + 1, value[i].arrivalTime, value[i].burstTime);
        value[i].remainingTime = value[i].burstTime;        // sets the remaining time values the same as the burst time
    }

    printf("\n\tEnter the Time Quantum: %d", quantum);

    // start with P1, minus 20 sa burst time, then add 20 to the idle time of P2, P3, and P4
    // if below time quantum ang remaining time sang isa ka process, then other processes += value[i].burstTime para ma add ang idle time sang other processes
    // then we set, value[i].burstTime = 0

    /* typedef struct {
        int processNum, arrivalTime, burstTime, remainingTime, idleTime, processDone;
    } Values; */

    /* typedef struct {        // stores the history of processes
        int pNum, pBurst, totalBurst;
    } Gantt; */








/* 

    int tempBurst = 0, totalBurst = 0, toPrint = 0;          // here, amu ni gamiton nga burst time sa pag add sang idle time sang other processess if in case the burst time of a process is now less than the time quantum

    printf("\n\nGANTT CHART\n");
    //printf("Current i = -\t\tP1 = %d\t\tP2 = %d\t\tP3 = %d\t\tP4 = %d\t\tTotal Time = %d\n", value[0].burstTime, value[1].burstTime, value[2].burstTime, value[3].burstTime, totalTime(value, processes));
    for (int i = 0; i < processes; i++) {

        if (value[i].burstTime == 0) {          // here, if the burst time of the current process is now zero - skip the process
            toPrint = 1;
        }
        else if (value[i].burstTime < quantum) {
            tempBurst = value[i].burstTime;     // here, we store the burst time of the current process - nga less than the time quantum na - to a temporary variable
            totalBurst += tempBurst;            // here, i=add ang current burst time sa total burst sang whole thing
            value[i].burstTime = 0;             // then i-set na ang burst time of the current process to zero
        }
        else {
            totalBurst += quantum;              // here, i-add lang sa total burst time ang quantum
            value[i].burstTime -= quantum;      // then minusan lang ang burst time sang current process sang time quantum
        }

        // LAST PROGRESS IS FIXING THE IDLE TIME OF THE PROCESSES
        // adding the idle time happens here
        for (int j = 0; j < processes; j++) {
            if (j == i || value[j].burstTime == 0) {        // here, we skip the current process in the loop kay ang i-addan lang idle time is ang other processes
                continue;
            }
            else if (value[i].burstTime < quantum) {        // here, is if ang remaining burst time sang current process is below the quantum, so amuna ang i-add nga idle time to other processess
                value[j].idleTime += tempBurst;
            }
            else {
                value[j].idleTime += quantum;               // here, we simply add the idle time of other processess by the given quantum
            }
        }

        // check data
        if (toPrint == 0) {
            //printf("P%d\t", value[i].processNum);     // FINAL PROCESS NUMBER DISPLAY
            //printf("%d\t", totalBurst);               // FINAL BURST DISPLAY
            
            printf("Current i = %d\t\tP1 = %d\t\tP2 = %d\t\tP3 = %d\t\tP4 = %d\t\tTotal Time = %d\nIdle Times:\t\tP1 = %d\t\tP2 = %d\t\tP3 = %d\t\tP4 = %d\n\n",\
            i + 1, value[0].burstTime, value[1].burstTime, value[2].burstTime, value[3].burstTime, totalTime(value, processes), value[0].idleTime, value[1].idleTime, value[2].idleTime, value[3].idleTime);
        }
        else {
            toPrint = 0;
        }





        if (totalTime(value, processes) == 0) {
            printf("\nDONE");
            break;
        }
        else if (i == 3) {
            i -= processes;
        }


        // sa end, if the total remaining time is not yet zero, then i-set ta liwat ang i to zero
        /* if (i == 3) {

            if (totalTime(value, processes) == 0) {
                printf("\ndone");
                break;
            }
            i -= processes;
        } 

    }
    printf("\n\n");

    printf("Idle Time:\t\tP1 = %d\t\tP2 = %d\t\tP3 = %d\t\tP4 = %d", value[0].idleTime, value[1].idleTime, value[2].idleTime, value[3].idleTime);



 */
}

// ---------------------------------------------------------------------------------------------------------------------------------------------- OTHER FUNCTIONS
int printReadyQueue(PROCESS readyQueue[MAX_COUNT], int readyQueueSize, PROCESS value[MAX_COUNT], int processes) {

    float turnaroundSum = 0, waitingSum = 0;

    printf("\n\tGANTT CHART\n\t");
    for (int i = 0; i < readyQueueSize; i++) {                  // prints the order of processess
        printf("P%d\t", readyQueue[i].processNum);
    }
    printf("\n\t");
    for (int i = 0; i < readyQueueSize; i++) {                  // prints the time each process executes
        printf("%d\t", readyQueue[i].executeTime);
    }
    printf("%d\n", value[0].finalTimer);      // prints the end time of execution

    printf("\nIDLE TIMES\n");                                   // prins the idle times of every process
    for (int i = 0; i < processes; i++) {
        printf("P%d: %d\n", readyQueue[i].processNum, readyQueue[i].idleTime);
    }

    // Turnaround time - time when the process finished executing (time finished executing - arrival time)
    printf("\nTURNAROUND TIMES\n");
    for (int i = 0; i < processes; i++) {
        printf("TA%d: %d\n", value[i].processNum, value[i].terminatedTime);
    }

    for (int i = 0; i < processes; i++) {
        turnaroundSum += value[i].terminatedTime;
        waitingSum += readyQueue[i].idleTime;

    }

    printf("\n");
    printf("turnaroundSum = %f", turnaroundSum);

    printf("\n\n\t** Average Turnaround Time:\t%.6f **", turnaroundSum/processes);
    printf("\n\t** Average Waiting Time:\t%.6f **\n", waitingSum/processes);
}

int roundRobinGanttChart(GANTT ganttChart[MAX_COUNT]) {



}












// to remove?
int totalTime(PROCESS value[MAX_COUNT], int processes) {

    // this function adds all the remaining burst times, if it is equal to zero, all processes are done
    int total = 0;
    for (int i = 0; i < processes; i++) {
        total += value[i].burstTime;
    }

    return total;
}

// to remove?
void bubbleSort(PROCESS value[MAX_COUNT], int processes) {

    int i, j, temp1, temp2;
    for (i = 0; i < processes - 1; i++) {
        for (j = 0; j < processes - i - 1; j++) {
            if (value[j].burstTime > value[j + 1].burstTime) {
                temp1 = value[j].burstTime;
                temp2 = value[j].processNum;

                value[j].burstTime = value[j + 1].burstTime;
                value[j].processNum = value[j + 1].processNum;

                value[j + 1].burstTime = temp1;
                value[j + 1].processNum = temp2;
            }
        }
    }
    
}










/* int printSummary(PROCESS value[MAX_COUNT], int processes) {

    for (int i = 0; i < processes; i++) {
        printf("%d\t", value[i].executeTime);
    }
    
    
    printf("\n\nRESULTS ARE PRINTED HERE\n");

    printf("Waiting Times:\n");
    for (int i = 0; i < processes; i++) {
        printf("\nP%d: %d", value[i].processNum, value[i].idleTime);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    int burstArray[MAX_COUNT], turnaroundArray[MAX_COUNT], waitingArray[MAX_COUNT];     // arrays
    float turnaroundSum = 0, waitingSum = 0;                                            // sums
    int currentBurst = 0;                                                               // burst tracker

    printf("\n\tGANTT CHART\n");                                                        // determines at what time each process started
    for (int i = 0; i < processes; i++) {
        printf("\tP%d", value[i].processNum);
    }
    printf("\n");
    for (int i = 0; i < processes + 1; i++) {

        if (i == 0) {   // Process 1
            burstArray[i] = currentBurst;
            waitingArray[i] = currentBurst;                                             // start time of current process, is the end time of previous process
            value[i].idleTime = 0;
        }
        else {
            currentBurst += value[i - 1].burstTime;                                     // current burst of current process is tracked by adding each burst
            burstArray[i] = currentBurst;                                               // every burst is stored in an array

            turnaroundArray[i - 1] = currentBurst - value[i - 1].arrivalTime;           // turnaround time of every process is stored in an array
            waitingArray[i] = currentBurst - value[i].arrivalTime;                                             // waiting time of a process is the sum of previous bursts
        }
        printf("\t%d", currentBurst);
    }
    
    printf("\n\nTurnaround Times:");
    for (int i = 0; i < processes; i++) {
        printf("\nP%d: %d", value[i].processNum, turnaroundArray[i]);
        turnaroundSum += turnaroundArray[i];
    }

    printf("\n\nWaiting Times:");
    for (int i = 0; i < processes; i++) {
        printf("\nP%d: %d", value[i].processNum, waitingArray[i]);
        waitingSum += waitingArray[i];
    }

    printf("\n\n\t** Average Turnaround Time:\t%.6f **", turnaroundSum/processes);
    printf("\n\t** Average Waiting Time:\t%.6f **\n", waitingSum/processes);

} */
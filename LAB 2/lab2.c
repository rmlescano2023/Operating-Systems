#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_COUNT 1000

//---------------------------------------------------------------------------------------------------------------------------------------------- FUNCTION DECLARATIONS
typedef struct {
    int processNum, priorityNum, arrivalTime, burstTime, remainingTime, idleTime, executeTime, terminatedTime, processDone, toRun, addedToQueue, finalTimer;
} PROCESS;

typedef struct {
    char name[100];     // store the Process number
    int startProcess;   // store the Process time to execute
 } GANTT;

void FCFS(int processes);
void SJF(int processes);
void priorityBased(int processes);
void roundRobin(int processes);

int printReadyQueue(PROCESS readyQueue[MAX_COUNT], int readyQueueSize, PROCESS value[MAX_COUNT], GANTT ganttChart[MAX_COUNT], int ganttChartIndex, int processes, int schedulingAlgorithm);

int totalTime(PROCESS value[MAX_COUNT], int processes);
void bubbleSort(PROCESS value[MAX_COUNT], int processes);


// ---------------------------------------------------------------------------------------------------------------------------------------------- MAIN
int main() {

    int decision, processes;
    char choice = 'Y';

    while(choice == 'Y' || choice == 'y') {

        system("cls");

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
                system("cls");

                printf("\n\t>> FCFS Scheduling <<\n\tSpecify the number of processes: ");
                scanf("%d", &processes);

                FCFS(processes);
                break;

            case 2:
                system("cls");

                printf("\n\t>> SJF Scheduling <<\n\tSpecify the number of processes: ");
                scanf("%d", &processes);

                SJF(processes);
                break;

            case 3:
                system("cls");

                printf("\n\t>> Priority Based Scheduling <<\n\tSpecify the number of processes: ");
                scanf("%d", &processes);

                priorityBased(processes);
                break;

            case 4:
                system("cls");

                printf("\n\t>> Round Robin  Scheduling <<\n\tSpecify the number of processes: ");
                scanf("%d", &processes);

                roundRobin(processes);
                break;

        }

        printf("\n\n\tDo you want to continue using the program? (Y/N): ");
        scanf(" %c", &choice); 

    }

    printf("\n");
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------- SCHEDULING ALGORITHMS
void FCFS(int processes) {

    PROCESS value[MAX_COUNT];                   // stores input values
    PROCESS readyQueue[MAX_COUNT];              // stores the processess that are currently being processed
    GANTT ganttChart[MAX_COUNT];                // stores values for the Gantt chart (for printing)

    for (int i = 0; i < processes; i++) {       // reset the DONE indicator of every process
        value[i].processDone = 0;
        readyQueue[i].processDone = 0;
    }

    //SAMPLE DATA
    value[0].processNum = 1, value[0].arrivalTime = 30, value[0].burstTime = 5, value[0].priorityNum = 4;
    value[1].processNum = 2, value[1].arrivalTime = 3, value[1].burstTime = 13, value[1].priorityNum = 2;
    value[2].processNum = 3, value[2].arrivalTime = 33, value[2].burstTime = 2, value[2].priorityNum = 6;
    value[3].processNum = 4, value[3].arrivalTime = 27, value[3].burstTime = 8, value[3].priorityNum = 5;
    //SAMPLE DATA

    int timer = 0, readyQueueSize = 0, ganttChartIndex = 0;          // the timer represents the clock cycle of the process executions
    bool toPrintIdle = true;

    printf("\n\tEnter the Arrival Time and Burst Time for every process.\n\tFORMAT: Process #: [Arrival Time] [Burst Time]\n\n");
    for (int i = 0; i < processes; i++) {
        printf("\tProcess %d: %d %d\n", i + 1, value[i].arrivalTime, value[i].burstTime);
    }

    // MAIN FCFS ALGORITHM
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
            break;
        }

        // Find a process that can be processed that has the lowest Arrival time, set it to be processed first
        int toProcessIndex = -1, smallestArrivalTime = -1;
        for (int i = 0; i < processes; i++) {

            if (value[i].arrivalTime <= timer && value[i].processDone == 0) {   // if the arrival time of the current process is less than or equal to the current tick of the timer
                
                if (smallestArrivalTime == -1) {                          
                    smallestArrivalTime = value[i].arrivalTime;         // Assigns the first smallest process
                    toProcessIndex = i;                                 // fetches the index of the current process to be processed
                    
                }
                else if (value[i].arrivalTime < smallestArrivalTime) {  // for the succeeding processes, fetch the process that is smaller than the current smallest arrival time
                    smallestArrivalTime = value[i].arrivalTime;
                    toProcessIndex = i;
                }

            }

        }

        // Actual running of the process
        if (toProcessIndex != -1) {     // if a process arrives at a time when a different process is still executing, it is waiting for a time being
            readyQueue[readyQueueSize] = value[toProcessIndex];
            readyQueue[readyQueueSize].executeTime = timer;
            readyQueue[readyQueueSize].idleTime = timer - value[toProcessIndex].arrivalTime;            // waiting time
            readyQueueSize++;

            sprintf(ganttChart[ganttChartIndex].name, "P%d", value[toProcessIndex].processNum);         // store the process flow in the Gantt chart as a string
            ganttChart[ganttChartIndex].startProcess = timer;                                           // stores the time of each process when it started executing
            ganttChartIndex++;

            timer += value[toProcessIndex].burstTime;           // here, we add the burst time to the timer to skip the entire execution time of that process

            value[toProcessIndex].processDone = 1, readyQueue[toProcessIndex].processDone = 1;          // then, we set the state of that process to DONE
            value[toProcessIndex].terminatedTime = timer - value[toProcessIndex].arrivalTime;           // here, we set the time that the process finished executing

            toPrintIdle = true;         // if set to True, meaning we could print an idle time (if there is any) in the next column of the ganttChart
            
            continue;
        }
        
        // if there are no any process currently being processed, the system is idle
        if (toProcessIndex == -1 && toPrintIdle == true) {      // Idle time is added to ganttChart here
            sprintf(ganttChart[ganttChartIndex].name, "-");
            ganttChart[ganttChartIndex].startProcess = timer;
            ganttChartIndex++;

            toPrintIdle = false;        // set to False, since we only need to print the idle value once
        }

        timer++;    // timer increments for the whole clock cycle
    }

    printReadyQueue(readyQueue, readyQueueSize, value, ganttChart, ganttChartIndex, processes, 1);

}

void SJF(int processes) {

    PROCESS value[MAX_COUNT];                   // stores input values
    PROCESS readyQueue[MAX_COUNT];              // stores the processess that are currently being processed
    GANTT ganttChart[MAX_COUNT];                // stores values for the Gantt chart (for printing)

    for (int i = 0; i < processes; i++) {       // reset the DONE indicator of every process
        value[i].processDone = 0;
        readyQueue[i].processDone = 0;
    }

    //SAMPLE DATA
    value[0].processNum = 1, value[0].arrivalTime = 30, value[0].burstTime = 5, value[0].priorityNum = 4;
    value[1].processNum = 2, value[1].arrivalTime = 3, value[1].burstTime = 13, value[1].priorityNum = 2;
    value[2].processNum = 3, value[2].arrivalTime = 33, value[2].burstTime = 2, value[2].priorityNum = 6;
    value[3].processNum = 4, value[3].arrivalTime = 27, value[3].burstTime = 8, value[3].priorityNum = 5;
    //SAMPLE DATA

    int timer = 0, readyQueueSize = 0, ganttChartIndex = 0;          // the timer represents the clock cycle of the process executions
    bool toPrintIdle = true;

    printf("\n\tEnter the Arrival Time and Burst Time for every process.\n\tFORMAT: Process #: [Arrival Time] [Burst Time]\n\n");
    for (int i = 0; i < processes; i++) {
        printf("\tProcess %d: %d %d\n", i + 1, value[i].arrivalTime, value[i].burstTime);
    }

    // MAIN SJF ALGORITHM
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
            break;
        }

        // Find a process that can be processed that has the lowest Burst time, set it to be processed first
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

        // Actual running of the process
        if (toProcessIndex != -1) {     // if a process arrives at a time when a different process is still executing, it is waiting for a time being
            readyQueue[readyQueueSize] = value[toProcessIndex];
            readyQueue[readyQueueSize].executeTime = timer;
            readyQueue[readyQueueSize].idleTime = timer - value[toProcessIndex].arrivalTime;
            readyQueueSize++;

            sprintf(ganttChart[ganttChartIndex].name, "P%d", value[toProcessIndex].processNum);
            ganttChart[ganttChartIndex].startProcess = timer;
            ganttChartIndex++;

            timer += value[toProcessIndex].burstTime;           // here, we add the burst time to the timer to skip the entire execution time of that process

            value[toProcessIndex].processDone = 1, readyQueue[toProcessIndex].processDone = 1;              // then, we set the state of that process to DONE
            value[toProcessIndex].terminatedTime = timer - value[toProcessIndex].arrivalTime;               // here, we set the time that the process finished executing

            toPrintIdle = true;         // if set to True, meaning we could print an idle time (if there is any) in the next column of the ganttChart

            continue;
        }

        // if there are no any process currently being processed, the system is idle
        if (toProcessIndex == -1 && toPrintIdle == true) {      // Idle time is added to ganttChart here
            sprintf(ganttChart[ganttChartIndex].name, "-");
            ganttChart[ganttChartIndex].startProcess = timer;
            ganttChartIndex++;

            toPrintIdle = false;        // set to False, since we only need to print the idle value once
        }

        timer++;    // timer increments for the whole clock cycle
    }

    printReadyQueue(readyQueue, readyQueueSize, value, ganttChart, ganttChartIndex, processes, 2);

}

void priorityBased(int processes) {

    PROCESS value[MAX_COUNT];                   // stores input values
    PROCESS readyQueue[MAX_COUNT];              // stores the processess that are currently being processed
    GANTT ganttChart[MAX_COUNT];                // stores values for the Gantt chart (for printing)

    for (int i = 0; i < processes; i++) {       // reset the DONE indicator of every process
        value[i].processDone = 0;
        readyQueue[i].processDone = 0;
    }

    //SAMPLE DATA
    value[0].processNum = 1, value[0].arrivalTime = 30, value[0].burstTime = 5, value[0].priorityNum = 4;
    value[1].processNum = 2, value[1].arrivalTime = 3, value[1].burstTime = 13, value[1].priorityNum = 2;
    value[2].processNum = 3, value[2].arrivalTime = 33, value[2].burstTime = 2, value[2].priorityNum = 6;
    value[3].processNum = 4, value[3].arrivalTime = 27, value[3].burstTime = 8, value[3].priorityNum = 5;
    //SAMPLE DATA

    int timer = 0, readyQueueSize = 0, ganttChartIndex = 0;          // the timer represents the clock cycle of the process executions
    bool toPrintIdle = true;

    printf("\n\tEnter the Arrival Time and Burst Time for every process.\n\tFORMAT: Process #: [Arrival Time] [Burst Time]\n\n");
    for (int i = 0; i < processes; i++) {
        printf("\tProcess %d: %d %d %d\n", i + 1, value[i].arrivalTime, value[i].burstTime, value[i].priorityNum);
    }

    // MAIN PRIORITY BASED ALGORITHM
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
            break;
        }

        // Find a process that can be processed that has the lowest Arrival Time and has Higher Priority, set it to be processed first
        int toProcessIndex = -1, highestPriorityProcess = -1;
        for (int i = 0; i < processes; i++) {
            
            if (value[i].arrivalTime <= timer && value[i].processDone == 0) {

                if (highestPriorityProcess == -1) {                         // If there aren't any assigned process with the lowest arrival time yet
                    highestPriorityProcess = value[i].priorityNum;          // Assigns the first smallest process
                    toProcessIndex = i;                                     // fetches the index of the current process to be processed
                }
                else if (value[i].priorityNum < highestPriorityProcess) {   // for the succeeding processes, fetch the process that is smaller than the current smallest arrival time
                    highestPriorityProcess = value[i].priorityNum;
                    toProcessIndex = i;
                }

            }

        }

        // Actual running of the process
        if (toProcessIndex != -1) {     // if a process arrives at a time when a different process is still executing, it is waiting for a time being
            readyQueue[readyQueueSize] = value[toProcessIndex];
            readyQueue[readyQueueSize].executeTime = timer;
            readyQueue[readyQueueSize].idleTime = timer - value[toProcessIndex].arrivalTime;            // waiting time
            readyQueueSize++;

            sprintf(ganttChart[ganttChartIndex].name, "P%d", value[toProcessIndex].processNum);
            ganttChart[ganttChartIndex].startProcess = timer;
            ganttChartIndex++;

            timer += value[toProcessIndex].burstTime;           // here, we add the burst time to the timer to skip the entire execution time of that process

            value[toProcessIndex].processDone = 1, readyQueue[toProcessIndex].processDone = 1;              // then, we set the state of that process to DONE
            value[toProcessIndex].terminatedTime = timer - value[toProcessIndex].arrivalTime;               // here, we set the time that the process finished executing

            toPrintIdle = true;         // if set to True, meaning we could print an idle time (if there is any) in the next column of the ganttChart

            continue;
        }

        // if there are no any process currently being processed, the system is idle
        if (toProcessIndex == -1 && toPrintIdle == true) {      // Idle time is added to ganttChart here
            sprintf(ganttChart[ganttChartIndex].name, "-");
            ganttChart[ganttChartIndex].startProcess = timer;
            ganttChartIndex++;

            toPrintIdle = false;        // set to False, since we only need to print the idle value once
        }

        timer++;    // timer increments for the whole clock cycle
    }

    printReadyQueue(readyQueue, readyQueueSize, value, ganttChart, ganttChartIndex, processes, 3);
}

void roundRobin(int processes) {

    PROCESS value[MAX_COUNT];                   // stores input values
    PROCESS readyQueue[MAX_COUNT];              // stores the processess that are currently being processed
    GANTT ganttChart[MAX_COUNT];                // stores values for the Gantt chart (for printing)

    for (int i = 0; i < processes; i++) {       // reset the DONE indicators of every process
        value[i].processDone = 0;
        readyQueue[i].processDone = 0;
    }

    //SAMPLE DATA
    value[0].processNum = 1, value[0].arrivalTime = 30, value[0].burstTime = 5, value[0].priorityNum = 4, value[0].remainingTime = value[0].burstTime;
    value[1].processNum = 2, value[1].arrivalTime = 5, value[1].burstTime = 10, value[1].priorityNum = 2, value[1].remainingTime = value[1].burstTime;
    value[2].processNum = 3, value[2].arrivalTime = 29, value[2].burstTime = 8, value[2].priorityNum = 6, value[2].remainingTime = value[2].burstTime;
    value[3].processNum = 4, value[3].arrivalTime = 33, value[3].burstTime = 3, value[3].priorityNum = 5, value[3].remainingTime = value[3].burstTime;
    //SAMPLE DATA

    int timer = 0, quantum = 0, readyQueueSize = 0, beingProcessed = -1, roundArrayIndex = 0, ganttChartIndex = 0;
    bool toPrintIdle = true;

    printf("\n\tEnter the Arrival Time and Burst Time for every process.\n\tFORMAT: Process #: [Arrival Time] [Burst Time]\n\n");
    for (int i = 0; i < processes; i++) {
        printf("\tProcess %d: %d %d\n", i + 1, value[i].arrivalTime, value[i].burstTime);
    }

    quantum = 5;
    printf("\n\tEnter the Quantum time: %d", quantum);

    int remainingQuantum = quantum;


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
            break;
        }

        // Check if we can put some process to the ready queue
        while (true) {
            int toAddProccess = -1, smallestArrivalTime = -1;       // -1 value acts as a reset

            // Pick the process with the smallest arrival time
            for (int i = 0; i < processes; i++) {

                if (value[i].arrivalTime <= timer &&  value[i].addedToQueue == 0) {

                    if (smallestArrivalTime == -1) {                            // If the smallestArrivalTime was not yet set
                        smallestArrivalTime = value[i].arrivalTime;             // The first process will be identified as the one with the small
                        toAddProccess = i;                                      // Take note of the current process at index 'i' because this will be the process that will be added to the readyQueue
                    }
                    else if (value[i].arrivalTime < smallestArrivalTime) {      // If the arrival time of the current process in the current loop is less than the current smallestArrivalTime
                        smallestArrivalTime = value[i].arrivalTime;             // We set it as the new smallestArrivalTime
                        toAddProccess = i;
                    }

                }
            }

            if (toAddProccess != -1) {          // Here, we added the process (that arrived) to the readyQueue to prepare to be processed
                readyQueue[readyQueueSize] = value[toAddProccess];  
                readyQueueSize++;
                value[toAddProccess].addedToQueue = 1;      // Change the state of the process that was added - to just take note that it was added to the readyQueue
            }
            else {                              // If the toAddProcess value remains as -1, meaning there was no process to be added to the readyQueue anymore
                break;
            }

        }
        // While END


        // Check if we can end the program
        if (beingProcessed == -1) {         // If there is no process being processed
            for (int i = 0; i < readyQueueSize; i++) {
                if (readyQueue[i].processDone == 1) {   // Proceed to the next process that is in the readyQueue, check if that process is DONE processing
                    continue;
                }

                // If there exists a process in the readyQueue that is not yet DONE
                beingProcessed = i;                     // Take note of that process, and process it
                remainingQuantum = quantum;             // Reset the quantum
                
                sprintf(ganttChart[ganttChartIndex].name, "P%d", readyQueue[beingProcessed].processNum);
                ganttChart[ganttChartIndex].startProcess = timer;
                ganttChartIndex++;

                toPrintIdle = true;

                break;
            }
        }

        // Actual running of a process
        if (beingProcessed != -1) {         // if there is a process being processed
            readyQueue[beingProcessed].remainingTime = readyQueue[beingProcessed].remainingTime - 1;    // Decerement the remaining time of that process
            remainingQuantum--;                                                                         // Also decrement the quantum

            // Here, we increment the waiting time values of all other processess except from the current process being processed that is in the readyQueue
            for (int i = 0; i < processes; i++) {
                if (readyQueue[i].processDone != 1 && i != beingProcessed) {
                    readyQueue[i].idleTime++;
                }
            }

            // Here, if the process is done OR the remaining quantum is zero (meaning it finished a quantum burst), take note of that Process in the Gantt chart
            if (readyQueue[beingProcessed].remainingTime == 0 || remainingQuantum == 0) {

                if (readyQueue[beingProcessed].remainingTime == 0) {        // If the burst time of the process runs out
                    readyQueue[beingProcessed].processDone = 1;                                                             // set the state of the process as DONE processing
                    readyQueue[beingProcessed].terminatedTime = timer - readyQueue[beingProcessed].arrivalTime + 1;         // note the turnaround time of the process
                }
                remainingQuantum = quantum;     // reset the quantum

                // Here, we check if there are any other process/processess that is currently in the ready queue that is not yet DONE
                bool foundNextToProcess = false;
                for (int i = beingProcessed + 1; i < readyQueueSize; i++) {
                    
                    if (readyQueue[i].processDone == 1) {                       // proceed to the next process that is in the readyQueue
                        continue;
                    }
                    // If the code proceeds here after the if statement above, this means that there is/are existing process/processess in the ready queue that is not yet done
                    // Meaning, we found a next process to be processed

                    toPrintIdle = true;             // if set to True, meaning we could print an idle time (if there is any) in the next column of the ganttChart
                    beingProcessed = i;             // Take note of the index of that process
                    
                    sprintf(ganttChart[ganttChartIndex].name, "P%d", readyQueue[beingProcessed].processNum);
                    ganttChart[ganttChartIndex].startProcess = timer + 1;
                    ganttChartIndex++;

                    remainingQuantum = quantum;     // reset the quantum
                    foundNextToProcess = true;

                    break;
                }

                if (foundNextToProcess == false) {              // If there was no next process found
                    beingProcessed = -1;
                }
            }
            
        }
        else {      // this else block means that there's no process being processed at this time

            if (toPrintIdle == true) {      // Idle time is added to ganttChart here
                sprintf(ganttChart[ganttChartIndex].name, "-");
                ganttChart[ganttChartIndex].startProcess = timer;
                ganttChartIndex++;

                toPrintIdle = false;
            }
        }
 
        timer++;

    }
    printf("\n");

    printReadyQueue(readyQueue, readyQueueSize, value, ganttChart, ganttChartIndex, processes, 4);

}

// ---------------------------------------------------------------------------------------------------------------------------------------------- OTHER FUNCTIONS
int printReadyQueue(PROCESS readyQueue[MAX_COUNT], int readyQueueSize, PROCESS value[MAX_COUNT], GANTT ganttChart[MAX_COUNT], int ganttChartIndex, int processes, int schedulingAlgorithm) {

    float turnaroundSum = 0, waitingSum = 0;

    printf("\n\tGANTT CHART\n");
    for (int i = 0; i < ganttChartIndex; i++) {                  // prints the order of processess
        printf("\t%s", ganttChart[i].name);
    }
    printf("\n");
    
    for (int i = 0; i < ganttChartIndex; i++) {                  // prints the time each process executes
        printf("\t%d", ganttChart[i].startProcess);
    }

    printf("\t%d\n", value[0].finalTimer);                          // prints the end time of execution

    printf("\n\tWAITING TIMES\n");                                       // prints the idle times of every process
    for (int i = 0; i < processes; i++) {
        printf("\tP%d: %d\n", readyQueue[i].processNum, readyQueue[i].idleTime);
    }

    // Turnaround time - time when the process finished executing (time finished executing - arrival time)
    printf("\n\tTURNAROUND TIMES\n");
    for (int i = 0; i < processes; i++) {
        printf("\tP%d: %d\n", (schedulingAlgorithm == 4) ? readyQueue[i].processNum : value[i].processNum, (schedulingAlgorithm == 4) ? readyQueue[i].terminatedTime : value[i].terminatedTime);
    }

    for (int i = 0; i < processes; i++) {           // solves for the Sum of Turnaround and Waiting times
        turnaroundSum += (schedulingAlgorithm == 4) ? readyQueue[i].terminatedTime : value[i].terminatedTime;
        waitingSum += readyQueue[i].idleTime;

    }

    printf("\n\n\t** Average Turnaround Time:\t%.6f **", turnaroundSum/processes);
    printf("\n\t** Average Waiting Time:\t%.6f **\n", waitingSum/processes);

}

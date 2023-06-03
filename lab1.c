#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

	pid_t child1_pid, child2_pid;
	int n = 10;
	int fibonacci[n];

	fibonacci[0] = 0;
	fibonacci[1] = 1;

	//process fibonacci sequence here
	for (int i = 2; i < n; i++) {
		fibonacci[i] = fibonacci[i-1] + fibonacci[i-2];
	}


	child1_pid = fork();

	if (child1_pid == 0) {

		printf("I am child 1, my pid = %d, and the Fibonacci sequence are as follows: ", getpid());

		for (int j = 0; j < n; j++) {

			printf("%d ", fibonacci[j]);

		}
		printf("\n");
		exit(0);

	}
	else {

		if (child2_pid == 0) {

			printf("I am child 1, my pid = %d, and the Fibonacci sequence are as follows: ", getpid());

			for (int j = 0; j < n; j++) {

				printf("%d ", fibonacci[j]);

			}
			printf("\n");
			exit(0);

		}
		else {

			printf("I am parent, my pid = %d\nI am waiting for child %d to finish...\n", getpid(), child1_pid);
			waitpid(child1_pid, NULL, 0);

			printf("I am now waiting for child %d to finish...\n", child2_pid);
			waitpid(child2_pid, NULL, 0);

			printf("All child processes are now done, exiting...\n");
			exit(0);

		}

	}

}

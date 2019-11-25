#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	pid_t child_pid;
	int n, m;
	int child_status;
	child_pid = fork();
	switch (child_pid) {
		case -1: printf("Can't create process\n");
			return -1;
		case 0: printf("This is the child\n");
			for (n = 0; n < 3; n++) {
				printf("Child process ... %d\n", n);
				sleep(1);
			}
			return 0;
		default: printf("This is the parrent\n");
			for (m = 0; m < 5; m++) {
				printf("Parrent process 1 - ... %d\n", m);
				sleep(1);
			}
			//wait(&child_status);
			for (m = 0; m < 10; m++) {
				printf("Parrent process 2 - ... %d\n", m);
				sleep(1);
			}
			wait(&child_status);
			break;
	}
	return 0;
}

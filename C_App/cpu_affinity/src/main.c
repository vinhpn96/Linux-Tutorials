#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	cpu_set_t set;
	int cpu_num = atoi(argv[1]);
	CPU_ZERO(&set);
	CPU_SET(cpu_num, &set);
	sched_setaffinity(getpid(), sizeof(set), &set);

	while(1)
	{
		printf("vinhpn\n");
		sleep(1);
	}
}

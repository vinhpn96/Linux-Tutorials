#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

char messenger[] = "Hello world";

void *do_thread(void *data)
{
	int i;
	int me = (int*)data;

	printf("Thead function is executing ...\n");
	printf("Thead data is %s\n", messenger);
	for (i = 0; i < 5; i++)
	{
		printf("%d - Got %d\n", me, i);
		sleep(1);
	}
	strcpy(messenger, "Goodbye!");
	pthread_exit("Thanks for your using me!");
}

int main(void)
{
	int thr_id;
	pthread_t p_thread;
	int res;
	int a = 1;
	void *thread_result;

	res = pthread_create(&p_thread, NULL, do_thread, (void*)a);
	printf("Wait for thread to finish ...\n");
	res = pthread_join(p_thread, &thread_result);
	printf("Thead completed, it returned %s\n", (char*)thread_result);
	printf("Messenger is now: %s\n", messenger);
	return 0;
}
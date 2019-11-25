#include <stdio.h>
#include <pthread.h>

void *do_thread(void *data)
{
	int i;
	int me = (int*)data;
	for (i = 0; i < 5; i++)
	{
		printf("%d - Got %d\n", me, i);
		sleep(1);
	}
	pthread_exit(NULL);
}

int main(void)
{
	int thr_id;
	pthread_t p_thread;
	int a = 1;
	int b = 2;

	thr_id = pthread_create(&p_thread, NULL, do_thread, (void*)a);
	do_thread((void*)b);
	return 0;
}
#include <stdio.h>

int main()
{
	int index;
	for (index = 0; index < 10; index++) {
		printf("Hello world %d\n", index);
		sleep(1);
	}
	return 0;
}

#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("Execlp has just start ...");
	int temp = execlp("execlp_helloworld", "execlp_helloworld", NULL);
	printf("Done %d", temp);
	return 0;
}

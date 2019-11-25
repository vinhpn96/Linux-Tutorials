#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/input.h>

#define MOUSEFILE "/dev/input/event5"

int main()
{
	int fd;
	struct input_event ie;
	
	if ((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
		printf("open device file error");
		exit(-1);
	}
	
	while (read(fd, &ie, sizeof(struct input_event))) {
		printf("time %ld.%06ld\ttype %d\tcode %d\tvalue %d\n",
		       ie.time.tv_sec, ie.time.tv_usec, ie.type, ie.code, ie.value);
	}

	return 0;
}

/*
 * main.c
 *
 *  Created on: Oct 31, 2019
 *      Author: vinhpn
 */
#include <stdio.h>
#include <stdlib.h>

#include "part1/io.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("No file data input");
		return -1;
	}

	person_t* head = load_list_people(argv[1]);
	person_t* p;

	p = head;
	while (p != NULL) {
		stream_string(p->name);
		printf("%d\t\n", p->weight);
		p = p->next_person;
	}
}


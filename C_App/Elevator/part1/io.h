/*
 * io.h
 *
 *  Created on: Oct 31, 2019
 *      Author: vinhpn
 */

#ifndef PART1_IO_H_
#define PART1_IO_H_

typedef struct string_t {
	const char* buffer;
	int length;
} string_t;

typedef struct person_t {
	string_t name;
	int weight;
	struct person_t* next_person;
} person_t;

person_t* load_list_people(const char* file_name);
void stream_string(string_t str);

#endif /* PART1_IO_H_ */

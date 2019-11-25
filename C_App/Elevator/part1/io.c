/*
 * io.c
 *
 *  Created on: Oct 31, 2019
 *      Author: vinhpn
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "io.h"

static int read_line(string_t* source, string_t* line);
static int read_word(string_t* line, string_t* word);
static int string_to_uint(string_t* name, int* value);
static int parse_person(string_t* line, person_t* head);
static person_t* create_person();
static int add_person(person_t* head, string_t name, int weight);

person_t* load_list_people(const char* file_name)
{
	FILE* f;

	f = fopen(file_name, "r");
	if (!f)
	{
		printf("File not opened: %s\n", file_name);
		return -1;
	}

	fseek(f, 0, SEEK_END);
	int len = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* buf = (char*)malloc(len + 1);
	fread(buf, 1, len, f);
	fclose(f);
	buf[len] = 0;

	string_t source;
	string_t line;
	person_t* person = create_person();
	source.buffer = buf;
	source.length = len;

	while (source.length > 0) {
		read_line(&source, &line);
		if (line.length > 0) {
			if (!parse_person(&line, person))
				break;
		}
	}
	return person;
}

static int read_line(string_t* source, string_t* line) {
	if (source->length > 0) {
		line->buffer = source->buffer;
		line->length = 0;
		while (source->length > 0) {
			if (*source->buffer == 10 || *source->buffer == 13) {
				break;
			}
			else {
				source->buffer++;
				source->length--;
				line->length++;
			}
		}

		while (source->length > 0) {
			if (*source->buffer == 10 || *source->buffer == 13) {
				source->buffer++;
				source->length--;
			}
			else
				break;
		}
		return 1;
	}
	else
		return 0;
}

static int read_word(string_t* line, string_t* word) {
	while (line->length > 0) {
		if (*line->buffer == ' ' || *line->buffer == 9) {
			line->buffer++;
			line->length--;
		}
		else
			break;
	}

	if (line->length > 0) {
		word->buffer = line->buffer;
		word->length = 0;
		while (line->length > 0) {
			if (*line->buffer == ' ' || *line->buffer == 9) {
				break;
			}
			else {
				line->buffer++;
				line->length--;
				word->length++;
			}
		}
		return 1;
	}
	else
		return 0;
}

static int parse_person(string_t* line, person_t* head)
{
	string_t word;
	string_t name;
	int weight;
	if (read_word(line, &word)) {
		if (word.length > 0 && word.buffer[0] == '#')
			return 1;
		if (word.length > 0 && (word.buffer[0] < 'A' || word.buffer[0] > 'z'))
			return 0;
		else {
			name = word;
			if (read_word(line, &word)) {
				if (string_to_uint(&word, &weight)) {
					read_line(line, &word);
					if (line->length > 0) return 0;
					else {
						add_person(head, name, weight);
						return 1;
					}
				}
				else return 0;
			}
			else return 0;
		}
	}
	else return 0;
}

static int string_to_uint(string_t* name, int* value) {
	int index;
	int val = 0;
	if (name->length > 0) {
		for (index = 0; index < name->length; index++) {
			if (name->buffer[index] >= '0' && name->buffer[index] <= '9') {
				val = val * 10 + (name->buffer[index] - '0');
			}
			else
				return 0;
		}
		*value = val;
		return 1;
	}
	else {
		return 0;
	}
}

static int add_person(person_t* head, string_t name, int weight) {
	person_t* temp;
	person_t* p;
	temp = create_person();
	temp->name = name;
	temp->weight = weight;

	if (head->name.length == 0) {
		head->name = name;
		head->weight = weight;
	}
	else {
		p = head;
		while (p->next_person != NULL) {
			p = p->next_person;
		}
		p->next_person = temp;
	}

	return 1;
}

static person_t* create_person() {
	person_t* temp = (person_t*) malloc(sizeof(struct person_t));
	temp->name.buffer = NULL;
	temp->name.length = 0;
	temp->weight = 0;
	temp->next_person = NULL;
	return temp;
}

void stream_string(string_t str) {
	char* temp = (char*) malloc(str.length + 1);
	memcpy(temp, str.buffer, str.length);
	temp[str.length] = 0;
	printf("%s", temp);
	free(temp);
}

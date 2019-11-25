/*
 * input.h
 *
 *  Created on: Dec 9, 2018
 *      Author: dongnx
 */

#ifndef __SRS_INPUT_H__
#define __SRS_INPUT_H__

#define BUTTON_1_PIN		23
#define BUTTON_2_PIN		27
#define POWER_PIN			4
#define RB9603_RI_PIN		21

typedef struct input_t input_t;

typedef struct input_p {
	void(*dispose)(input_t*);
	int(*switch_state)(input_t*);
	int(*emergency_state)(input_t*);
	int(*power_state)(input_t*);
	int(*rb603_ri_state)(input_t*);
	int(*process)(input_t*, int*, int*);
} input_p;

struct input_t {
	const input_p* proc;
	gpio_t* switchs;
	gpio_t* emergency;
	gpio_t* power;
	gpio_t* rb9603_ri;
};

input_t* input_create(void);
void input_dispose(input_t* object);
int input_switch_state(input_t* object);
int input_emergency_state(input_t* object);
int input_power_state(input_t* object);
int input_rb9603_ri_state(input_t* object);
int input_process(input_t* object, int* pin, int* value);

#endif /* __SRS_INPUT_H__ */

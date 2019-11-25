/*
 * input.c
 *
 *  Created on: Dec 9, 2018
 *      Author: dongnx
 */
#include "../../sdk.h"
#include "../../services/util/util.h"
#include "../../drivers/io/io.h"
#include <poll.h>
#include "input.h"

static input_p input_proc = {
		input_dispose,
		input_switch_state,
		input_emergency_state,
		input_power_state,
		input_rb9603_ri_state,
		input_process
};

input_t* input_create(void) {
	gpio_t* power = gpio_create(4);
	gpio_t* switchs = gpio_create(23);
	gpio_t* emergency = gpio_create(27);
	gpio_t* rb9603_ri = gpio_create(21);

	usleep(500000);

	if ((power == NULL) || ((power != NULL) && (power->proc->set_edge(power, "both") < 0)))return NULL;
	if ((switchs == NULL) || ((switchs != NULL) && (switchs->proc->set_edge(switchs, "both") < 0))) return NULL;
	if ((emergency == NULL) || ((emergency != NULL) && (emergency->proc->set_edge(emergency, "both") < 0))) return NULL;
	if ((rb9603_ri == NULL) || ((rb9603_ri != NULL) && (rb9603_ri->proc->set_edge(rb9603_ri, "both") < 0))) return NULL;

	input_t* object = (input_t*)mem_alloc(sizeof(input_t));
	object->proc = &input_proc;
	object->switchs = switchs;
	object->emergency = emergency;
	object->power = power;
	object->rb9603_ri = rb9603_ri;

	char *buf[64];
	struct pollfd fdset[4] = { 0 };
	fdset[0].fd = object->switchs->fd;
	fdset[0].events = POLLPRI;
	fdset[1].fd = object->emergency->fd;
	fdset[1].events = POLLPRI;
	fdset[2].fd = object->power->fd;
	fdset[2].events = POLLPRI;
	fdset[3].fd = object->rb9603_ri->fd;
	fdset[3].events = POLLPRI;

	fflush(stdout);
	int rc = poll(fdset, 4, 10);
	if (rc >= 0)
	{
		if (fdset[0].revents & POLLPRI) {
			lseek(fdset[0].fd, 0, SEEK_SET);
			read(fdset[0].fd, buf, 64);
		}
		if (fdset[1].revents & POLLPRI) {
			lseek(fdset[1].fd, 0, SEEK_SET);
			read(fdset[1].fd, buf, 64);
		}
		if (fdset[2].revents & POLLPRI) {
			lseek(fdset[2].fd, 0, SEEK_SET);
			read(fdset[2].fd, buf, 64);
		}
		if (fdset[3].revents & POLLPRI) {
			lseek(fdset[3].fd, 0, SEEK_SET);
			read(fdset[3].fd, buf, 64);
		}
	}

	logi("input create");
	fflush(stdout);
	return object;
}

void input_dispose(input_t* object) {
//	object->switchs->proc->dispose(object->switchs);
//	object->emergency->proc->dispose(object->emergency);
//	object->power->proc->dispose(object->power);
	mem_free(object);
}

int input_switch_state(input_t* object) {
	return object->switchs->proc->get_state(object->switchs);
}

int input_emergency_state(input_t* object) {
	return object->emergency->proc->get_state(object->emergency);
}

int input_power_state(input_t* object) {
	return object->power->proc->get_state(object->power);
}

int input_rb9603_ri_state(input_t* object) {
	return object->rb9603_ri->proc->get_state(object->rb9603_ri);
}

int input_process(input_t* object, int* pin, int* value) {
	char *buf[64];
	struct pollfd fdset[4];

	memset((void*)fdset, 0, 4 * sizeof(fdset));

	fdset[0].fd = object->switchs->fd;
	fdset[0].events = POLLPRI;
	fdset[1].fd = object->emergency->fd;
	fdset[1].events = POLLPRI;
	fdset[2].fd = object->power->fd;
	fdset[2].events = POLLPRI;
	fdset[3].fd = object->rb9603_ri->fd;
	fdset[3].events = POLLPRI;

	fflush(stdout);
	int rc = poll(fdset, 4, -1);
	if (rc < 0) {
		return (-1);
	}

	if (fdset[0].revents & POLLPRI) {
			lseek(fdset[0].fd, 0, SEEK_SET);
			read(fdset[0].fd, buf, 64);
			*pin = object->switchs->pin;
			*value = object->switchs->proc->get_state(object->switchs);
	}

	if (fdset[1].revents & POLLPRI) {
			lseek(fdset[1].fd, 0, SEEK_SET);
			read(fdset[1].fd, buf, 64);
			*pin = object->emergency->pin;
			*value = object->emergency->proc->get_state(object->emergency);
	}

	if (fdset[2].revents & POLLPRI) {
			lseek(fdset[2].fd, 0, SEEK_SET);
			read(fdset[2].fd, buf, 64);
			*pin = object->power->pin;
			*value = object->power->proc->get_state(object->power);
	}

	if (fdset[3].revents & POLLPRI) {
			lseek(fdset[3].fd, 0, SEEK_SET);
			read(fdset[3].fd, buf, 64);
			*pin = object->rb9603_ri->pin;
			*value = object->rb9603_ri->proc->get_state(object->rb9603_ri);
	}

	logi("input interrupt: pin = %d, value = %d", *pin, *value);
	fflush(stdout);
	return 0;
}

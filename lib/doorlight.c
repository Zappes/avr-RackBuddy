/*
 * doorlight.c
 *
 *  Created on: 21.11.2014
 *      Author: zaphod
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "doorlight.h"

void init_doorlight() {
	DL_DDR &= ~_BV(DL_PIN_DOOR);
	DL_DDR |= _BV(DL_PIN_LIGHT);
}

uint8_t dl_door_closed() {
	return DL_IN & _BV(DL_PIN_DOOR);
}

void dl_set_light_status(uint8_t status) {
	if(status) {
		DL_OUT |= _BV(DL_PIN_LIGHT);
	}
	else {
		DL_OUT &= ~_BV(DL_PIN_LIGHT);
	}
}

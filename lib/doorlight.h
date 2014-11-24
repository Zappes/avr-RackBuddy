/*
 * doorlight.h
 *
 *  Created on: 21.11.2014
 *      Author: zaphod
 */

#ifndef DOORLIGHT_H_
#define DOORLIGHT_H_

#include <avr/io.h>

#define DL_PIN_DOOR		PC0
#define DL_PIN_LIGHT	PC1
#define DL_IN					PINC
#define DL_OUT				PORTC
#define DL_DDR				DDRC

void init_doorlight();
uint8_t dl_door_closed();

void dl_set_light_status(uint8_t status);

#endif /* DOORLIGHT_H_ */

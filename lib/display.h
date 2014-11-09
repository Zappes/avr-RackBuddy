/*
 * display.h
 *
 *  Created on: 14.10.2014
 *      Author: zaphod
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <avr/io.h>
#include <util/delay.h>

#define DSP_DDR_SPI DDRB
#define DSP_PORT PORTB
#define DSP_MOSI DDB3
#define DSP_MISO DDB4
#define DSP_SCK DDB5
#define DSP_LATCH DDB2
#define DSP_RESET DDB1

#define DSP_MASK_COLON 1

void init_display();
void display_values(uint8_t hi_cur, uint8_t hi_avg, uint8_t lo_cur, uint8_t lo_avg);
void refresh_display();

#endif /* DISPLAY_H_ */

/*
 * display.c
 *
 *  Created on: 14.10.2014
 *      Author: zaphod
 */
#include <avr/io.h>
#include <util/delay.h>
#include "display.h"

uint8_t chars[] = {
		0b11111101,
		0b01100001,
		0b11011011,
		0b11110011,
		0b01100111,
		0b10110111,
		0b10111111,
		0b11100001,
		0b11111111,
		0b11110111,
		0b00000000
};

uint8_t numbers[] = {10,10,10,10,10,10,10,10};

void pulse_pin(uint8_t bit) {
	DSP_PORT |= _BV(bit);
	DSP_PORT &= ~_BV(bit);
}

void write_symbol(uint8_t symbol) {
	SPDR = symbol;
	while(!(SPSR & _BV(SPIF)));
	pulse_pin(DSP_LATCH);
}

void set_digits(uint8_t number, uint8_t offset) {
	if(number > 99) number = 99;
	if(offset > 6) offset = 6;

	numbers[offset] = number / 10;
	numbers[offset+1] = number % 10;
}

void refresh_display() {
	static uint8_t digit = 1;

	uint8_t symbol = chars[numbers[digit++]];

	if(digit > 7) digit = 0;

	write_symbol(symbol);
}

void display_values(uint8_t hi_cur, uint8_t hi_avg, uint8_t lo_cur, uint8_t lo_avg) {
	set_digits(hi_cur, 0);
	set_digits(hi_avg, 2);
	set_digits(lo_cur, 4);
	set_digits(lo_avg, 6);
}

void init_display() {
	// set the relevant pins as outputs
	DSP_DDR_SPI = _BV(DSP_MOSI) | _BV(DSP_SCK) | _BV(DSP_LATCH) | _BV(DSP_RESET);

	// activate SPI
	SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);

	// reset decade counter
	pulse_pin(DSP_RESET);
}

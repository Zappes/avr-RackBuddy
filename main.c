/*
 * Controller code for a device that reads analog sensor values and sleeps
 * while it isn't used. Check the README at Github for more information.
 *
 * Written by Gregor Ottmann, https://github.com/Zappes
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "lib/display.h"
#include "lib/uart.h"
#include "lib/onewire.h"
#include "lib/ds18x20.h"

char buffer[64];
char dc_buffer[10];
uint16_t temps[2];

void init_comm() {
	uart_init(UART_BAUD_SELECT(9600, F_CPU));
}

/**
 * Sets up a timer interrupt that will be called every 8192 clock cycles, i.e. with
 * about 2kHz.
 */
void init_main_loop() {
	cli();

	// CTC with OCR0A as TOP
	TCCR0A = (1 << WGM01);

	// clk_io/1024 (From prescaler)
	TCCR0B = ((1 << CS02) | (1 << CS00));

	// Generate an interrupt every 8192 clock cycles
	OCR0A = 7;

	// Enable Timer/Counter0 Compare Match A interrupt
	TIMSK0 |= (1 << OCIE0A);

	sei();
}

ISR(TIMER0_COMPA_vect) {
	refresh_display();
}

uint8_t gSensorIDs[2][8] = { { 0x10, 0xA1, 0x3E, 0xE8, 0x02, 0x08, 0x00, 0xDA },
		{ 0x10, 0xA0, 0x12, 0xE8, 0x02, 0x08, 0x00, 0x34 } };

int main(void) {
	init_display();
	init_comm();
	init_main_loop();

	int16_t decicelsius;

#ifndef OW_ONE_BUS
	ow_set_bus(&PINB,&PORTB,&DDRB,PB0);
#endif

	while (1) {
//		sprintf(buffer, "Fuck.");
//		uart_puts(buffer);

//		display_values(0,1,2,3);

		if (DS18X20_start_meas( DS18X20_POWER_EXTERN, NULL) == DS18X20_OK) {
			_delay_ms( DS18B20_TCONV_12BIT);
			for (int i = 0; i < 2; i++) {
				if (DS18X20_read_decicelsius(&gSensorIDs[i][0],&decicelsius) == DS18X20_OK) {
					temps[i] = decicelsius;
				} else {
					sprintf(buffer, "Sensor #%d FAILED\r\n", i);
					uart_puts(buffer);
					temps[i] = 0;
				}
			}
		}
		else {
			uart_puts("Le Fuck.\r\n");
		}

		DS18X20_format_from_decicelsius( temps[0], dc_buffer, 10 );
		sprintf(buffer, "Sensor #%d=%s (%d)\r\n", 0, dc_buffer, temps[0]);
		uart_puts(buffer);
		DS18X20_format_from_decicelsius( temps[1], dc_buffer, 10 );
		sprintf(buffer, "Sensor #%d=%s (%d)\r\n\r\n", 1, dc_buffer, temps[1]);
		uart_puts(buffer);


		display_values(temps[0] / 10, (temps[0] % 10) * 10, temps[1] / 10, (temps[1] % 10) * 10);
		_delay_ms(10000);
	}
	return 0;
}

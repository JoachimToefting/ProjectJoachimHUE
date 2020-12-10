/*
 * USART.c
 *
 * Created: 08/12/2020 08.35.56
 *  Author: Joachim
 *	Taget fra Ilearn
 */ 

#include "USART.h"
#include <avr/io.h>
#include <util/setbaud.h>

void USART_Transmit(unsigned char data)
{
	while (	!(UCSR0A & (1<<UDRE0)));		// Wait for buffer to be ready to send (22.10.2 s. 219)
	UDR0 = data;							// Put data into buffer, sends the data
}

void USART_Init()
{
	UBRR0H = UBRRH_VALUE;					// set UART0 baud rate with setbaud-library
	UBRR0L = UBRRL_VALUE;

	UCSR0B = (1<<RXEN0) | (1<<TXEN0);		// Enable Receiver and Transmitter (22.10.3, s. 220)
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);		// Set frame format: 8 bit data, 1 stop bit (22.10.4 s. 221)
}
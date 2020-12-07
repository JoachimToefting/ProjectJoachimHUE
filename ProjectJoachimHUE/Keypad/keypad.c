/*
 * keypad.c
 *
 * Created: 07/12/2020 08.21.01
 *  Author: Joachim
 */ 
/*
* main.c
*
* Created: 27-05-2019 09:03:25
* Author: Egon Rasmussen

* Porte og pins
=============
* Name = Port = Arduino Pin = Keyboard cable #
* R1 = PK0 = pin A8  = #8
* R2 = PK1 = pin A9  = #7
* R3 = PK2 = pin A10 = #6
* R4 = PK3 = pin A11 = #5

* C1 = PK4 = pin A12 = #4
* C2 = PK5 = pin A13 = #3
* C3 = PK6 = pin A14 = #2
* C4 = PK7 = pin A15 = #1

* Dekodning
=========
R1 R2 R3 R4 C1 C2 C3 C4  Key
0  1  1  1  0  1  1  1    1
1  0  1  1  0  1  1  1    4
1  1  0  1  0  1  1  1    7
1  1  1  0  0  1  1  1    *

0  1  1  1  1  0  1  1    2
1  0  1  1  1  0  1  1    5
1  1  0  1  1  0  1  1    8
1  1  1  0  1  0  1  1    0

0  1  1  1  1  1  0  1    3
1  0  1  1  1  1  0  1    6
1  1  0  1  1  1  0  1    9
1  1  1  0  1  1  0  1    #

0  1  1  1  1  1  1  0    A
1  0  1  1  1  1  1  0    B
1  1  0  1  1  1  1  0    C
1  1  1  0  1  1  1  0    D
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

char DecodeKey(char data);

void Keypad_Init()
{
	DDRK = 0b00001111;		// Use PortK, upper nibble = input (rows), lower nibble = output (columns)
	PORTK |= 0b11110000;	// Enable Pull-up on Row pins (upper nibble)
}

void ColumnScan()
{
	static int columnCounter = 1;			// Value 1 - 4 represents selected Column
	
	char portBuffer = PORTK & 0b11110000;	// Prevents glitches in bit manipulation, clear all column bits
	
	switch (columnCounter)
	{
		case 1:
		portBuffer |= 0b00000111;			// reset C1
		PORTK = portBuffer;
		break;
		case 2:
		portBuffer |= 0b00001011;			// reset C2
		PORTK = portBuffer;
		break;
		case 3:
		portBuffer |= 0b00001101;			// reset C3
		PORTK = portBuffer;
		break;
		case 4:
		portBuffer |= 0b00001110;			// reset C4
		PORTK = portBuffer;
		break;
	}
	columnCounter++;
	if (columnCounter > 4)
	{
		columnCounter = 1;					// Reset back to first Column
	}
}

char ReadRows()
{
	char pinRead = PINK;
	if (pinRead < 0xF0)			// test for a row line going low
	{
		return DecodeKey(pinRead);
	}
	return '.';
}

char DecodeKey(char data)
{
	char key;
	switch (data)
	{
		case 0b01110111: key = '1'; break;
		case 0b10110111: key = '4'; break;
		case 0b11010111: key = '7'; break;
		case 0b11100111: key = '*'; break;
		case 0b01111011: key = '2'; break;
		case 0b10111011: key = '5'; break;
		case 0b11011011: key = '8'; break;
		case 0b11101011: key = '0'; break;
		case 0b01111101: key = '3'; break;
		case 0b10111101: key = '6'; break;
		case 0b11011101: key = '9'; break;
		case 0b11101101: key = '#'; break;
		case 0b01111110: key = 'A'; break;
		case 0b10111110: key = 'B'; break;
		case 0b11011110: key = 'C'; break;
		case 0b11101110: key = 'D'; break;
		default:         key = '.'; break;
	}
	return key;
}

char DisplayKey(char data)
{
	if (data != '.')
	{
		//printf("%c",data);
		_delay_ms(400);
	}
}
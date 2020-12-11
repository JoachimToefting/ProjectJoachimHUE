/*
* ProjectJoachimHUE.c
*
* Created: 03/12/2020 13.03.11
* Author : Joachim
*/
#define F_CPU 16000000UL
#define SWITCH_PRESSED bit_is_clear(PINB, PB6)
#define MENUPOINTS_COUNT 4
#define PREDEFINED_COLORS 8

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "main.h"
#include "LCDfolder/lcd.h"
#include "Keypad/keypad.h"
#include "USART/USART.h"

unsigned char color[9];		// indexing red 0-2, green 3-5, blue 6-8
unsigned char menucnt;

unsigned char prdecolorcnt;
unsigned char prdecolor[PREDEFINED_COLORS][9] = {
	{'0','0','0','0','0','0','0','0','0'}
	,{'2','5','5','0','0','0','0','0','0'}
	,{'0','0','0','2','5','5','0','0','0'}
	,{'0','0','0','0','0','0','2','5','5'}
	,{'2','5','5','0','0','0','0','6','9'}
	,{'1','2','7','0','0','0','2','5','5'}
	,{'1','0','0','2','5','5','0','0','0'}
	,{'2','5','5','2','5','5','2','5','5'}
};

ISR(PCINT0_vect){
	if (SWITCH_PRESSED)
	{
		SendData(prdecolor[prdecolorcnt]);
		if (prdecolorcnt < PREDEFINED_COLORS-1)
		{
			prdecolorcnt++;
		}
		else
		{
			prdecolorcnt = 0;
		}
	}
}

int main(void)
{
	Init();
	
	while (1)
	{
		//ColorInput();
		Menu();
	}
}

void Init(void){
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	char *str = "Initializing...";
	lcd_puts(str);
	Keypad_Init();
	USART_Init();
	prdecolorcnt = 0;
	//initailize button
	DDRB &= ~(1<<PB6);		// InputPort
	PORTB |= (1<<PB6);		// Pullup resistor
	PCICR |= (1<<PCIE0);		// Interupt on group 0
	PCMSK0 |= (1<<PCINT6);		// Masking for PCINT6
	sei();
}

unsigned char menupoints[MENUPOINTS_COUNT][25] = {{"Welcome to JoachimHue\n"},{"A: Custom color\n"},{"B: Color choice\n"},{"C: Nothing\n"}};
unsigned char menucnt = 0;

void Menu(void){
	
	unsigned char input = '.';
	PrintMenu(menucnt,menupoints);
	
	while (input == '.')
	{
		_delay_ms(70);
		ColumnScan();
		input = ReadRows();
	}
	switch (input)
	{
		case '#':
		if (menucnt < MENUPOINTS_COUNT-2) // 2 is the number of rows displayet
		{
			menucnt++;
		}
		break;
		
		case '*':
		if (menucnt > 0)
		{
			menucnt--;
		}
		break;
		
		case 'A':
		ColorInput();
		break;
		
		case 'B':
		//nothing
		break;
		
		default:
		break;
	}
}

void PrintMenu(char menucnt)
{
	lcd_clrscr();
	lcd_puts(menupoints[menucnt]);
	lcd_puts(menupoints[menucnt+1]);
}

void ColorInput(void){
	//reset color
	for (char cnt = 0; cnt < 9; cnt++)
	{
		color[cnt] = '.';
	}
	
	for (char colorcnt = 0; colorcnt < 9; colorcnt++)
	{
		switch (colorcnt)
		{
			case 0:			//Red
			lcd_clrscr();
			lcd_puts("Color:\nRed: ");
			lcd_gotoxy(13,1);
			break;
			case 3:			//Green
			_delay_ms(500);
			lcd_clrscr();
			lcd_puts("Color:\nGreen: ");
			lcd_gotoxy(13,1);
			break;
			case 6:			//Blue
			_delay_ms(500);
			lcd_clrscr();
			lcd_puts("Color:\nBlue: ");
			lcd_gotoxy(13,1);
			break;
			default:
			lcd_gotoxy(13+(colorcnt % 3),1);
			break;
		}
		InputColor(colorcnt);
		lcd_putc(color[colorcnt]);
	}
	ValidateAndSend();
}

void InputColor(char colorcnt){
	while (color[colorcnt] == '.' )
	{
		_delay_ms(100);
		ColumnScan();
		color[colorcnt] = ReadRows();
	}
}

void ValidateAndSend(void){
	int colorState = ColorValidator(color);
	if (colorState < 0)
	{
		ErrorPrint(colorState);
	}
	else
	{
		SendData(color);
	}
}

void ErrorPrint(int colorState){
	lcd_clrscr();
	lcd_puts("No color set,\nError: ");
	char errornr[9];
	itoa(colorState, errornr, 10);
	lcd_puts(errornr);
	_delay_ms(2500);
}

int ColorValidator(char *p_fullcolor){
	char cnt;
	for (cnt = 0; cnt < 9; cnt += 3)
	{
		int colorValue = ValueValidator(p_fullcolor + cnt);
		if (colorValue < 0)
		{
			return colorValue;
		}
	}
	return 0;
	
}

int ValueValidator(char *p_color){
	char cnt;
	for (cnt = 0; cnt < 3; cnt++)
	{
		if (*(p_color + cnt) < '0' || *(p_color + cnt) > '9')
		{
			return -1; // input er ikke et tal
		}
	}
	
	int number = (((*p_color)-48) * 100) + (((*(p_color+1))-48) * 10) + ((*(p_color+2))-48); //tal værdien for en farve ascii char til int
	
	if (number > 255 || number < 0)
	{
		return -2; // input værdien er for høj eller lav
	}
	return number;
}

void SendData(unsigned char color[9]){
	char startbyte = 'S';
	
	USART_Transmit(startbyte);
	for (char cnt = 0; cnt < 9; cnt++)
	{
		USART_Transmit(color[cnt]);
	}
}
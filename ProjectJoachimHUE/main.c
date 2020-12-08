/*
* ProjectJoachimHUE.c
*
* Created: 03/12/2020 13.03.11
* Author : Joachim
*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCDfolder/lcd.h"
#include "Keypad/keypad.h"
#include "main.h"

int main(void)
{
	Init();
	
	while (1)
	{
		ColorInput();
	}
}

void Init(void){
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	char *str = "Welcome";
	lcd_puts(str);
	Keypad_Init();
	
}

void ColorInput(void){
	char colorcnt;
	unsigned char color[9] = {'.','.','.','.','.','.','.','.','.'}; // indexing red 0-2, green 3-5, blue 6-8 (alle char er '.' for at gensætte værdierne)
	
	for (colorcnt = 0; colorcnt < 9; colorcnt++)
	{
		switch (colorcnt)
		{
			case 0:			//Rød
			lcd_clrscr();
			lcd_puts("Color:\nRed: ");
			lcd_gotoxy(13,1);
			break;
			case 3:			//Grøn
			lcd_clrscr();
			lcd_puts("Color:\nGreen: ");
			lcd_gotoxy(13,1);
			break;
			case 6:			//Blå
			lcd_clrscr();
			lcd_puts("Color:\nBlue: ");
			lcd_gotoxy(13,1);
			break;
			default:
			lcd_gotoxy(13+(colorcnt % 3),1);
			break;
		}
		while (color[colorcnt] == '.' ) //Keypad input
		{
			_delay_ms(100);
			ColumnScan();
			color[colorcnt] = ReadRows();
		}
		lcd_putc(color[colorcnt]);
	}
	ColorValidator(color);
}

void ColorValidator(char *p_fullcolor){
	char cnt;
	for (cnt = 0; cnt < 9; cnt += 3)
	{
		int colorValue = ValueValidator(p_fullcolor + cnt);
		if (colorValue < 0)
		{
			lcd_clrscr();
			lcd_puts("No color set,\nError: ");
			char errornr[9];
			itoa(colorValue, errornr, 10);
			lcd_puts(errornr);
			_delay_ms(2500);
			break;
		}
	}
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



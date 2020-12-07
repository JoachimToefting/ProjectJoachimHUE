/*
* LCDStarter.c
*
* Created: 04/12/2020 09.32.10
*  Author: Joachim
*/
#include "lcd.h"

void LCDStart(void){
	lcd_init(LCD_DISP_ON);
}
void printLCD(unsigned char* str){
	lcd_puts(str);
}
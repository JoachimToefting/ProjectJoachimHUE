/*
 * keypad.h
 *
 * Created: 07/12/2020 08.21.12
 *  Author: Joachim
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

void Keypad_Init(void);
void ColumnScan(void);
char ReadRows(void);
char DecodeKey(char data);
void DisplayKey(char data);


#endif /* KEYPAD_H_ */
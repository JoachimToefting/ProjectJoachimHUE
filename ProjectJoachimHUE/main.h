/*
 * main.h
 *
 * Created: 07/12/2020 09.03.14
 *  Author: Joachim
 */ 


#ifndef MAIN_H_
#define MAIN_H_

void Init(void);
void ColorInput(void);
int ColorValidator(char *p_fullcolor);
int ValueValidator(char *p_color);
void SendData(unsigned char color[9]);

#endif /* MAIN_H_ */
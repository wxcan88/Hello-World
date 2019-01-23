#ifndef __ADC_H
#define __ADC_H	
#include <stm32l1xx.h>
#define u8  unsigned char 
#define u16 unsigned int
#define u32 unsigned long

#define LSENS_READ_TIMES	10		//定义光敏传感器读取次数,读这么多次,然后取平均值

#define GP2Y_High GPIO_SetBits(GPIOA,GPIO_Pin_6);
#define GP2Y_Low  GPIO_ResetBits(GPIOA,GPIO_Pin_6);

void ADC_Config(void);



float Get_GP2Y(void);
float Get_Adc(void);
void printfshuju(void);

float Get_Val(void);
u8 Lsens_Get_Val(void);
#endif 
  
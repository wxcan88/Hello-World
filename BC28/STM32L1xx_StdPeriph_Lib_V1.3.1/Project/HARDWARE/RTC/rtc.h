#ifndef _RTC_H_
#define _RTC_H_

#include"stm32l1xx.h"


#define u8  unsigned char 
#define u16 unsigned int
#define u32 unsigned long

	
u8 RTC_Config(void);						//RTC��ʼ��
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTCʱ������
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC��������
void RTC_AlarmConfig(void);		//��������ʱ��(����������,24Сʱ��)
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					//�����Ի��Ѷ�ʱ������

void printftime(void);
#endif



















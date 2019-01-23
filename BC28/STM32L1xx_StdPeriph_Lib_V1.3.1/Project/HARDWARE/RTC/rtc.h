#ifndef _RTC_H_
#define _RTC_H_

#include"stm32l1xx.h"


#define u8  unsigned char 
#define u16 unsigned int
#define u32 unsigned long

	
u8 RTC_Config(void);						//RTC初始化
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTC时间设置
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC日期设置
void RTC_AlarmConfig(void);		//设置闹钟时间(按星期闹铃,24小时制)
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					//周期性唤醒定时器设置

void printftime(void);
#endif



















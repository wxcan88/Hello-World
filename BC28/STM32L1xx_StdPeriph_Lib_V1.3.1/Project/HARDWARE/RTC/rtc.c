#include "RTC.h"
#include "usart.h"
#include "led.h"
#include "exti.h"
#include "bc28.h"

NVIC_InitTypeDef   NVIC_InitStructure;
extern BC28 BC28_Status;
//RTC时间设置  
//hour,min,sec:小时,分钟,秒钟  
//ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM  
//返回值:SUCEE(1),成功  
//       ERROR(0),进入初始化模式失败   
ErrorStatus RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm)  
{  
    RTC_TimeTypeDef RTC_TimeTypeInitStructure;  
      
    RTC_TimeTypeInitStructure.RTC_Hours=hour;  
    RTC_TimeTypeInitStructure.RTC_Minutes=min;  
    RTC_TimeTypeInitStructure.RTC_Seconds=sec;  
    RTC_TimeTypeInitStructure.RTC_H12=ampm;  
      
    return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);  
      
}
//RTC日期设置  
//year,month,date:年(0~99),月(1~12),日(0~31)  
//week:星期(1~7,0,非法!)  
//返回值:SUCEE(1),成功  
//       ERROR(0),进入初始化模式失败   
ErrorStatus RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week)  
{  
      
    RTC_DateTypeDef RTC_DateTypeInitStructure;  
    RTC_DateTypeInitStructure.RTC_Date=date;  
    RTC_DateTypeInitStructure.RTC_Month=month;  
    RTC_DateTypeInitStructure.RTC_WeekDay=week;  
    RTC_DateTypeInitStructure.RTC_Year=year;  
    return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);  
	
} 
//RTC初始化
//返回值:0,初始化成功;
//       1,LSE开启失败;
//       2,进入初始化模式失败;
unsigned char RTC_Config(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0X1FFF;	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	PWR_RTCAccessCmd(ENABLE);
	RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5051)		//是否第一次配置?
	{
		RCC_LSEConfig(RCC_LSE_ON);//LSE 开启    
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
			{
			retry++;
			delay_ms(10);
			}
		if(retry==0)return 1;		//LSE 开启失败. 
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟 

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC异步分频系数(1~0X7F)
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTC同步分频系数(0~7FFF)
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC设置为,24小时格式
    RTC_Init(&RTC_InitStructure);
 
		RTC_Set_Time(BC28_Status.hours,BC28_Status.minutes,BC28_Status.seconds,RTC_H12_AM);	//设置时间
		RTC_Set_Date(BC28_Status.year,BC28_Status.mouth,BC28_Status.day,2);		//设置日期
	 
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5051);	//标记已经初始化过了



	} 
 
	return 0;
}

//void RTC_AlarmConfig(void)
//{
//	char setAlarmSecond = 0;			//设置闹钟，秒
//	NVIC_InitTypeDef  NVIC_InitStructure;
//	EXTI_InitTypeDef  EXTI_InitStructure;
//	RTC_AlarmTypeDef RTC_AlarmStructure;
//	
////	setAlarmSecond = 58;
//  /* EXTI configuration */
//	EXTI_ClearITPendingBit(EXTI_Line17);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//	/* Enable the RTC Alarm Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
// 
////	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
////	RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
//		
//	
//	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
//	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
//	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
//	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
//	
//	/* Set AlarmA subseconds and enable SubSec Alarm : generate 8 interripts per Second */
//	RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0xFF, RTC_AlarmSubSecondMask_SS14_5);
//	
//			/* Clear PWR WakeUp flag */
//	PWR_ClearFlag(PWR_FLAG_WU);

//	/* Clear RTC Alarm A flag */ 
//	RTC_ClearFlag(RTC_FLAG_ALRAF);
//	
//			/* Enable AlarmA interrupt */
//	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
//	
//	/* Enable the alarmA */
//	RTC_AlarmCmd(RTC_Alarm_A, ENABLE); 
//	
//	RTC_ClearFlag(RTC_FLAG_ALRAF );

//}


////周期性唤醒定时器设置  
///*wksel:  @ref RTC_Wakeup_Timer_Definitions
//#define RTC_WakeUpClock_RTCCLK_Div16        ((uint32_t)0x00000000)
//#define RTC_WakeUpClock_RTCCLK_Div8         ((uint32_t)0x00000001)
//#define RTC_WakeUpClock_RTCCLK_Div4         ((uint32_t)0x00000002)
//#define RTC_WakeUpClock_RTCCLK_Div2         ((uint32_t)0x00000003)
//#define RTC_WakeUpClock_CK_SPRE_16bits      ((uint32_t)0x00000004)
//#define RTC_WakeUpClock_CK_SPRE_17bits      ((uint32_t)0x00000006)
//*/
////cnt:自动重装载值.减到0,产生中断.
//void RTC_Set_WakeUp(u32 wksel,u16 cnt)
//{ 
//	EXTI_InitTypeDef   EXTI_InitStructure;
//	
//	RTC_WakeUpCmd(DISABLE);//关闭WAKE UP
//	
//	RTC_WakeUpClockConfig(wksel);//唤醒时钟选择
//	
//	RTC_SetWakeUpCounter(cnt);//设置WAKE UP自动重装载寄存器
//	
//	
//	RTC_ClearITPendingBit(RTC_IT_WUT); //清除RTC WAKE UP的标志
//  EXTI_ClearITPendingBit(EXTI_Line22);//清除LINE22上的中断标志位 
//	 
//	RTC_ITConfig(RTC_IT_WUT,ENABLE);//开启WAKE UP 定时器中断
//	RTC_WakeUpCmd( ENABLE);//开启WAKE UP 定时器　
//	
//	EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE22
//  EXTI_Init(&EXTI_InitStructure);//配置
// 
// 
//	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级1
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
//  NVIC_Init(&NVIC_InitStructure);//配置
//}


//RTC闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A中断?
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);//清除中断标志
		printf("ALARM A!\r\n");
	}   
	EXTI_ClearITPendingBit(EXTI_Line17);	//清除中断线17的中断标志 											 
}

//RTC WAKE UP中断服务函数
//void RTC_WKUP_IRQHandler(void)
//{    
//	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)//WK_UP中断?
//	{ 
//		RTC_ClearFlag(RTC_FLAG_WUTF);	//清除中断标志
//		LED0_ON;
//		LED0_OFF;
//	}   
//	EXTI_ClearITPendingBit(EXTI_Line22);//清除中断线22的中断标志 								
//}
 

void printftime(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	u8 tbuf[40];
	u8 t=0;
	
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);		
	printf("Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 	
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);			
	printf("Date:20%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
	
}











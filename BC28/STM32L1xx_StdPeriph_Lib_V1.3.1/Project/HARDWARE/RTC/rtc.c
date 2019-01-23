#include "RTC.h"
#include "usart.h"
#include "led.h"
#include "exti.h"
#include "bc28.h"

NVIC_InitTypeDef   NVIC_InitStructure;
extern BC28 BC28_Status;
//RTCʱ������  
//hour,min,sec:Сʱ,����,����  
//ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM  
//����ֵ:SUCEE(1),�ɹ�  
//       ERROR(0),�����ʼ��ģʽʧ��   
ErrorStatus RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm)  
{  
    RTC_TimeTypeDef RTC_TimeTypeInitStructure;  
      
    RTC_TimeTypeInitStructure.RTC_Hours=hour;  
    RTC_TimeTypeInitStructure.RTC_Minutes=min;  
    RTC_TimeTypeInitStructure.RTC_Seconds=sec;  
    RTC_TimeTypeInitStructure.RTC_H12=ampm;  
      
    return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);  
      
}
//RTC��������  
//year,month,date:��(0~99),��(1~12),��(0~31)  
//week:����(1~7,0,�Ƿ�!)  
//����ֵ:SUCEE(1),�ɹ�  
//       ERROR(0),�����ʼ��ģʽʧ��   
ErrorStatus RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week)  
{  
      
    RTC_DateTypeDef RTC_DateTypeInitStructure;  
    RTC_DateTypeInitStructure.RTC_Date=date;  
    RTC_DateTypeInitStructure.RTC_Month=month;  
    RTC_DateTypeInitStructure.RTC_WeekDay=week;  
    RTC_DateTypeInitStructure.RTC_Year=year;  
    return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);  
	
} 
//RTC��ʼ��
//����ֵ:0,��ʼ���ɹ�;
//       1,LSE����ʧ��;
//       2,�����ʼ��ģʽʧ��;
unsigned char RTC_Config(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0X1FFF;	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��
	PWR_RTCAccessCmd(ENABLE);
	RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5051)		//�Ƿ��һ������?
	{
		RCC_LSEConfig(RCC_LSE_ON);//LSE ����    
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
			{
			retry++;
			delay_ms(10);
			}
		if(retry==0)return 1;		//LSE ����ʧ��. 
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ�� 

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC�첽��Ƶϵ��(1~0X7F)
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTCͬ����Ƶϵ��(0~7FFF)
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC����Ϊ,24Сʱ��ʽ
    RTC_Init(&RTC_InitStructure);
 
		RTC_Set_Time(BC28_Status.hours,BC28_Status.minutes,BC28_Status.seconds,RTC_H12_AM);	//����ʱ��
		RTC_Set_Date(BC28_Status.year,BC28_Status.mouth,BC28_Status.day,2);		//��������
	 
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5051);	//����Ѿ���ʼ������



	} 
 
	return 0;
}

//void RTC_AlarmConfig(void)
//{
//	char setAlarmSecond = 0;			//�������ӣ���
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


////�����Ի��Ѷ�ʱ������  
///*wksel:  @ref RTC_Wakeup_Timer_Definitions
//#define RTC_WakeUpClock_RTCCLK_Div16        ((uint32_t)0x00000000)
//#define RTC_WakeUpClock_RTCCLK_Div8         ((uint32_t)0x00000001)
//#define RTC_WakeUpClock_RTCCLK_Div4         ((uint32_t)0x00000002)
//#define RTC_WakeUpClock_RTCCLK_Div2         ((uint32_t)0x00000003)
//#define RTC_WakeUpClock_CK_SPRE_16bits      ((uint32_t)0x00000004)
//#define RTC_WakeUpClock_CK_SPRE_17bits      ((uint32_t)0x00000006)
//*/
////cnt:�Զ���װ��ֵ.����0,�����ж�.
//void RTC_Set_WakeUp(u32 wksel,u16 cnt)
//{ 
//	EXTI_InitTypeDef   EXTI_InitStructure;
//	
//	RTC_WakeUpCmd(DISABLE);//�ر�WAKE UP
//	
//	RTC_WakeUpClockConfig(wksel);//����ʱ��ѡ��
//	
//	RTC_SetWakeUpCounter(cnt);//����WAKE UP�Զ���װ�ؼĴ���
//	
//	
//	RTC_ClearITPendingBit(RTC_IT_WUT); //���RTC WAKE UP�ı�־
//  EXTI_ClearITPendingBit(EXTI_Line22);//���LINE22�ϵ��жϱ�־λ 
//	 
//	RTC_ITConfig(RTC_IT_WUT,ENABLE);//����WAKE UP ��ʱ���ж�
//	RTC_WakeUpCmd( ENABLE);//����WAKE UP ��ʱ����
//	
//	EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE22
//  EXTI_Init(&EXTI_InitStructure);//����
// 
// 
//	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
//  NVIC_Init(&NVIC_InitStructure);//����
//}


//RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A�ж�?
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);//����жϱ�־
		printf("ALARM A!\r\n");
	}   
	EXTI_ClearITPendingBit(EXTI_Line17);	//����ж���17���жϱ�־ 											 
}

//RTC WAKE UP�жϷ�����
//void RTC_WKUP_IRQHandler(void)
//{    
//	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)//WK_UP�ж�?
//	{ 
//		RTC_ClearFlag(RTC_FLAG_WUTF);	//����жϱ�־
//		LED0_ON;
//		LED0_OFF;
//	}   
//	EXTI_ClearITPendingBit(EXTI_Line22);//����ж���22���жϱ�־ 								
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











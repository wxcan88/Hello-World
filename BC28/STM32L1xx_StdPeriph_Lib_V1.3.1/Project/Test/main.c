/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    16-May-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "timer.h"
#include "BC28.h"
#include "led.h"
#include "string.h"
#include "stdio.h"
#include "delay.h"
#include "dht11.h"
#include "exti.h"
#include "RTC.h"
#include "adc.h"

void Getipdata(void);
void GetTime(void);
void GetTempdata(void);
void GetHumidata(void);
void dustDensi(void);
void SendLsens(void);

GPIO_InitTypeDef GPIO_InitStructure;
static __IO uint32_t TimingDelay;
extern unsigned char uart2_getok;

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
unsigned char sendata[100];
int main(void)
{
  if (SysTick_Config(SystemCoreClock / 1000))//设置24bit定时器 1ms中断一次
  { 
    /* Capture error */ 
    while (1);
  }
delay_init();
LED_Init();
while(DHT11_Init());//初始化DHT11

uart_init(9600);
uart2_init(9600);
uart3_init(9600);
TIM3_Int_Init(499,3199);//50ms一次中断	
TIM4_Int_Init(4999,3199);//500ms一次中断
BC28_Init();
BC28_PDPACT();
BC28_ConUDP();
	ADC_Config();
	RTC_Config();
  while (1)
  {
	
		GetTempdata();
		delay_ms(500);
		delay_ms(500);
		GetHumidata();
		delay_ms(500);
		delay_ms(500);
		dustDensi();
		delay_ms(500);
		delay_ms(500);
		SendLsens();    
		delay_ms(500);
		delay_ms(500);		
	  GPIO_ToggleBits(GPIOB,GPIO_Pin_9);

  }
}

void Getipdata(void)
{
	int ip1 = 47;
	int ip2 = 96;
	int ip3 = 118;
	int ip4 = 115;
				
	sendata[0]='6';	
	sendata[1]='9';
	sendata[2]='2';	
	sendata[3]='c';
		
	sendata[4]='3';
	sendata[5]=ip1/10%10+0x30;
	sendata[6]='3';
	sendata[7]=ip1%10+0x30;
	sendata[8]='2';
	sendata[9]='e';
	sendata[10]='3';
	sendata[11]=ip2/10%10+0x30;
	sendata[12]='3';
	sendata[13]=ip2%10+0x30;
	sendata[14]='2';
	sendata[15]='e';
	sendata[16]='3';
	sendata[17]=ip3/100%10+0x30;
	sendata[18]='3';
	sendata[19]=ip3/10%10+0x30;
	sendata[20]='3';
	sendata[21]=ip3%10+0x30;
	sendata[22]='2';
	sendata[23]='e';
	sendata[24]='3';
	sendata[25]=ip4/100%10+0x30;
	sendata[26]='3';
	sendata[27]=ip4/10%10+0x30;		
	sendata[28]='3';
	sendata[29]=ip4%10+0x30;
	sendata[30]='2';	
	sendata[31]='c';
		
	sendata[32]='7';	
	sendata[33]='3';
	sendata[34]='6';	
	sendata[35]='5';
	sendata[36]='6';	
	sendata[37]='e';
	sendata[38]='7';	
	sendata[39]='3';
	sendata[40]='6';	
	sendata[41]='f';
	sendata[42]='7';	
	sendata[43]='2';	
	sendata[44]='3';	
	sendata[45]='a';
}
void GetTempdata(void)
{
	u8 temp,humi;	
	DHT11_Read_Data(&temp,&humi);//读取温湿度数据
	Getipdata();
	sendata[46]='3';	
	sendata[47]='1';
	sendata[48]='3';	
	sendata[49]='2';
	sendata[50]='3';	
	sendata[51]='a';
  sendata[52]='3';
	sendata[53]=temp/10%10+0x30;
	sendata[54]='3';
	sendata[55]=temp%10+0x30;
	sendata[56]='2';
	sendata[57]='0';
	GetTime();
	BC28_Senddata("47",sendata);
}
void GetHumidata(void)
{
	u8 temp,humi;	
	DHT11_Read_Data(&temp,&humi);//读取温湿度数据
	Getipdata();
	sendata[46]='3';	
	sendata[47]='1';
	sendata[48]='3';	
	sendata[49]='3';
	sendata[50]='3';	
	sendata[51]='a';
  sendata[52]='3';
	sendata[53]=humi/10%10+0x30;
	sendata[54]='3';
	sendata[55]=humi%10+0x30;
	sendata[56]='2';
	sendata[57]='0';
	GetTime();
  BC28_Senddata("47",sendata);	
}
void dustDensi(void)
{
	int pm;
	float dustDensity;
	dustDensity = Get_GP2Y();
	pm=dustDensity;
	Getipdata();
	sendata[46]='3';	
	sendata[47]='1';
	sendata[48]='3';	
	sendata[49]='7';
	sendata[50]='3';	
	sendata[51]='a';
	sendata[52]='3';
	sendata[53]=pm/100%10+0x30;
	sendata[54]='3';
	sendata[55]=pm/10%10+0x30;
	sendata[56]='3';
	sendata[57]=pm%10+0x30;
	GetTime();
	BC28_Senddata("47",sendata);
}

void SendLsens(void)
{
		u16 adcx1;
		u16 led0pwmval=0;
	  adcx1=Lsens_Get_Val();
		led0pwmval=100-adcx1; 		
		TIM_SetCompare2(TIM3,led0pwmval);
	  Getipdata();
		sendata[46]='3';	
		sendata[47]='1';
		sendata[48]='3';	
		sendata[49]='1';
		sendata[50]='3';	
		sendata[51]='a';
		sendata[52]='3';
		sendata[53]=led0pwmval/10%10+0x30;
		sendata[54]='3';
		sendata[55]=led0pwmval%10+0x30;
		sendata[56]='2';
		sendata[57]='0';
		GetTime();
		BC28_Senddata("47",sendata);	
}

void GetTime(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	char tbuf3[6];
	char tbuf4[6];
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	sprintf((char*)tbuf3,"%02d%02d%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 	  
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);	
	sprintf((char*)tbuf4,"%02d%02d%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date);
		
	sendata[58]='2';
	sendata[59]='c';
	sendata[60]='3';
	sendata[61]=tbuf4[0];
	sendata[62]='3';
	sendata[63]=tbuf4[1];
	sendata[64]='2';
	sendata[65]='d';
	sendata[66]='3';
	sendata[67]=tbuf4[2];
	sendata[68]='3';
	sendata[69]=tbuf4[3];
	sendata[70]='2';
	sendata[71]='d';
	sendata[72]='3';
	sendata[73]=tbuf4[4];
	sendata[74]='3';
	sendata[75]=tbuf4[5];
	sendata[76]='2';
	sendata[77]='0';
	sendata[78]='3';
	sendata[79]=tbuf3[0];
	sendata[80]='3';
	sendata[81]=tbuf3[1];
	sendata[82]='3';
	sendata[83]='a';
	sendata[84]='3';
	sendata[85]=tbuf3[2];
	sendata[86]='3';
	sendata[87]=tbuf3[3];
	sendata[88]='3';
	sendata[89]='a';
	sendata[90]='3';
	sendata[91]=tbuf3[4];
	sendata[92]='3';
	sendata[93]=tbuf3[5];
	sendata[94]=0;
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
//void Delay(__IO uint32_t nTime)
//{ 
//  TimingDelay = nTime;

//  while(TimingDelay != 0);
//}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

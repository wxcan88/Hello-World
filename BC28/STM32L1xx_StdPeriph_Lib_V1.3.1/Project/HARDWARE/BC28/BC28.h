#include "usart.h"
#include <stm32l1xx.h>
void Clear_Buffer(void);//��ջ���	
void BC28_Init(void);
void BC28_PDPACT(void);
void BC28_ConUDP(void);
void BC28_RECData(void);
void BC28_Senddata(uint8_t *len,uint8_t *data);
typedef struct
{
    uint8_t CSQ;    
    uint8_t Socketnum;   //���
    uint8_t reclen[10];   //��ȡ�����ݵĳ���
    uint8_t res;      
    uint8_t recdatalen[10];
    uint8_t recdata[100];
    uint8_t netstatus;//����ָʾ��
			 uint8_t year;
	 uint8_t mouth;
	 uint8_t day;
   uint8_t hours;
	 uint8_t minutes;
   uint8_t seconds;
} BC28;


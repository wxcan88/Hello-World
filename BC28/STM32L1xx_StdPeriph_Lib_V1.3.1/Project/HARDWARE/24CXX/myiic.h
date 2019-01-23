#ifndef __MYIIC_H
#define __MYIIC_H
#include <stm32l1xx.h>
#define u8 unsigned char 



//IO��������	 
#define IIC_SCL_1   GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define IIC_SCL_0   GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define IIC_SDA_1   GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define IIC_SDA_0   GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define READ_SDA   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

















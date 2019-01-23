#include "BC28.h"
#include "main.h"
#include "string.h"
char *strx,*extstrx;
char *strx2;
extern char  RxBuffer[100],RxCounter;
BC28 BC28_Status;
void Clear_Buffer(void)//��ջ���
{
		u8 i;
		Uart1_SendStr(RxBuffer);
//	for(i=0;i<100;i++)
	//RxBuffer[i]=0;
		RxCounter=0;
}
void BC28_Init(void)
{
    printf("AT\r\n"); 
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
    BC28_Status.netstatus=1;//��˸ûע������
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//��460
    Clear_Buffer();	
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
    }
    printf("AT+CGATT=1\r\n");//�������磬PDP
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����ɹ�
    Clear_Buffer();	
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CGATT=1\r\n");//��������
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����ɹ�
    }
    printf("AT+CGATT?\r\n");//��ѯ����PDP
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��1
    Clear_Buffer();	
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//��ȡ����״̬
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//����1,����ע���ɹ�
    }
    printf("AT+CSQ\r\n");//�鿴��ȡCSQֵ
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//����CSQ
  if(strx)
    {
        BC28_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//��ȡCSQ
        if(BC28_Status.CSQ==99)//˵��ɨ��ʧ��
        {
            while(1)
            {
                Uart1_SendStr("�ź�����ʧ�ܣ���鿴ԭ��!\r\n");
                Delay(300);
            }
        }
        BC28_Status.netstatus=4;//ע���ɹ�
     } 
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CSQ\r\n");//�鿴��ȡCSQֵ
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//
        if(strx)
        {
            BC28_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//��ȡCSQ
            if(BC28_Status.CSQ==99)//˵��ɨ��ʧ��
            {
                while(1)
                {
                    Uart1_SendStr("�ź�����ʧ�ܣ���鿴ԭ��!\r\n");
                    Delay(300);
                }
            }
         }
    }
    Clear_Buffer();	
		
		printf("AT+CCLK?\r\n");	
		Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CCLK:");	
    if(strx)
    {
			BC28_Status.year=(strx[6]-0x30)*10+(strx[7]-0x30);
			BC28_Status.mouth=(strx[9]-0x30)*10+(strx[10]-0x30);
			BC28_Status.day=(strx[12]-0x30)*10+(strx[13]-0x30);
			BC28_Status.hours=(strx[15]-0x30)*10+(strx[16]-0x30)+8;
			BC28_Status.minutes=(strx[18]-0x30)*10+(strx[19]-0x30);
			BC28_Status.seconds=(strx[21]-0x30)*10+(strx[22]-0x30);			 		
		}
		Clear_Buffer();	
    printf("AT+CEREG?\r\n");
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//����ע��״̬
    extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//����ע��״̬
    Clear_Buffer();	
  while(strx==NULL&&extstrx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CEREG?\r\n");//�ж���Ӫ��
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//����ע��״̬
        extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//����ע��״̬
    }
    printf("AT+CEREG=1\r\n");
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
  while(strx==NULL&&extstrx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CEREG=1\r\n");//�ж���Ӫ��
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
     }
	/*	 printf("AT+COPS?\r\n");//�ж���Ӫ��
			Delay(300);
			strx=strstr((const char*)RxBuffer,(const char*)"46011");//���ص�����Ӫ��
			Clear_Buffer();	
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("AT+COPS?\r\n");//�ж���Ӫ��
			Delay(300);
			strx=strstr((const char*)RxBuffer,(const char*)"46011");//���ص�����Ӫ��
		}
		*///ż������������ ����ע���������ģ�����COPS 2,2,""�����Դ˴������ε�
}

void BC28_PDPACT(void)//�������Ϊ���ӷ�������׼��
{
    printf("AT+CGDCONT=1,\042IP\042,\042HUAWEI.COM\042\r\n");//����APN
    Delay(300);
    printf("AT+CGATT=1\r\n");//�����
    Delay(300);
    printf("AT+CGATT?\r\n");//�����
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)" +CGATT:1");//ע����������Ϣ
    Clear_Buffer();	
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//�����
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//���ص�����Ӫ��
    }
    printf("AT+CSCON?\r\n");//�ж�����״̬������1���ǳɹ�
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CSCON:0,1");//��������
    extstrx=strstr((const char*)RxBuffer,(const char*)"+CSCON:0,0");//���ӶϿ�
    Clear_Buffer();	
  while(strx==NULL&&extstrx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CSCON?\r\n");//
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CSCON:0,1");//
        extstrx=strstr((const char*)RxBuffer,(const char*)"+CSCON:0,0");//
    }
	 
}

void BC28_ConUDP(void)
{
    uint8_t i;
	  u8 flag=0;
loop:	  if(flag == 1)
		    {
			    BC28_Init();
			    BC28_PDPACT();
		    }
    printf("AT+NSOCL=0\r\n");//�ر�socekt����
    Delay(300);
    printf("AT+NSOCL=1\r\n");//�ر�socekt����
    Delay(300); 
    printf("AT+NSOCL=2\r\n");//�ر�socekt����
    Delay(300);
    Clear_Buffer();	
    printf("AT+NSOCR=DGRAM,17,3568,1\r\n");//����һ��Socket
    Delay(300);
   strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
  while(strx==NULL)
  {
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
		strx2=strstr((const char*)RxBuffer,(const char*)"ERROR");
		if(strx2)
		{
			flag=1;
			goto loop;
		}
  }
  BC28_Status.Socketnum=RxBuffer[2];
   Clear_Buffer();	
    printf("AT+NSOST=%c,47.96.118.115,12738,%c,%s\r\n",BC28_Status.Socketnum,'8',"276C441AC2311234");//����0socketIP�Ͷ˿ں������Ӧ���ݳ����Լ�����,
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
  while(strx==NULL)
  {
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
  }
    Clear_Buffer();	
    for(i=0;i<100;i++)
    RxBuffer[i]=0x00;
}
void BC28_Senddata(uint8_t *len,uint8_t *data)
{
//	 printf("AT+NSOST=%c,120.24.184.124,8010,%s,%s\r\n", BC28_Status.Socketnum,len,data);//����0 socketIP�Ͷ˿ں������Ӧ���ݳ����Լ�����,727394ACB8221234
   printf("AT+NSOST=%c,47.96.118.115,12738,%s,%s\r\n", BC28_Status.Socketnum,len,data);
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	while(strx==NULL)
	{
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	}
	Clear_Buffer();	
	
}
void BC28_RECData(void)
{
	char i;
	static char nexti;
     strx=strstr((const char*)RxBuffer,(const char*)"+NSONMI:");//����+NSONMI:���������յ�UDP���������ص�����
    if(strx)
        {
            Clear_Buffer();	
            BC28_Status.Socketnum=strx[8];//���
            for(i=0;;i++)
            {
                if(strx[i+10]==0x0D)
                    break;
                BC28_Status.reclen[i]=strx[i+10];//����
            }
            printf("AT+NSORF=%c,%s\r\n",BC28_Status.Socketnum,BC28_Status.reclen);//�����Լ����
            Delay(300);
            strx=strstr((const char*)RxBuffer,(const char*)",");//��ȡ����һ������
            strx=strstr((const char*)(strx+1),(const char*)",");//��ȡ���ڶ�������
            strx=strstr((const char*)(strx+1),(const char*)",");//��ȡ������������
        for(i=0;;i++)
        { 
            if(strx[i+1]==',')
            break;
            BC28_Status.recdatalen[i]=strx[i+1];//��ȡ���ݳ���
            
        }
            strx=strstr((const char*)(strx+1),(const char*)",");//��ȡ�����ĸ�����
        for(i=0;;i++)
        {
            if(strx[i+1]==',')
            break;
            BC28_Status.recdata[i]=strx[i+1];//��ȡ��������
        }		
       }
}
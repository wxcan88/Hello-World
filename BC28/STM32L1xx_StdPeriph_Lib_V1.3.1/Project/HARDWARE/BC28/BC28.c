#include "BC28.h"
#include "main.h"
#include "string.h"
char *strx,*extstrx;
char *strx2;
extern char  RxBuffer[100],RxCounter;
BC28 BC28_Status;
void Clear_Buffer(void)//清空缓存
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
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();	
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    }
    BC28_Status.netstatus=1;//闪烁没注册网络
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//返460
    Clear_Buffer();	
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//返回OK,说明卡是存在的
    }
    printf("AT+CGATT=1\r\n");//激活网络，PDP
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//激活成功
    Clear_Buffer();	
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CGATT=1\r\n");//激活网络
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//激活成功
    }
    printf("AT+CGATT?\r\n");//查询激活PDP
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返1
    Clear_Buffer();	
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//获取激活状态
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返回1,表明注网成功
    }
    printf("AT+CSQ\r\n");//查看获取CSQ值
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//返回CSQ
  if(strx)
    {
        BC28_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//获取CSQ
        if(BC28_Status.CSQ==99)//说明扫网失败
        {
            while(1)
            {
                Uart1_SendStr("信号搜索失败，请查看原因!\r\n");
                Delay(300);
            }
        }
        BC28_Status.netstatus=4;//注网成功
     } 
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CSQ\r\n");//查看获取CSQ值
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CSQ");//
        if(strx)
        {
            BC28_Status.CSQ=(strx[5]-0x30)*10+(strx[6]-0x30);//获取CSQ
            if(BC28_Status.CSQ==99)//说明扫网失败
            {
                while(1)
                {
                    Uart1_SendStr("信号搜索失败，请查看原因!\r\n");
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
    strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//返回注册状态
    extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//返回注册状态
    Clear_Buffer();	
  while(strx==NULL&&extstrx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CEREG?\r\n");//判断运营商
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CEREG:0,1");//返回注册状态
        extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG:1,1");//返回注册状态
    }
    printf("AT+CEREG=1\r\n");
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();	
  while(strx==NULL&&extstrx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CEREG=1\r\n");//判断运营商
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
     }
	/*	 printf("AT+COPS?\r\n");//判断运营商
			Delay(300);
			strx=strstr((const char*)RxBuffer,(const char*)"46011");//返回电信运营商
			Clear_Buffer();	
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("AT+COPS?\r\n");//判断运营商
			Delay(300);
			strx=strstr((const char*)RxBuffer,(const char*)"46011");//返回电信运营商
		}
		*///偶尔会搜索不到 但是注册是正常的，返回COPS 2,2,""，所以此处先屏蔽掉
}

void BC28_PDPACT(void)//激活场景，为连接服务器做准备
{
    printf("AT+CGDCONT=1,\042IP\042,\042HUAWEI.COM\042\r\n");//设置APN
    Delay(300);
    printf("AT+CGATT=1\r\n");//激活场景
    Delay(300);
    printf("AT+CGATT?\r\n");//激活场景
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)" +CGATT:1");//注册上网络信息
    Clear_Buffer();	
  while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//激活场景
        Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返回电信运营商
    }
    printf("AT+CSCON?\r\n");//判断连接状态，返回1就是成功
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CSCON:0,1");//正常连接
    extstrx=strstr((const char*)RxBuffer,(const char*)"+CSCON:0,0");//连接断开
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
    printf("AT+NSOCL=0\r\n");//关闭socekt连接
    Delay(300);
    printf("AT+NSOCL=1\r\n");//关闭socekt连接
    Delay(300); 
    printf("AT+NSOCL=2\r\n");//关闭socekt连接
    Delay(300);
    Clear_Buffer();	
    printf("AT+NSOCR=DGRAM,17,3568,1\r\n");//创建一个Socket
    Delay(300);
   strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
  while(strx==NULL)
  {
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
		strx2=strstr((const char*)RxBuffer,(const char*)"ERROR");
		if(strx2)
		{
			flag=1;
			goto loop;
		}
  }
  BC28_Status.Socketnum=RxBuffer[2];
   Clear_Buffer();	
    printf("AT+NSOST=%c,47.96.118.115,12738,%c,%s\r\n",BC28_Status.Socketnum,'8',"276C441AC2311234");//发送0socketIP和端口后面跟对应数据长度以及数据,
    Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
  while(strx==NULL)
  {
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
  }
    Clear_Buffer();	
    for(i=0;i<100;i++)
    RxBuffer[i]=0x00;
}
void BC28_Senddata(uint8_t *len,uint8_t *data)
{
//	 printf("AT+NSOST=%c,120.24.184.124,8010,%s,%s\r\n", BC28_Status.Socketnum,len,data);//发送0 socketIP和端口后面跟对应数据长度以及数据,727394ACB8221234
   printf("AT+NSOST=%c,47.96.118.115,12738,%s,%s\r\n", BC28_Status.Socketnum,len,data);
	Delay(300);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
	while(strx==NULL)
	{
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
	}
	Clear_Buffer();	
	
}
void BC28_RECData(void)
{
	char i;
	static char nexti;
     strx=strstr((const char*)RxBuffer,(const char*)"+NSONMI:");//返回+NSONMI:，表明接收到UDP服务器发回的数据
    if(strx)
        {
            Clear_Buffer();	
            BC28_Status.Socketnum=strx[8];//编号
            for(i=0;;i++)
            {
                if(strx[i+10]==0x0D)
                    break;
                BC28_Status.reclen[i]=strx[i+10];//长度
            }
            printf("AT+NSORF=%c,%s\r\n",BC28_Status.Socketnum,BC28_Status.reclen);//长度以及编号
            Delay(300);
            strx=strstr((const char*)RxBuffer,(const char*)",");//获取到第一个逗号
            strx=strstr((const char*)(strx+1),(const char*)",");//获取到第二个逗号
            strx=strstr((const char*)(strx+1),(const char*)",");//获取到第三个逗号
        for(i=0;;i++)
        { 
            if(strx[i+1]==',')
            break;
            BC28_Status.recdatalen[i]=strx[i+1];//获取数据长度
            
        }
            strx=strstr((const char*)(strx+1),(const char*)",");//获取到第四个逗号
        for(i=0;;i++)
        {
            if(strx[i+1]==',')
            break;
            BC28_Status.recdata[i]=strx[i+1];//获取数据内容
        }		
       }
}
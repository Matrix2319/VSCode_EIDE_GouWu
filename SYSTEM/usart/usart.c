#include "sys.h"
#include "usart.h"
#include "ui.h"

extern int tx_qian;
extern int tx_hou;

int USART_PRINTF_FLAG = 1;//默认串口2

////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{
	if(USART_PRINTF_FLAG ==  1)//uart1
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		USART_SendData(USART1,(uint8_t)ch);
	}
	if(USART_PRINTF_FLAG == 2)//uart2
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
		USART_SendData(USART2,(uint8_t)ch);
	}
	if(USART_PRINTF_FLAG == 3)//uart3
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
		USART_SendData(USART3,(uint8_t)ch);
	}
	return ch;
}
#endif 

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART1_RX_STA=0;       //接收状态标记	  
  
void uart1_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1  这里不能关，很神奇

}

u8 flag_6 = 0; // 抛去第一位数据（用于6轴）
float angle_6=0;
void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART1); // 读取接收到的数据

		if (flag_6 == 1)
		{
			flag_6 = 0;
			USART1_RX_STA = 0;// 接收错误,重新开始
		}
		else
		{
			if ((USART1_RX_STA & 0x8000) == 0) // 接收未完成
			{
				if (USART1_RX_STA & 0x4000) // 接收到了0x68
				{
					if (Res != 0x46)
						flag_6 = 1;				
					else
					{
						// sprintf(OLED_BUF, "%x", USART1_RX_BUF[6]);
						// LCD_16_HanZi_ASCII(10, 0, OLED_BUF);
						// sprintf(OLED_BUF, "%x", USART1_RX_BUF[7]);
						// LCD_16_HanZi_ASCII(10, 2, OLED_BUF);
						angle_6 = ((int)(USART1_RX_BUF[7] << 8 | USART1_RX_BUF[6]))/ 32768.0 * 180.0;
						// sprintf(OLED_BUF, "%.3f", angle_6);
						// LCD_16_HanZi_ASCII(8, 4, OLED_BUF);
						flag_6 = 1;
						USART1_RX_STA = 0;
						// 接收函数
					}
				}
				else // 还没收到0X0D
				{
					if (Res == 0x6C)
						USART1_RX_STA |= 0x4000;
					else
					{
						USART1_RX_BUF[USART1_RX_STA & 0X3FFF] = Res;
						USART1_RX_STA++;
						if (USART1_RX_STA > (USART1_REC_LEN - 1))
							USART1_RX_STA = 0; // 接收数据错误,重新开始接收
					}
				}
			}
		}
	}
    
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	
/**********************************************************
** 函数名:USART1_Putc
** 功能描述: 串口1发送一字符
** 输入参数: c
** 输出参数: 无
***********************************************************/
void USART1_Putc(unsigned char c)
{
	USART1->DR = (u8)c; //要发送的字符赋给串口数据寄存器 
	while((USART1->SR&0X40)==0); //等待发送完成 
}
/**********************************************************
** 函数名:USART2_Puts
** 功能描述: 串口1发送一字符串
** 输入参数: 指针str
** 输出参数: 无
***********************************************************/

void USART1_Puts(char * str)
{
	while(*str)
	{
		USART1->DR= *str++;
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待发送完成 ?
	}
}

#if EN_USART2_RX   //如果使能了串口2接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	  
 
void uart2_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//USART1_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
   
  //USART1_RX	  GPIOA.3初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级2  //优先级也要改哦
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口1 

}


void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
		USART_PRINTF_FLAG = 2;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART2);	//读取接收到的数据
		
		if((USART2_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART2_RX_STA&0x4000)//接收到了0x0d
				{
					if(Res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
					else 
					{
						
							//接收函数
						USART2_RX_STA=0;
					}
					
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
					{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

/**********************************************************
** 函数名:USART2_Putc
** 功能描述: 串口2发送一字符
** 输入参数: c
** 输出参数: 无
***********************************************************/
void USART2_Putc(unsigned char c)
{
	USART2->DR = (u8)c; //要发送的字符赋给串口数据寄存器 
	while((USART2->SR&0X40)==0); //等待发送完成 
}
/**********************************************************
** 函数名:USART2_Puts
** 功能描述: 串口2发送一字符串
** 输入参数: 指针str
** 输出参数: 无
***********************************************************/
void USART2_Puts(char * str)
{
	while(*str)
	{
		USART2->DR= *str++;
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);//等待发送完成 ?
	}
}

#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.#define USART_REC_LEN  			200  	//定义最大接收字节数 200
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	  
 
void uart3_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
 
	 //USART3_TX   PC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
   
	//USART3_RX	  PC.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  

	//Usart2 NVIC 配置
	//
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART3, ENABLE);                    //使能串口 

}
u16 RX3_count=0;
void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	static u8 Lcnt=0,Rcnt=0;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	//USART_PRINTF_FLAG = 3;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART3);	//读取接收到的数据
		if((USART3_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART3_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)//接收错误,重新开始
				{
					USART3_RX_STA=0;
					switch(Infoflag)
					{
						case 1:USART3_Puts("1error");break;
						case 2:USART3_Puts("2error");break;
						default:USART3_Puts("Error");break;
					}
					
				}
				else {
					
					tx_qian=(USART3_RX_BUF[0]-'0')*16+(USART3_RX_BUF[1]-'0');
					tx_hou=(USART3_RX_BUF[2]-'0')*16+(USART3_RX_BUF[3]-'0');
//					sprintf(OLED_BUF,"%d 	",tx_qian);
//					LCD_16_HanZi_ASCII(0,0,OLED_BUF);		
//					sprintf(OLED_BUF,"%d 	",tx_hou);
//					LCD_16_HanZi_ASCII(0,2,OLED_BUF);	                                                                      //接收部分
					USART3_RX_STA|=0x8000;
				}	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART3_RX_STA|=0x4000;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
						if(USART3_RX_STA>(USART3_REC_LEN-1))//接收数据错误,重新开始接收	
						{
							USART3_RX_STA=0;
							switch(Infoflag)
							{
								case 1:USART3_Puts("1error");break;
								case 2:USART3_Puts("2error");break;
								default:USART3_Puts("Error");break;
							}
						}  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}
#endif
/**********************************************************
** 函数名:USART3_Putc
** 功能描述: 串口3发送一字符
** 输入参数: c
** 输出参数: 无
***********************************************************/
void USART3_Putc(unsigned char c)
{
	USART3->DR = (u8)c; //要发送的字符赋给串口数据寄存器 
	while((USART3->SR&0X40)==0); //等待发送完成 
}
/**********************************************************
** 函数名:USART3_Puts
** 功能描述: 串口3发送一字符串
** 输入参数: 指针str
** 输出参数: 无
***********************************************************/
void USART3_Puts(char * str)
{
	while(*str)
	{
		USART3->DR= *str++;
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);//等待发送完成 ?
	}
}


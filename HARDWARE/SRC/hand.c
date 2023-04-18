#include "hand.h"
#include "timer.h"
#include "math.h"
#include <stdlib.h>
#include "gpio.h"
#include "usart.h"
#include "moto.h"
#include "usart.h"
#include "delay.h"

u8 Hand_LOW_GO(void)
{
	int cnt=0;
	LS_FL=0;
	DJ_Low_PWM=1;
	DJ_Low_Direction=0;

	while(LS_FL==1)
	{
		cnt++;
		sprintf(OLED_BUF,"%6d",cnt);
		LCD_16_HanZi_ASCII(0,6,OLED_BUF);
		if(cnt>=830) 
		{
			
			cnt=0;
			DJ_Low_PWM=0;
			DJ_Low_Direction=0;

			return 0;
			break;
		}
	}
	delay_ms(150);
	DJ_Low_PWM=0;
	DJ_Low_Direction=0;

	return 1;
	
}

u8 Hand_LOW_BACK(void)
{
	int cnt=0;
	LS_BL=0;
	DJ_Low_PWM=1;
	DJ_Low_Direction=1;

	while(LS_BL==1)
	{
		cnt++;
		sprintf(OLED_BUF,"%6d",cnt);
		LCD_16_HanZi_ASCII(0,6,OLED_BUF);
		if(cnt>=1100) 
		{
			cnt=0;
			DJ_Low_PWM=0;
			DJ_Low_Direction=1;

			return 0;
			break;
		}
	}
	
	DJ_Low_PWM=0;
	DJ_Low_Direction=1;

	return 1;
	
}

u8 Hand_HIGH_GO(void)
{
	int cnt=0;
	LS_FH==0;
	DJ_High_PWM=1;
	DJ_High_Direction=0;

	while(LS_FH==1)
	{
		cnt++;
		sprintf(OLED_BUF,"%6d",cnt);
		LCD_16_HanZi_ASCII(0,6,OLED_BUF);
		if(cnt>=655) 
		{
			cnt=0;
			DJ_High_PWM=0;
			DJ_High_Direction=0;

			return 0;
			break;
		}
	}
	delay_ms(50);
	DJ_High_PWM=0;
	DJ_High_Direction=0;

	return 1;
	
}

u8 Hand_HIGH_BACK(void)
{
	int cnt=0;
	LS_BH==0;
	DJ_High_PWM=1;
	DJ_High_Direction=1;

	while(LS_BH==1)
	{
		cnt++;
		sprintf(OLED_BUF,"%6d",cnt);
		LCD_16_HanZi_ASCII(0,6,OLED_BUF);
		if(cnt>=700) 
		{
			cnt=0;
			DJ_High_PWM=0;
			DJ_High_Direction=1;

			return 0;
			break;
		}
	}
	
	DJ_High_PWM=0;
	DJ_High_Direction=1;

	return 1;
	
}
//0种类1状态23远近45左右
void Zhua_mid(u16 i)
{
	int pianyi=0;//便宜x量
	int des=0;//种类 1小 2中 3大
	int status=0;//状态 1立 2躺横抓 3躺竖抓
	int dis=0;//距离
	int real_x=20;//舵机所在的位置(x位置)
	int flag_weizhi=0;//判断上电机在前或者后 0后 1前
	int qqq=0;
	des=i/1000;
	status=(i%1000)/100;
	dis=i%100;
	stop();
	if(des==1||des==2)//小、中AD
	{
		if(dis<10)                                        //0-10
		{
			flag_weizhi=0;
			if(status==3)//躺着
			{
					printf("$DGT:15-18,1!");
				//delay_ms(500);
			}
			else if(status==1)//竖着
			{
				printf("$DGT:57-60,1!");//10cm内竖抓
			}
			else if(status==2)
			{
				printf("$DGT:34-38,1!");
			}
		}
		else if(dis>=10&&dis<20)                          //10-20
		{
			flag_weizhi=0;
			if(status==3)//躺着
			{
					printf("$DGT:19-22,1!");
				//delay_ms(500);
			}
			else if(status==1)//竖着
			{
				printf("$DGT:62-65,1!");//10cm内竖抓
			}
			else if(status==2)
			{
				printf("$DGT:39-44,1!");
			}
		}
		
		
		
		else if(dis>=20&&dis<30)													//15-25
		{
			flag_weizhi=1;
			Hand_HIGH_GO();
			delay_ms(500);
			if(status==3)//躺着
			{
					printf("$DGT:23-25,1!");
					qqq=6;
				//delay_ms(500);
			}
			else if(status==1)//竖着
			{
				printf("$DGT:66-68,1!");//10cm内竖抓
				qqq=2;
			}
			else if(status==2)
			{
				printf("$DGT:45-48,1!");
				qqq=4;
			}
		}
		else if(dis>=30&&dis<=35)													//25-35
		{
			flag_weizhi=1;
			Hand_HIGH_GO();
			delay_ms(500);
			if(status==3)//躺着
			{
					printf("$DGT:28-30,1!");
					qqq=5;
				//delay_ms(500);
			}
			else if(status==1)//竖着
			{
				printf("$DGT:71-73,1!");//10cm内竖抓
				qqq=1;
			}
			else if(status==2)
			{
				printf("$DGT:51-54,1!");
				qqq=3;
			}
		}
//		else if(dis>35&&status==1)
//		{
//			flag_weizhi=1;
//			Hand_HIGH_GO();
//			delay_ms(500);
//			printf("$DGT:75-80,1!");//30-40cm内竖抓
//		}
	}
	for(i=0;i<4;i++)
	{
		delay_ms(1000);
	}
	
	//收回部分
	if(flag_weizhi==1)
	{
		Hand_HIGH_BACK();
		delay_ms(500);
	}
	if(qqq==1)
	{
		printf("$DGT:74-75,1!");
	}
	else if(qqq==2)
	{
		printf("$DGT:69-70,1!");
	}
	else if(qqq==3)
	{
		printf("$DGT:55-56,1!");
	}
	else if(qqq==4)
	{
		printf("$DGT:49-50,1!");
	}
	else if(qqq==5)
	{
		printf("$DGT:31-32,1!");
	}
	else if(qqq==6)
	{
		printf("$DGT:26-27,1!");
	}
	for(i=0;i<4;i++)
	{
		delay_ms(1000);
	}
	
	//板抬板下
	printf("$DGT:8-8,1!");
	delay_ms(1000);
	printf("$DGT:9-9,1!");
	delay_ms(500);
	
	printf("$DGT:12-12,1!");
	delay_ms(500);
	
}

void Zhua(u16 i)//抓取动作，i为臂指向 收0 张1
{
	u8 zlow=0,zhigh=0;
	zlow=i/16;zhigh=i%16;
	if((zhigh>0&&zhigh<5)||(zlow>0&&zlow<5))
	{
		Hand_LOW_GO();
		if(zhigh>0&&zhigh<5)
		{
			switch (zhigh)		//上层
			{
				case 1:printf("$DGT:4-4,1!");break;
				case 2:printf("$DGT:4-4,1!");break;
				case 3:printf("$DGT:4-4,1!");break;
				case 4:printf("$DGT:2-2,1!");delay_ms(1000);printf("$DGT:11-11,1!");delay_ms(1000);printf("$DGT:8-8,1!");delay_ms(1000);printf("$DGT:9-9,1!");delay_ms(500);printf("$DGT:12-12,1!");delay_ms(1000);printf("$DGT:3-3,1!");break;
				
				default:break;
			}
			delay_ms(1000);
			printf("$DGT:11-11,1!");
			delay_ms(1000);
			printf("$DGT:8-8,1!");
			delay_ms(1000);
			printf("$DGT:9-9,1!");
			delay_ms(500);

			
		}
		
		if(zlow>0&&zlow<5)
		{
			if(!(zhigh>0&&zhigh<5))
			{
				printf("$DGT:11-11,1!");
				delay_ms(1000);
			}
			switch (zlow)		//下层
			{
				case 1:printf("$DGT:7-7,1!");break;
				case 2:printf("$DGT:7-7,1!");break;
				case 3:printf("$DGT:7-7,1!");break;
				case 4:printf("$DGT:5-5,1!");delay_ms(1000);printf("$DGT:8-8,1!");delay_ms(1000);printf("$DGT:9-9,1!");delay_ms(1000);printf("$DGT:6-6,1!");break;
				
				default:break;
			}
			delay_ms(1000);
			printf("$DGT:8-8,1!");
			delay_ms(1000);
			printf("$DGT:9-9,1!");
			delay_ms(500);
			
			
		}
		printf("$DGT:12-12,1!");
		delay_ms(1000);
		printf("$DGT:1-1,1!");
		delay_ms(1000);
		Hand_LOW_BACK();
	}
		
	
//	if(Hand_LOW_GO()==1)
//	{
//		delay_ms(500);
//		if(zhigh>0&&zhigh<5)
//		{
//			switch (zhigh)		//上层
//			{
//				case 1:printf("$DGT:15-15,1!");break;
//				
//				case 2:printf("$DGT:16-16,1!");break;
//				case 3:printf("$DGT:17-17,1!");break;
//				
//				case 4:
//							 printf("$DGT:15-15,1!");
//							 delay_ms(1000);
//							 printf("$DGT:22-22,1!");
//							 delay_ms(500);
//							 printf("$DGT:21-21,1!");
//							 delay_ms(500);
//							 printf("$DGT:4-4,1!");
//							 delay_ms(500);
//							 printf("$DGT:3-3,1!");
//							 delay_ms(500);
//							 printf("$DGT:6-6,1!");break;
//				
//				default:break;
//			}
//			delay_ms(1000);
//			printf("$DGT:22-22,1!");
//		  delay_ms(500);
//		  printf("$DGT:21-21,1!");
//		  delay_ms(500);
//			printf("$DGT:4-4,1!");
//			delay_ms(500);
//			printf("$DGT:3-3,1!");
//			delay_ms(500);
//		}	
//		if(zlow>0&&zlow<=5)
//		{
//			switch (zlow)//下层
//			{
//				case 1:printf("$DGT:11-11,1!");break;
//				
//				case 2:printf("$DGT:12-12,1!");break;
//				case 3:printf("$DGT:13-13,1!");break;
//				
//				case 4:printf("$DGT:11-11,1!");
//							 delay_ms(1000);
//							 printf("$DGT:22-22,1!");
//							 delay_ms(500);
//							 printf("$DGT:21-21,1!");
//							 delay_ms(500);
//							 printf("$DGT:4-4,1!");
//							 delay_ms(500);
//							 printf("$DGT:3-3,1!");
//							 delay_ms(500);
//							 printf("$DGT:9-9,1!");break;	
//				default:break;
//			}
//			delay_ms(1000);
//			printf("$DGT:22-22,1!");
//		  delay_ms(500);
//		  printf("$DGT:21-21,1!");
//		  delay_ms(500);
//			printf("$DGT:4-4,1!");
//			delay_ms(500);
//			printf("$DGT:3-3,1!");
//			delay_ms(500);
//		}
//		
//	}
//	printf("$DGT:1-1,1!");
//	delay_ms(500);
//	
//	Hand_LOW_BACK();
	
	
}

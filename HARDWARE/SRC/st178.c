#include "st178.h"

#include "moto.h"
#include "delay.h"
#include "gpio.h"
#include "oled.h"
#include "math.h"
#include "move.h"

u8 oled_st188 = 0;

u8 DEF_FLAGE=1;

int chazhi_qh = 0;
int chazhi_zy = 0;

u8 q_q = 0;
u8 h_h = 0;
u8 q_q_q = 0;
u8 h_h_h = 0;
u8 z_z = 0;
u8 y_y = 0;
u8 z_z_z = 0;
u8 y_y_y = 0;


u8 BX_FLAGE = 0;

int st188_date1sum[2];
int st188_date2sum[2];
int st188_date3sum[2];
int st188_date4sum[2];

int qian_sum;
int hou_sum;
int zuo_sum;
int you_sum;

u8 shu_q[16];
u8 shu_h[16];
u8 shu_z[8];
u8 shu_y[8];

//u16 a,b,c,d, e,f,g,h;

static u8 baixian;

s8 St178_Scanf(u8 fi)		//1可以转弯0
{
	u8 bx1 = 0;
	int i,t;
	z_z = 0;
	y_y = 0;
	z_z_z = 0;
	y_y_y = 0;

//	static u8 temp;
		
	for(t=0;t<1;t++)
	{	 
		st188_date2sum[t] = 0;
		st188_date3sum[t] = 0;

		for(i=0;i<=7;i++)
		{
			St188_CS1=1;
			St188_CS2=1;
			St188_CS3=1;
			St188_CS4=1;
			
			St188_A=i%2;
			St188_B=i/2%2;
			St188_C=i/2/2%2;

			St188_CS2=0;
			St188_CS3=1;

			if(St188_Y2_1==1)	{    //1是黑
				shu_y[i] = 0;
			}
			else{
				shu_y[i] = 1;
				if(i == 3||i == 4)y_y_y++;
				bx1++;
				y_y++;	
			}

			St188_CS2=1;
			St188_CS3=0;

			if(St188_Y3_1==1){
				shu_z[i] = 0;
			}
			else {
				shu_z[i] = 1;
				if(i == 3||i == 4)z_z_z++;
				z_z++;	
			}
		}
		
		//前 8 4 2 1 -1 -2 -4 -6 -8 后
		
		//左
		//右
			
		for(i = 0;i <= 3;i++)
		{
			st188_date2sum[t] = st188_date2sum[t] + shu_z[i] * pow(2,3-i);			
			st188_date2sum[t] = st188_date2sum[t] - shu_z[i+4] * pow(2,i);				
			st188_date3sum[t] = st188_date3sum[t] + shu_y[i+4] * pow(2,i);	
			st188_date3sum[t] = st188_date3sum[t] - shu_y[i] * pow(2,3-i);			
		}			
		if((z_z == 1) && ((st188_date2sum[t] == 8 && shu_z[0] == 1)||(st188_date2sum[t] == -8 && shu_z[7] == 1)))
		{
			st188_date2sum[t] = st188_date2sum[t]*3;
		}
		if((y_y == 1) && ((st188_date3sum[t] == 8 && shu_y[7] == 1)||(st188_date3sum[t] == -8 && shu_y[0] == 1)))
		{
			st188_date3sum[t] = st188_date3sum[t]*3;
		}
	}
	
	if(oled_st188 == 1)
	{
		sprintf(OLED_BUF,"%d  ",st188_date2sum[0]);
		LCD_16_HanZi_ASCII(10,2,OLED_BUF);
		sprintf(OLED_BUF,"%d  ",st188_date3sum[0]);
		LCD_16_HanZi_ASCII(10,4,OLED_BUF);
				
		for(i=0;i<=7;i++)
		{
			sprintf(OLED_BUF,"%d",shu_z[i]);
			LCD_16_HanZi_ASCII(i,2,OLED_BUF);
			
			sprintf(OLED_BUF,"%d",shu_y[i]);
			LCD_16_HanZi_ASCII(i,4,OLED_BUF);
		}
	}
	
	//-8  -4  -2  -1  1  2  4   8
	//  -12 -6  -3   0  3  6  12		两个灯
	//   -14  -7  -2  2  7  14			三个灯
	
	if(z_z == 3)                                       //很神奇
	{
		if(st188_date2sum[0] > 0)	
		{
			st188_date2sum[0] = st188_date2sum[0]-3;
			
			if(st188_date2sum[0] < 0)
				st188_date2sum[0] = -st188_date2sum[0];
		}
			
		else if(st188_date2sum[0] < 0)	
		{
			st188_date2sum[0] = st188_date2sum[0]+3;
			
			if(st188_date2sum[0] > 0)
				st188_date2sum[0] = -st188_date2sum[0];
		}
	}
	
	if(y_y == 3)
	{
		if(st188_date3sum[0] > 0)	
		{
			st188_date3sum[0] = st188_date3sum[0]-3;
			
			if(st188_date3sum[0] < 0)
				st188_date3sum[0] = -st188_date3sum[0];
		}
			
		else if(st188_date3sum[0] < 0)	
		{
			st188_date3sum[0] = st188_date3sum[0]+3;
			
			if(st188_date3sum[0] > 0)
				st188_date3sum[0] = -st188_date3sum[0];
		}
	}
	
	chazhi_zy = st188_date2sum[0] - st188_date3sum[0];
	
	zuo_sum = st188_date2sum[0];
	you_sum = st188_date3sum[0];
	
	if(FX==1)
	{
		if(((z_z>=1&&z_z<5)&&(y_y>=1&&y_y<5))||((z_z>=1&&z_z<5)&&y_y>4))	//&& temp == 0
		{		
			//temp = bx1;
				if((zuo_sum < 3 && zuo_sum >-2))//(zuo_sum < 3 && zuo_sum > -2)||
					
					return 1;		//可以转弯
						
				if(zuo_sum >=3)
					
					return 3;		//过于靠后
				
				if(zuo_sum <=-2 )
					
					return 4;		//过于靠前

		}	
		else if((y_y>=1&&y_y<5)&&z_z>4)	//&& temp == 0(z_z>=1&&z_z<5)||
		{		
			//temp = bx1;
				if((you_sum < 3 && you_sum > -2))//||(you_sum < 3 && you_sum >-2)
					
					return 1;		//可以转弯
						
				if(you_sum >=3)
					
					return 3;		//过于靠后
				
				if(you_sum <= -2 )
					
					return 4;		//过于靠前

		}	
	}
	else if(FX==0)
	{
		if(((z_z>=1&&z_z<5)&&(y_y>=1&&y_y<5))||((z_z>=1&&z_z<5)&&y_y>4))	//&& temp == 0||
		{		
			//temp = bx1;
				if((zuo_sum < 2 && zuo_sum >-3))//||(you_sum <= 3 && you_sum >= 0)
					
					return 1;		//可以转弯
						
				if(zuo_sum >=3)
					
					return 3;		//过于靠后
				
				if(zuo_sum <=-3  )
					
					return 4;		//过于靠前

		}	
		else if((y_y>=1&&y_y<5)&&z_z>4)
		{
			if((you_sum < 2 && you_sum >= -3))//||(you_sum <= 3 && you_sum >= 0)||(you_sum < 2 && you_sum >-3)
					
					return 1;		//可以转弯
						
				if(you_sum >=3)
					
					return 3;		//过于靠后
				
				if(you_sum <= -3 )
					
					return 4;		//过于靠前
		}

//		if(FX_flag==3||FX_flag==0||FX_flag==1)
//		{
//			if((z_z>=1&&z_z<5)&&(y_y>=1&&y_y<5))	//&& temp == 0
//			{		
//					if((zuo_sum <=-9 && zuo_sum >= -24))//||(you_sum <= 3 && you_sum >= 0)
//						
//						return 1;		//可以转弯
//							
//					if(you_sum >=-9||zuo_sum >=-9)
//						
//						return 3;		//过于靠后
//					
//					if(you_sum <-24 ||zuo_sum <-24)
//						
//						return 4;		//过于靠前

//			}	
//		}
//		else if(FX_flag==4)
//		{
//			if((z_z>=1&&z_z<5)&&(y_y>=1&&y_y<5))	//&& temp == 0
//			{		
//					if((zuo_sum <-9 && zuo_sum >= -24))//||(you_sum <= 3 && you_sum >= 0)
//						
//						return 1;		//可以转弯
//							
//					if(you_sum >=-9||zuo_sum >=-9)
//						
//						return 3;		//过于靠后
//					
//					if(you_sum <-24 ||zuo_sum <-24)
//						
//						return 4;		//过于靠前

//			}	
//		}
		
	}

	return 0;
			
}

s8 St188_Scanf(u8 forward)		//前后
{
	u8 bx = 0;
	int canshu[8]={-4,-2,-1,1,2,4,6,8};
	int i,t;
	static u8 tem;
	
	if(forward == 5)
		baixian = 0;
	
	if(FX == 0)
	{
		for(t=0;t<1;t++)
		{	 
			st188_date1sum[t] = 0;
			st188_date4sum[t] = 0;

			for(i=0;i<=7;i++)
			{
				St188_CS1=1;
				St188_CS2=1;
				St188_CS3=1;
				St188_CS4=1;
				
				St188_A=i%2;
				St188_B=i/2%2;
				St188_C=i/2/2%2;

				St188_CS1=0;
				St188_CS4=1;

				if(St188_Y1_1==1)	shu_h[i] = 0;
				else				shu_h[i] = 1;

				St188_CS1=1;
				St188_CS4=0;

				if(St188_Y4_1==1)  	shu_q[i] = 0;
				else				shu_q[i] = 1;
			}
			
			//左 -8  -4  -2  -1  1  2  4  8 右
				
			for(i = 0;i <= 7;i++)
			{			
				st188_date1sum[t] = st188_date1sum[t] + shu_q[i] * canshu[7-i];
//				st188_date1sum[t] = st188_date1sum[t] - shu_q[i] * 8;
//				st188_date1sum[t] = st188_date1sum[t] - shu_q[i] * 4;
//				st188_date1sum[t] = st188_date1sum[t] - shu_q[i] * 2;
//				st188_date1sum[t] = st188_date1sum[t] - shu_q[i] * 1;
//				st188_date1sum[t] = st188_date1sum[t] + shu_q[i] * 1;
//				st188_date1sum[t] = st188_date1sum[t] + shu_q[i] * 2;
//				st188_date1sum[t] = st188_date1sum[t] + shu_q[i] * 4;
				
				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * canshu[i];
//				st188_date4sum[t] = st188_date4sum[t] - shu_h[i] * 2;
//				st188_date4sum[t] = st188_date4sum[t] - shu_h[i] * 1;
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * 1;
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * 2;
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * 4;
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * 8;
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * 16;
				
//				st188_date1sum[t] = st188_date1sum[t] - shu_q[i] * pow(2,3-i);			
//				st188_date1sum[t] = st188_date1sum[t] + shu_q[i+4] * pow(2,i);				
//				st188_date4sum[t] = st188_date4sum[t] - shu_h[i+4] * pow(2,i);	
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * pow(2,3-i);			
			}
//			for(i = 0;i <= 3;i++)
//			{			
//				st188_date1sum[t] = st188_date1sum[t] + shu_q[i] * pow(2,3-i);			
//				st188_date1sum[t] = st188_date1sum[t] - shu_q[i+4] * pow(2,i);				
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i+4] * pow(2,i);	
//				st188_date4sum[t] = st188_date4sum[t] - shu_h[i] * pow(2,3-i);			
//			}
			if((st188_date1sum[t] == 8 && shu_q[0] == 1)||(st188_date1sum[t] == -8 && shu_q[7] == 1))
			{
				st188_date1sum[t] = st188_date1sum[t]*3;
			}
			if((st188_date4sum[t] == 8 && shu_h[7] == 1)||(st188_date4sum[t] == -8 && shu_h[0] == 1))
			{
				st188_date4sum[t] = st188_date4sum[t]*3;
			}
		}
		bx = 0;
		q_q = 0;
		h_h = 0;
		q_q_q = 0;
		h_h_h = 0;
		for(i = 0;i <= 7;i++)
		{
			if(shu_q[i]==1)
			{
				
				if(i>=2&&i<=5)//
					q_q_q++;
				
				q_q++;
			}
			if(shu_h[i]==1)
			{
				bx++;
				if(i>=2&&i<=5)//
					h_h_h++;
				h_h++;
			}
		}
		
		//-8  -4  -2  -1  1  2  4   8
		//  -12 -6  -3   0  3  6  12		两个灯
		//   -14  -7  -2  2  7  14			三个灯
		
		if(q_q == 3)
		{
			if(st188_date1sum[0] > 0)	
			{
				st188_date1sum[0] = st188_date1sum[0]-3;
				
				if(st188_date1sum[0] < 0)
					st188_date1sum[0] = -st188_date1sum[0];
			}
				
			else if(st188_date1sum[0] < 0)	
			{
				st188_date1sum[0] = st188_date1sum[0]+3;
				
				if(st188_date1sum[0] > 0)
					st188_date1sum[0] = -st188_date1sum[0];
			}
		}
		if(h_h == 3)
		{
			if(st188_date4sum[0] > 0)	
			{
				st188_date4sum[0] = st188_date4sum[0]-3;
				
				if(st188_date4sum[0] < 0)
					st188_date4sum[0] = -st188_date4sum[0];
			}
				
			else if(st188_date4sum[0] < 0)	
			{
				st188_date4sum[0] = st188_date4sum[0]+3;
				
				if(st188_date4sum[0] > 0)
					st188_date4sum[0] = -st188_date4sum[0];
			}
		}
		qian_sum = st188_date1sum[0];
		hou_sum  = st188_date4sum[0];
		chazhi_qh = st188_date1sum[0] - st188_date4sum[0];
		
		if(oled_st188 == 1)
		{
			sprintf(OLED_BUF,"%d  ",st188_date1sum[0]);
			LCD_16_HanZi_ASCII(10,0,OLED_BUF);
			sprintf(OLED_BUF,"%d  ",st188_date4sum[0]);
			LCD_16_HanZi_ASCII(10,6,OLED_BUF);

			for(i=0;i<=7;i++)
			{
				sprintf(OLED_BUF,"%d",shu_q[i]);
				LCD_16_HanZi_ASCII(i,0,OLED_BUF);

				sprintf(OLED_BUF,"%d",shu_h[i]);
				LCD_16_HanZi_ASCII(i,6,OLED_BUF);
			}

		}
		
		
		if(h_h<=2)
		{
			tem = 0;
		}
		if((z_z <= 0||y_y <= 0)&&tem == 0)
		{				
			if(bx >= 6)	
			{
				BX_FLAGE = 1;
				tem = 1;
			}			
			else	
				BX_FLAGE = 0;		
		}
		else	BX_FLAGE = 0;
			
		if(BX_FLAGE == 1)
		{
			baixian++;
		}
		return baixian;	
		
	}
	
	
	else if(FX == 1||FX == 2||FX == 3)
	{
		for(t=0;t<1;t++)
		{	 
			st188_date1sum[t] = 0;
			st188_date4sum[t] = 0;

			for(i=0;i<=7;i++)
			{
				St188_CS1=1;
				St188_CS2=1;
				St188_CS3=1;
				St188_CS4=1;
				
				St188_A=i%2;
				St188_B=i/2%2;
				St188_C=i/2/2%2;

				St188_CS1=0;
				St188_CS4=1;

				if(St188_Y1_1==1)	shu_h[i] = 0;
				else				shu_h[i] = 1;

				St188_CS1=1;
				St188_CS4=0;

				if(St188_Y4_1==1)  	shu_q[i] = 0;
				else				shu_q[i] = 1;
			}
			
			//左 8  4  2  1  -1  -2  -4  -8 右
			//左 4  2  1  -1  -2  -4  -8  -16 右   
			//char canshu[8]={-4,-2,-1,1,2,4,6,8};
			for(i = 0;i <= 7;i++)
			{			
				st188_date1sum[t] = st188_date1sum[t] - shu_q[i] * canshu[7-i];
//				st188_date1sum[t] = st188_date1sum[t] - shu_q[i] * 8;
//				st188_date1sum[t] = st188_date1sum[t] - shu_q[i] * 4;
//				st188_date1sum[t] = st188_date1sum[t] - shu_q[i] * 2;
//				st188_date1sum[t] = st188_date1sum[t] - shu_q[i] * 1;
//				st188_date1sum[t] = st188_date1sum[t] + shu_q[i] * 1;
//				st188_date1sum[t] = st188_date1sum[t] + shu_q[i] * 2;
//				st188_date1sum[t] = st188_date1sum[t] + shu_q[i] * 4;
				
				st188_date4sum[t] = st188_date4sum[t] - shu_h[i] * canshu[i];
//				st188_date4sum[t] = st188_date4sum[t] - shu_h[i] * 2;
//				st188_date4sum[t] = st188_date4sum[t] - shu_h[i] * 1;
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * 1;
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * 2;
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * 4;
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * 8;
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * 16;
				
//				st188_date1sum[t] = st188_date1sum[t] - shu_q[i] * pow(2,3-i);			
//				st188_date1sum[t] = st188_date1sum[t] + shu_q[i+4] * pow(2,i);				
//				st188_date4sum[t] = st188_date4sum[t] - shu_h[i+4] * pow(2,i);	
//				st188_date4sum[t] = st188_date4sum[t] + shu_h[i] * pow(2,3-i);			
			}
			if((st188_date1sum[t] == -8 && shu_q[0] == 1))//||(st188_date1sum[t] == 4 && shu_q[7] == 1)
			{
				st188_date1sum[t] = st188_date1sum[t]*3;
			}
			if((st188_date4sum[t] == -8 && shu_h[7] == 1))//||(st188_date4sum[t] == 4 && shu_h[0] == 1)
			{
				st188_date4sum[t] = st188_date4sum[t]*3;
			}
//			if((st188_date1sum[t] == -8 && shu_q[0] == 1)||(st188_date1sum[t] == 8 && shu_q[7] == 1))
//			{
//				st188_date1sum[t] = st188_date1sum[t]*3;
//			}
//			if((st188_date4sum[t] == -8 && shu_h[7] == 1)||(st188_date4sum[t] == 8 && shu_h[0] == 1))
//			{
//				st188_date4sum[t] = st188_date4sum[t]*3;
//			}
		}
		
		bx = 0;
		q_q = 0;
		h_h = 0;
		q_q_q = 0;
		h_h_h = 0;
		for(i = 0;i <= 7;i++)
		{
			if(shu_q[i]==1)
			{
//				if(i>=3&&i<=4)//
//					q_q_q++;
				bx++;
				q_q++;
			}
			if(shu_h[i]==1)
			{
				if(i>=3&&i<=4)//
					h_h_h++;
				h_h++;
			}
		}
		
		//-8  -4  -2  -1  1  2  4   8
		//  -12 -6  -3   0  3  6  12		两个灯
		//   -14  -7  -2  2  7  14			三个灯
		if(q_q>3)
			st188_date1sum[0]=0;
		if(h_h>3)
			st188_date4sum[0]=0;
//		if(q_q == 3)
//		{
//			if(st188_date1sum[0] > 0)	
//			{
//				st188_date1sum[0] = st188_date1sum[0]-3;
//				
//				if(st188_date1sum[0] < 0)
//					st188_date1sum[0] = -st188_date1sum[0];
//			}
//				
//			else if(st188_date1sum[0] < 0)	
//			{
//				st188_date1sum[0] = st188_date1sum[0]+3;
//				
//				if(st188_date1sum[0] > 0)
//					st188_date1sum[0] = -st188_date1sum[0];
//			}
//		}
//		if(h_h == 3)
//		{
//			if(st188_date4sum[0] > 0)	
//			{
//				st188_date4sum[0] = st188_date4sum[0]-3;
//				
//				if(st188_date4sum[0] < 0)
//					st188_date4sum[0] = -st188_date4sum[0];
//			}
//				
//			else if(st188_date4sum[0] < 0)	
//			{
//				st188_date4sum[0] = st188_date4sum[0]+3;
//				
//				if(st188_date4sum[0] > 0)
//					st188_date4sum[0] = -st188_date4sum[0];
//			}
//		}
		qian_sum = st188_date1sum[0];
		hou_sum  = st188_date4sum[0];
		chazhi_qh = st188_date4sum[0] - st188_date1sum[0];
		
		if(oled_st188 == 1)
		{
			sprintf(OLED_BUF,"%d  ",st188_date1sum[0]);
			LCD_16_HanZi_ASCII(10,0,OLED_BUF);
			sprintf(OLED_BUF,"%d  ",st188_date4sum[0]);
			LCD_16_HanZi_ASCII(10,6,OLED_BUF);

			for(i=0;i<=7;i++)
			{
				sprintf(OLED_BUF,"%d",shu_q[i]);
				LCD_16_HanZi_ASCII(i,0,OLED_BUF);

				sprintf(OLED_BUF,"%d",shu_h[i]);
				LCD_16_HanZi_ASCII(i,6,OLED_BUF);
			}

		}
		
		if(q_q<=3)//&&h_h<=5
		{
			tem = 0;
		}
		if((z_z <= 0||y_y <= 0)&&tem == 0)		//z_z == 0&&y_y == 0
		{				
			if(bx >= 6)	
			{
				BX_FLAGE = 1;
				tem = 1;
			}
				
			else	
				BX_FLAGE = 0;		
		}
		else	BX_FLAGE = 0;
		
		if(BX_FLAGE == 1)
		{
			baixian++;
		}
		
		return baixian;		
		
	}
	if(FX == 2||FX == 3)return 0;
	return baixian;	
}
void bxcls(void)
{
	baixian = 0;
}





/*
  ******************************************************************************
  * @整理            雷必成(668484)
  * @STM32库版本     V3.5.0
  * @最后修改日期    2016-6-16
  * @功能            OLED的初始化，引配置请参见IO_config.h
  * @使用            显示汉字与ASCII混合用LCD_16_HanZi_ASCII函数
                     可以显示的汉字在ST16x16_Idx[]数组中定义，并ST16x16[]数组中存入点阵字库
  ******************************************************************************
*/

#include "oled.h"
#include "oledfont.h"


char OLED_BUF[64];//OLED显示缓冲区每行16个字符，共4行。

	#define XLevelL		0x00
	#define XLevelH		0x10
	#define XLevel		((XLevelH&0x0F)*16+XLevelL)
	#define Max_Column	128
	#define Max_Row		  64
	#define	Brightness	0xCF
	
	
	
	//#define D0 GPIO_Pin_2	  //已经在IO_config.h中定义过了
	//#define D1 GPIO_Pin_1
	//#define RST GPIO_Pin_11
	//#define DC GPIO_Pin_10
	//#define OLED_GPIO GPIOB
	
	
	/*
	4线SPI使用说明：
	VBT 供内部DC-DC电压，3.3~4.3V，如果使用5V电压，为保险起见串一个100~500欧的电阻
	VCC 供内部逻辑电压 1.8~6V
	GND 地
	
	BS0 低电平
	BS1 低电平
	BS2 低电平
	
	CS  片选管脚
	DC  命令数据选择管脚
	RES 模块复位管脚
	D0（SCLK） ，时钟脚，由MCU控制
	D1（MOSI） ，主输出从输入数据脚，由MCU控制
	
	D2 悬空
	D3-D7 ， 低电平 ， 也可悬空，但最好设为低电平
	RD  低电平 ，也可悬空，但最好设为低电平
	RW  低电平 ，也可悬空，但最好设为低电平
	RD  低电平 ，也可悬空，但最好设为低电平
	C7---D0（SCLK）时钟脚，由MCU控制
	C6---D1（MOSI）主输出从输入数据脚，由MCU控制
	C0---RST 模块复位管脚
	C4---DC  命令数据选择管脚*/
	
	/**************************** 引脚定义 ***********************************
															PE2    RST
															PE2    DC
															PE3    D1
															PE4    D0
	*************************************************************************/ 
	#define X_WIDTH 128
	#define Y_WIDTH 64
	//======================================
	
	/*
	A13---D0（SCLK）时钟脚，由MCU控制    13
	A12---D1（MOSI）主输出从输入数据脚，由MCU控制   12
	A15---RST 模块复位管脚  15
	A14---DC  命令数据选择管脚     14*/  
	void OLED_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
		
		GPIO_InitStructure.GPIO_Pin  = DC|D0|D1|RST|CS|GPIO_Pin_0|GPIO_Pin_1;
		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
		
		
		GPIO_Init(OLED_GPIO, &GPIO_InitStructure);
		
		CSLOW();
		DCHIGH();
		CLKHIGH();

		RESLOW();
		LCD_DLY_ms(50);
		RESHIGH();
	
		LCD_WrCmd(0xae);//--turn off oled panel
		LCD_WrCmd(0x00);//---set low column address
		LCD_WrCmd(0x10);//---set high column address
		LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
		LCD_WrCmd(0x81);//--set contrast control register
		LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
		LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
		LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
		//   LCD_WrCmd(0xa0);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
		//   LCD_WrCmd(0xc0);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
		LCD_WrCmd(0xa6);//--set normal display
		LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
		LCD_WrCmd(0x3f);//--1/64 duty
		LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
		LCD_WrCmd(0x00);//-not offset
		LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
		LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
		LCD_WrCmd(0xd9);//--set pre-charge period
		LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
		LCD_WrCmd(0xda);//--set com pins hardware configuration
		LCD_WrCmd(0x12);
		LCD_WrCmd(0xdb);//--set vcomh
		LCD_WrCmd(0x40);//Set VCOM Deselect Level
		LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
		LCD_WrCmd(0x02);//
		LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
		LCD_WrCmd(0x14);//--set(0x10) disable
		LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
		LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
		LCD_WrCmd(0xaf);//--turn on oled panel
		OLCD_Fill(0x00);  //初始清屏
		LCD_Set_Pos(0,0);
	//	TZXH_LOGO();
	//	DZXH_LOGO();
	}
	/*
	A26---D0（SCLK）时钟脚，由MCU控制    13
	A27---D1（MOSI）主输出从输入数据脚，由MCU控制   12
	A24---RST 模块复位管脚  15
	A25---DC  命令数据选择管脚     14*/  
	void LCD_WrDat(byte data)
	{
		byte i=8;
		CSLOW();
		//LCD_CS=0;;
		DCHIGH();;;;//DC=1
		  __NOP();
		 CLKLOW();;;;//D0=0
		 __NOP();
	  while(i--)
	  {
		if(data&0x80)
			{MISOHIGH();;;;}//D1=1
		else
			{MISOLOW();;;;}//D1==0
	   CLKHIGH();//D0=1
	   __NOP();;;;
			//asm("nop");
	   CLKLOW();;;;;//D0==0
		data<<=1;
	  }
		//LCD_CS=1;
	}
	
	/*
	A26---D0（SCLK）时钟脚，由MCU控制 E4
	A27---D1（MOSI）主输出从输入数据脚，由MCU控制 E3
	A24---RST 模块复位管脚 E6
	A25---DC  命令数据选择管脚  E5 */
	void LCD_WrCmd(byte cmd)
	{
		byte i=8;
		CSLOW();
		//LCD_CS=0;;
	DCLOW();;;;;
	 CLKLOW();;;;;
	  //asm("nop");
	  
	  while(i--)
	  {
		if(cmd&0x80)
			{MISOHIGH();;;;;}
		else
			{MISOLOW();;;;;;}
		CLKHIGH();;;;;
		__NOP();;;;
			//asm("nop");
	   CLKLOW();;;;;
		cmd<<=1;;;;;
	  } 	
		//LCD_CS=1;
	}
	
	
	void LCD_Set_Pos(byte x, byte y)
	{
	  LCD_WrCmd(0xb0+y);
	  LCD_WrCmd(((x&0xf0)>>4)|0x10);
	  LCD_WrCmd(x&0x0f);//|0x01);原来的版本图像整体右移一列，导致低128列移到第一列显示，不知道这么做是为什么，暂时先改回来
	}
	void OLCD_Fill(byte bmp_data)
	{
		byte y,x;
		
		for(y=0;y<8;y++)
		{
			LCD_WrCmd(0xb0+y);
			LCD_WrCmd(0x01);
			LCD_WrCmd(0x10);
			for(x=0;x<X_WIDTH;x++)
				LCD_WrDat(bmp_data);
		}
	}
	void LCD_CLS(void)
	{
		byte y,x;	
		for(y=0;y<8;y++)
		{
			LCD_WrCmd(0xb0+y);
			LCD_WrCmd(0x01);
			LCD_WrCmd(0x10);
			for(x=0;x<X_WIDTH;x++)
				LCD_WrDat(0);
		}
	}
	void LCD_CLS_06(void)
	{
		byte y,x;	
		for(y=0;y<2;y++)
		{
			LCD_WrCmd(0xb0+y);
			LCD_WrCmd(0x01);
			LCD_WrCmd(0x10);
			for(x=0;x<X_WIDTH;x++)
				LCD_WrDat(0);
		}
	}
	void LCD_DLY_ms(word ms)
	{
	  word a;
	  while(ms)
	  {
		a=13350;
		while(a--);
		ms--;
	  }
	  return;
	}
	
	//==============================================================
	//函数名： void LCD_PutPixel(byte x,byte y)
	//功能描述：绘制一个点（x,y）
	//参数：真实坐标值(x,y),x的范围0～127，y的范围0～64
	//返回：无
	//==============================================================
	void LCD_PutPixel(byte x,byte y)
	{
		byte data1;  //data1当前点的数据
		
			LCD_Set_Pos(x,y);
		data1 = 0x01<<(y%8); 	
		LCD_WrCmd(0xb0+(y>>3));
		LCD_WrCmd(((x&0xf0)>>4)|0x10);
		LCD_WrCmd((x&0x0f)|0x00);
		LCD_WrDat(data1); 	 	
	}
	void LCD_xian(byte x1,byte y1,byte x2,byte y2)
	{
		byte n,n1;
		for(n = x1;n < x2;n++)
		{
			for(n1 = y1;n1 < y2;n1++)
			{
				LCD_PutPixel(n,n1);
			}
			
		}
	}
	
	//==============================================================
	//函数名： void LCD_Rectangle(byte x1,byte y1,
	//                   byte x2,byte y2,byte color,byte gif)
	//功能描述：绘制一个实心矩形
	//参数：左上角坐标（x1,y1）,右下角坐标（x2，y2）,是否动画
	//      其中x1、x2的范围0～127，y1，y2的范围0～63，即真实坐标值
	//返回：无
	//==============================================================
	void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif)
	{
		byte n;
			
		LCD_Set_Pos(x1,y1>>3);
		for(n=x1;n<=x2;n++)
		{
			LCD_WrDat(0x01<<(y1%8)); 			
			if(gif == 1) 	LCD_DLY_ms(50);
		}
		LCD_Set_Pos(x1,y2>>3);
		for(n=x1;n<=x2;n++)
		{
			LCD_WrDat(0x01<<(y2%8)); 			
			if(gif == 1) 	LCD_DLY_ms(5);
		}
		
	}
	//==============================================================
	//函数名：LCD_P6x8Str(byte x,byte y,byte *p)
	//功能描述：写入一组标准ASCII字符串
	//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
	//返回：无
	//==============================================================
	void LCD_P6x8Str(byte x,byte y,byte ch[])
	{
	  byte c=0,i=0,j=0;
	  while (ch[j]!='\0')
	  {
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		LCD_Set_Pos(x,y);
		for(i=0;i<6;i++)
		  LCD_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	  }
	}
	//==============================================================
	//函数名：LCD_P8x16Str(byte x,byte y,byte *p)
	//功能描述：写入一组标准ASCII字符串
	//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
	//返回：无
	//==============================================================
	void LCD_P8x16Str(byte x,byte y,byte ch[])
	{
	  byte c=0,i=0,j=0;
	
	  while (ch[j]!='\0')
	  {
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		LCD_Set_Pos(x,y);
		for(i=0;i<8;i++)
		  LCD_WrDat(F8X16[c*16+i]);
		LCD_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		  LCD_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	  }
	}
	//输出汉字字符串
	void LCD_P14x16Str(byte x,byte y,byte ch[])
	{
		byte wm=0,ii = 0;
		word adder=1;
		
		while(ch[ii] != '\0')
		{
		wm = 0;
		adder = 1;
		while(F14x16_Idx[wm] > 127)
		{
			if(F14x16_Idx[wm] == ch[ii])
			{
				if(F14x16_Idx[wm + 1] == ch[ii + 1])
				{
					adder = wm * 14;
					break;
				}
			}
			wm += 2;			
		}
		if(x>118){x=0;y++;}
		LCD_Set_Pos(x , y);
		if(adder != 1)// 显示汉字					
		{
			LCD_Set_Pos(x , y);
			for(wm = 0;wm < 14;wm++)
			{
				LCD_WrDat(F14x16[adder]);	
				adder += 1;
			}
			LCD_Set_Pos(x,y + 1);
			for(wm = 0;wm < 14;wm++)
			{
				LCD_WrDat(F14x16[adder]);
				adder += 1;
			}   		
		}
		else			  //显示空白字符			
		{
			ii += 1;
		  LCD_Set_Pos(x,y);
			for(wm = 0;wm < 16;wm++)
			{
					LCD_WrDat(0);
			}
			LCD_Set_Pos(x,y + 1);
			for(wm = 0;wm < 16;wm++)
			{   		
					LCD_WrDat(0);	
			}
		}
		x += 14;
		ii += 2;
		}
	}
	//输出汉字和字符混合字符串
	void LCD_Print(byte x, byte y, byte ch[])
	{
		byte ch2[3];
		byte ii=0;
		while(ch[ii] != '\0')
		{
			if(ch[ii] > 127)
			{
				ch2[0] = ch[ii];
				ch2[1] = ch[ii + 1];
				ch2[2] = '\0';			//汉字为两个字节
				LCD_P14x16Str(x , y, ch2);	//显示汉字
				x += 14;
				ii += 2;
			}
			else
			{
				ch2[0] = ch[ii];	
				ch2[1] = '\0';			//字母占一个字节
				LCD_P8x16Str(x , y , ch2);	//显示字母
				x += 8;
				ii+= 1;
			}
		}
	}
	
	//==============================================================
	//函数名： void Draw_BMP(byte x,byte y)
	//功能描述：显示BMP图片128×64
	//参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～7终止点坐标（x1，y1）
	//返回：无
	//==============================================================
	void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte *bmp)
	{ 	
	//  word ii=0;
	  byte x,y;
	
	  if(y1%8==0)
		y=y1/8;
	  else
		y=y1/8+1;
	  for(y=y0;y<=y1;y++)
	  {
		LCD_Set_Pos(x0,y);				
		for(x=x0;x<=x1;x++)
		{
		  LCD_WrDat(*bmp++);	    	
		}
	  }
	}
	
	
	void Draw_BMP2(byte x0,byte y0,byte x1,byte y1,byte *bmp)
	{ 	
	//  word ii=0;
	  byte x,y;
	
	  if(y1%8==0)
		y=y1/8;
	  else
		y=y1/8+1;
	  for(y=y0;y<=y1;y+=2)
	  {
		LCD_Set_Pos(x0,y);				
		for(x=x0;x<=x1;x++)
		{if((*bmp+=3)>0x80)
		  LCD_WrDat(0x01);  
		else
			LCD_WrDat(0x00);  	
		}
	  }
	}
	
	void Dis_Num(byte y, byte x, unsigned int num,byte N)
	{
	  byte j=0;
	  byte n[6]={0};
	  x=x*8;
	  n[0]=(num/10000)%10;
	  n[1]=(num/1000)%10;
	  n[2]=(num/100)%10;
	  n[3]=(num/10)%10;
	  n[4]=num%10;
	  n[5]='\0';
	  for(j=0;j<5;j++)
		n[j]=n[j]+16+32;	 
	  LCD_P8x16Str(x,y,&n[5-N]);//从ACSII码表中读取字节，然后写入液晶
	}
	
	void DZXH_LOGO()
	{
		Draw_BMP(0,0,127,7,DZXYlogo);
		delay_ms(1400);            //显示LOGO延时
		OLCD_Fill(0x00);  //初始清屏
		
	}
	
	void TZXH_LOGO()
	{
		Draw_BMP(32,0,95,7,TZXYlogo);
		delay_ms(1400);            //显示LOGO延时
//		OLCD_Fill(0x00);  //初始清屏
		
	}
	
	//显示数据 
	//输入参数；（x，y）坐标 ，数据
	//250————+02.50
	void Dis_err(byte y, byte x, int16 num)
	{
	  byte n[6]={0};
	  x=x*8;
	  if(num<0)
	  {
		n[0]=45;
		num=-num;
	  }
	  else if(num>=0)
	  {
		n[0]=43;
	  }
	  n[1]=(num/1000)%10+16+32;
	  n[2]=(num/100)%10+16+32;
	  n[3]=46;
	  n[4]=(num/10)%10+16+32;
	  n[5]=num%10+16+32;
	  LCD_P8x16Str(x,y,&n[0]);//从ACSII码表中读取字节，然后写入液晶
	}
	
	//右上角显示电量 
	//输入参数：电压值840以上满格630以下空格，0图片显示1数字显示
	void Draw_Power(uint16 TSC,uint8 qie)		
	{
	  byte is;
	  byte n[5]={0};
	  if(qie==0)
	  {
		if(TSC<=630)
		  TSC=630;
		TSC=TSC-630;
		TSC=TSC/10;
		if(TSC>21)
		   TSC=21;
		for(is=0;is<21;is++)
		{
			dianchi[8+is]=0x04;
			dianchi[40+is]=0x20;
		}	
		for(is=0;is<TSC;is++)
		{
			dianchi[28-is]=0xF4;
			dianchi[60-is]=0x2F;
		}
		Draw_BMP(96,0,127,1,dianchi);  	
	  }
	  if(qie==1)
	  {
		TSC=TSC;
		n[0]=(TSC/100)%10+16+32;
		n[1]=46;
		n[2]=(TSC/10)%10+16+32;
		n[3]=TSC%10+16+32;
	//    n[4]='\n';
		LCD_P8x16Str(96,0,&n[0]);//从ACSII码表中读取字节，然后写入液晶
		//Dis_Num(0,11,TSC,4) ;//3
	  }
	
	}
	
	void Draw_XH()
	{
		Draw_BMP(0,0,31,1,xinghao);
		Draw_Power(840,0);
		//Draw_WZ(-30);
	}
	
	 void Dis_Num_point(byte y, byte x, float num,byte N)
	{
	  unsigned int temp;
	  byte j=0;
	  byte n[5]={0};
	  temp = (unsigned int)(num*100);
	  x=x*8;
	  n[0]=temp/100;
	//  n[1]='.';
	  n[2]=(temp/10)%10;
	  n[3]=temp%10;
	//  n[5]='/0';
	  for(j=0;j<4;j++)
		n[j]=n[j]+16+32;
	  n[1] = '.';
	  LCD_P8x16Str(x,y,&n[4-N]);//从ACSII码表中读取字节，然后写入液晶
	
	}
	
	//长条上显示位置
	//输入参数+-60
	void Draw_WZ(int16 WEI2)
	{
		byte iss;
		byte WEI;
		WEI=WEI2+63;
		for(iss=0;iss<128;iss++)
		{
		  weizhi[iss]=0x00;
		}	
		weizhi[WEI]=0x7C;
		weizhi[WEI+1]=0x7C;
		Draw_BMP(0,2,127,3,weizhi);
	}
	
	
	/*****************************************************************************
	 函 数 名 : LED_PrintImage
	 功能描述 : 将图像显示出来
	 输入参数 : UCHAR8 *pucTable 二维图像数组的地址
				 USHORT16 usRowNum 二维图像的行数1~64
				 USHORT16 usColumnNum 二维图像的列数1~128
	 输出参数 : none
	 返 回 值 : none
	*****************************************************************************/
	void LED_PrintImage(byte *pucTable, word usRowNum, word usColumnNum)
	{
		byte ucData;
		word i,j,k,m,n;
		word usRowTmp;
	
		m = usRowNum >> 3;   //计算图片行数以8位为一组完整的组数
		n = usRowNum % 8;    //计算分完组后剩下的行数
	
		for(i = 0; i < m; i++) //完整组行扫描
		{
			LCD_Set_Pos(0,i);
			usRowTmp = i << 3;    //计算当前所在行的下标
			for(j = 0; j < usColumnNum; j++) //列扫描
			{
				ucData = 0;
				for(k = 0; k < 8; k++) //在i组中对这8行扫描
				{
					ucData = ucData >> 1;
					if((pucTable + (usRowTmp + k) * usColumnNum)[j] == 0)
					{
						ucData = ucData | 0x80;
					}
	
				}
				LCD_WrDat(ucData);
			}
		}
	
		LCD_Set_Pos(0,(u8)i); //设置剩下的行显示的起始坐标
		usRowTmp = i << 3;       //计算当前所在行的下标
		for(j = 0; j < usColumnNum; j++) //列扫描
		{
			ucData = 0;
			for(k = 0; k < n; k++) //对剩下的行扫描
			{
				ucData = ucData >> 1;
				if((pucTable + (usRowTmp + k) * usColumnNum)[j] == 0)
				{
					ucData = ucData | 0x80;
				}
	
			}
			ucData = ucData >> (8 - n);
			LCD_WrDat(ucData);
		}
	
		return;
	}
	//输出汉字字符串
	void LCD_P16x16Str(byte x,byte y,byte ch[])
	{
		byte wm=0,ii = 0;
		word adder=1;
		
		while(ch[ii] != '\0')
		{
			wm = 0;	 //16*16表中的汉字ACSII码位置
			adder = 1;
			while(ST16x16_Idx[wm] > 127)
			{
				if(ST16x16_Idx[wm] == ch[ii]) //汉字第一个码相等
				{
					if(ST16x16_Idx[wm + 1] == ch[ii + 1])//汉字第二个码相等
					{
						adder = wm * 16; //得到16*16点阵的起始位置
						break;
					}
				}
				wm += 2;			
			}	//得到汉字的位置adder
			
			if(x>118){x=0;y=y+2;}
			LCD_Set_Pos(x , y); 
			if(adder != 1)// 显示汉字					
			{  
				LCD_Set_Pos(x , y);
				for(wm = 0;wm < 16;wm++)//16行点阵，一行一行显示
				{
					LCD_WrDat(ST16x16[adder]);	
					adder += 1;
				}
				LCD_Set_Pos(x,y + 1);
				for(wm = 0;wm < 16;wm++)
				{
					LCD_WrDat(ST16x16[adder]);
					adder += 1;
				}   		
			}
			else			  //显示空白字符			
			{
				ii += 1;
			  LCD_Set_Pos(x,y);
				for(wm = 0;wm < 16;wm++)
				{
						LCD_WrDat(0);
				}
				LCD_Set_Pos(x,y + 1);
				for(wm = 0;wm < 16;wm++)
				{   		
						LCD_WrDat(0);	
				}
			}
			x += 16;
			ii += 2;
		}
	}
	//输出汉字和字符混合字符串
	void LCD_16_HanZi_ASCII(byte x, byte y, char ch[])
	{
		byte ch2[3];
		byte ii=0;
		while(ch[ii] != '\0')
		{
			if(ch[ii] > 127)
			{
				ch2[0] = ch[ii];
				ch2[1] = ch[ii + 1];
				ch2[2] = '\0';			//汉字为两个字节
				if(x*8>118){x=0;y=y+2;}
				LCD_P16x16Str(x*8 , y, ch2);	//显示汉字
//					x += 16;
				 x=x+2;
				ii += 2;
			}
			else
			{
				ch2[0] = ch[ii];	
				ch2[1] = '\0';			//字母占一个字节
				if(x*8>120){x=0;y=y+2;}
				LCD_P8x16Str(x*8 , y , ch2);	//显示字母
//					x += 8;
				x++;
				ii+= 1;
			}
		}
	}


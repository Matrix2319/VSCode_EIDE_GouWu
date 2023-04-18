//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PB13（SCL）
//              D1   接PB15（SDA）
//              RES  接PB11
//              DC   接PB10
//              CS   接P12               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __OLED2_H
#define __OLED2_H			  	 
#include "sys.h"
#include "stdlib.h"	 
#include "stm32f10x.h"

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE    16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    

//-----------------测试LED端口定义---------------- 
//#define LED_ON GPIO_ResetBits(GPIOD,GPIO_Pin_2)
//#define LED_OFF GPIO_SetBits(GPIOD,GPIO_Pin_2)

//-----------------OLED端口定义----------------  					   
#define  oled_d0   GPIO_Pin_6
#define oled_d1    GPIO_Pin_5 
#define oled_res   GPIO_Pin_4
#define oled_dc    GPIO_Pin_3
#define oled_cs    GPIO_Pin_2
#define oled_gpio  GPIOG


#define OLED_SCLK_Clr() GPIO_ResetBits(oled_gpio,oled_d0)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(oled_gpio,oled_d0)

#define OLED_SDIN_Clr() GPIO_ResetBits(oled_gpio,oled_d1)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(oled_gpio,oled_d1)

#define OLED_RST_Clr() GPIO_ResetBits(oled_gpio,oled_res)//RES
#define OLED_RST_Set() GPIO_SetBits(oled_gpio,oled_res)

#define OLED_DC_Clr() GPIO_ResetBits(oled_gpio,oled_dc)//DC
#define OLED_DC_Set() GPIO_SetBits(oled_gpio,oled_dc)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(oled_gpio,oled_cs)//CS
#define OLED_CS_Set()  GPIO_SetBits(oled_gpio,oled_cs)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

extern char OLED2_BUF[100];


//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    

void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
//void OLED_Init(void);

void OLED_DrawBMP_yunpiao(void);
void OLED_Refresh(void);
void OLED_CLS(u8 mode);
void OLED_CLS_67(u8 mode);
void OLED_DrawHanzi(u8 x,u8 y,char ch[],u8 mode);//画个汉字，最终更新GRAM数组;
void OLED_DrawPicture(u8 x,u8 y,u8 high,u8 csize,u8 *p,u8 mode);
void OLED_ShowxcString(u8 x,u8 y,const u8 *p,u8 size,u8 mode);
void OLED_DrawHanzi_ASCII(u8 x,u8 y,char ch[],u8 mode);//画个汉字，最终更新GRAM数组
void OLED_ShowxcNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 mode);

#endif  
	 




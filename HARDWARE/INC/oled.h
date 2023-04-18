#ifndef	_OLED_H_
#define _OLED_H_

#include "sys.h"
#include "stdlib.h"	
#include "stdio.h"
#include "delay.h"

extern char OLED_BUF[64];//OLED显示缓冲区每行16个字符，共4行。

#define D0 			GPIO_Pin_6	
#define D1 			GPIO_Pin_5
#define RST 		GPIO_Pin_4
#define DC 			GPIO_Pin_3
#define CS 			GPIO_Pin_2
#define OLED_GPIO 	GPIOG

#define DCLOW() GPIO_ResetBits(OLED_GPIO, DC)
#define DCHIGH() GPIO_SetBits(OLED_GPIO, DC)
#define CLKLOW() GPIO_ResetBits(OLED_GPIO, D0)
#define CLKHIGH() GPIO_SetBits(OLED_GPIO, D0)
#define MISOLOW() GPIO_ResetBits(OLED_GPIO, D1)
#define MISOHIGH() GPIO_SetBits(OLED_GPIO, D1)	
#define RESLOW() GPIO_ResetBits(OLED_GPIO, RST)
#define RESHIGH()  GPIO_SetBits(OLED_GPIO, RST)	
#define CSLOW() GPIO_ResetBits(OLED_GPIO, CS)
#define CSHIGH()  GPIO_SetBits(OLED_GPIO, CS)	


#define uint16 u16 
#define uint8 u8 
#define int16 s16 
#define int8 s8 

#define byte u8
#define word u16
#define GPIO_PIN_MASK      0x1Fu    //0x1f=31,限制位数为0--31有效
#define GPIO_PIN(x)        (((1)<<(x & GPIO_PIN_MASK)))  //把当前位置1
// extern byte beyond96x64[512];
// extern byte beyond64x64[512];
void OLED_Init(void);
void LCD_CLS(void);
void LCD_CLS_06(void);
void LCD_P6x8Str(byte x,byte y,byte ch[]);
void LCD_P8x16Str(byte x,byte y,byte ch[]);
void LCD_P14x16Str(byte x,byte y,byte ch[]);
void LCD_Print(byte x, byte y, byte ch[]);
void LCD_PutPixel(byte x,byte y);
void LCD_xian(byte x1,byte y1,byte x2,byte y2);
void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
void Draw_LQLogo(void);
void Draw_LibLogo(void);
void Draw_Image(void);
void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]);
void OLCD_Fill(byte dat);
void Dis_Num(byte y, byte x, unsigned int num,byte N) ;
void Draw_Power(uint16 TSC,uint8 qie);
void Draw_XH(void);
void Draw_WZ(int16 WEI2);
void Dis_err(byte y, byte x, int16 num);
void DZXH_LOGO(void);
void TZXH_LOGO(void); 
void NAME_LOGO(void);
void LCD_WrCmd(byte cmd);
void LCD_Set_Pos(byte x, byte y);
void LCD_DLY_ms(word ms);
void LED_PrintImage(byte *pucTable, word usRowNum, word usColumnNum);
void Dis_Num_point(byte, byte , float,byte);
void LCD_P16x16Str(byte x,byte y,byte ch[]);
void LCD_16_HanZi_ASCII(byte x, byte y, char ch[]);

#endif

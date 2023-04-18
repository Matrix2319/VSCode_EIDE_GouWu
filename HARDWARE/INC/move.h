#ifndef _MOVE_H_
#define _MOVE_H_

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio.h"
#include "timer.h"
#include "moto.h"
#include "oled.h"

#include "string.h"
#include "ADC_Config.h"


extern u8 move_BUFF_SET_hang;
extern u8 move_BUFF_SET[15][2];
extern u8 move_BUFF_SET2_hang;
extern u8 move_BUFF_SET2[15][2];
extern u8 move_BUFF_SET3_hang;
extern u8 move_BUFF_SET3[15][2];
extern u8 ce_move1_BUFF[10][11];
extern u8 ce_move2_BUFF[10][11];
extern u8 FX_flag;

void CESHI(void);

void HAND_ZHUA(void);
void HAND_FANG(void);


void DJ_MOVE_ZONG(u8 t,u8 f,u8 sp,u8 r,u16 tim,u8 dj_1,u8 dj_2,u8 dj_3,u8 dj_4,u8 YS);

void WanSheng(void);

void ce(void);
void ce_1(void);
void ce_2(void);
	

void WJ(void);
void chu_1_1(void);
void chu_2_1(void);
void chu_3_1(void);
void chu_4_1(void);

void chong_1(void);
void chong_2(void);
void chong_3(void);
void chong_4(void);
	
void xian(void);
void xian2(void);
void xian3(void);
void xian4(void);
	
void DJ_MOVE_BUFF_shuzu(u8 a[12][2],u8 hang);
void DJ_MOVE_BUFF_SET(u8 t,u8 f, u8 yanshi);
void HAND_FUWEI(void);
void HAND_ZHUAMUKUAI(void);
void LUXIAN_1(void);
void LUXIAN_2(void);

#endif





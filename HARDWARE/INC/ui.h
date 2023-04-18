#ifndef _UI_H
#define _UI_H


#include "sys.h"
#include "delay.h"
#include "time.h"
#include "stdlib.h"
#include "hand.h"
#include "usart.h"
#include "ADC_Config.h"
#include "oled.h"
#include "oled2.h"
#include "moto.h"
#include "move.h"
#include "stmflash.h"
#include "gpio.h"
#include "st178.h"
#include "hand.h"

extern u8 Infoflag;

void buff(void);

void Drive(void);                //初始化主菜单界面	默认时钟
void Debug(void);		    //设置
void Temp_Set(void);		    //图形
void Clock(void);		    //时钟
void Internet_Set(void);           //网络

void Debug_Set(void);
void Set_a(void);
void Set_b(void);
void Set_c(void);
void Set_d(void);
void Set_e(void);
void Set_f(void);
void Set_g(void);
void Set_188(void);
void Set_188_a(void);	

void Clock_a(void);
void Clock_b(void);
void Clock_c(void);
void Clock_d(void);
void Clock_e(void);

void Drive_a(void);
void Drive_b(void);
void Drive_c(void);
void Drive_d(void);

void Net_a(void);
void Net_b(void);
void Net_c(void);
void Net_d(void);
void Net_e(void);
void Net_f(void);
void Net_g(void);
void Net_h(void);

void speed1(void);
void speed2(void);
void save_current(void);
void load_current(void);
void start_go(void);
void xc(void);

void MenuOperate(void);
void read_from_flash(void);
void save_to_flash(void);

void change(unsigned char (*a)[5],unsigned char len);

#endif 

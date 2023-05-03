#ifndef _GPIO_H
#define _GPIO_H

#include "sys.h"

#define   S_D_LED   PBout(14)

#define   CAR_BL   PDout(2) //电机方向
#define   CAR_FL   PDout(1) //电机方向
#define   CAR_FR   PDout(0) //电机方向
#define   CAR_BR   PDout(3) //电机方向

#define   Bluetooth2   PFin(11)//串口2使能
#define   Bluetooth3   PDin(10)//串口3使能

/*选择器共用地址*/
#define  St188_A     PEout(4)
#define  St188_B     PEout(5)
#define  St188_C     PEout(6)

/*循迹使能端*/
#define St188_CS1   PFout(1)//前
#define St188_CS2   PFout(4)//左
#define St188_CS3  	PEout(2)//右
#define St188_CS4   PBout(8)//后

/*循迹返回端输入值*/
#define St188_Y1_1    PFin(2)
#define St188_Y2_1    PFin(5)
#define St188_Y3_1    PEin(3)
#define St188_Y4_1    PBin(9)


#define LS_FH  PFin(8)
#define LS_FL  PFin(7)
#define LS_BH  PEin(1)
#define LS_BL  PFin(12)

#define DJ_High_PWM  PEout(11)
#define DJ_High_Direction  PEout(12)
#define DJ_Low_PWM  PEout(13)
#define DJ_Low_Direction   PBout(12)
#define LED  PBout(13)


void SHOP_GPIO_init(void);
void All_GPIO_Init(void);


#endif


#ifndef _HAND_H
#define _HAND_H

#include "sys.h"

// A,B,C,D区域
extern u8 exFlag_HuoJia;    
//存放轮盘内的信息
extern u8 LunPan[7];
extern u8 Tui[2][7];
extern u8 Tuii;
//D区域的数组
extern u8 D_S[3][7];
extern u8 D_X[3][7];
extern u8 D_i;

//void CSD(void);

//处理抓的函数，传入货架
void Zhua(u8 Flag_HuoJia);
void BuJin_GPIO_Init();
//步进电机控制升降，I最低初始化，X为下层货架，S为上层货架
void BuJin_Zhuan(u8 flag);
//轮盘转动函数
void LunPan_Zhuan();
void Nano_ChuLi(u8 Flag_HuoJia);
//初始化一些参数
void HandInit();
#endif 


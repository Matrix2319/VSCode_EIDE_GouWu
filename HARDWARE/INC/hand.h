#ifndef _HAND_H
#define _HAND_H

#include "sys.h"


//动态路线
extern u8 LuXian_DongTai[50][5];
extern u8 LuXian_DongTaii;

//统计前进的格数
extern u8 Sum_Qian;

// A,B,C,D区域
extern u8 exFlag_HuoJia;    

//存放轮盘内的信息
extern u8 LunPan[7];
extern u8 Tui[2][7];
extern u8 Tuii;
extern u8 LunPani;

//A区域数组
extern u8 A_S[7];
extern u8 A_X[7];
extern u8 A_i;

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
//轮盘找东西然后推(c/d 方块/饮料 r/g/b)
void LunPan_Zhao_Tui(u8 f);
//给二维数组按行幅值
//二维数组，哪一行，传入的行
void Routecpy(u8 st1,u8 ed2,u8 rd3,u8 th4,u8 th5);
//靠边抓CD货架
void KaoBian_Zhua(u8 HuoJia_F,u8 HuoJia_i);
//B货架推
void B_Tui(u8 HuoJia_i);
#endif 


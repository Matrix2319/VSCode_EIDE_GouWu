#ifndef _ST178_H 
#define _ST178_H

#include "sys.h"

extern int chazhi_qh;
extern int chazhi_zy;

extern int qian_sum;
extern int hou_sum;
extern int zuo_sum;
extern int you_sum;
extern u8 oled_st188;

extern u8 q_q;
extern u8 h_h;
extern u8 q_q_q;
extern u8 h_h_h;
extern u8 z_z;
extern u8 y_y;

extern u8 DEF_FLAGE;
extern u8 BX_FLAGE;

s8 St178_Scanf(u8 fi);
s8 St188_Scanf(u8 forward);
void bxcls(void);







#endif




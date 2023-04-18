#ifndef _EXTI_H
#define _EXTI_H
#include "sys.h"

extern char cunchu[4];
extern u8 fz_flag;

void fzopen();
void fzclose();
void Init_TIM6(u16 arr ,u16 psc);

#endif 
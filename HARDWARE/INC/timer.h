#ifndef _TIMER_H
#define _TIMER_H


#include "sys.h"

//电机正反转标志位
//为1正传 为0 反转

void Init_TIM8_PWM(u16 arr ,u16 psc);

void Init_TIM6_PWM(u16 arr ,u16 psc);

void Init_TIM1_PWM(u16 arr ,u16 psc);

void Init_BMQ(void);



#endif 

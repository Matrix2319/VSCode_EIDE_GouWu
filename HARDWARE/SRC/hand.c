#include "hand.h"
#include "timer.h"
#include "math.h"
#include <stdlib.h>
#include "gpio.h"
#include "usart.h"
#include "moto.h"
#include "usart.h"
#include "delay.h"

void BuJin_GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12; // 步进电机翻转标志位
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE, 12);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 | GPIO_Pin_8|GPIO_Pin_9; // 限位开关7上8下9轮盘
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOF,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);  
   
}

void BuJin_Zhuan(u8 flag)
{
    if (flag == 'I') {
        PEout(12) = 0;//上下动标志位（0为下）
        TIM_Cmd(TIM1, ENABLE);
        TIM_SetCompare2(TIM1, 700);
        while (PFin(8) == 0) {//7上8下
            ;
        }
        TIM_SetCompare2(TIM1, 0);
        TIM_Cmd(TIM1, DISABLE);
    }
    if (flag == 'S') {
        PEout(12) = 1;
        TIM_Cmd(TIM1, ENABLE);
        TIM_SetCompare2(TIM1, 700);
        while (PFin(7) == 0) {
            ;
        }
        TIM_SetCompare2(TIM1, 0);
        TIM_Cmd(TIM1, DISABLE);
    }
    if (flag == 'X') {
        if (PFin(8) == 1) // 在下面
        {
            delay_ms(200);
            PEout(12) = 1;
            delay_ms(200);
            TIM_Cmd(TIM1, ENABLE);
            delay_ms(200);
            TIM_SetCompare2(TIM1, 700);
            delay_ms(200);
            for (u8 it = 0; it < 81; it++)
                delay_ms(100);
            TIM_SetCompare2(TIM1, 0);
            TIM_Cmd(TIM1, DISABLE);

        } else if (PFin(7) == 1) // 在上面
        {
            delay_ms(200);//延时是必要的
            PEout(12) = 0;
            delay_ms(200);
            TIM_Cmd(TIM1, ENABLE);
            delay_ms(200);
            TIM_SetCompare2(TIM1, 700);
            delay_ms(200);
            for (u8 it = 0; it < 235; it++) delay_ms(100);
            TIM_SetCompare2(TIM1, 0);
            TIM_Cmd(TIM1, DISABLE);
        } else // 哪都不在就先下后上
        {
            delay_ms(200);
            PEout(12) = 0;
            delay_ms(200);
            TIM_Cmd(TIM1, ENABLE);
            delay_ms(200);
            TIM_SetCompare2(TIM1, 700);
            while (PFin(8) == 0) {
                ;
            }
            delay_ms(200);
            TIM_SetCompare2(TIM1, 0);
            delay_ms(200);
            PEout(12) = 1;
            delay_ms(200);
            TIM_SetCompare2(TIM1, 700);
            delay_ms(200);
            for (u8 it = 0; it < 82; it++)
                delay_ms(100);
            TIM_SetCompare2(TIM1, 0);
            TIM_Cmd(TIM1, DISABLE);
        }
    }
    if (flag == 'T') {
        TIM_SetCompare2(TIM1, 0);
        TIM_Cmd(TIM1, DISABLE);
    }
}
void LunPan_Zhuan()
{
    TIM_Cmd(TIM1, ENABLE);
    TIM_SetCompare3(TIM1, 700);
    while (PFin(9)==1);  
    delay_ms(500);
    while (PFin(9)==0);   
    TIM_SetCompare3(TIM1, 0); // 轮盘舵机500最快，800慢
    delay_ms(500);
    TIM_Cmd(TIM1, DISABLE);
}
void Zhua() // 抓取动作，i为臂指向 收0 张1
{
    ;
}


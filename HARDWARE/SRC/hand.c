#include "hand.h"
#include "timer.h"
#include "ui.h"
#include "math.h"
#include <stdlib.h>
#include "gpio.h"
#include "usart.h"
#include "moto.h"
#include "usart.h"
#include "delay.h"

u8 LunPan[10]           = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // 轮盘存放的东西
u8 LunPani              = 0;                              // 轮盘的下标
u8 Flag_HuoJia          = 0;                              // A,B,C,D区域
u8 Flag_HuoJia_ShangXia = 0;                              // 区分上下货架

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
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; // 限位开关7上8下9轮盘
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOF, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
}

void BuJin_Zhuan(u8 flag)
{
    if (flag == 'I') {
        Flag_HuoJia_ShangXia = 0;
        PEout(12)            = 0; // 上下动标志位（0为下）
        TIM_Cmd(TIM1, ENABLE);
        TIM_SetCompare2(TIM1, 700);
        while (PFin(8) == 0) { // 7上8下
            ;
        }
        TIM_SetCompare2(TIM1, 0);
        TIM_Cmd(TIM1, DISABLE);
    }
    if (flag == 'S') {
        Flag_HuoJia_ShangXia = 'S';
        PEout(12)            = 1;
        TIM_Cmd(TIM1, ENABLE);
        TIM_SetCompare2(TIM1, 700);
        while (PFin(7) == 0) {
            ;
        }
        TIM_SetCompare2(TIM1, 0);
        TIM_Cmd(TIM1, DISABLE);
    }
    if (flag == 'X') {
        Flag_HuoJia_ShangXia = 'X';
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
            delay_ms(200); // 延时是必要的
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
    while (PFin(9) == 1)
        ;
    delay_ms(500);
    while (PFin(9) == 0)
        ;
    TIM_SetCompare3(TIM1, 0); // 轮盘舵机500最快，800慢
    delay_ms(500);
    TIM_Cmd(TIM1, DISABLE);
}
void Zhua(u8 Flag_HuoJia)
{
    if (Flag_HuoJia == 'A') {
        memset(USART3_RX_BUF, 0, 10); // 将数组清0
        USART3_RX_STA = 0;
        delay_ms(10);
        Printf(USART3, "%c", 'A');
        while (1) {
            delay_ms(100);
            if (USART3_RX_STA & 0x8000) // 接收完成
            {
                LCD_CLS();
                sprintf(OLED_BUF, "%c %c!", USART3_RX_BUF[0], USART3_RX_BUF[1]); // 显示
                LCD_16_HanZi_ASCII(0, 4, OLED_BUF);
                sprintf(OLED_BUF, "%c", Flag_HuoJia_ShangXia); // 显示
                LCD_16_HanZi_ASCII(0, 2, OLED_BUF);
                if (Flag_HuoJia_ShangXia == 'X') {
                    if (USART3_RX_BUF[1] == 'r') // 货架为空，要推
                    {
                        for (u8 i = 0; i <= 5; i++) {
                            if (LunPan[i] == 'r') {
                                LunPan[i] = 'k';
                                while (LunPani != i) {
                                    LunPan_Zhuan();
                                    LunPani++;
                                    if (LunPani == 6)
                                        LunPani = 0;
                                }
                                Printf(USART2, zhiling[1]);
                                break;
                            }
                        }
                        for (u8 itt = 0; itt < 15; itt++)
                            delay_ms(100);
                        break;
                    } else if (USART3_RX_BUF[1] == 'g') {
                        for (u8 i = 0; i <= 5; i++) {
                            if (LunPan[i] == 'g') {
                                LunPan[i] = 'k';
                                while (LunPani != i) {
                                    LunPan_Zhuan();
                                    LunPani++;
                                    if (LunPani == 6)
                                        LunPani = 0;
                                }
                                Printf(USART2, zhiling[1]);
                                break;
                            }
                        }
                        for (u8 itt = 0; itt < 15; itt++)
                            delay_ms(100);
                        break;
                    } else if (USART3_RX_BUF[1] == 'b') {
                        for (u8 i = 0; i <= 5; i++) {
                            if (LunPan[i] == 'b') {
                                LunPan[i] = 'k';
                                while (LunPani != i) {
                                    LunPan_Zhuan();
                                    LunPani++;
                                    if (LunPani == 6)
                                        LunPani = 0;
                                }
                                Printf(USART2, zhiling[1]);
                                break;
                            }
                        }
                        for (u8 itt = 0; itt < 15; itt++)
                            delay_ms(100);
                        break;
                    } else
                        break;
                }
                if (Flag_HuoJia_ShangXia == 'S') {
                    if (USART3_RX_BUF[0] == 'r') // 货架为空，要推
                    {
                        for (u8 i = 0; i <= 5; i++) {
                            if (LunPan[i] == 'r') {
                                LunPan[i] = 'k';
                                while (LunPani != i) {
                                    LunPan_Zhuan();
                                    LunPani++;
                                    if (LunPani == 6)
                                        LunPani = 0;
                                }
                                Printf(USART2, zhiling[1]);
                                break;
                            }
                        }
                        for (u8 itt = 0; itt < 15; itt++)
                            delay_ms(100);
                        break;
                    } else if (USART3_RX_BUF[0] == 'g') {
                        for (u8 i = 0; i <= 5; i++) {
                            if (LunPan[i] == 'g') {
                                LunPan[i] = 'k';
                                while (LunPani != i) {
                                    LunPan_Zhuan();
                                    LunPani++;
                                    if (LunPani == 6)
                                        LunPani = 0;
                                }
                                Printf(USART2, zhiling[1]);
                                break;
                            }
                        }
                        for (u8 itt = 0; itt < 15; itt++)
                            delay_ms(100);
                        break;
                    } else if (USART3_RX_BUF[0] == 'b') {
                        for (u8 i = 0; i <= 5; i++) {
                            if (LunPan[i] == 'b') {
                                LunPan[i] = 'k';
                                while (LunPani != i) {
                                    LunPan_Zhuan();
                                    LunPani++;
                                    if (LunPani == 6)
                                        LunPani = 0;
                                }
                                Printf(USART2, zhiling[1]);
                                break;
                            }
                        }
                        for (u8 itt = 0; itt < 15; itt++)
                            delay_ms(100);
                        break;
                    } else
                        break;
                }
                break;
            }
        }
    }
}
void Nano_ChuLi(u8 Flag_HuoJia)
{
    if (Flag_HuoJia == 'A') {
        memset(USART3_RX_BUF, 0, 10); // 将数组清0
        USART3_RX_STA = 0;
        delay_ms(500);
        USART3_Putc('A');
        Flag_HuoJia = 'A';
        while (1) {
            if (USART3_RX_STA & 0x8000) // 接收完成
            {

                switch (USART3_RX_BUF[0]) // 上层
                {
                    case 'r':
                        Printf(USART2, "%s", zhiling[1]);
                        LunPan[LunPani] = 'r';
                        LunPani++;
                        if (LunPani == 6) LunPani = 0;
                        LunPan_Zhuan();
                        break;
                    case 'g':
                        Printf(USART2, "%s", zhiling[1]);
                        LunPan[LunPani] = 'g';
                        LunPani++;
                        if (LunPani == 6) LunPani = 0;
                        LunPan_Zhuan();
                        break;
                    case 'b':
                        Printf(USART2, "%s", zhiling[1]);
                        LunPan[LunPani] = 'b';
                        LunPani++;
                        if (LunPani == 6) LunPani = 0;
                        LunPan_Zhuan();
                        break;
                    default:
                        break;
                }
                switch (USART3_RX_BUF[1]) // 下层
                {
                    case 'r':
                        Printf(USART2, "%s", zhiling[1]);
                        LunPan[LunPani] = 'r';
                        LunPani++;
                        if (LunPani == 6) LunPani = 0;
                        LunPan_Zhuan();
                        break;
                    case 'g':
                        Printf(USART2, "%s", zhiling[1]);
                        LunPan[LunPani] = 'g';
                        LunPani++;
                        if (LunPani == 6) LunPani = 0;
                        LunPan_Zhuan();
                        break;
                    case 'b':
                        Printf(USART2, "%s", zhiling[1]);
                        LunPan[LunPani] = 'b';
                        LunPani++;
                        if (LunPani == 6) LunPani = 0;
                        LunPan_Zhuan();
                        break;
                    default:
                        break;
                }
                USART3_RX_STA = 0;
                break;
            }
        }
    }
}

void HandInit()
{
            memset(LunPan, 0, 10);
            LunPani = 0;
}
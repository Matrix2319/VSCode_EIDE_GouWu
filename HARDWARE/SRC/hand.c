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

u8 exFlag_HuoJia          = 0; // 全局的A,B,C,D区域
u8 Flag_HuoJia_ShangXia = 0; // 区分上下货架

u8 LunPan[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // 轮盘存放的东西
u8 LunPani    = 0;                              // 轮盘的下标

u8 Tui[2][10];
u8 Tuii = 0;

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
            for (u8 it = 0; it < 52; it++)
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
            for (u8 it = 0; it < 153; it++) delay_ms(100);
            TIM_SetCompare2(TIM1, 0);
            TIM_Cmd(TIM1, DISABLE);
        } else // 哪都不在就先下后上
        {
            BuJin_Zhuan('I');
            Flag_HuoJia_ShangXia = 'X'; // 调用了I所以要重新给
            PEout(12)            = 1;
            delay_ms(200);
            TIM_Cmd(TIM1, ENABLE);
            delay_ms(200);
            TIM_SetCompare2(TIM1, 700);
            delay_ms(200);
            for (u8 it = 0; it < 52; it++)
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
            delay_ms(10);
            if (USART3_RX_STA & 0x8000) // 接收完成
            {
                    delay_ms(10);
                if (Flag_HuoJia_ShangXia == 'X') {
                    for (u8 i = 0; i <= 5; i++) {
                        if (LunPan[i] == USART3_RX_BUF[1]) {
                            LunPan[i] = 'k';
                            while (LunPani != i) {
                                LunPan_Zhuan();
                                LunPani++;
                                if (LunPani == 6)
                                    LunPani = 0;
                            }
                            Printf(USART2, zhiling[1]);
                            delay_ms(500);
                            delay_ms(500);
                            delay_ms(500);
                            Printf(USART2, zhiling[2]);
                            break;
                        }
                    }
                    break;
                }
                if (Flag_HuoJia_ShangXia == 'S') {
                    for (u8 i = 0; i <= 5; i++) {
                        if (LunPan[i] == USART3_RX_BUF[0]) {
                            LunPan[i] = 'k';
                            while (LunPani != i) {
                                LunPan_Zhuan();
                                LunPani++;
                                if (LunPani == 6)
                                    LunPani = 0;
                            }
                            Printf(USART2, zhiling[1]);
                            delay_ms(500);
                            delay_ms(500);
                            delay_ms(500);
                            Printf(USART2, zhiling[2]);
                            break;
                        }
                    }
                    break;
                }
            }
        }
        stop();
    }
    if (Flag_HuoJia == 'B') {
        if (Flag_HuoJia_ShangXia == 'X') {
            delay_ms(100);
            Tuii=Tuii-1;
            delay_ms(100);
            LCD_CLS();
            sprintf(OLED_BUF, "%c %c %c %c %c %c", Tui[1][0], Tui[1][1], Tui[1][2], Tui[1][3], Tui[1][4], Tui[1][5]); // 显示
            LCD_16_HanZi_ASCII(0, 4, OLED_BUF);
            sprintf(OLED_BUF, "%c %c %c %c %c %c", Tui[0][0], Tui[0][1], Tui[0][2], Tui[0][3], Tui[0][4], Tui[0][5]); // 显示
            LCD_16_HanZi_ASCII(0, 2, OLED_BUF);
            sprintf(OLED_BUF, "%c %c", USART3_RX_BUF[0], USART3_RX_BUF[1]); // 显示
            LCD_16_HanZi_ASCII(0, 0, OLED_BUF);
            sprintf(OLED_BUF, "%d %c %c", Tuii,Tui[0][Tuii],Tui[1][Tuii]); // 显示
            LCD_16_HanZi_ASCII(6, 0, OLED_BUF);
            delay_ms(50);
            if(Tui[1][Tuii]!='o')
            {
            if (Tui[1][Tuii] == 'r')
                    BMQ_MOVE(0, 15, 0); // 往右走
            else if (Tui[1][Tuii] == 'l')
                    BMQ_MOVE(1, 15, 0); // 往左走
            Printf(USART2, "%s", zhiling[3]); // 推中间
            delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            Printf(USART2, "%s", zhiling[2]); 
            }

        } else if (Flag_HuoJia_ShangXia == 'S') {
            if(Tui[0][Tuii]!='o')
            {
            if (Tui[0][Tuii] == 'r')
                BMQ_MOVE(0, 15, 0); // 往右走
            else if (Tui[0][Tuii] == 'l')
                BMQ_MOVE(1, 15, 0); // 往左走
            Printf(USART2, "%s", zhiling[3]); // 推中间
            delay_ms(500);
            delay_ms(500);
            delay_ms(500);
            Printf(USART2, "%s", zhiling[2]); // 推中间
            }
            Tuii++;
        }
        stop();
    }
}
void Nano_ChuLi(u8 Flag_HuoJia)
{
    if (Flag_HuoJia == 'A') {
        exFlag_HuoJia = 'A';
        memset(USART3_RX_BUF, 0, 10); // 将数组清0
        USART3_RX_STA = 0;
        delay_ms(500);
        USART3_Putc('A');
        while (1) {
            if (USART3_RX_STA & 0x8000) // 接收完成
            {

                if (USART3_RX_BUF[0] != '1') // 上层要抓
                {
                    Printf(USART2, "%s", zhiling[4]); // 抓上中间木块
                    for (u8 t = 0; t < 10; t++)
                        delay_ms(1000);
                    Printf(USART2, "%s", zhiling[0]); // 复位
                    LunPan[LunPani] = USART3_RX_BUF[0];
                    LunPani++;
                    if (LunPani == 6) LunPani = 0;
                    LunPan_Zhuan();
                } 
                if (USART3_RX_BUF[1] != '1') // 下层要抓
                {
                    Printf(USART2, "%s", zhiling[5]); // 抓下中间木块
                    for (u8 t = 0; t < 10; t++)
                        delay_ms(1000);
                    Printf(USART2, "%s", zhiling[0]); // 复位
                    LunPan[LunPani] = USART3_RX_BUF[1];
                    LunPani++;
                    if (LunPani == 6) LunPani = 0;
                    LunPan_Zhuan();
                } 
                USART3_RX_STA = 0;
                break;
            }
        }
    }
    if (Flag_HuoJia == 'B') {
        exFlag_HuoJia = 'B';
        memset(USART3_RX_BUF, 0, 10); // 将数组清0
        USART3_RX_STA = 0;
        delay_ms(100);
        Printf(USART3,"%c",'B');
        while (1) {
            delay_ms(10);
            if (USART3_RX_STA & 0x8000) // 接收完成
            {
                Tui[0][Tuii] = USART3_RX_BUF[0];
                Tui[1][Tuii] = USART3_RX_BUF[1];
                Tuii++;
                USART3_RX_STA = 0;
                break;
            }
        }
    }
    USART3_RX_STA = 0;
}
void HandInit()
{
    Printf(USART2, "%s", zhiling[0]); // 复位
    memset(LunPan, 0, 10);
    memset(Tui, 0, 12);
    LunPani = 0;
    Tuii    = 0;
}
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

u8 exFlag_HuoJia        = 0; // 全局的A,B,C,D区域
u8 Flag_HuoJia_ShangXia = 0; // 区分上下货架

u8 LuXian_DongTai[20][5];
u8 LuXian_DongTaii;

u8 Sum_Qian;

u8 LunPan[7];
u8 LunPani = 0;

u8 Tui[2][7];
u8 Tuii = 0;

u8 A_S[7];
u8 A_X[7];
u8 A_i;

u8 D_S[3][7];
u8 D_X[3][7];
u8 D_i;

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
            for (u8 it = 0; it < 45; it++)
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
            for (u8 it = 0; it < 45; it++)
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
    TIM_SetCompare3(TIM1, 680);
    while (PFin(9) == 1)
        ;
    delay_ms(500);
    while (PFin(9) == 0)
        ;
    TIM_SetCompare3(TIM1, 0); // 轮盘舵机500最快，800慢
    delay_ms(10);
    TIM_Cmd(TIM1, DISABLE);
}
void Zhua(u8 Flag_HuoJia)
{
    if (Flag_HuoJia == 'A') {
        A_i             = 0;
        Sum_Qian        = 0;
        LuXian_DongTaii = 0;
        memset(LuXian_DongTai, 0, 100);
        if (Flag_HuoJia_ShangXia == 'X') {
            if (A_X[5] != 'r') {
                Routecpy(0, 2, 0, 'r', 0);
            }
            Sum_Qian++;
            if (A_X[4] != 'r') {
                Routecpy(Sum_Qian, 0, 0, 255, 0);
                Sum_Qian = 0;
                Routecpy(0, 2, 0, 'r', 0);
            }
            Sum_Qian++;
            if (A_X[3] != 'g') {
                Routecpy(Sum_Qian, 0, 0, 255, 0);
                Sum_Qian = 0;
                Routecpy(0, 2, 0, 'g', 0);
            }
            Sum_Qian++;
            if (A_X[2] != 'g') {
                Routecpy(Sum_Qian, 0, 0, 255, 0);
                Sum_Qian = 0;
                Routecpy(0, 2, 0, 'g', 0);
            }
            Sum_Qian++;
            if (A_X[1] != 'b') {
                Routecpy(Sum_Qian, 0, 0, 255, 0);
                Sum_Qian = 0;
                Routecpy(0, 2, 0, 'b', 0);
            }
            Sum_Qian++;
            if (A_X[0] != 'b') {
                Routecpy(Sum_Qian, 0, 0, 255, 0);
                Sum_Qian = 0;
                Routecpy(0, 2, 0, 'b', 0);
            }
            Routecpy(Sum_Qian, 0, 0, 255, 0);
            Sum_Qian = 0;
            Routecpy(0, 0, 0, 0, 0);
            delay_ms(10);
            change_DongTai(LuXian_DongTai, 10);
        } else if (Flag_HuoJia_ShangXia == 'S') {
            if (A_S[0] != 'b') {
                Routecpy(0, 2, 0, 'b', 0);
            }
            Sum_Qian++;
            if (A_S[1] != 'b') {
                Routecpy(Sum_Qian, 0, 0, 255, 0);
                Sum_Qian = 0;
                Routecpy(0, 2, 0, 'b', 0);
            }
            Sum_Qian++;
            if (A_S[2] != 'g') {
                Routecpy(Sum_Qian, 0, 0, 255, 0);
                Sum_Qian = 0;
                Routecpy(0, 2, 0, 'g', 0);
            }
            Sum_Qian++;
            if (A_S[3] != 'g') {
                Routecpy(Sum_Qian, 0, 0, 255, 0);
                Sum_Qian = 0;
                Routecpy(0, 2, 0, 'g', 0);
            }
            Sum_Qian++;
            if (A_S[4] != 'r') {
                Routecpy(Sum_Qian, 0, 0, 255, 0);
                Sum_Qian = 0;
                Routecpy(0, 2, 0, 'r', 0);
            }
            Sum_Qian++;
            if (A_S[5] != 'r') {
                Routecpy(Sum_Qian, 0, 0, 255, 0);
                Sum_Qian = 0;
                Routecpy(0, 2, 0, 'r', 0);
            }
            Routecpy(Sum_Qian, 0, 0, 255, 0);
            Sum_Qian = 0;
            Routecpy(0, 0, 0, 0, 0);
            delay_ms(10);
            change_DongTai(LuXian_DongTai, 10);
        }
    }

    if (Flag_HuoJia == 'B') {
        if (Flag_HuoJia_ShangXia == 'X') {
            delay_us(100);
            Tuii = Tuii - 1;
            delay_us(100);
            if (Tui[1][Tuii] != 'o') {
                if (Tui[1][Tuii] == 'r')
                    BMQ_MOVE(0, 15, 0); // 往右走
                else if (Tui[1][Tuii] == 'l')
                    BMQ_MOVE(1, 15, 0);           // 往左走
                Printf(USART2, "%s", zhiling[3]); // 推中间
                delay_ms(500);
                delay_ms(500);
                delay_ms(500);
                Printf(USART2, "%s", zhiling[2]);
            }

        } else if (Flag_HuoJia_ShangXia == 'S') {
            if (Tui[0][Tuii] != 'o') {
                if (Tui[0][Tuii] == 'r')
                    BMQ_MOVE(0, 15, 0); // 往右走
                else if (Tui[0][Tuii] == 'l')
                    BMQ_MOVE(1, 15, 0);           // 往左走
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
    if (Flag_HuoJia == 'C') {
        memset(USART3_RX_BUF, 0, 10); // 将数组清0
        USART3_RX_STA = 0;
        delay_us(10);
        Printf(USART3, "%c", 'C');
        while (1) {
            delay_ms(10);
            if (USART3_RX_STA & 0x8000) // 接收完成
            {
                delay_ms(10);
                if (Flag_HuoJia_ShangXia == 'X') {
                    if (USART3_RX_BUF[1] == 'k') {
                        for (u8 i = 0; i < 6; i++) {
                            if (LunPan[i] == 'm') {
                                LunPan[i] = 'k';
                                while (LunPani != i) {
                                    LunPan_Zhuan();
                                    LunPani++;
                                    if (LunPani == 6)
                                        LunPani = 0;
                                }
                                break;
                            }
                        }
                        Printf(USART2, "%s", zhiling[4]); // 推中间
                        delay_ms(500);
                        delay_ms(500);
                        delay_ms(500);
                        Printf(USART2, "%s", zhiling[2]);
                    }
                }
                if (Flag_HuoJia_ShangXia == 'S') {
                    if (USART3_RX_BUF[0] == 'k') {
                        for (u8 i = 0; i < 6; i++) {
                            if (LunPan[i] == 'm') {
                                LunPan[i] = 'k';
                                while (LunPani != i) {
                                    LunPan_Zhuan();
                                    LunPani++;
                                    if (LunPani == 6)
                                        LunPani = 0;
                                }
                                break;
                            }
                        }
                        Printf(USART2, "%s", zhiling[4]); // 推中间
                        delay_ms(500);
                        delay_ms(500);
                        delay_ms(500);
                        Printf(USART2, "%s", zhiling[2]);
                    }
                }
                break;
            }
        }
    }
    if (Flag_HuoJia == 'D') {
    }
}
void Nano_ChuLi(u8 Flag_HuoJia)
{
    if (Flag_HuoJia == 'A') {
        exFlag_HuoJia = 'A';
        memset(USART3_RX_BUF, 0, 10); // 将数组清0
        USART3_RX_STA = 0;
        delay_ms(5);
        USART3_Putc('A');
        while (1) {
            if (USART3_RX_STA & 0x8000) // 接收完成
            {

                A_S[A_i] = USART3_RX_BUF[0];
                A_X[A_i] = USART3_RX_BUF[1];
                A_i++;
                if (USART3_RX_BUF[2] == 's' || USART3_RX_BUF[2] == 'd') // 上层要抓
                {
                    Printf(USART2, "%s", zhiling[6]); // 抓上中间木块
                    for (u8 t = 0; t < 10; t++)
                        delay_ms(1000);
                    Printf(USART2, "%s", zhiling[0]); // 复位
                    LunPan[LunPani] = USART3_RX_BUF[0];
                    LunPani++;
                    if (LunPani == 6) LunPani = 0;
                    LunPan_Zhuan();
                }
                if (USART3_RX_BUF[2] == 'x' || USART3_RX_BUF[2] == 'd') // 下层要抓
                {
                    Printf(USART2, "%s", zhiling[8]); // 抓下中间木块
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
        delay_ms(5);
        Printf(USART3, "%c", 'B');
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
    if (Flag_HuoJia == 'C') {
        exFlag_HuoJia = 'C';
        memset(USART3_RX_BUF, 0, 10); // 将数组清0
        USART3_RX_STA = 0;
        delay_ms(5);
        Printf(USART3, "%c", 'C');
        while (1) {
            delay_ms(10);
            if (USART3_RX_STA & 0x8000) // 接收完成
            {
                if (USART3_RX_BUF[0] == 'm') // 上层要抓
                {
                    Printf(USART2, "%s", zhiling[5]); // 抓上中间饮料
                    for (u8 t = 0; t < 9; t++)
                        delay_ms(1000);
                    Printf(USART2, "%s", zhiling[0]); // 复位
                    LunPan[LunPani] = USART3_RX_BUF[0];
                    LunPani++;
                    if (LunPani == 6) LunPani = 0;
                    LunPan_Zhuan();
                    delay_ms(10);
                }
                if (USART3_RX_BUF[1] == 'm') // 下层要抓
                {
                    Printf(USART2, "%s", zhiling[7]); // 抓下中间木块
                    for (u8 t = 0; t < 11; t++)
                        delay_ms(1000);
                    Printf(USART2, "%s", zhiling[0]); // 复位
                    LunPan[LunPani] = USART3_RX_BUF[1];
                    LunPani++;
                    if (LunPani == 6) LunPani = 0;
                    LunPan_Zhuan();
                    delay_ms(10);
                }
                USART3_RX_STA = 0;
                break;
            }
        }
    }
    if (Flag_HuoJia == 'D') {
        exFlag_HuoJia = 'D';
        memset(USART3_RX_BUF, 0, 10); // 将数组清0
        USART3_RX_STA = 0;
        delay_ms(5);
        Printf(USART3, "%c", 'D');
        while (1) {
            delay_ms(10);
            if (USART3_RX_STA & 0x8000) // 接收完成
            {
                for (u8 i = 0; i < 3; i++) {
                    D_S[i][D_i] = USART3_RX_BUF[i];
                }
                for (u8 i = 3; i < 6; i++) {
                    D_X[i - 3][D_i] = USART3_RX_BUF[i];
                }
                D_i++;
                break;
            }
        }
    }
    USART3_RX_STA = 0;
}
void HandInit()
{
    Printf(USART2, "%s", zhiling[0]); // 复位
    memset(A_S, 0, 7);
    memset(A_X, 0, 7);
    memset(LunPan, 0, 10);
    memset(Tui, 0, 12);
    memset(D_S, 0, 21);
    memset(D_X, 0, 21);
    Sum_Qian        = 0;
    LuXian_DongTaii = 0;
    LunPani         = 0;
    Tuii            = 0;
    A_i             = 0;
    D_i             = 0;
}
void LunPan_Zhao_Tui(u8 f)
{
    if (f == 'r' || f == 'g' || f == 'b') {
        for (u8 i = 0; i <= 5; i++) {
            if (LunPan[i] == f) {
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
                delay_ms(500);
                break;
            }
        }
    }
}

void Routecpy(u8 st1, u8 ed2, u8 rd3, u8 th4, u8 th5)
{
    LuXian_DongTai[LuXian_DongTaii][0] = st1;
    LuXian_DongTai[LuXian_DongTaii][1] = ed2;
    LuXian_DongTai[LuXian_DongTaii][2] = rd3;
    LuXian_DongTai[LuXian_DongTaii][3] = th4;
    LuXian_DongTai[LuXian_DongTaii][4] = th5;
    LuXian_DongTaii++;
}
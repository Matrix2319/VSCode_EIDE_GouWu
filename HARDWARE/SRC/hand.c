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
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;    //步进电机翻转标志位
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	GPIO_SetBits(GPIOE,12);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;    //限位开关
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);

}

void BuJin_Zhuan(u8 flag)
{
    if (flag == 'I') {
		GPIO_SetBits(GPIOE,12);
        TIM_Cmd(TIM1, ENABLE);
        TIM_SetCompare2(TIM1, 100);
        while (GPIO_ReadInputDataBit(GPIOE, 8) == 0)
            ;
        TIM_SetCompare2(TIM1, 0);
        TIM_Cmd(TIM1, DISABLE);
    }
    if (flag == 'S') {
		GPIO_ResetBits(GPIOE,12);
        TIM_Cmd(TIM1, ENABLE);
        TIM_SetCompare2(TIM1, 100);
        while (GPIO_ReadInputDataBit(GPIOE, 8) == 0)
            ;
        TIM_SetCompare2(TIM1, 0);
        TIM_Cmd(TIM1, DISABLE);
    }
    if (flag == 'X') {
    }
}

void Zhua()//抓取动作，i为臂指向 收0 张1
{
	;
}
	
	


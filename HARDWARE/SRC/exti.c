#include "exti.h"
#include "timer.h"
#include "sys.h"
#include "st178.h"
#include "moto.h"
#include "ui.h"

char cunchu[4]={0,0,0,0};
char z_cha=0,y_cha=0,q_cha=0,h_cha=0;
u16 count_fz=0;
u8 fz_flag=0;

void fzopen()
{
	// cunchu[0] = zuo_sum;
	// cunchu[1] = you_sum;
	// cunchu[2] = qian_sum;
	// cunchu[3] = hou_sum;

	// Init_TIM6(99, 7199);
}

void fzclose()
{
	//TIM_Cmd(TIM6, DISABLE);
}

void Init_TIM6(u16 arr ,u16 psc)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能

	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	 
	//中断优先级NVIC设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM6, ENABLE);
}

void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		z_cha=zuo_sum-cunchu[0];
		if(z_cha<0)
			z_cha=-z_cha;
		y_cha=you_sum-cunchu[1];
		if(y_cha<0)
			y_cha=-y_cha;
		q_cha=qian_sum-cunchu[2];
		if(q_cha<0)
			q_cha=-q_cha;
		h_cha=hou_sum-cunchu[3];
		if(h_cha<0)
			h_cha=-h_cha;
		if(q_cha>0||h_cha>0 ||z_cha>0||y_cha>0)
		{
			count_fz=0;
			cunchu[0]=zuo_sum;
			cunchu[1]=you_sum;
			cunchu[2]=qian_sum;
			cunchu[3]=hou_sum;
		}
		else
		{
			count_fz++;
		}
		if(count_fz>=300)
		{
			fz_flag=1;
			stop();
			count_fz=0;
		}
		
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);	
	}

}




#include "timer.h"
#include "oled.h"




void Init_TIM8_PWM(u16 arr ,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);// 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //使能GPIO外设时钟使能
	
	//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);
	//设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);//CH1 ----A PC6   
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);//CH2 ----B PC7
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);//CH3 ----C PC8
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);//CH4 ----D PC9
	TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE 主输出使能	
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器

	TIM_Cmd(TIM8, ENABLE);  //使能TIM1

	TIM_SetCompare1(TIM8,0);	//FL	左前
	TIM_SetCompare2(TIM8,0);	//BL	左后
	TIM_SetCompare3(TIM8,0);	//BR	右后
	TIM_SetCompare4(TIM8,0);	//FR	右前

}

//在使用是要注释gpio里PE11和PE13的初始化
void Init_TIM1_PWM(u16 arr ,u16 psc)//19999  71    4999  71
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  //
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //使能GPIO外设时钟使能
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
	//设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_11 | GPIO_Pin_13; // TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;							  //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure); //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); // CH2 ----B PE11
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); // CH3 ----C PE13

	TIM_CtrlPWMOutputs(TIM1, ENABLE); // MOE 主输出使能

	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	 TIM_SetCompare2(TIM1, 0);
	  TIM_SetCompare3(TIM1, 0);

	TIM_Cmd(TIM1, DISABLE); //不使能TIM1
}


void Init_TIM6_PWM(u16 arr ,u16 psc)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure6;	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能

	//定时器TIM3初始化
	TIM_TimeBaseStructure6.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure6.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure6.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	
	TIM_TimeBaseStructure6 .TIM_CounterMode= TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure6); //根据指定的参数初始化TIMx的时间基数单位
	TIM_ClearFlag(TIM6, TIM_FLAG_Update); 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	TIM_Cmd(TIM6, ENABLE); 
	//中断优先级NVIC设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
}


/*四个编码器用到的定时器以及引脚
	TIME2 PA15 PB3
	TIME3 PA6  PA7
	TIME4 PB6  PB7
	TIME5 PA0  PA1
*/
void Init_BMQ(void)//编码器初始化
{
  	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	
	/* TIM2 clock source enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//禁用jtag,不然无法实现编码器计数
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);//重映像
	/*- 正交编码器输入引脚 PA->15-*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	/*- 正交编码器输入引脚 PB->3-*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	/*- TIM2编码器模式配置 -*/	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);              
                 
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性
	
	TIM_ICStructInit(&TIM_ICInitStructure);																																		//配置滤波器
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
		
	TIM2->CNT = 0;
	TIM_Cmd(TIM2, ENABLE);   
	
	
 /* TIM3 clock source enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*- 正交编码器输入引脚 PA->6   PA->7 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                           
	/*- TIM3编码器模式配置 -*/
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              
                 
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性
	
	TIM_ICStructInit(&TIM_ICInitStructure);																																		//配置滤波器
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
		
	TIM3->CNT = 0;
	TIM_Cmd(TIM3, ENABLE);   

	
 /* TIM4 clock source enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*- 正交编码器输入引脚 PB6   PB7 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                           
	/*- TIM4编码器模式配置 -*/	
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
               
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性
	
	TIM_ICStructInit(&TIM_ICInitStructure);																																		//配置滤波器
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
		
	TIM4->CNT = 0;
	TIM_Cmd(TIM4, ENABLE);   //启动TIM4定时器



 /* TIM5 clock source enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*- 正交编码器输入引脚 PA0   PA1 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                           
	/*- TIM5编码器模式配置 -*/	
	TIM_DeInit(TIM5);
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);  
	
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性

	TIM_ICStructInit(&TIM_ICInitStructure);																																		//配置滤波器
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
		
	TIM5->CNT = 0;
	TIM_Cmd(TIM5, ENABLE);   //启动TIM5定时器

}


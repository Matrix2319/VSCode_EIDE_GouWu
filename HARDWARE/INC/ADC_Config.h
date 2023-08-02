#ifndef _ADC_CONFIG_H
#define _ADC_CONFIG_H

#include "sys.h"


#define shang 88
#define xia	  22
#define zuo   44
#define you   66
#define zhong 55


#define ADC_DMA_CH0	 ADC_Channel_10	  //PC0
#define ADC_DMA_IO0  GPIO_Pin_0
#define ADC_DMA_PORT GPIOC

extern vu16 ADC_DMA_ConvertedValue[6];	//带DMA的6通道AD采样值
extern u8  Key_Num;
extern u8 Keykey_Flag;      //按键生效标志位
extern u8 KEY_TYPE;

void ADC_DMA_Configuration(void); 
void AD_AnJian(void);

void ADKey_Init(void);
u16 Get_Key_Adc(u8 ch);
u16 Get_Key_Adc_Average(u8 ch,u8 times);
u8 ADKey_Deal(u16 ADKey_Value);
void AnJian_Scan(void);


#endif

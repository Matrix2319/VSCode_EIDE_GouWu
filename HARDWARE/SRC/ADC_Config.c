/*
  ******************************************************************************
  * @����            �ױس�(668484)
  * @STM32��汾     V3.5.0
  * @����޸�����    2016-6-16
  * @����            ��DMA��AD����������������μ�IO_config.h
  * @ʹ��            6��ͨ������ֱֵ�Ӵ洢������ADC_DMA_ConvertedValue[6]�У�
  *                  ��ֱ�ӵ���
  ******************************************************************************
*/

#include "ADC_Config.h"

#include "oled.h"
#include "usart.h"	
#include "zuobiao.h"

u8  Key_Num;
u8 Keykey_Flag;      //������Ч��־λ
u8 KEY_TYPE;

vu16 ADC_DMA_ConvertedValue[6];	//��DMA��6ͨ��AD����ֵ

void ADC_DMAInit(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����

	DMA_DeInit(DMA1_Channel1); //�� DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStruct.DMA_PeripheralBaseAddr =(u32)&ADC1->DR;//DMA��Ӧ�������ַ	
	DMA_InitStruct.DMA_MemoryBaseAddr = (vu32)ADC_DMA_ConvertedValue;//�����ڴ���׵�ַ
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;		//������Ϊ���ݴ������Դ
	DMA_InitStruct.DMA_BufferSize = 1;	//DMA�����С
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ�Ĵ�������
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݿ��Ϊ 16 λ
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //�ڴ����ݿ��Ϊ 16 λ
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;//������ѭ������ģʽ
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;//DMAͨ�� xӵ�и����ȼ� 
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;//DMAͨ�� xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);

	DMA_Cmd(DMA1_Channel1, ENABLE);
}
void ADC_DMA_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	ADC_DMAInit();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);	  
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //����ADC��Ƶ����6 72M/6=12 

	GPIO_InitStructure.GPIO_Pin = ADC_DMA_IO0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_DMA_PORT,&GPIO_InitStructure);


	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 		//  ADC�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;   			//��ͨ��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	 	//ģ��ת������������ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC �����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 				//˳����й���ת���� ADC ͨ������Ŀ
	ADC_Init(ADC1,&ADC_InitStructure);


	ADC_RegularChannelConfig(ADC1, ADC_DMA_CH0, 1, ADC_SampleTime_55Cycles5);

	ADC_DMACmd(ADC1, ENABLE); //ʹ��ADC1��DMA
	ADC_Cmd(ADC1, ENABLE);//ʹ��ADC1

	ADC_ResetCalibration(ADC1);//����У׼
	while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ�����У׼���	
	ADC_StartCalibration(ADC1);	  //��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC1));  //�ȴ�У׼���

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//����ADCת��
}

void ADKey_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF |RCC_APB2Periph_ADC3, ENABLE );	  //ʹ��ADC3ͨ��ʱ��

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PF6 ��Ϊģ��ͨ����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOF, &GPIO_InitStructure);	

	ADC_DeInit(ADC3);  //��λADC3

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC3�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	
	ADC_Init(ADC3, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_Cmd(ADC3, ENABLE);	//ʹ��ָ����ADC3
	
	ADC_ResetCalibration(ADC3);	//ʹ�ܸ�λУ׼
	while(ADC_GetResetCalibrationStatus(ADC3));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC3);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC3));	 //�ȴ�У׼����
}

u16 Get_Key_Adc(u8 ch)
{
	ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC3,ADCͨ��,����ʱ��Ϊ239.5����
  
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);		//ʹ��ָ����ADC3�����ת����������
	 
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC3);	//�������һ��ADC3�������ת�����
}

u16 Get_Key_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for(t = 0;t < times;t++)
	{
		temp_val += Get_Key_Adc(ch);
	}
	return temp_val / times;
}

u8 ADKey_Deal(u16 ADKey_Value)
{
//	
//	sprintf(OLED_BUF,"%d   ",ADKey_Value/10);
//	LCD_16_HanZi_ASCII(0,0,OLED_BUF);
	
//	u16 adcx;

//	float temp;
//	

//	
//	adcx=ADC_DMA_ConvertedValue[0];    //����PC0-PC2�������ŵ�12λADֵ
//	temp=(float)adcx*(3.3/4096);		//ת���ɵ�ѹֵ
//			
//	if(temp>0.1  && temp<0.50)			return 's';
//	else if(temp>2.80 && temp<3.30)		return 'x';
//	else if(temp>1.40 && temp<1.80)		return 'z';
//	else if(temp>2.00 && temp<2.40)		return 'y';
//	else if(temp>0.90 && temp<1.25)		return 'o';
	
	
	
//	ADKey_Value = ADKey_Value/10;

	
	if(ADKey_Value > 2398 && ADKey_Value < 2498)return '*';   //.
	else if(ADKey_Value > 2830 && ADKey_Value < 2930)return '#';	   //#
	else if(ADKey_Value > 3665 && ADKey_Value < 3765)return 'y';   //��
	else if(ADKey_Value > 3251 && ADKey_Value < 3351)return 'x';   //��
	else if(ADKey_Value > 3447 && ADKey_Value < 3547)return 'z';   //��
	else if(ADKey_Value > 3870 && ADKey_Value < 3970)return 'o';  //��
	else if(ADKey_Value > 3064 && ADKey_Value < 3164)return 's'; //��
	else if(ADKey_Value > 2610 && ADKey_Value < 2710)return '0';
	else if(ADKey_Value > 2211 && ADKey_Value < 2311)return '9';
	else if(ADKey_Value > 1589 && ADKey_Value < 1689)return '6';
	else if(ADKey_Value > 970 && ADKey_Value < 1070)return '3';
	else if(ADKey_Value > 764 && ADKey_Value < 864)return '2';
	else if(ADKey_Value > 1417 && ADKey_Value < 1517)return '5';
	else if(ADKey_Value > 2011 && ADKey_Value < 2111)return '8';
	else if(ADKey_Value > 1812 && ADKey_Value < 1912)return '7';
	else if(ADKey_Value > 1188 && ADKey_Value < 1288)return '4';
	else if(ADKey_Value > 557 && ADKey_Value < 657)return '1';
	else return 0;

//	if(ADKey_Value > 2410 && ADKey_Value < 2470)return '*';   //.
//	else if(ADKey_Value > 2830 && ADKey_Value < 2930)return '#';	   //#
//	else if(ADKey_Value > 3670 && ADKey_Value < 3771)return 'y';   //?
//	else if(ADKey_Value > 3260&& ADKey_Value < 3360)return 'x';   //?
//	else if(ADKey_Value > 3450 && ADKey_Value < 3550)return 'z';   //?
//	else if(ADKey_Value > 3870 && ADKey_Value < 3970)return 'o';  //?
//	else if(ADKey_Value > 3070 && ADKey_Value < 3170)return 's'; //?
//	else if(ADKey_Value > 2620 && ADKey_Value < 2740)return '0';
//	else if(ADKey_Value > 2220 && ADKey_Value < 2330)return '9';
//	else if(ADKey_Value > 1600 && ADKey_Value < 1750)return '6';
//	else if(ADKey_Value > 980 && ADKey_Value < 1080)return '3';
//	else if(ADKey_Value > 770 && ADKey_Value <870 )return '2';
//	else if(ADKey_Value > 1420 && ADKey_Value < 1520)return '5';
//	else if(ADKey_Value > 2010 && ADKey_Value < 2110)return '8';
//	else if(ADKey_Value > 1825 && ADKey_Value < 1925)return '7';
//	else if(ADKey_Value > 1200 && ADKey_Value < 1300)return '4';
//	else if(ADKey_Value > 560 && ADKey_Value < 660)return '1';
//	else return 0;

			
		
}
u8 DMA_AD(void)
{
	u16 adcx;

	float temp;
	
//	sprintf(OLED_BUF,"%.1f     ",temp);
//	LCD_16_HanZi_ASCII(0,0,OLED_BUF);	
	
	adcx=ADC_DMA_ConvertedValue[0];    //����PC0-PC2�������ŵ�12λADֵ
	temp=(float)adcx*(3.3/4096);		//ת���ɵ�ѹֵ
			
	if(temp>0.1  && temp<0.50)			return 's';
	else if(temp>2.80 && temp<3.30)		return 'x';
	else if(temp>1.40 && temp<1.80)		return 'z';
	else if(temp>2.00 && temp<2.40)		return 'y';
	else if(temp>0.90 && temp<1.25)		return 'o';
	else 	return 0;
}


void AD_AnJian(void)
{	
	u8 AD_Get = 0;
	
	static uint8 ADKey_State=0;//0�����ɿ�״̬��1��ʾ����״̬
	static uint16 AD_Time=0,Long_count=0,Long_flag=0; 
	
	AD_Get = ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4,1));
//		sprintf(OLED_BUF,"%c     ",AD_Get);
//	LCD_16_HanZi_ASCII(0,0,OLED_BUF);	
	if(AD_Get)
	{
		Long_count++;
		if(Long_count>=7500)//��������ʱ��
		{
			Long_count=0;//����������ɨ���ٶ�
			Long_flag = 1;
			Keykey_Flag=1;
			Key_Num=AD_Get;
			KEY_TYPE=AD_Get;
			
//			BUZZ_YIN(3370);
		}

	}
	else 
	{
		Long_count = 0;
		Long_flag  = 0;
	}

	if(Long_flag == 0)
	{
		if(AD_Get>0&&ADKey_State==0)    //����������ȥ
		{
			AD_Time++;                  //����
			if(AD_Time>=20)
			{ 
				AD_Time=0;

				ADKey_State=1;            //ȷ���м�����
				Keykey_Flag=1;

				Key_Num=AD_Get;
				KEY_TYPE=AD_Get;
//				BUZZ_YIN(3370);
			}
		}

		if(ADKey_State&&AD_Get==0)//����ȥ������
		{

			ADKey_State=0;

		}
	}
//	if(SHAUNG_UI == 1)//˫���жϱ�־
//	{
//		Keykey_Flag=1;
//		KEY_TYPE='#';
//	}
}

void AnJian_Scan()
{
    if (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == '1') {
        while (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == '1')
            ;
        ZuoBiao_Go(-250, 250);
    }
    if (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == '3') {
        while (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == '3')
            ;
        ZuoBiao_Go(250, 250);
    }
    if (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == '4') {
        while (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == '4')
            ;
        ZuoBiao_Go(-250, -250);
    }
    if (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == '6') {
        while (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == '6')
            ;
        ZuoBiao_Go(250, -250);
    }
    if (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == 'o') {
        while (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == 'o')
            ;
        ZuoBiao_Go(0, 0); 
    }
	    if (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == '2') {
        while (ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)) == '2')
            ;
        ZuoBiao_Go(-250, 0);
        delay_ms(1000);
        ZuoBiao_Go(-250, 250);
        delay_ms(1000);
        ZuoBiao_Go(250, 250);
        delay_ms(1000);
        ZuoBiao_Go(250, -250);
        delay_ms(1000);
        ZuoBiao_Go(-250, -250);
        delay_ms(1000);
        ZuoBiao_Go(-250, 0);
        delay_ms(1000);
        ZuoBiao_Go(0, 0);
    }
}

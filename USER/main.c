#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "oled2.h"
#include "timer.h"
#include "gpio.h"
#include "ADC_Config.h"
#include "moto.h"
#include "move.h"
#include "st178.h"
#include "ui.h"
#include "stmflash.h"
#include "hand.h"

int AD_test = 0;
int tx_qian = 0;
int tx_hou  = 0;

int main(void)
{

    int tx_flag = 0;
    delay_init(); // ��ʱ��ʼ��
    OLED_Init();  // OLED��ʼ��
    LCD_CLS();    // OLED����
    //	TZXH_LOGO();//̨��ѧԺlogo��ʾ
    All_GPIO_Init(); // �õ���IO�ڳ�ʼ��
    // SHOP_GPIO_init();
    // BuJin_GPIO_Init();
    uart1_init(115200); // ����1
    uart2_init(115200);   // ����2
    uart3_init(9600);   // ����3

    Init_BMQ();              // ��������ʼ��
    Init_TIM1_PWM(2999, 71); // 0.5ms
    ADC_DMA_Configuration(); // ADC1��ʼ��(��֪����û���õ����ϳ�����)
    ADKey_Init();            // ACD3��ʼ��(�õ�)

    // Init_TIM8_PWM(899, 7); // ���PWM��

    read_from_flash(); // ��flash�ж�ȡ����

    LCD_CLS();
    Printf(USART3, "%s\r\n", "tlcdb");

    sprintf(OLED_BUF, "%s", "tlcdb");
    LCD_16_HanZi_ASCII(0, 0, OLED_BUF);
    delay_ms(100);
    TIM_SetCompare2(TIM1, 500);

    while (1) {



        // Printf(USART1, "%s\r\n", "TLCDB1!");
        // delay_ms(500);
        // Printf(USART2, "%s\r\n", "TLCDB2!");
        // delay_ms(500);
        // Printf(USART3, "%s\r\n", "TLCDB3!");

        // sprintf(OLED_BUF, "%c", ADKey_Deal(Get_Key_Adc_Average(ADC_Channel_4, 1)));
        // LCD_16_HanZi_ASCII(0, 0, OLED_BUF);



        // MenuOperate();
    }
}

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
    delay_init(); // 延时初始化
    OLED_Init();  // OLED初始化
    LCD_CLS();    // OLED清屏
    //	TZXH_LOGO();//台州学院logo显示
    All_GPIO_Init(); // 用到的IO口初始化
    SHOP_GPIO_init();
    uart1_init(115200); // 串口1
    uart2_init(9600);   // 串口2
    uart3_init(9600);   // 串口3

    Init_BMQ(); // 编码器初始化
    Init_TIM1_PWM(1499,69);//步进电机1499,69为600HZ
    ADC_DMA_Configuration(); // ADC1初始化(不知道有没有用到，上车调试)
    ADKey_Init();            // ACD3初始化(用到)

    Init_TIM8_PWM(899, 7); // 电机PWM波
    //	Init_TIM1_PWM(19999,71);//PE11 PE13舵机(预留)

    read_from_flash(); // 从flash中读取数据

    LCD_CLS();

    while (1) {
        MenuOperate();
    }
}

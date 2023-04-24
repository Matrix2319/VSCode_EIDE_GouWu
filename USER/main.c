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
    BuJin_GPIO_Init();
    uart1_init(115200); // 串口1
    uart2_init(9600);   // 串口2
    uart3_init(9600);   // 串口3

    Init_BMQ();               // 编码器初始化
    Init_TIM1_PWM(1499, 109); // 步进电机1499,69为600HZ,数字越打越慢
    ADC_DMA_Configuration();  // ADC1初始化(不知道有没有用到，上车调试)
    ADKey_Init();             // ACD3初始化(用到)

    Init_TIM8_PWM(899, 7); // 电机PWM波
    //	Init_TIM1_PWM(19999,71);//PE11 PE13舵机(预留)

    read_from_flash(); // 从flash中读取数据

    LCD_CLS();
    // BuJin_Zhuan('S');
    delay_ms(100);
    BuJin_Zhuan('I');
    delay_ms(100);
    LunPan_Zhuan();
    delay_ms(100);
        delay_ms(100);
    Printf(USART2, "%s\r\n", "tlcdb");
     Printf(USART3, "%s\r\n", "tlcdb");
    while (1) {
        // Printf(USART1, "%s\r\n", "TLCDB1!");
        // delay_ms(500);
        // Printf(USART2, "%s\r\n", "TLCDB2!");
        // delay_ms(500);
        // Printf(USART3, "%s\r\n", "TLCDB3!");
        // sprintf(OLED_BUF, "E7=%d", 1);
        // LCD_16_HanZi_ASCII(10, 6, OLED_BUF);

        // delay_ms(500);
        MenuOperate();
    }
}

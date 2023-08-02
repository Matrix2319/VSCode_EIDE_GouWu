#include "zuobiao.h"
#include "timer.h"
#include "delay.h"
#include "oled.h"
u16 x_JuLi_last = 1500;
u16 y_JuLi_last = 1500;

void ZuoBiao_Init()
{
   TIM_SetCompare3(TIM1, 1500); // PE13
   TIM_SetCompare2(TIM1, 1500); // PE13
}

void ZuoBiao_Go(float x, float y)// PE11��Ӧch2Ϊx��Ĵ����󣬸�С����  PE13��Ӧch3Ϊy��Ĵ����ϣ���С����
{
    u16 x_JuLi      = 1500;
    u16 y_JuLi      = 1500;
    x_JuLi = (u16)(-x*0.63  +1500); // x����pwm��Ӧ����
    y_JuLi = (u16)(y *0.63 + 1490);

    while (x_JuLi != x_JuLi_last) {
        if (x_JuLi > x_JuLi_last) {
            x_JuLi_last++;
            delay_ms(5);
        } else {
            x_JuLi_last--;
            delay_ms(5);
        }
        TIM_SetCompare2(TIM1, x_JuLi_last); // PE13
    }

    while (y_JuLi != y_JuLi_last) {
        if (y_JuLi > y_JuLi_last) {
            y_JuLi_last++;
            delay_ms(5);
        } else {
            y_JuLi_last--;
            delay_ms(5);
        }
        TIM_SetCompare3(TIM1, y_JuLi_last); // PE13
    }
    // sprintf(OLED_BUF, "%d  %d",x_JuLi, y_JuLi);
        // LCD_16_HanZi_ASCII(0, 0, OLED_BUF);
   
    
    
}
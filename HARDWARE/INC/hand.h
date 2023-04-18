#ifndef _HAND_H
#define _HAND_H

#include "sys.h"


//传送带
//void CSD(void);
u8 Hand_LOW_GO(void);
u8 Hand_LOW_BACK(void);
u8 Hand_HIGH_GO(void);
u8 Hand_HIGH_BACK(void);
////上层移动电机	
void Zhua();
void Zhua_mid(u16 i);
#endif 


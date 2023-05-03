#ifndef __SYSTICK_H
#define __SYSTICK_H


#include "stm32f10x.h"


void SysTick_Init(void);
void Delay_us(__IO u32 nTime);         // ��λ1us


#define delay_ms(x) delay_us(1000*x)	 //��λms


#endif /* __SYSTICK_H */

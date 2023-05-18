#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f10x.h"



#define BEEP_GPIO_PORT    	GPIOA			              
#define BEEP_GPIO_CLK 	    RCC_APB2Periph_GPIOA		
#define BEEP_GPIO_PIN		  GPIO_Pin_8			        


#define ON  1
#define OFF 0


#define BEEP(a)	if (a)	\
					GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);\
					else		\
					GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN)

void BEEP_GPIO_Config(void);
					
#endif /* __BEEP_H */

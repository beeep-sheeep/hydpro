#ifndef __LED_H
#define	__LED_H


#include "stm32f10x.h"


// R-red
#define LED1_GPIO_PORT    	GPIOB			              /* GPIO port */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO port clock */
#define LED1_GPIO_PIN		GPIO_Pin_5			        /* connect to SCL GPIO */

// G-green
#define LED2_GPIO_PORT    	GPIOB			              /* GPIO port  */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO port clock */
#define LED2_GPIO_PIN		GPIO_Pin_0			        /* connect to SCL GPIO */

// B-blue
#define LED3_GPIO_PORT    	GPIOB			              /* GPIO port */
#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO port clock */
#define LED3_GPIO_PIN		GPIO_Pin_1			        /* connect to SCL GPIO */


/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON1  0
#define OFF1 1


#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)



#define	digitalHi(p,i)		 {p->BSRR=i;}	 		
#define digitalLo(p,i)		 {p->BRR=i;}	 
#define digitalToggle(p,i) {p->ODR ^=i;} 


#define LED1_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)


//red
#define LED_RED  \
					LED1_ON;\
					LED2_OFF\
					LED3_OFF

//green
#define LED_GREEN		\
					LED1_OFF;\
					LED2_ON\
					LED3_OFF

//blue
#define LED_BLUE	\
					LED1_OFF;\
					LED2_OFF\
					LED3_ON

					
//yellow				
#define LED_YELLOW	\
					LED1_ON;\
					LED2_ON\
					LED3_OFF
//purple
#define LED_PURPLE	\
					LED1_ON;\
					LED2_OFF\
					LED3_ON

//cyan
#define LED_CYAN \
					LED1_OFF;\
					LED2_ON\
					LED3_ON
					
//white
#define LED_WHITE	\
					LED1_ON;\
					LED2_ON\
					LED3_ON
					
//black
#define LED_RGBOFF	\
					LED1_OFF;\
					LED2_OFF\
					LED3_OFF

void LED_GPIO_Config(void);

#endif /* __LED_H */

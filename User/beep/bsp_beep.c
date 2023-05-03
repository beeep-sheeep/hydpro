#include "bsp_beep.h"   

 /**
  * @brief  initial beep IO
  * @param  no
  * @retval no
  */
void BEEP_GPIO_Config(void)
{		
		/*define GPIO_InitTypeDef stucture*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*open beep GPIO clcok*/
		RCC_APB2PeriphClockCmd( BEEP_GPIO_CLK, ENABLE); 

		/*pin of GPIO*/															   
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;	

		/*push and pull*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*set GPIO speed is 50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*initial beep GPIO*/
		GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);			 
    
    /* beep off*/
		GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);	 
}
/*********************************************END OF FILE**********************/

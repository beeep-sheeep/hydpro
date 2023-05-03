#include "bsp_SysTick.h"

static __IO u32 TimingDelay;
 
 
 
void SysTick_Init( void )
{
	/* SystemFrequency / 1000    1ms 
	 * SystemFrequency / 100000	 10us
	 * SystemFrequency / 1000000 1us
	 */
	if ( SysTick_Config(SystemCoreClock / 100) )	// ST3.5.0 library
	{ 
		/* Capture error */ 
		while (1);
	}
	
  // enable dida clock  10ms 1 time
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
  
}



void delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	// enable dida clock
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
}



void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
/*********************************************END OF FILE**********************/

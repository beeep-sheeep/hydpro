#include "./dwt_delay/core_delay.h"   


#if USE_DWT_DELAY


#define  DWT_CR      *(__IO uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(__IO uint32_t *)0xE0001004
#define  DEM_CR      *(__IO uint32_t *)0xE000EDFC


#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)


void CPU_TS_TmrInit(void)
{
    /* enable DWT */
    DEM_CR |= (uint32_t)DEM_CR_TRCENA;                

    /* set DWT CYCCNT register to 0 */
    DWT_CYCCNT = (uint32_t)0u;

    /* enable Cortex-M DWT CYCCNT register */
    DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
}

uint32_t CPU_TS_TmrRd(void)
{        
  return ((uint32_t)DWT_CYCCNT);
}

//  * @brief  Read the current timestamp
//  */
//uint32_t HAL_GetTick(void)
//{        
//  return ((uint32_t)DWT_CYCCNT*1000/SysClockFreq);
//}

/**
  * @brief  Use CPU internal counting to achieve accurate delay, 32-bit counter
  */
void CPU_TS_Tmr_Delay_US(__IO uint32_t us)
{
  uint32_t ticks;
  uint32_t told,tnow,tcnt=0;

  /* Initializes the timestamp register inside the function£¬ */  
#if (CPU_TS_INIT_IN_DELAY_FUNCTION)  
  CPU_TS_TmrInit();
#endif
  
  ticks = us * (GET_CPU_ClkFreq() / 1000000);  /* beat number */      
  tcnt = 0;
  told = (uint32_t)CPU_TS_TmrRd();         /* The counter value at the time of entry */

  while(1)
  {
    tnow = (uint32_t)CPU_TS_TmrRd();  
    if(tnow != told)
    { 
 
      if(tnow > told)
      {
        tcnt += tnow - told;  
      }
      /* reload */
      else 
      {
        tcnt += UINT32_MAX - told + tnow; 
      } 
      
      told = tnow;

      if(tcnt >= ticks)break;
    }
  }
}

#endif

/*********************************************END OF FILE**********************/

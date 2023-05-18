#ifndef __COMMON_H
#define __COMMON_H



#include "stm32f10x.h"



/******************************* MACRO ***************************/
#define            macNVIC_PriorityGroup_x                     NVIC_PriorityGroup_2



/********************************** function ***************************************/
void                     USART_printf                       ( USART_TypeDef * USARTx, char * Data, ... );



#endif /* __COMMON_H */


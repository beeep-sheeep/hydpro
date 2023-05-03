#include "bsp_usart.h"

 /**
  * @brief  config NVIC
  * @param  no
  * @retval no
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* config USART iqr */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* priority*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* subpriority */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* enable iqr */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* intial config NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO config
  * @param  no
  * @retval no
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// open GPIO clock
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// open APB clock
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// USART Tx GPIO push and pull
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // USART Rx GPIO floating input
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// baud rate
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// wordlength
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// stop bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// parity number
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// hardware flow control
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// config work mode
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// USART init
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// priority
	NVIC_Configuration();
	
	// enable irq
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// enable serial port
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}

/*****************  send 1 bit  **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* send 1 bit to USART */
	USART_SendData(pUSARTx,ch);
		
	/* wait empty register */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** send 8 byte array ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* send 1 bit to USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* wait send finish */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  send string **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* wait finish */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/***************** send 16 byte data **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* high 8 bype */
	temp_h = (ch&0XFF00)>>8;
	/* low 8 bype */
	temp_l = ch&0XFF;
	
	/* send high 8*/
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* send low 8 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///printf function
int fputc(int ch, FILE *f)
{
		/* send 1 bit to serial port */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* wait */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///scanf¡¢getchar function
int fgetc(FILE *f)
{
		/* wait serial port input data */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}


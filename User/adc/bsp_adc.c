#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0,0,0};

/**
  * @brief  ADC GPIO initial
  * @param  no
  * @retval no
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// open ADC IO serial clock
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	// config ADC IO pin mode
	GPIO_InitStructure.GPIO_Pin = 	  ADC_PIN1|
																		ADC_PIN2|
																		ADC_PIN3|
																		ADC_PIN4|
																		ADC_PIN5|
																		ADC_PIN6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// initial ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);				
}

/**
  * @brief  config ADC work mode
  * @param  no
  * @retval no
  */
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	// open DMA clock
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	// open ADC clock
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	
	// deinit DMA controller
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	// config DMA initstructure
	// base adress£ºADC data register adress 
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC_x->DR ) );
	
	// memory address
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
	// data source for peripheral
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// buffer size£¬equal to the size of data destination 
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;
	
	// only one register£¬disable add adress
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// memory adress add
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// 2bit data
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// half word meory data size
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// circular transfer mode
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA high priority
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// disable memory to memory
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// initial DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// enable DMA channel
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	
	// ADC mode config
	// only use 1 ADC£¬single mode
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// enable scan mode
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 

	// enable continuous convert mode
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// disable external trigger
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// flush right
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// number of convert channel
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	
		
	// initial ADC
	ADC_Init(ADC_x, &ADC_InitStructure);
	
	// config ADC clock frequency 8 divid£¬which is 9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// config ADC channel transfer order and time
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
	
	// enable ADC DMA request
	ADC_DMACmd(ADC_x, ENABLE);
	
	// open ADC £¬enable convert
	ADC_Cmd(ADC_x, ENABLE);
	
	// initial ADC calibration register  
	ADC_ResetCalibration(ADC_x);
	// wait 
	while(ADC_GetResetCalibrationStatus(ADC_x));
	
	// ADC start calibrate
	ADC_StartCalibration(ADC_x);
	// wait 
	while(ADC_GetCalibrationStatus(ADC_x));
	
	// software start ADC convert 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

/**
  * @brief  ADC init
  * @param  no
  * @retval no
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
/*********************************************END OF FILE**********************/

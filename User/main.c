#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "bsp_esp8266.h"
#include "bsp_esp8266_test.h"
#include "bsp_adc.h"
#include "bsp_beep.h"
#include "math.h"
#include "./dwt_delay/core_delay.h"

// MDA to SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// calculate
extern uint16_t lcdid;
char set_c_char[10],set_l_char[10];     //threshold
char get_c_char[10],get_l_char[10];			//detect
u8 set_c=120,set_l=1;                   //threshold set

// store value	 
double ADC_ConvertedValueLocal[NOFCHANEL];        
double ppm[NOFCHANEL];  
double leakage[NOFCHANEL]; 

void LCD_show(void);
void volcal(void);
void LCD_data(void);
void ESP8266_SendDataTest(void);

// delay
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 


int main(void)
{		
	//initial
	USART_Config();
	CPU_TS_TmrInit();
	
	ADCx_Init();
	BEEP_GPIO_Config();
	ESP8266_Init();	
	ILI9341_Init();
	
	
	ILI9341_GramScan(6);
	ESP8266_StaTcpClient_Unvarnish_ConfigTest();
	
	LCD_SetFont(&Font8x16);  					
	LCD_SetColors(WHITE,BLACK);				
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* clear screen */
	LCD_show();
	
	while (1)
	{	
			volcal();
			LCD_data();
			ESP8266_SendDataTest();
			printf("\r\n hydrogen concentration = %f ppm \r\n",ppm[2]);
		
			printf("\r\n\r\n");
			Delay(0xffffee);		 
	}
}

void volcal(void)
{
			ADC_ConvertedValueLocal[2] =(double) ADC_ConvertedValue[2]/4096*3.3;
			ADC_ConvertedValueLocal[3] =(double) ADC_ConvertedValue[3]/4096*3.3;
			ADC_ConvertedValueLocal[4] =(double) ADC_ConvertedValue[4]/4096*3.3;
			ADC_ConvertedValueLocal[5] =(double) ADC_ConvertedValue[5]/4096*3.3;
		
			ppm[2]= pow(10,-2.5*log10(ADC_ConvertedValueLocal[2]/3.3)+0.5);
			ppm[3]= pow(10,-2.5*log10(ADC_ConvertedValueLocal[3]/3.3)+0.5);
			ppm[4]= pow(10,-2.5*log10(ADC_ConvertedValueLocal[4]/3.3)+0.5);
			ppm[5]= pow(10,-2.5*log10(ADC_ConvertedValueLocal[5]/3.3)+0.5);
}

void ESP8266_SendDataTest(void)
{
	char aaa[100];
	uint8_t ucStatus;
	if(ppm[2]>=100)
		sprintf(aaa,"Hydrogen concentration is : %g ppm \r\n",ppm[2]);
	printf ( "%s", aaa );                                             //print
  ESP8266_SendString ( ENABLE, aaa, 0, Single_ID_0 );               //send data
  
  if ( ucTcpClosedFlag )                                             //dectect whether disconnect
  {
    ESP8266_ExitUnvarnishSend ();                                    //quit transform
    do ucStatus = ESP8266_Get_LinkStatus ();                         //get connecting state
    while ( ! ucStatus );
    if ( ucStatus == 4 )                                             //reconnect
    {
      while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );  
      while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
    }
    while ( ! ESP8266_UnvarnishSend () );		
  }
}

void LCD_data(void)
{
	char aaa[100];
			if(ppm[2] < 100)
			{
			LCD_ClearLine(LINE(16));
			LCD_SetTextColor(GREEN);
			ILI9341_DispStringLine_EN_CH(LINE(9),"-not reached minimum value-");
			BEEP( 0 ); 	
			}
			else if(ppm[2]>=100 && ppm[2]<= set_c){                 //normal display
			LCD_ClearLine(LINE(16));
			LCD_SetTextColor(GREEN);
			sprintf(aaa,"Hydrogen CONC: %g",ppm[2]);
			LCD_ClearLine(LINE(9));	
			ILI9341_DispStringLine_EN_CH(LINE(9),aaa);
			BEEP( 0 );                 //beep off
			}
			else{                            
			LCD_SetTextColor(RED); //red
			sprintf(aaa,"Hydrogen CONC: %g",ppm[2]);
			LCD_ClearLine(LINE(9));	
			ILI9341_DispStringLine_EN_CH(LINE(9),aaa);
			ILI9341_DispStringLine_EN_CH(LINE(16),"     £¡£¡£¡WARRING £¡£¡£¡   ");
			printf("£¡£¡£¡The concentration exceeds the threshold£¡£¡£¡");         //serial output
			BEEP( 1 );                 //beeep on
		  }
}

void LCD_show(void)
{
  ILI9341_DispStringLine_EN_CH(LINE(3),"Hydrogen detection system");
	
	LCD_SetTextColor(GREEN);
	sprintf(set_c_char,"CONC THR:%d ppm ",set_c);
  LCD_ClearLine(LINE(6));
	ILI9341_DispStringLine_EN_CH(LINE(6),set_c_char);                    
	
	LCD_SetTextColor(GREEN);
	sprintf(set_l_char,"leakage rate THR:%d",set_l);
  LCD_ClearLine(LINE(7));
	ILI9341_DispStringLine_EN_CH(LINE(7),set_l_char); 
	
	LCD_SetTextColor(GREEN);
	sprintf(get_c_char,"Hydrogen CONC:%g ppm ",ppm[2]);
  LCD_ClearLine(LINE(9));
	ILI9341_DispStringLine_EN_CH(LINE(9),get_c_char);                    
	
	LCD_SetTextColor(GREEN);
	sprintf(get_l_char,"Leakage rate:%g ",ppm[3]);
  LCD_ClearLine(LINE(10));
	ILI9341_DispStringLine_EN_CH(LINE(10),get_l_char); 
	
	LCD_ClearLine(LINE(13));
	
}
/*********************************************END OF FILE**********************/


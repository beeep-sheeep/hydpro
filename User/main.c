#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "bsp_esp8266.h"
#include "bsp_esp8266_test.h"
#include "bsp_adc.h"
#include "bsp_beep.h"
#include "bsp_led.h" 
#include "key.h"
#include "math.h"
#include "./dwt_delay/core_delay.h"
#include "ff.h"

//Kalman parameter
typedef struct Kalman
{
    double Last_P;//Last time estimated covariance(can not be 0)
    double Now_P;//Current estimated covariance
    double out;//Kalman filter output
    double Kg;//kalman gain
    double Q;//Process noise covariance
    double R;//Observed noise covariance
}Kalman;
 

// MDA to SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// calculate
struct Kalman kfp;
extern uint16_t lcdid;
char set_c_char[10],set_l_char[10];     //threshold
char get_c_char[10],get_l_char[10];			//detect
double set_c=1.5,set_l=1;                   //threshold set

// store value	 
double ADC_ConvertedValueLocal[NOFCHANEL];        
double ppm[NOFCHANEL];  
double leakage[NOFCHANEL]; 
int beep;

//LCD display
void LCD_show(void);
void LCD_data(void);

//filter and calculate
void Kalman_Init(void);
double KalmanFilter(Kalman *kfp,double input);
void volcal(void);

//Wifi send data
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
	LED_GPIO_Config();
	ADCx_Init();
	BEEP_GPIO_Config();
	Kalman_Init();
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
			printf("%f %d \r\n" ,ppm[2],beep);
			Delay(0xffffff);
	}
}

void volcal(void)
{
			ADC_ConvertedValue[2]=KalmanFilter(&kfp,ADC_ConvertedValue[2]);
			ADC_ConvertedValueLocal[2] =(double) ADC_ConvertedValue[2]/4096*3.3;		
			ppm[2]= pow(10,-2.5*log10(ADC_ConvertedValueLocal[2]/3.3)-1.3);

}

void ESP8266_SendDataTest(void)
{
	char aaa[100];
	uint8_t ucStatus;
	sprintf(aaa,"Hydrogen concentration is : %g ppm \r\n",ppm[2]);
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

void KEY1_IRQHandler(void)
{
}

void LCD_data(void)
{
	char aaa[100];
			if(ppm[2] < set_c)
			{
			LCD_ClearLine(LINE(16));
			LCD_SetTextColor(GREEN);
			sprintf(aaa,"Hydrogen CONC: %g ppm",ppm[2]);
			LCD_ClearLine(LINE(9));	
			ILI9341_DispStringLine_EN_CH(LINE(9),aaa);
			LED1_OFF;
			beep=0;
			BEEP( OFF );                 //beep off
			}
			else{                            
			LCD_SetTextColor(RED); //red
			sprintf(aaa,"Hydrogen CONC: %g ppm",ppm[2]);
			LCD_ClearLine(LINE(9));	
			ILI9341_DispStringLine_EN_CH(LINE(9),aaa);
			ILI9341_DispStringLine_EN_CH(LINE(16),"     £¡£¡£¡WARNING £¡£¡£¡   ");
			printf("£¡£¡£¡The concentration exceeds the threshold£¡£¡£¡");         //serial output
			LED_RED;
			beep=1;
			BEEP( ON );                 //beeep on
		  }
}

void LCD_show(void)
{
  ILI9341_DispStringLine_EN_CH(LINE(3),"Hydrogen detection system");
	
	LCD_SetTextColor(GREEN);
	sprintf(set_c_char,"CONC THR:%g ppm ",set_c);
  LCD_ClearLine(LINE(6));
	ILI9341_DispStringLine_EN_CH(LINE(6),set_c_char);                    
	
	LCD_SetTextColor(GREEN);
	sprintf(set_l_char,"leakage rate THR:%g",set_l);
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


//Kalman filter

void Kalman_Init()
{
	kfp.Last_P = 1;			
	kfp.Now_P = 0;		
	kfp.out = 0;			
	kfp.Kg = 0;		
	kfp.Q = 0;
	kfp.R = 0.01;
}

double KalmanFilter(Kalman *kfp,double input)
{
   //Prediction covariance equation: system estimation covariance at time k = system covariance at time k-1 + process noise covariance
   kfp->Now_P = kfp->Last_P + kfp->Q;
   //Kalman gain equation: Kalman gain = system estimation covariance at time k/(system estimation covariance at time k + observed noise covariance)
   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
   //Updated optimal value equation: optimal value of state variable at moment k = predicted value of state variable + Kalman gain * (measured value - predicted value of state variable)
   kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//The predicted value this time is the output value last time
   //Update the covariance equation: the covariance of this system is paid to kfp->LastP to prepare the next operation.
   kfp->Last_P = (1-kfp->Kg) * kfp->Now_P;
   return kfp->out;
}
/*********************************************END OF FILE**********************/


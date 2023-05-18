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

#include "./sdio/bsp_sdio_sdcard.h"
#include "ff.h"


//SD Card
FATFS fs;													/* FatFs file system object */
FIL fnew;													/* file object */
FRESULT res_sd;                		/* File operation results */
UINT fnum;            					  /* Number of successful reads and writes of files */
BYTE ReadBuffer[1024]={0};        /* read buffer */
char WriteBuffer[30] =  {0};
extern  SD_CardInfo SDCardInfo;

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
double set_c=20,set_l=1;                   //threshold set

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

//
void KEY1_IRQHandler(void);

// delay
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 

bool writeflag = 0;
int i = 0;

int main(void)
{		
	//initial
	LED_GPIO_Config();
	USART_Config();
	EXTI_Key_Config(); 
	//Mount SD card
	res_sd = f_mount(&fs,"0:",1);
	/*****SD test*************************************************************/
		if(res_sd == FR_NO_FILESYSTEM)
	{
		printf("The SD card does not have a file system yet and is about to be formatted! \r\n");
    //format
		res_sd=f_mkfs("0:",0,0);							
		if(res_sd == FR_OK)
		{
			printf("SD has successfully formatted the file system!£\\r\n");
      //Unmount after formatting
			res_sd = f_mount(NULL,"0:",1);			
      //remount		
			res_sd = f_mount(&fs,"0:",1);
		}
		else
		{
			printf("format failed!!! \r\n");
			while(1);
		}
	}
  else if(res_sd!=FR_OK)
  {
    printf("SD failed to mount file system!!!(%d)\r\n",res_sd);
		while(1);
  }
  else
  {
    printf("SD successfully mounted file system! \r\n");
  }
	/* Open the file and create it if it does not exist */
	res_sd = f_open(&fnew, "hydrogen.txt",FA_CREATE_ALWAYS | FA_WRITE );
	/*********************************************************************************/	

	CPU_TS_TmrInit();
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
			printf("%f %d %f %f \r\n" ,ppm[2],beep, ADC_ConvertedValueLocal[3],ADC_ConvertedValueLocal[4]);
			Delay(0xffffff);
	}
}

void volcal(void)
{
			ADC_ConvertedValue[2]=KalmanFilter(&kfp,ADC_ConvertedValue[2]);
			ADC_ConvertedValueLocal[2] =(double) ADC_ConvertedValue[2]/4096*3.3;		
			ppm[2]= pow(10,-2.5*log10(ADC_ConvertedValueLocal[2]/3.3)-1.1);
			if(writeflag == 1)
			{
				sprintf(WriteBuffer,"%f\r\n",ppm[2]);
				res_sd=f_write(&fnew,WriteBuffer,sizeof(ppm[2])+2,&fnum);
				f_sync(&fnew);
			}
}

void KEY1_IRQHandler(void)
{
		if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
			{
				//
				writeflag = !writeflag;
				//
				EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
			}
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
			beep=1;
			LED_RED;
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


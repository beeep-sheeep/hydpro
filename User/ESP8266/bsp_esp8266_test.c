#include "bsp_esp8266_test.h"
#include "bsp_esp8266.h"
#include "./dwt_delay/core_delay.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "bsp_usart.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "bsp_SysTick.h"
#include "stm32f10x.h"

#define LED_CMD_NUMBER   8
char *ledCmd[8] = { "LED_RED","LED_GREEN","LED_BLUE","LED_YELLOW","LED_PURPLE","LED_CYAN","LED_WHITE","LED_RGBOFF" };
volatile uint8_t ucTcpClosedFlag = 0;

void ESP8266_StaTcpClient_Unvarnish_ConfigTest(void)
{
	
	LCD_SetFont(&Font8x16);
	LCD_SetColors(WHITE,BLACK);
	

	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* clear all */
  ILI9341_DispStringLine_EN_CH(LINE(1),"Configuring ESP8266 ......");
	ILI9341_DispStringLine_EN_CH(LINE(2),"Enable ESP8266 ......");
	
	macESP8266_CH_ENABLE();
	while( ! ESP8266_AT_Test() );
  while( ! ESP8266_DHCP_CUR () );  
	
	while( ! ESP8266_Net_Mode_Choose ( STA ) );

	ILI9341_DispStringLine_EN_CH(LINE(8),"Connecting WiFi...... ");
  while( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	ILI9341_DispStringLine_EN_CH(LINE(9),"WiFi connect...... success!");
	
	while( ! ESP8266_Enable_MultipleId ( DISABLE ) );
	
	ILI9341_DispStringLine_EN_CH(LINE(10),"Connecting Server ......  ");
	while( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	ILI9341_DispStringLine_EN_CH(LINE(11),"Server connect......  success");
	
	while( ! ESP8266_UnvarnishSend () );
	ILI9341_DispStringLine_EN_CH(LINE(12),"Enter transparent transmission mode");
	
	ILI9341_DispStringLine_EN_CH(LINE(13),"ESP8266 is configured");
  ILI9341_DispStringLine_EN_CH(LINE(14),"Read MQ8 success!");
	
}




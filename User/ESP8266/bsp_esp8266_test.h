#ifndef  __ESP8266_TEST_H
#define	 __ESP8266_TEST_H



#include "stm32f10x.h"



/********************************** parmeters**********************************/
#define      macUser_ESP8266_ApSsid                       "Impart 303"                //wifi
#define      macUser_ESP8266_ApPwd                        "Zyf0623.."           //password

#define      macUser_ESP8266_TcpServer_IP                 "192.168.31.27"      //host IP
#define      macUser_ESP8266_TcpServer_Port               "2668"               //host port


 
/********************************** external variable***************************************/
extern volatile uint8_t ucTcpClosedFlag;
extern int read_dht11_finish;



/********************************** test functions ***************************************/
void ESP8266_StaTcpClient_Unvarnish_ConfigTest(void);
//void ESP8266_SendDHT11DataTest(void);

#endif


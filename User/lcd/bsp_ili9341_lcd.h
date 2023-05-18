#ifndef      __BSP_ILI9341_LCD_H
#define	     __BSP_ILI9341_LCD_H


#include "stm32f10x.h"
#include "./font/fonts.h"



/******************************* ILI9341 FSMC parameter ***************************/
//FSMC_Bank1_NORSRAM LCD command address
#define      FSMC_Addr_ILI9341_CMD         ( ( uint32_t ) 0x60000000 )

//FSMC_Bank1_NORSRAM using for LCD data operate address      
#define      FSMC_Addr_ILI9341_DATA        ( ( uint32_t ) 0x60020000 )

//NOR/SRAM
#define      FSMC_Bank1_NORSRAMx           FSMC_Bank1_NORSRAM1



/******************************* ILI9341 8080 pin ***************************/
/******control signal line******/
//select NOR/SRAM
#define      ILI9341_CS_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_CS_PORT               GPIOD
#define      ILI9341_CS_PIN                GPIO_Pin_7

//DCpin
//PD11ΪFSMC_A16
#define      ILI9341_DC_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_DC_PORT               GPIOD
#define      ILI9341_DC_PIN                GPIO_Pin_11

//write enable
#define      ILI9341_WR_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_WR_PORT               GPIOD
#define      ILI9341_WR_PIN                GPIO_Pin_5

//read enable
#define      ILI9341_RD_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_RD_PORT               GPIOD
#define      ILI9341_RD_PIN                GPIO_Pin_4

//reset pin
#define      ILI9341_RST_CLK               RCC_APB2Periph_GPIOE
#define      ILI9341_RST_PORT              GPIOE
#define      ILI9341_RST_PIN               GPIO_Pin_1

//light pin
#define      ILI9341_BK_CLK                RCC_APB2Periph_GPIOD    
#define      ILI9341_BK_PORT               GPIOD
#define      ILI9341_BK_PIN                GPIO_Pin_12

/********data signal line***************/
#define      ILI9341_D0_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D0_PORT               GPIOD
#define      ILI9341_D0_PIN                GPIO_Pin_14

#define      ILI9341_D1_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D1_PORT               GPIOD
#define      ILI9341_D1_PIN                GPIO_Pin_15

#define      ILI9341_D2_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D2_PORT               GPIOD
#define      ILI9341_D2_PIN                GPIO_Pin_0

#define      ILI9341_D3_CLK                RCC_APB2Periph_GPIOD  
#define      ILI9341_D3_PORT               GPIOD
#define      ILI9341_D3_PIN                GPIO_Pin_1

#define      ILI9341_D4_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D4_PORT               GPIOE
#define      ILI9341_D4_PIN                GPIO_Pin_7

#define      ILI9341_D5_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D5_PORT               GPIOE
#define      ILI9341_D5_PIN                GPIO_Pin_8

#define      ILI9341_D6_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D6_PORT               GPIOE
#define      ILI9341_D6_PIN                GPIO_Pin_9

#define      ILI9341_D7_CLK                RCC_APB2Periph_GPIOE  
#define      ILI9341_D7_PORT               GPIOE
#define      ILI9341_D7_PIN                GPIO_Pin_10

#define      ILI9341_D8_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D8_PORT               GPIOE
#define      ILI9341_D8_PIN                GPIO_Pin_11

#define      ILI9341_D9_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D9_PORT               GPIOE
#define      ILI9341_D9_PIN                GPIO_Pin_12

#define      ILI9341_D10_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D10_PORT               GPIOE
#define      ILI9341_D10_PIN                GPIO_Pin_13

#define      ILI9341_D11_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D11_PORT               GPIOE
#define      ILI9341_D11_PIN                GPIO_Pin_14

#define      ILI9341_D12_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D12_PORT               GPIOE
#define      ILI9341_D12_PIN                GPIO_Pin_15

#define      ILI9341_D13_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D13_PORT               GPIOD
#define      ILI9341_D13_PIN                GPIO_Pin_8

#define      ILI9341_D14_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D14_PORT               GPIOD
#define      ILI9341_D14_PIN                GPIO_Pin_9

#define      ILI9341_D15_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D15_PORT               GPIOD
#define      ILI9341_D15_PIN                GPIO_Pin_10

/*************************************** debug ******************************************/
#define      DEBUG_DELAY()                

/***************************** ILI934 X label and Y label ***************************/
#define      ILI9341_DispWindow_X_Star		    0     //X label start
#define      ILI9341_DispWindow_Y_Star		    0     //Y label start

#define 			ILI9341_LESS_PIXEL	  							240			//height 
#define 			ILI9341_MORE_PIXEL	 								320			//width

extern uint16_t LCD_X_LENGTH,LCD_Y_LENGTH; 

//scan mode
//value is from 0-7
extern uint8_t LCD_SCAN_MODE;

/******************************* ILI934 common color ********************************/
#define      BACKGROUND		                BLACK   //backgroud black

#define      WHITE		 		                  0xFFFF	   //white
#define      BLACK                         0x0000	   //black
#define      GREY                          0xF7DE	   //gray
#define      BLUE                          0x001F	   //blue 
#define      BLUE2                         0x051F	   //light blue 
#define      RED                           0xF800	   //red 
#define      MAGENTA                       0xF81F	   //red purple
#define      GREEN                         0x07E0	   //green
#define      CYAN                          0x7FFF	   //blue green
#define      YELLOW                        0xFFE0	   //yellow
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF



/******************************* define ILI934 command ********************************/
#define      CMD_SetCoordinateX		 		    0x2A	     //set X label
#define      CMD_SetCoordinateY		 		    0x2B	     //set Y label
#define      CMD_SetPixel		 		        0x2C	     //full pixel


/* define LCD driver ID */
#define     LCDID_UNKNOWN             0
#define     LCDID_ILI9341             0x9341
#define     LCDID_ST7789V             0x8552


/********************************** declare ILI934 function ***************************************/
void                     ILI9341_Init                    ( void );
uint16_t                 ILI9341_ReadID                 ( void );
void                     ILI9341_Rst                     ( void );
void                     ILI9341_BackLed_Control         ( FunctionalState enumState );
void                     ILI9341_GramScan                ( uint8_t ucOtion );
void                     ILI9341_OpenWindow              ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void                     ILI9341_Clear                   ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void                     ILI9341_SetPointPixel           ( uint16_t usX, uint16_t usY );
uint16_t                 ILI9341_GetPointPixel           ( uint16_t usX , uint16_t usY );
void                     ILI9341_DrawLine                ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 );
void                     ILI9341_DrawRectangle           ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight,uint8_t ucFilled );
void                     ILI9341_DrawCircle              ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint8_t ucFilled );
void                     ILI9341_DispChar_EN             ( uint16_t usX, uint16_t usY, const char cChar );
void                     ILI9341_DispStringLine_EN      ( uint16_t line, char * pStr );
void                     ILI9341_DispString_EN      			( uint16_t usX, uint16_t usY, char * pStr );
void                     ILI9341_DispChar_CH             ( uint16_t usX, uint16_t usY, uint16_t usChar );
void                     ILI9341_DispString_CH           ( uint16_t usX, uint16_t usY,  char * pStr );
void                     ILI9341_DispString_EN_CH        (	uint16_t usX, uint16_t usY,  char * pStr );
void 											ILI9341_DispStringLine_EN_CH 	(  uint16_t line, char * pStr );
void 											ILI9341_DispString_EN_YDir 		(   uint16_t usX,uint16_t usY ,  char * pStr );
void 											ILI9341_DispString_EN_CH_YDir 	(   uint16_t usX,uint16_t usY , char * pStr );

void 											LCD_SetFont											(sFONT *fonts);
sFONT 										*LCD_GetFont											(void);
void 											LCD_ClearLine										(uint16_t Line);
void 											LCD_SetBackColor								(uint16_t Color);
void 											LCD_SetTextColor								(uint16_t Color)	;
void 											LCD_SetColors										(uint16_t TextColor, uint16_t BackColor);
void 											LCD_GetColors										(uint16_t *TextColor, uint16_t *BackColor);

void ILI9341_DisplayStringEx(uint16_t x, 		//x
																 uint16_t y, 				//y
																 uint16_t Font_width,	//width
																 uint16_t Font_Height,	//height
																 uint8_t *ptr,					//str
																 uint16_t DrawModel);  

void ILI9341_DisplayStringEx_YDir(uint16_t x, 		
																			 uint16_t y, 				
																			 uint16_t Font_width,	
																			 uint16_t Font_Height,	
																			 uint8_t *ptr,					
																			 uint16_t DrawModel);  

#endif /* __BSP_ILI9341_ILI9341_H */



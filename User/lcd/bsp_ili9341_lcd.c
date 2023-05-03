#include "./lcd/bsp_ili9341_lcd.h"
#include "./font/fonts.h"	
#include "stm32f10x_fsmc.h"


uint16_t LCD_X_LENGTH = ILI9341_LESS_PIXEL;
uint16_t LCD_Y_LENGTH = ILI9341_MORE_PIXEL;

// LCD screen scanning mode, this variable is mainly used to facilitate the selection of the calculation parameters of the touch screen
// The value ranges from 0 to 7
// Call ILI9341_GramScan to set the direction automatically changed
// The default value is used when the LCD is just initialized
uint8_t LCD_SCAN_MODE = 6;

//drive ic ID
uint16_t lcdid = LCDID_UNKNOWN;

static sFONT *LCD_Currentfonts = &Font8x16;  //english
static uint16_t CurrentTextColor   = BLACK;//text color
static uint16_t CurrentBackColor   = WHITE;//back color

__inline void                 ILI9341_Write_Cmd           ( uint16_t usCmd );
__inline void                 ILI9341_Write_Data          ( uint16_t usData );
__inline uint16_t             ILI9341_Read_Data           ( void );
static void                   ILI9341_Delay               ( __IO uint32_t nCount );
static void                   ILI9341_GPIO_Config         ( void );
static void                   ILI9341_FSMC_Config         ( void );
static void                   ILI9341_REG_Config          ( void );
static void                   ILI9341_SetCursor           ( uint16_t usX, uint16_t usY );
static __inline void          ILI9341_FillColor           ( uint32_t ulAmout_Point, uint16_t usColor );
static uint16_t               ILI9341_Read_PixelData      ( void );




/**
  * @brief  Write command to ILI9341
  * @param  usCmd: command to write (table register address)
  * @retval no
  */	
__inline void ILI9341_Write_Cmd ( uint16_t usCmd )
{
	* ( __IO uint16_t * ) ( FSMC_Addr_ILI9341_CMD ) = usCmd;
	
}


/**
  * @brief  Writes data to ILI9341
  * @param  usData :The data to be written
  * @retval 无
  */	
__inline void ILI9341_Write_Data ( uint16_t usData )
{
	* ( __IO uint16_t * ) ( FSMC_Addr_ILI9341_DATA ) = usData;
	
}


/**
  * @brief  read data from ILI9341
  * @param  no
  * @retval read data
  */	
__inline uint16_t ILI9341_Read_Data ( void )
{
	return ( * ( __IO uint16_t * ) ( FSMC_Addr_ILI9341_DATA ) );
	
}


/**
  * @brief  ILI9341 simple delay function
  * @param  nCount ：delay count
  * @retval no
  */	
static void ILI9341_Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}


/**
  * @brief  initial ILI9341 IO pin
  * @param  no
  * @retval no
  */
static void ILI9341_GPIO_Config ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* enable FSMC pin clock*/
	RCC_APB2PeriphClockCmd ( 	
													/*control signal*/
													ILI9341_CS_CLK|ILI9341_DC_CLK|ILI9341_WR_CLK|
													ILI9341_RD_CLK	|ILI9341_BK_CLK|ILI9341_RST_CLK|
													/*data signal*/
													ILI9341_D0_CLK|ILI9341_D1_CLK|	ILI9341_D2_CLK | 
													ILI9341_D3_CLK | ILI9341_D4_CLK|ILI9341_D5_CLK|
													ILI9341_D6_CLK | ILI9341_D7_CLK|ILI9341_D8_CLK|
													ILI9341_D9_CLK | ILI9341_D10_CLK|ILI9341_D11_CLK|
													ILI9341_D12_CLK | ILI9341_D13_CLK|ILI9341_D14_CLK|
													ILI9341_D15_CLK	, ENABLE );
		
	
	/* config FSMC data pin,FSMC-D0~D15 */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D0_PIN;
	GPIO_Init ( ILI9341_D0_PORT, & GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = ILI9341_D1_PIN;
	GPIO_Init ( ILI9341_D1_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D2_PIN;
	GPIO_Init ( ILI9341_D2_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D3_PIN;
	GPIO_Init ( ILI9341_D3_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D4_PIN;
	GPIO_Init ( ILI9341_D4_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D5_PIN;
	GPIO_Init ( ILI9341_D5_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D6_PIN;
	GPIO_Init ( ILI9341_D6_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D7_PIN;
	GPIO_Init ( ILI9341_D7_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D8_PIN;
	GPIO_Init ( ILI9341_D8_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D9_PIN;
	GPIO_Init ( ILI9341_D9_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D10_PIN;
	GPIO_Init ( ILI9341_D10_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D11_PIN;
	GPIO_Init ( ILI9341_D11_PORT, & GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = ILI9341_D12_PIN;
	GPIO_Init ( ILI9341_D12_PORT, & GPIO_InitStructure );	
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D13_PIN;
	GPIO_Init ( ILI9341_D13_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D14_PIN;
	GPIO_Init ( ILI9341_D14_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D15_PIN;
	GPIO_Init ( ILI9341_D15_PORT, & GPIO_InitStructure );
	

	
	/* config FSMC corresponded data pin
	 * FSMC_NOE   :LCD-RD
	 * FSMC_NWE   :LCD-WR
	 * FSMC_NE1   :LCD-CS
	 * FSMC_A16  	:LCD-DC
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_RD_PIN; 
	GPIO_Init (ILI9341_RD_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_WR_PIN; 
	GPIO_Init (ILI9341_WR_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_CS_PIN; 
	GPIO_Init ( ILI9341_CS_PORT, & GPIO_InitStructure );  
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_DC_PIN; 
	GPIO_Init ( ILI9341_DC_PORT, & GPIO_InitStructure );
	

  /* config LCD reset RST control pin*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_RST_PIN; 
	GPIO_Init ( ILI9341_RST_PORT, & GPIO_InitStructure );
	
	
	/* config LCD back light control pin BK*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_BK_PIN; 
	GPIO_Init ( ILI9341_BK_PORT, & GPIO_InitStructure );
}


 /**
  * @brief  LCD  FSMC mode config
  * @param  no
  * @retval no
  */
static void ILI9341_FSMC_Config ( void )
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure={0};
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming={0}; 	
	
	/* enable FSMC clock*/
	RCC_AHBPeriphClockCmd ( RCC_AHBPeriph_FSMC, ENABLE );

	//ADDSET is 1 HCLK 2/72M=28ns
	readWriteTiming.FSMC_AddressSetupTime      = 0x01;	 //adress set time
	//DATAST+ 1 HCLK = 5/72M=70ns	
	readWriteTiming.FSMC_DataSetupTime         = 0x04;	 //data set time
	//choose control mode
	//mode B, s NOR FLASH mode ，match to ILI9341 8080
	readWriteTiming.FSMC_AccessMode            = FSMC_AccessMode_B;	
	

	//ADDHLD mode A
	readWriteTiming.FSMC_AddressHoldTime       = 0x00;	 //adress hold time
	//Set the bus transition cycle only for NOR operations that reuse modes
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	//Set clock frequency division for synchronous type memory only
	readWriteTiming.FSMC_CLKDivision           = 0x00;
	//Data retention time, only for synchronous NOR
	readWriteTiming.FSMC_DataLatency           = 0x00;	

	
	FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAMx;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &readWriteTiming;  
	
	FSMC_NORSRAMInit ( & FSMC_NORSRAMInitStructure ); 
	
	
	/* enable FSMC_Bank1_NORSRAM4 */
	FSMC_NORSRAMCmd ( FSMC_Bank1_NORSRAMx, ENABLE );  
		
		
}


/**
 * @brief  initial ILI9341 register
 * @param  no
 * @retval no
 */
static void ILI9341_REG_Config ( void )
{
  lcdid = ILI9341_ReadID();
  
  if(lcdid == LCDID_ILI9341)
  {
    /*  Power control B (CFh)  */
    DEBUG_DELAY  ();
    ILI9341_Write_Cmd ( 0xCF  );
    ILI9341_Write_Data ( 0x00  );
    ILI9341_Write_Data ( 0x81  );
    ILI9341_Write_Data ( 0x30  );
    
    /*  Power on sequence control (EDh) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xED );
    ILI9341_Write_Data ( 0x64 );
    ILI9341_Write_Data ( 0x03 );
    ILI9341_Write_Data ( 0x12 );
    ILI9341_Write_Data ( 0x81 );
    
    /*  Driver timing control A (E8h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xE8 );
    ILI9341_Write_Data ( 0x85 );
    ILI9341_Write_Data ( 0x10 );
    ILI9341_Write_Data ( 0x78 );
    
    /*  Power control A (CBh) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xCB );
    ILI9341_Write_Data ( 0x39 );
    ILI9341_Write_Data ( 0x2C );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x34 );
    ILI9341_Write_Data ( 0x03 );
    
    /* Pump ratio control (F7h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xF7 );
    ILI9341_Write_Data ( 0x20 );
    
    /* Driver timing control B */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xEA );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    
    /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xB1 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x1B );
    
    /*  Display Function Control (B6h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xB6 );
    ILI9341_Write_Data ( 0x0A );
    ILI9341_Write_Data ( 0xA2 );
    
    /* Power Control 1 (C0h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xC0 );
    ILI9341_Write_Data ( 0x35 );
    
    /* Power Control 2 (C1h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xC1 );
    ILI9341_Write_Data ( 0x11 );
    
    /* VCOM Control 1 (C5h) */
    ILI9341_Write_Cmd ( 0xC5 );
    ILI9341_Write_Data ( 0x45 );
    ILI9341_Write_Data ( 0x45 );
    
    /*  VCOM Control 2 (C7h)  */
    ILI9341_Write_Cmd ( 0xC7 );
    ILI9341_Write_Data ( 0xA2 );
    
    /* Enable 3G (F2h) */
    ILI9341_Write_Cmd ( 0xF2 );
    ILI9341_Write_Data ( 0x00 );
    
    /* Gamma Set (26h) */
    ILI9341_Write_Cmd ( 0x26 );
    ILI9341_Write_Data ( 0x01 );
    DEBUG_DELAY ();
    
    /* Positive Gamma Correction */
    ILI9341_Write_Cmd ( 0xE0 ); //Set Gamma
    ILI9341_Write_Data ( 0x0F );
    ILI9341_Write_Data ( 0x26 );
    ILI9341_Write_Data ( 0x24 );
    ILI9341_Write_Data ( 0x0B );
    ILI9341_Write_Data ( 0x0E );
    ILI9341_Write_Data ( 0x09 );
    ILI9341_Write_Data ( 0x54 );
    ILI9341_Write_Data ( 0xA8 );
    ILI9341_Write_Data ( 0x46 );
    ILI9341_Write_Data ( 0x0C );
    ILI9341_Write_Data ( 0x17 );
    ILI9341_Write_Data ( 0x09 );
    ILI9341_Write_Data ( 0x0F );
    ILI9341_Write_Data ( 0x07 );
    ILI9341_Write_Data ( 0x00 );
    
    /* Negative Gamma Correction (E1h) */
    ILI9341_Write_Cmd ( 0XE1 ); //Set Gamma
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x19 );
    ILI9341_Write_Data ( 0x1B );
    ILI9341_Write_Data ( 0x04 );
    ILI9341_Write_Data ( 0x10 );
    ILI9341_Write_Data ( 0x07 );
    ILI9341_Write_Data ( 0x2A );
    ILI9341_Write_Data ( 0x47 );
    ILI9341_Write_Data ( 0x39 );
    ILI9341_Write_Data ( 0x03 );
    ILI9341_Write_Data ( 0x06 );
    ILI9341_Write_Data ( 0x06 );
    ILI9341_Write_Data ( 0x30 );
    ILI9341_Write_Data ( 0x38 );
    ILI9341_Write_Data ( 0x0F );
    
    /* memory access control set */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0x36 ); 	
    ILI9341_Write_Data ( 0xC8 );    /*Portrait scanning mode from upper left corner to lower right corner (starting point)*/
    DEBUG_DELAY ();
    
    /* column address control set */
    ILI9341_Write_Cmd ( CMD_SetCoordinateX ); 
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0xEF );
    
    /* page address control set */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( CMD_SetCoordinateY ); 
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x01 );
    ILI9341_Write_Data ( 0x3F );
    
    /*  Pixel Format Set (3Ah)  */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0x3a ); 
    ILI9341_Write_Data ( 0x55 );
    
    /* Sleep Out (11h)  */
    ILI9341_Write_Cmd ( 0x11 );	
    ILI9341_Delay ( 0xAFFf<<2 );
    DEBUG_DELAY ();
    
    /* Display ON (29h) */
    ILI9341_Write_Cmd ( 0x29 ); 
	}
  
  else if(lcdid == LCDID_ST7789V)
  {
    /*  Power control B (CFh)  */
    DEBUG_DELAY  ();
    ILI9341_Write_Cmd ( 0xCF  );
    ILI9341_Write_Data ( 0x00  );
    ILI9341_Write_Data ( 0xC1  );
    ILI9341_Write_Data ( 0x30  );
    
    /*  Power on sequence control (EDh) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xED );
    ILI9341_Write_Data ( 0x64 );
    ILI9341_Write_Data ( 0x03 );
    ILI9341_Write_Data ( 0x12 );
    ILI9341_Write_Data ( 0x81 );
    
    /*  Driver timing control A (E8h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xE8 );
    ILI9341_Write_Data ( 0x85 );
    ILI9341_Write_Data ( 0x10 );
    ILI9341_Write_Data ( 0x78 );
    
    /*  Power control A (CBh) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xCB );
    ILI9341_Write_Data ( 0x39 );
    ILI9341_Write_Data ( 0x2C );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x34 );
    ILI9341_Write_Data ( 0x02 );
    
    /* Pump ratio control (F7h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xF7 );
    ILI9341_Write_Data ( 0x20 );
    
    /* Driver timing control B */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xEA );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    
    
    /* Power Control 1 (C0h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xC0 );   //Power control
    ILI9341_Write_Data ( 0x21 );  //VRH[5:0]
    
    /* Power Control 2 (C1h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xC1 );   //Power control
    ILI9341_Write_Data ( 0x11 );  //SAP[2:0];BT[3:0]
    
    /* VCOM Control 1 (C5h) */
    ILI9341_Write_Cmd ( 0xC5 );
    ILI9341_Write_Data ( 0x2D );
    ILI9341_Write_Data ( 0x33 );
    
    /*  VCOM Control 2 (C7h)  */
//	ILI9341_Write_Cmd ( 0xC7 );
//	ILI9341_Write_Data ( 0XC0 );
    
    /* memory access control set */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0x36 );   //Memory Access Control
    ILI9341_Write_Data ( 0x00 );  /*Portrait scanning mode from upper left corner to lower right corner (starting point)*/
    DEBUG_DELAY ();
    
    ILI9341_Write_Cmd(0x3A);   
    ILI9341_Write_Data(0x55); 
    
      /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xB1 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x17 );
    
    /*  Display Function Control (B6h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xB6 );
    ILI9341_Write_Data ( 0x0A );
    ILI9341_Write_Data ( 0xA2 );
    
    ILI9341_Write_Cmd(0xF6);    			
    ILI9341_Write_Data(0x01); 
    ILI9341_Write_Data(0x30); 
    
    /* Enable 3G (F2h) */
    ILI9341_Write_Cmd ( 0xF2 );
    ILI9341_Write_Data ( 0x00 );
    
    /* Gamma Set (26h) */
    ILI9341_Write_Cmd ( 0x26 );
    ILI9341_Write_Data ( 0x01 );
    DEBUG_DELAY ();
    
    /* Positive Gamma Correction */
    ILI9341_Write_Cmd(0xe0); //Positive gamma
    ILI9341_Write_Data(0xd0);         
    ILI9341_Write_Data(0x00); 
    ILI9341_Write_Data(0x02); 
    ILI9341_Write_Data(0x07); 
    ILI9341_Write_Data(0x0b); 
    ILI9341_Write_Data(0x1a); 
    ILI9341_Write_Data(0x31); 
    ILI9341_Write_Data(0x54); 
    ILI9341_Write_Data(0x40); 
    ILI9341_Write_Data(0x29); 
    ILI9341_Write_Data(0x12); 
    ILI9341_Write_Data(0x12); 
    ILI9341_Write_Data(0x12); 
    ILI9341_Write_Data(0x17);

    /* Negative Gamma Correction (E1h) */
    ILI9341_Write_Cmd(0xe1); //Negative gamma
    ILI9341_Write_Data(0xd0); 
    ILI9341_Write_Data(0x00); 
    ILI9341_Write_Data(0x02); 
    ILI9341_Write_Data(0x07); 
    ILI9341_Write_Data(0x05); 
    ILI9341_Write_Data(0x25); 
    ILI9341_Write_Data(0x2d); 
    ILI9341_Write_Data(0x44); 
    ILI9341_Write_Data(0x45); 
    ILI9341_Write_Data(0x1c); 
    ILI9341_Write_Data(0x18); 
    ILI9341_Write_Data(0x16); 
    ILI9341_Write_Data(0x1c); 
    ILI9341_Write_Data(0x1d); 
  
	
//	/* column address control set */
//	ILI9341_Write_Cmd ( CMD_SetCoordinateX ); 
//	ILI9341_Write_Data ( 0x00 );
//	ILI9341_Write_Data ( 0x00 );
//	ILI9341_Write_Data ( 0x00 );
//	ILI9341_Write_Data ( 0xEF );
//	
//	/* page address control set */
//	DEBUG_DELAY ();
//	ILI9341_Write_Cmd ( CMD_SetCoordinateY ); 
//	ILI9341_Write_Data ( 0x00 );
//	ILI9341_Write_Data ( 0x00 );
//	ILI9341_Write_Data ( 0x01 );
//	ILI9341_Write_Data ( 0x3F );
	
	
    /* Sleep Out (11h)  */
    ILI9341_Write_Cmd ( 0x11 );	  //Exit Sleep
    ILI9341_Delay ( 0xAFFf<<2 );
    DEBUG_DELAY ();
    
    /* Display ON (29h) */
    ILI9341_Write_Cmd ( 0x29 );   //Display on
    
    ILI9341_Write_Cmd(0x2c);
  }
	
}


/**
 * @brief  ILI9341 initial 
 * @param  no
 * @retval no
 */
void ILI9341_Init ( void )
{
	ILI9341_GPIO_Config ();
	ILI9341_FSMC_Config ();
	
	ILI9341_BackLed_Control ( ENABLE );      //light up LCD
	ILI9341_Rst ();
	ILI9341_REG_Config ();
	
	ILI9341_GramScan(LCD_SCAN_MODE);
}


/**
 * @brief  ILI9341 back light LED control
 * @param  enumState ：whether use LED
  *   This parameter is one of the following values：
  *     @arg ENABLE :enable LED
  *     @arg DISABLE :disable LED
 * @retval no
 */
void ILI9341_BackLed_Control ( FunctionalState enumState )
{
	if ( enumState )
		GPIO_ResetBits ( ILI9341_BK_PORT, ILI9341_BK_PIN );	
	else
		GPIO_SetBits ( ILI9341_BK_PORT, ILI9341_BK_PIN );
		
}


/**
 * @brief  Read LCD driver chip ID function, can be used to test the underlying read and write function
 * @param  no
 * @retval Normally, the LCD driver is returned ID: LCDID_ILI9341/LCDID_ST7789V
 *         else returned: LCDID_UNKNOWN
 */
uint16_t ILI9341_ReadID(void)
{
	uint16_t id = 0;
	
	ILI9341_Write_Cmd(0x04);
	ILI9341_Read_Data();
	ILI9341_Read_Data();
	id = ILI9341_Read_Data();
	id <<= 8;
	id |= ILI9341_Read_Data();
	
  if(id == LCDID_ST7789V)
  {
    return id;
  }
  else
  {
    ILI9341_Write_Cmd(0xD3);
    ILI9341_Read_Data();
    ILI9341_Read_Data();
    id = ILI9341_Read_Data();
    id <<= 8;
    id |= ILI9341_Read_Data();
    if(id == LCDID_ILI9341)
    {
      return id;
    }
  }
  
	return LCDID_UNKNOWN;
}


/**
 * @brief  ILI9341 software reset
 * @param  no
 * @retval no
 */
void ILI9341_Rst ( void )
{			
	GPIO_ResetBits ( ILI9341_RST_PORT, ILI9341_RST_PIN );	 //low level reset

	ILI9341_Delay ( 0xAFF ); 					   

	GPIO_SetBits ( ILI9341_RST_PORT, ILI9341_RST_PIN );		 	 

	ILI9341_Delay ( 0xAFF ); 	
	
}




/**
 * @brief  Set scanning direction for GRAM of ILI9341 
 * @param  ucOption ：Select the scanning direction for GRAM
 *     @arg 0-7 :Parameter Optional values range from 0 to 7 in eight directions
 *
 *!! Modes 0, 3, 5 and 6 are suitable for displaying text from left to right.
 * It is not recommended to use other modes to display text		
 *		
 * The pixels in X direction and Y direction for modes 0, 2, 4 and 6 are 240 and 320 respectively
 * In the modes 1, 3, 5 and 7, the pixel in X direction is 320 and the pixel in Y direction is 240
 *
 * Mode 6 is the default display direction for most LCD routines
 * Mode 3 is the direction used by the camera routine
 * The 0 mode is the direction used by the BMP picture display routine
 *
 * @retval no


------------------------------------------------------------
mode0：				.		mode 1：		.	mode 2：			.	mode 3：					
					A		.					A		.		A					.		A									
					|		.					|		.		|					.		|							
					Y		.					X		.		Y					.		X					
					0		.					1		.		2					.		3					
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->	
------------------------------------------------------------	
mode 4：				.	mode 5：	  .	mode 6：			.	mode7：					
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->	
					4		.					5		.		6					.		7	
					Y		.					X		.		Y					.		X						
					|		.					|		.		|					.		|							
					V		.					V		.		V					.		V		
---------------------------------------------------------				
											 LCD example
								|-----------------|
								|			  Logo   		|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|-----------------|
								 positive screens（width 240，height 320）

 *******************************************************/
void ILI9341_GramScan ( uint8_t ucOption )
{	
	//For parameter check, only 0 to 7 can be entered
	if(ucOption >7 )
		return;
	
	//The value of LCD_SCAN_MODE is updated based on the mode. It is used to select calculation parameters on the touch screen
	LCD_SCAN_MODE = ucOption;
	
	//Update the pixel width in the XY direction based on the mode
	if(ucOption%2 == 0)	
	{
		//In mode 0 24 6, the width of pixels in the X direction is 240 and that in the Y direction is 320
		LCD_X_LENGTH = ILI9341_LESS_PIXEL;
		LCD_Y_LENGTH =	ILI9341_MORE_PIXEL;
	}
	else				
	{
		//In 1, 3, 5, 7 mode, the width of pixels in the X direction is 320 and that in the Y direction is 240
		LCD_X_LENGTH = ILI9341_MORE_PIXEL;
		LCD_Y_LENGTH =	ILI9341_LESS_PIXEL; 
	}

	//00x36 The higher 3 bits of the command argument can be used to set the GRAM scanning direction
	ILI9341_Write_Cmd ( 0x36 );
  if(lcdid == LCDID_ILI9341)
  {
    ILI9341_Write_Data ( 0x08 |(ucOption<<5));//LCD parameters can be set according to the value of ucOption. There are 0 to 7 modes
  }
  else if(lcdid == LCDID_ST7789V)
  {
    ILI9341_Write_Data ( 0x00 |(ucOption<<5));//LCD parameters can be set according to the value of ucOption. There are 0 to 7 modes
  }
	ILI9341_Write_Cmd ( CMD_SetCoordinateX ); 
	ILI9341_Write_Data ( 0x00 );		/* x start adress high 8 */
	ILI9341_Write_Data ( 0x00 );		/* x start adress low 8 */
	ILI9341_Write_Data ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x finish adress high 8 */	
	ILI9341_Write_Data ( (LCD_X_LENGTH-1)&0xFF );				/* x finish adress low 8 */

	ILI9341_Write_Cmd ( CMD_SetCoordinateY ); 
	ILI9341_Write_Data ( 0x00 );		/* y start adress high 8 */
	ILI9341_Write_Data ( 0x00 );		/* y start adress low 8 */
	ILI9341_Write_Data ( ((LCD_Y_LENGTH-1)>>8)&0xFF );	/* y finish adress high 8 */	 
	ILI9341_Write_Data ( (LCD_Y_LENGTH-1)&0xFF );				/* y finish adress low 8 */

	/* write gram start */
	ILI9341_Write_Cmd ( CMD_SetPixel );	
}



/**
 * @brief  Open a window on the ILI9341 monitor
 * @param  usX ：The starting X coordinate of the window under a particular scan direction
 * @param  usY ：The Y coordinate of the starting point of the window under a particular scan direction
 * @param  usWidth ：Width of window
 * @param  usHeight ：Height of window
 * @retval no
 */
void ILI9341_OpenWindow ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{	
	ILI9341_Write_Cmd ( CMD_SetCoordinateX ); 				 /* set X label */
	ILI9341_Write_Data ( usX >> 8  );	 /* high 8，then low 8 */
	ILI9341_Write_Data ( usX & 0xff  );	 /* set start and finish*/
	ILI9341_Write_Data ( ( usX + usWidth - 1 ) >> 8  );
	ILI9341_Write_Data ( ( usX + usWidth - 1 ) & 0xff  );

	ILI9341_Write_Cmd ( CMD_SetCoordinateY ); 			     /* set Y label*/
	ILI9341_Write_Data ( usY >> 8  );
	ILI9341_Write_Data ( usY & 0xff  );
	ILI9341_Write_Data ( ( usY + usHeight - 1 ) >> 8 );
	ILI9341_Write_Data ( ( usY + usHeight - 1) & 0xff );
	
}


/**
 * @brief  Set cursor coordinates for ILI9341
 * @param  usX ：The X coordinate of the cursor under a particular scan direction
 * @param  usY ：The Y coordinate of the cursor under a particular scanning direction
 * @retval no
 */
static void ILI9341_SetCursor ( uint16_t usX, uint16_t usY )	
{
	ILI9341_OpenWindow ( usX, usY, 1, 1 );
}


/**
 * @brief  Fills pixels with a certain color on the ILI9341 monitor
 * @param  ulAmout_Point ：The total number of pixels to fill the color with
 * @param  usColor ：color
 * @retval no
 */
static __inline void ILI9341_FillColor ( uint32_t ulAmout_Point, uint16_t usColor )
{
	uint32_t i = 0;
	
	
	/* memory write */
	ILI9341_Write_Cmd ( CMD_SetPixel );	
		
	for ( i = 0; i < ulAmout_Point; i ++ )
		ILI9341_Write_Data ( usColor );
		
	
}


/**
 * @brief  Clear a window of the ILI9341 monitor in a certain color
 * @param  usX ：The starting X coordinate of the window under a particular scan direction
 * @param  usY ：The Y coordinate of the starting point of the window under a particular scan direction
 * @param  usWidth ：Width of window
 * @param  usHeight ：Height of window
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no
 */
void ILI9341_Clear ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{
	ILI9341_OpenWindow ( usX, usY, usWidth, usHeight );

	ILI9341_FillColor ( usWidth * usHeight, CurrentBackColor );		
	
}


/**
 * @brief  Fill a point of the I9341 display with a certain color
 * @param  usX ：The x-coordinate of the point in a particular scanning direction
 * @param  usY ：The Y coordinate of the point in a particular scanning direction
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors set color
 * @retval no
 */
void ILI9341_SetPointPixel ( uint16_t usX, uint16_t usY )	
{	
	if ( ( usX < LCD_X_LENGTH ) && ( usY < LCD_Y_LENGTH ) )
  {
		ILI9341_SetCursor ( usX, usY );
		
		ILI9341_FillColor ( 1, CurrentTextColor );
	}
	
}


/**
 * @brief  Read a pixel of GRAM
 * @param  no
 * @retval pixel data
 */
static uint16_t ILI9341_Read_PixelData ( void )	
{	
	uint16_t usRG=0, usB=0 ;

	
	ILI9341_Write_Cmd ( 0x2E );   /* read data*/
	//Discard the result of the previous read
	ILI9341_Read_Data (); 	      /*FIRST READ OUT DUMMY DATA*/
	
	//Gets the values of the red and green channels
	usRG = ILI9341_Read_Data ();  	/*READ OUT RED AND GREEN DATA  */
	usB = ILI9341_Read_Data ();  		/*READ OUT BLUE DATA*/

  return ( (usRG&0xF800)| ((usRG<<3)&0x7E0) | (usB>>11) );
}


/**
 * @brief  Obtain pixel data of a coordinate point on the ILI9341 display
 * @param  usX ：The x-coordinate of the point in a particular scanning direction
 * @param  usY ：The Y coordinate of the point in a particular scanning direction
 * @retval pixel data
 */
uint16_t ILI9341_GetPointPixel ( uint16_t usX, uint16_t usY )
{ 
	uint16_t usPixelData;

	
	ILI9341_SetCursor ( usX, usY );
	
	usPixelData = ILI9341_Read_PixelData ();
	
	return usPixelData;
	
}


/**
 * @brief  Bresenham algorithm is used to draw line segments on the ILI9341 display
 * @param  usX1 ：An endpoint X coordinate of a line segment under a particular scanning direction
 * @param  usY1 ：An endpoint Y coordinate of a line segment under a particular scanning direction
 * @param  usX2 ：The X coordinate of the other endpoint of the line segment under a particular scanning direction
 * @param  usY2 ：The Y coordinate of the other endpoint of the line segment under a particular scanning direction
 * @note  use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no
 */
void ILI9341_DrawLine ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 )
{
	uint16_t us; 
	uint16_t usX_Current, usY_Current;
	
	int32_t lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance; 
	int32_t lIncrease_X, lIncrease_Y; 	
	
	
	lDelta_X = usX2 - usX1; //Computed coordinate increment
	lDelta_Y = usY2 - usY1; 
	
	usX_Current = usX1; 
	usY_Current = usY1; 
	
	
	if ( lDelta_X > 0 ) 
		lIncrease_X = 1; //Set the single-step direction
	
	else if ( lDelta_X == 0 ) 
		lIncrease_X = 0;//perpendicular line

	
	else 
  { 
    lIncrease_X = -1;
    lDelta_X = - lDelta_X;
  } 

	
	if ( lDelta_Y > 0 )
		lIncrease_Y = 1; 
	
	else if ( lDelta_Y == 0 )
		lIncrease_Y = 0;//horizontal line 
	
	else 
  {
    lIncrease_Y = -1;
    lDelta_Y = - lDelta_Y;
  } 

	
	if (  lDelta_X > lDelta_Y )
		lDistance = lDelta_X; //Select the basic increment axes
	
	else 
		lDistance = lDelta_Y; 

	
	for ( us = 0; us <= lDistance + 1; us ++ )//draw Line output
	{  
		ILI9341_SetPointPixel ( usX_Current, usY_Current );//draw point 
		
		lError_X += lDelta_X ; 
		lError_Y += lDelta_Y ; 
		
		if ( lError_X > lDistance ) 
		{ 
			lError_X -= lDistance; 
			usX_Current += lIncrease_X; 
		}  
		
		if ( lError_Y > lDistance ) 
		{ 
			lError_Y -= lDistance; 
			usY_Current += lIncrease_Y; 
		} 
		
	}  
	
	
}   


/**
 * @brief  Draw a rectangle on the ILI9341 monitor
 * @param  usX_Start ：The X coordinate of the starting point of the rectangle under a particular scanning direction
 * @param  usY_Start ：The Y coordinate of the starting point of the rectangle in a particular scanning direction
 * @param  usWidth：The width of the rectangle in pixels
 * @param  usHeight：The height of the rectangle in pixels
 * @param  ucFilled ：Select whether to fill the rectangle
  *   This parameter is one of the following values:
  *     @arg 0 :Outlined Rectangle
  *     @arg 1 :Filled Rectangle 
 * @note  use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no
 */
void ILI9341_DrawRectangle ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint8_t ucFilled )
{
	if ( ucFilled )
	{
		ILI9341_OpenWindow ( usX_Start, usY_Start, usWidth, usHeight );
		ILI9341_FillColor ( usWidth * usHeight ,CurrentTextColor);	
	}
	else
	{
		ILI9341_DrawLine ( usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start );
		ILI9341_DrawLine ( usX_Start, usY_Start + usHeight - 1, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );
		ILI9341_DrawLine ( usX_Start, usY_Start, usX_Start, usY_Start + usHeight - 1 );
		ILI9341_DrawLine ( usX_Start + usWidth - 1, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );		
	}

}


/**
 * @brief Circle drawing using Bresenham algorithm on ILI9341 display
 * @param  usX_Center ：The x-coordinate of the center of a circle under a particular scanning direction
 * @param  usY_Center ：The Y coordinate of the center of the circle under a particular scanning direction
 * @param  usRadius：The radius of a circle in pixels
 * @param  ucFilled ：Select whether to fill the circle
  *   This parameter is one of the following values:
  *     @arg 0 :hollow circle
  *     @arg 1 :solid circle
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no
 */
void ILI9341_DrawCircle ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint8_t ucFilled )
{
	int16_t sCurrentX, sCurrentY;
	int16_t sError;
	
	
	sCurrentX = 0; sCurrentY = usRadius;	  
	
	sError = 3 - ( usRadius << 1 );     //A marker to determine the position of the next point
	
	
	while ( sCurrentX <= sCurrentY )
	{
		int16_t sCountY;
		
		
		if ( ucFilled ) 			
			for ( sCountY = sCurrentX; sCountY <= sCurrentY; sCountY ++ ) 
			{                      
				ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCountY );           //1
				ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCountY );           //2       
				ILI9341_SetPointPixel ( usX_Center - sCountY,   usY_Center + sCurrentX );           //3
				ILI9341_SetPointPixel ( usX_Center - sCountY,   usY_Center - sCurrentX );           //4
				ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCountY );           //5    
        ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCountY );           //6
				ILI9341_SetPointPixel ( usX_Center + sCountY,   usY_Center - sCurrentX );           //7 	
        ILI9341_SetPointPixel ( usX_Center + sCountY,   usY_Center + sCurrentX );           //0				
			}
		
		else
		{          
			ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCurrentY );             //1
			ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCurrentY );             //2      
			ILI9341_SetPointPixel ( usX_Center - sCurrentY, usY_Center + sCurrentX );             //3
			ILI9341_SetPointPixel ( usX_Center - sCurrentY, usY_Center - sCurrentX );             //4
			ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCurrentY );             //5       
			ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCurrentY );             //6
			ILI9341_SetPointPixel ( usX_Center + sCurrentY, usY_Center - sCurrentX );             //7 
			ILI9341_SetPointPixel ( usX_Center + sCurrentY, usY_Center + sCurrentX );             //0
    }			
		
		
		sCurrentX ++;

		
		if ( sError < 0 ) 
			sError += 4 * sCurrentX + 6;	  
		
		else
		{
			sError += 10 + 4 * ( sCurrentX - sCurrentY );   
			sCurrentY --;
		} 	
		
		
	}
	
	
}

/**
 * @brief  Display an English character on the ILI9341 monitor
 * @param  usX ：The starting X coordinate of a character in a particular scan direction
 * @param  usY ：The starting Y coordinate of the point in a particular scanning direction
 * @param  cChar ：The English character to display
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no
 */
void ILI9341_DispChar_EN ( uint16_t usX, uint16_t usY, const char cChar )
{
	uint8_t  byteCount, bitCount,fontLength;	
	uint16_t ucRelativePositon;
	uint8_t *Pfont;
	
	//Offset ascii table (font table does not contain the first 32 non-graphic symbols of ASCII table)
	ucRelativePositon = cChar - ' ';
	
	//The number of bytes per font
	fontLength = (LCD_Currentfonts->Width*LCD_Currentfonts->Height)/8;
		
	// Prefix address of the font
	/* The ascii table offset is multiplied by the number of bytes per font to find the offset position of the font */
	Pfont = (uint8_t *)&LCD_Currentfonts->table[ucRelativePositon * fontLength];
	
	//Set display window
	ILI9341_OpenWindow ( usX, usY, LCD_Currentfonts->Width, LCD_Currentfonts->Height);
	
	ILI9341_Write_Cmd ( CMD_SetPixel );			

	// Read the font data in bytes
	// Since the display window is set directly above, the display data will be wrapped automatically
	for ( byteCount = 0; byteCount < fontLength; byteCount++ )
	{
			//Process the colors you want to display one by one
			for ( bitCount = 0; bitCount < 8; bitCount++ )
			{
					if ( Pfont[byteCount] & (0x80>>bitCount) )
						ILI9341_Write_Data ( CurrentTextColor );			
					else
						ILI9341_Write_Data ( CurrentBackColor );
			}	
	}	
}


/**
 * @brief  Displays an English string on the ILI9341 monitor
 * @param  line ：The starting Y coordinate of the string in a particular scan direction
  *   This parameter can use macros such as LINE(0) and LINE(1) to specify literal coordinates.
  *   The macro LINE(x) calculates the Y coordinate value based on the currently selected font.
	*		When displaying Chinese and using the LINE macro, you need to set the English font to Font8x16
 * @param  pStr ：The first address of the English string to be displayed
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no 
 */
void ILI9341_DispStringLine_EN (  uint16_t line,  char * pStr )
{
	uint16_t usX = 0;
	
	while ( * pStr != '\0' )
	{
		if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			line += LCD_Currentfonts->Height;
		}
		
		if ( ( line - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			line = ILI9341_DispWindow_Y_Star;
		}
		
		ILI9341_DispChar_EN ( usX, line, * pStr);
		
		pStr ++;
		
		usX += LCD_Currentfonts->Width;
		
	}
	
}


/**
 * @brief  Displays an English string on the ILI9341 monitor
 * @param  usX ：The starting X coordinate of a character in a particular scan direction
 * @param  usY ：The starting Y coordinate of a character in a particular scan direction
 * @param  pStr ：The first address of the English string to be displayed
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no
 */
void ILI9341_DispString_EN ( 	uint16_t usX ,uint16_t usY,  char * pStr )
{
	while ( * pStr != '\0' )
	{
		if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY += LCD_Currentfonts->Height;
		}
		
		if ( ( usY - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY = ILI9341_DispWindow_Y_Star;
		}
		
		ILI9341_DispChar_EN ( usX, usY, * pStr);
		
		pStr ++;
		
		usX += LCD_Currentfonts->Width;
		
	}
	
}


/**
 * @brief  Display English string on the ILI9341 monitor (along the Y axis)
 * @param  usX ：The starting X coordinate of a character in a particular scan direction
 * @param  usY ：The starting Y coordinate of a character in a particular scan direction
 * @param  pStr ：The first address of the English string to be displayed
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no
 */
void ILI9341_DispString_EN_YDir (	 uint16_t usX,uint16_t usY ,  char * pStr )
{	
	while ( * pStr != '\0' )
	{
		if ( ( usY - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) >LCD_Y_LENGTH  )
		{
			usY = ILI9341_DispWindow_Y_Star;
			usX += LCD_Currentfonts->Width;
		}
		
		if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) >  LCD_X_LENGTH)
		{
			usX = ILI9341_DispWindow_X_Star;
			usY = ILI9341_DispWindow_Y_Star;
		}
		
		ILI9341_DispChar_EN ( usX, usY, * pStr);
		
		pStr ++;
		
		usY += LCD_Currentfonts->Height;		
	}	
}

/**
 * @brief  Display a Chinese character on the ILI9341 monitor
 * @param  usX ：The starting X coordinate of a character in a particular scan direction
 * @param  usY ：The starting Y coordinate of a character in a particular scan direction
 * @param  usChar ：Chinese character to be displayed (GB code)
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no
 */ 
void ILI9341_DispChar_CH ( uint16_t usX, uint16_t usY, uint16_t usChar )
{
	uint8_t rowCount, bitCount;
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	
  uint16_t usTemp; 	

	//Set display window
	ILI9341_OpenWindow ( usX, usY, WIDTH_CH_CHAR, HEIGHT_CH_CHAR );
	
	ILI9341_Write_Cmd ( CMD_SetPixel );
	
	//Take the font data
  GetGBKCode ( ucBuffer, usChar );	
	
	for ( rowCount = 0; rowCount < HEIGHT_CH_CHAR; rowCount++ )
	{
    /* Take out two bytes of data, on the lcd is a Chinese character line */
		usTemp = ucBuffer [ rowCount * 2 ];
		usTemp = ( usTemp << 8 );
		usTemp |= ucBuffer [ rowCount * 2 + 1 ];
		
		for ( bitCount = 0; bitCount < WIDTH_CH_CHAR; bitCount ++ )
		{			
			if ( usTemp & ( 0x8000 >> bitCount ) )  // MSB First
			  ILI9341_Write_Data ( CurrentTextColor );				
			else
				ILI9341_Write_Data ( CurrentBackColor );			
		}		
	}
	
}


/**
 * @brief  Display Chinese string on the ILI9341 monitor
 * @param  line ：The starting Y coordinate of the string in a particular scan direction
		* This parameter can use macros such as LINE(0) and LINE(1) to specify text coordinates.
		* The LINE(x) macro calculates the Y coordinate value based on the currently selected font.
		* When displaying Chinese and using the LINE macro, you need to set the English font to Font8x16
 * @param  pStr ：The first address of the English string to be displayed
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no
 */
void ILI9341_DispString_CH ( 	uint16_t usX , uint16_t usY, char * pStr )
{	
	uint16_t usCh;

	
	while( * pStr != '\0' )
	{		
		if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY += HEIGHT_CH_CHAR;
		}
		
		if ( ( usY - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY = ILI9341_DispWindow_Y_Star;
		}	
		
		usCh = * ( uint16_t * ) pStr;	
	  usCh = ( usCh << 8 ) + ( usCh >> 8 );

		ILI9341_DispChar_CH ( usX, usY, usCh );
		
		usX += WIDTH_CH_CHAR;
		
		pStr += 2;           //A Chinese character has two bytes

	}	   
	
}


/**
 * @brief  Chinese and English strings are displayed on the ILI9341 monitor
 * @param  line ：The starting Y coordinate of the string in a particular scan direction
		* This parameter can use macros such as LINE(0) and LINE(1) to specify text coordinates.
		* The LINE(x) macro calculates the Y coordinate value based on the currently selected font.
		* When displaying Chinese and using the LINE macro, you need to set the English font to Font8x16
 * @param  pStr ：The first address of the string to display
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color 
 * @retval no
 */
void ILI9341_DispStringLine_EN_CH (  uint16_t line, char * pStr )
{
	uint16_t usCh;
	uint16_t usX = 0;
	
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//en string
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line += LCD_Currentfonts->Height;
			}
			
			if ( ( line - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line = ILI9341_DispWindow_Y_Star;
			}			
		
		  ILI9341_DispChar_EN ( usX, line, * pStr );
			
			usX +=  LCD_Currentfonts->Width;
		
		  pStr ++;

		}
		
		else	                            //ch string
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line += HEIGHT_CH_CHAR;
			}
			
			if ( ( line - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line = ILI9341_DispWindow_Y_Star;
			}	
			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			ILI9341_DispChar_CH ( usX, line, usCh );
			
			usX += WIDTH_CH_CHAR;
			
			pStr += 2;           //A Chinese character has two bytes
		
    }
		
  }	
} 

/**
 * @brief  Chinese and English strings are displayed on the ILI9341 monitor
 * @param  usX ：The starting X coordinate of a character in a particular scan direction
 * @param  usY ：The starting Y coordinate of a character in a particular scan direction
 * @param  pStr ：The first address of the string to display
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no
 */
void ILI9341_DispString_EN_CH ( 	uint16_t usX , uint16_t usY, char * pStr )
{
	uint16_t usCh;
	
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//en str
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY += LCD_Currentfonts->Height;
			}
			
			if ( ( usY - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY = ILI9341_DispWindow_Y_Star;
			}			
		
		  ILI9341_DispChar_EN ( usX, usY, * pStr );
			
			usX +=  LCD_Currentfonts->Width;
		
		  pStr ++;

		}
		
		else	                            //ch str
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY += HEIGHT_CH_CHAR;
			}
			
			if ( ( usY - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY = ILI9341_DispWindow_Y_Star;
			}	
			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			ILI9341_DispChar_CH ( usX, usY, usCh );
			
			usX += WIDTH_CH_CHAR;
			
			pStr += 2;           //1 ch 2 bytes
		
    }
		
  }	
} 

/**
 * @brief  Display strings in English and Chinese on ILI9341 monitor (along the Y-axis)
 * @param  usX ：The starting X coordinate of a character in a particular scan direction
 * @param  usY ：The starting Y coordinate of a character in a particular scan direction
 * @param  pStr ：The first address of the string to be displayed in both English and Chinese
 * @note use LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors function set color
 * @retval no
 */
void ILI9341_DispString_EN_CH_YDir (  uint16_t usX,uint16_t usY , char * pStr )
{
	uint16_t usCh;
	
	while( * pStr != '\0' )
	{			
			//The width and height of Chinese characters are uniformly used to calculate line breaks
			if ( ( usY - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) >LCD_Y_LENGTH  )
			{
				usY = ILI9341_DispWindow_Y_Star;
				usX += WIDTH_CH_CHAR;
			}			
			if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) >  LCD_X_LENGTH)
			{
				usX = ILI9341_DispWindow_X_Star;
				usY = ILI9341_DispWindow_Y_Star;
			}
			
		//display
		if ( * pStr <= 126 )	           	//en
		{			
			ILI9341_DispChar_EN ( usX, usY, * pStr);
			
			pStr ++;
			
			usY += HEIGHT_CH_CHAR;		
		}
		else	                            //ch
		{			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			ILI9341_DispChar_CH ( usX,usY , usCh );
			
			usY += HEIGHT_CH_CHAR;
			
			pStr += 2;           //1 ch 2 bytes
		
    }
		
  }	
} 

/***********************zoom****************************/
#define ZOOMMAXBUFF 16384
uint8_t zoomBuff[ZOOMMAXBUFF] = {0};	//store buffer，maximum 128*128
uint8_t zoomTempBuff[1024] = {0};

/**
	* @brief scales the font. The scaled font is represented by 1 pixel point and 8 data bits
		0x01 indicates handwriting and 0x00 indicates blank space
	* @param in_width: indicates the original character width
	* @param in_heig: indicates the original character height
	* @param out_width: Character width after scaling
	* @param out_heig: Character height after scaling
	* @param in_ptr: font input pointer Note: 1pixel 1bit
	* @param out_ptr: Scaled character output pointer Note: 1pixel 8bit
	* out_ptr does not actually have normal output, it is now output directly to the global pointer zoomBuff
	* @param en_cn: 0 indicates English, 1 indicates Chinese
	* @retval None
 */
void ILI9341_zoomChar(uint16_t in_width,	//Original character width
									uint16_t in_heig,		//Original character height
									uint16_t out_width,	//The width of the character after zoom
									uint16_t out_heig,	//The height of the character after zoom
									uint8_t *in_ptr,	//Font input pointer Note: 1pixel 1bit
									uint8_t *out_ptr, //Zoomed character output pointer Note: 1pixel 8bit
									uint8_t en_cn)		//0 is English and 1 is Chinese
{
	uint8_t *pts,*ots;
	//The scale factor is set according to the size of source and target font. The left shift of 16 is to convert floating-point operation into fixed-point operation
	unsigned int xrIntFloat_16=(in_width<<16)/out_width+1; 
  unsigned int yrIntFloat_16=(in_heig<<16)/out_heig+1;
	
	unsigned int srcy_16=0;
	unsigned int y,x;
	uint8_t *pSrcLine;
	
	uint16_t byteCount,bitCount;
	
	//Check whether the parameter is valid
	if(in_width >= 32) return;												//Font should not exceed 32 pixels
	if(in_width * in_heig == 0) return;	
	if(in_width * in_heig >= 1024 ) return; 					//The maximum input is 32 x 32
	
	if(out_width * out_heig == 0) return;	
	if(out_width * out_heig >= ZOOMMAXBUFF ) return; //Limit maximum zoom to 128*128
	pts = (uint8_t*)&zoomTempBuff;
	
	// For ease of calculation, the data in the font is mapped from 1pixel/ 1bit to 1pixel/8bit
	//0x01 indicates handwriting and 0x00 indicates blank space
	if(en_cn == 0x00)//en
	{
		//The upper and lower boundaries of English and Chinese characters are not correct. You can adjust them here. Note that tempBuff prevents overflows
			for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
			{
				for(bitCount=0;bitCount<8;bitCount++)
					{						
						//Map source font data from bits to bytes
						//If bitX in in_ptr is 1, the entire byte in pts is 1
						//If bitX in in_ptr is 0, the entire byte in pts is 0
						*pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
					}
			}				
	}
	else //中文
	{			
			for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
			{
				for(bitCount=0;bitCount<8;bitCount++)
					{						
						//Map source font data from bits to bytes
						//If bitX in in_ptr is 1, the entire byte in pts is 1
						//If bitX in in_ptr is 0, the entire byte in pts is 0
						*pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
					}
			}		
	}

	//zoom过程
	pts = (uint8_t*)&zoomTempBuff;	//A pointer to the mapped source data
	ots = (uint8_t*)&zoomBuff;	//A pointer to output data
	for (y=0;y<out_heig;y++)	/*traversal*/
    {
				unsigned int srcx_16=0;
        pSrcLine=pts+in_width*(srcy_16>>16);				
        for (x=0;x<out_width;x++) /*inline traversal*/
        {
            ots[x]=pSrcLine[srcx_16>>16]; //Copy the source font data to the target pointer
            srcx_16+=xrIntFloat_16;			//Offset source pixel points proportionally
        }
        srcy_16+=yrIntFloat_16;				  //Offset source pixel points proportionally
        ots+=out_width;						
    }
	out_ptr = (uint8_t*)&zoomBuff;	

	out_ptr++; 
}			


/**
* @brief displays characters using a scaled font
* @param Xpos: character display position x
* @param Ypos: Character display position y
* @param Font_width: character width
* @param Font_Heig: character height
* @param c: Font data to be displayed
* @param DrawModel: Whether to invert colors
* @retval None
 */
void ILI9341_DrawChar_Ex(uint16_t usX, //Character display position x
												uint16_t usY, //Character display position y
												uint16_t Font_width, //str width
												uint16_t Font_Height,  //str height 
												uint8_t *c,						//matrix data
												uint16_t DrawModel)		
{
  uint32_t index = 0, counter = 0;

	//set display window
	ILI9341_OpenWindow ( usX, usY, Font_width, Font_Height);
	
	ILI9341_Write_Cmd ( CMD_SetPixel );		
	
	// Read the font data in bytes
	// Since the display window is set directly above, the display data will be wrapped automatically
	for ( index = 0; index < Font_Height; index++ )
	{
			//Process the colors you want to display one by one
			for ( counter = 0; counter < Font_width; counter++ )
			{
					// Scaled font data, representing one pixel bit in one byte
					// A byte value of 1 indicates that the pixel is handwriting
					// An entire byte value of 0 indicates that the pixel is the background
					if ( *c++ == DrawModel )
						ILI9341_Write_Data ( CurrentBackColor );			
					else
						ILI9341_Write_Data ( CurrentTextColor );
			}	
	}	
}


/**
 * @brief Displays a string using a scaled font
 * @param  Xpos ：Character display position x
 * @param  Ypos ：Character display position y
 * @param  Font_width ：Character width, English characters on this basis /2. Note that it's even
 * @param  Font_Heig：Character height, note that it is even
 * @param  c ：The string to display
 * @param  DrawModel ：Whether to reverse color display 
 * @retval no
 */
void ILI9341_DisplayStringEx(uint16_t x, 		//Character display position x
														 uint16_t y, 				//Character display position y
														 uint16_t Font_width,	//Font width to display, English characters based on this /2. Note that it's even
														 uint16_t Font_Height,	//Note that the font height to be displayed is even
														 uint8_t *ptr,					//The character content displayed
														 uint16_t DrawModel)  //Whether to reverse color display


{
	uint16_t Charwidth = Font_width; //The default value is Font_width. The English width is half the Chinese width
	uint8_t *psr;
	uint8_t Ascii;	//en
	uint16_t usCh;  //ch
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	
	
	while ( *ptr != '\0')
	{
			/****deal with tab*****/
			if ( ( x - ILI9341_DispWindow_X_Star + Charwidth ) > LCD_X_LENGTH )
			{
				x = ILI9341_DispWindow_X_Star;
				y += Font_Height;
			}
			
			if ( ( y - ILI9341_DispWindow_Y_Star + Font_Height ) > LCD_Y_LENGTH )
			{
				x = ILI9341_DispWindow_X_Star;
				y = ILI9341_DispWindow_Y_Star;
			}	
			
		if(*ptr > 0x80) //if chinese
		{			
			Charwidth = Font_width;
			usCh = * ( uint16_t * ) ptr;				
			usCh = ( usCh << 8 ) + ( usCh >> 8 );
			GetGBKCode ( ucBuffer, usCh );	//Take the font data
			//Scale font data, source font is 16*16
			ILI9341_zoomChar(WIDTH_CH_CHAR,HEIGHT_CH_CHAR,Charwidth,Font_Height,(uint8_t *)&ucBuffer,psr,1); 
			//Display single character
			ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
			x+=Charwidth;
			ptr+=2;
		}
		else
		{
				Charwidth = Font_width / 2;
				Ascii = *ptr - 32;
				//Scale font data using a 16*24 font
				ILI9341_zoomChar(16,24,Charwidth,Font_Height,(uint8_t *)&Font16x24.table[Ascii * Font16x24.Height*Font16x24.Width/8],psr,0);
			  //Display single character
				ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
				x+=Charwidth;
				ptr++;
		}
	}
}


/**
 * @brief  Display string with scaled font (along the Y-axis)
 * @param  Xpos ：Character display position x
 * @param  Ypos ：Character display position y
 * @param  Font_width ：Character width, English characters on this basis /2. Note that it's even
 * @param  Font_Heig：Character height, note that it is even
 * @param  c ：The string to display
 * @param  DrawModel ：Whether to reverse color display
 * @retval no
 */
void ILI9341_DisplayStringEx_YDir(uint16_t x, 		//Character display position x
																		 uint16_t y, 				//Character display position y
																		 uint16_t Font_width,	//Font width to display, English characters based on this /2. Note that it's even
																		 uint16_t Font_Height,	//Note that the font height to be displayed is even
																		 uint8_t *ptr,					//The character content displayed
																		 uint16_t DrawModel)  //Whether to reverse color display
{
	uint16_t Charwidth = Font_width; //The default value is Font_width. The English width is half the Chinese width
	uint8_t *psr;
	uint8_t Ascii;	//en
	uint16_t usCh;  //ch
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	
	
	while ( *ptr != '\0')
	{			
			//The width and height of Chinese characters are uniformly used to calculate line breaks
			if ( ( y - ILI9341_DispWindow_X_Star + Font_width ) > LCD_X_LENGTH )
			{
				y = ILI9341_DispWindow_X_Star;
				x += Font_width;
			}
			
			if ( ( x - ILI9341_DispWindow_Y_Star + Font_Height ) > LCD_Y_LENGTH )
			{
				y = ILI9341_DispWindow_X_Star;
				x = ILI9341_DispWindow_Y_Star;
			}	
			
		if(*ptr > 0x80) //if chinese
		{			
			Charwidth = Font_width;
			usCh = * ( uint16_t * ) ptr;				
			usCh = ( usCh << 8 ) + ( usCh >> 8 );
			GetGBKCode ( ucBuffer, usCh );	//Take the font data
			//Scale font data, source font is 16*16
			ILI9341_zoomChar(WIDTH_CH_CHAR,HEIGHT_CH_CHAR,Charwidth,Font_Height,(uint8_t *)&ucBuffer,psr,1); 
			//Display single character
			ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
			y+=Font_Height;
			ptr+=2;
		}
		else
		{
				Charwidth = Font_width / 2;
				Ascii = *ptr - 32;
				//Scale font data using a 16*24 font
				ILI9341_zoomChar(16,24,Charwidth,Font_Height,(uint8_t *)&Font16x24.table[Ascii * Font16x24.Height*Font16x24.Width/8],psr,0);
			  //Display single character
				ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
				y+=Font_Height;
				ptr++;
		}
	}
}


/**
  * @brief  Set English font type
  * @param  fonts: Specifies the font to select
	*		Parameter is one of the following values
  * 	@arg：Font24x32;
  * 	@arg：Font16x24;
  * 	@arg：Font8x16;
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  获取当前字体类型
  * @param  None.
  * @retval 返回当前字体类型
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}


/**
  * @brief  Set the foreground (font) and background color of the LCD,RGB565
  * @param  TextColor: Specify the foreground (font) color
  * @param  BackColor: Specify background color
  * @retval None
  */
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor) 
{
  CurrentTextColor = TextColor; 
  CurrentBackColor = BackColor;
}

/**
  * @brief  Gets the foreground (font) and background color of the LCD,RGB565
  * @param  TextColor: Pointer variable used to store the foreground (font) color
  * @param  BackColor: A pointer variable used to store the background color
  * @retval None
  */
void LCD_GetColors(uint16_t *TextColor, uint16_t *BackColor)
{
  *TextColor = CurrentTextColor;
  *BackColor = CurrentBackColor;
}

/**
  * @brief  Set the foreground (font) color of the LCD,RGB565
  * @param  Color: Specify the foreground (font) color
  * @retval None
  */
void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
}

/**
  * @brief  Set the LCD background color,RGB565
  * @param  Color: Specify background color 
  * @retval None
  */
void LCD_SetBackColor(uint16_t Color)
{
  CurrentBackColor = Color;
}

/**
  * @brief Clear a line of text
  * @param  Line: Specifies the row to delete
  *   This parameter can use macros such as LINE(0) and LINE(1) to specify the line to be deleted.
  *   The macro LINE(x) calculates the y-coordinate value based on the currently selected font and deletes the x-th line of the current font height.
  * @retval None
  */
void LCD_ClearLine(uint16_t Line)
{
  ILI9341_Clear(0,Line,LCD_X_LENGTH,((sFONT *)LCD_GetFont())->Height);	/* 清屏，显示全黑 */

}
/*********************end of file*************************/




/**   
 ******************************************************************************   
 * @file    mylib/s4295255_radio.c 
 * @author  Mani Batra – 42952556   
 * @date    25032015   
 * @brief  Radio peripheral driver   
 *
 *			
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_radio_init() – initialise radio
 * s4295255_radio_setchan(unsigned char chan) – Set the channel of the radio
 * s4295255_radio_settxaddress(unsigned char *addr) – Set the transmit address of the radio
 * s4295255_radio_sendpacket(unsigned char *txpacket) - Send a packet
 * s4295255_radio_getpacket(unsigned char *txpacket) - Recieve a packet
 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 25/3/2015 - Created
 * 2. 19/3/2015 –  
 * 3. 23/3/2015 - 
 */


/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_ledbar.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define NRF24L01P_READ_REG        0x00	// Define read command to register
#define NRF24L01P_EN_AA           0x01	// 'Enable Auto Acknowledgment' register address
#define NRF24L01P_EN_RXADDR       0x02	// 'Enabled RX addresses' register address
#define NRF24L01P_WRITE_REG       0x20	// Define write command to register
#define NRF24L01P_TX_ADDR         0x10	// 'TX address' register address
#define NRF24L01P_RX_PW_P0        0x11	// 'RX payload width, pipe0' register address
#define NRF24L01P_RX_ADDR_P0      0x0A	// 'RX address pipe0' register address

#define NRF24L01P_TX_PLOAD_WIDTH  32  // 32 unsigned chars TX payload
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void write_to_buffer(uint8_t reg_addr, uint8_t *buffer, int buffer_len);
void write_to_register(uint8_t reg_addr, uint8_t val);
uint8_t sendRecv_Byte(uint8_t byte);


/**
  * @brief  Initialise the servo
  * @param  None
  * @retval None

  */

extern void s4295255_radio_init(){

	GPIO_InitTypeDef GPIO_spi;	

	BRD_LEDInit();		//Initialise Blue LED
	BRD_LEDOff();		//Turn off Blue LED

	/* Set SPI clock */
	__BRD_SPI_CLK();

	/* Enable GPIO Pin clocks */
	__BRD_SPI_SCK_GPIO_CLK();
	__BRD_SPI_MISO_GPIO_CLK();
	__BRD_SPI_MOSI_GPIO_CLK();
	__BRD_SPI_CS_GPIO_CLK();
	__BRD_D9_GPIO_CLK()
	
	/* Initialise SPI and Pin clocks*/
	/* SPI SCK pin configuration */
  	GPIO_spi.Pin = BRD_SPI_SCK_PIN;
  	GPIO_spi.Mode = GPIO_MODE_AF_PP;
  	GPIO_spi.Speed = GPIO_SPEED_HIGH;
	GPIO_spi.Pull = GPIO_PULLDOWN;
	GPIO_spi.Alternate = BRD_SPI_SCK_AF;
  	HAL_GPIO_Init(BRD_SPI_SCK_GPIO_PORT, &GPIO_spi);

  	/* SPI MISO pin configuration */
  	GPIO_spi.Pin = BRD_SPI_MISO_PIN;
	GPIO_spi.Mode = GPIO_MODE_AF_PP;
	GPIO_spi.Speed = GPIO_SPEED_FAST;
	GPIO_spi.Pull = GPIO_PULLUP;		//Must be set as pull up
	GPIO_spi.Alternate = BRD_SPI_MISO_AF;
  	HAL_GPIO_Init(BRD_SPI_MISO_GPIO_PORT, &GPIO_spi);

	/* SPI  MOSI pin configuration */
	GPIO_spi.Pin =  BRD_SPI_MOSI_PIN;
	GPIO_spi.Mode = GPIO_MODE_AF_PP;
	GPIO_spi.Pull = GPIO_PULLDOWN;
	GPIO_spi.Alternate = BRD_SPI_MOSI_AF;
  	HAL_GPIO_Init(BRD_SPI_MOSI_GPIO_PORT, &GPIO_spi);

	//Set SPI Module
	SpiHandle.Instance = (SPI_TypeDef *)(BRD_SPI);

	 __HAL_SPI_DISABLE(&SpiHandle);

	/* SPI to Master and 16 baudrate prescaler */
    SpiHandle.Init.Mode              = SPI_MODE_MASTER;
    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
    SpiHandle.Init.CRCPolynomial     = 0;
    SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS               = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;

	/* Initailise SPI parameters */
    HAL_SPI_Init(&SpiHandle);

	/* Enable SPI */
   __HAL_SPI_ENABLE(&SpiHandle);


	/* Configure GPIO PIN for SPI Chip select, TFT CS, TFT DC */
  	GPIO_spi.Pin = BRD_SPI_CS_PIN;				//Pin
  	GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	GPIO_spi.Pull = GPIO_PULLUP;			//Enable Pull up, down or no pull resister
  	GPIO_spi.Speed = GPIO_SPEED_FAST;			//Pin latency
  	HAL_GPIO_Init(BRD_SPI_CS_GPIO_PORT, &GPIO_spi);	//Initialise Pin

	/* Configure GPIO PIN for RX/TX mode */
  	GPIO_spi.Pin = NRF_MODE_PIN;
  	GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	GPIO_spi.Pull = GPIO_PULLUP;			//Enable Pull up, down or no pull resister
  	GPIO_spi.Speed = GPIO_SPEED_FAST;			//Pin latency
  	HAL_GPIO_Init(NRF_MODE_GPIO_PORT, &GPIO_spi);	//Initialise Pin

	/* Set chip select high */
	HAL_GPIO_WritePin(BRD_SPI_CS_GPIO_PORT, BRD_SPI_CS_PIN, 1);

}
extern void s4295255_radio_setchan(unsigned char chan){
	
    write_to_register(NRF24L01P_RF_CH, chan);        	// Select RF channel
    write_to_register(NRF24L01P_RF_SETUP, 0x06);   							// TX_PWR:0dBm, Datarate:1Mbps
    write_to_register(NRF24L01P_CONFIG, 0x02);	     							// Set PWR_UP bit, enable CRC(2 unsigned chars) & Prim:TX. MAX_RT & TX_DS enabled..

}

extern void s4295255_radio_settxaddress(unsigned char *addr){

	/* Set CE low for idle state */
	HAL_GPIO_WritePin(BRD_D9_GPIO_PORT, BRD_D9_PIN, 0);
	
	write_to_buffer(NRF24L01P_WRITE_REG | NRF24L01P_TX_ADDR, addr, 5);		// Writes TX_Address to nRF24L0
	write_to_buffer(NRF24L01P_WRITE_REG | NRF24L01P_RX_ADDR_P0, addr, 5);	//NRF24L01P_TX_ADR_WIDTH);

    write_to_register(NRF24L01P_EN_AA, 0x00);      							// Disable Auto.Ack
    write_to_register(NRF24L01P_EN_RXADDR, 0x01);  							// Enable Pipe0
    write_to_register(NRF24L01P_RX_PW_P0, NRF24L01P_TX_PLOAD_WIDTH); 			// Select same RX payload width as TX Payload width


}

extern void s4295255_radio_sendpacket(unsigned char *txpacket);
extern void s4295255_radio_getpacket(unsigned char *txpacket);


void write_to_buffer(uint8_t reg_addr, uint8_t *buffer, int buffer_len){

	int i;

	HAL_GPIO_WritePin(BRD_SPI_CS_GPIO_PORT, BRD_SPI_CS_PIN, 0)

	sendRecv_Byte(NRF24L01P_WRITE_REG | reg_addr);

	rfDelay(0x8FF);

#ifdef DEBUG
	debug_printf("DEBUG: WB: ");
#endif
	for (i = 0; i < buffer_len; i++) {
		
		/* Return the Byte read from the SPI bus */
		HAL_SPI_TransmitReceive(&SpiHandle, buffer[i], &rxbyte, 1, 1);
		rfDelay(0x100);	
#ifdef DEBUG
		debug_printf("%X ", buffer[i]);
#endif
	}

#ifdef DEBUG
	debug_printf("\n\r");
#endif

	HAL_GPIO_WritePin(BRD_SPI_CS_GPIO_PORT, BRD_SPI_CS_PIN, 1);

}

void write_to_register(uint8_t reg_addr, uint8_t val){

	HAL_GPIO_WritePin(BRD_SPI_CS_GPIO_PORT, BRD_SPI_CS_PIN, 0)

	sendRecv_Byte(NRF24L01P_WRITE_REG | reg_addr);
	sendRecv_Byte(val);

	HAL_GPIO_WritePin(BRD_SPI_CS_GPIO_PORT, BRD_SPI_CS_PIN, 1) 

}

uint8_t sendRecv_Byte(uint8_t byte) {

	uint8_t rxbyte;

	// *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout);
	HAL_SPI_TransmitReceive(&SpiHandle, &byte, &rxbyte, 1, 1);

	rfDelay(0x100);	
		
	return rxbyte; 
}
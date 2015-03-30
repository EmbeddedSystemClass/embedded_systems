/**
  ******************************************************************************
  * @file    ex10_spi/main.c 
  * @author  MDS
  * @date    03022015
  * @brief   SPI Read 32-bit Register nrf24l01plus status (0x07) register
  *			 NOTE: This example does not send or transmit with the nrf24l01plus.
  *			 REFERENCE: nrf24l01p_datasheet.pdf
  ******************************************************************************
  *  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SPI_HandleTypeDef SpiHandle;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO unsigned long nCount);
void HardwareInit();
uint8_t spi_sendbyte(uint8_t sendbyte);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {

	uint8_t status;
	
	BRD_init();
	HardwareInit();

	/* Main Processing Loop */
    while (1) {

		HAL_GPIO_WritePin(BRD_SPI_CS_GPIO_PORT, BRD_SPI_CS_PIN, 0);	//Set Chip Select low

		spi_sendbyte(0x07);										//Send status register address
	
		status = spi_sendbyte(0xFF);							//Send dummy byte, to read status register values			
		debug_printf("nrf24L01 Status Register Value: %X ", status);	//See page 59 of nrf24l01plus datasheet for status register definition
																//NOTE: default value of the status register is 0xE0		
		debug_printf("\n\r");
		HAL_GPIO_WritePin(BRD_SPI_CS_GPIO_PORT, BRD_SPI_CS_PIN, 1);		//Set Chip Select high	

    	BRD_LEDToggle();	//Toggle LED on/off
    	Delay(0x7FFF00);	//Delay function
  	}

}

/**
  * @brief  Initialise hardware modules
  * @param  None
  * @retval None
  */
void HardwareInit() {
	
	
}

/**
  * @brief  Send byte through SPI.
  * @param  sendbyte: byte to be transmitted via SPI.
  * @retval None
  */
uint8_t spi_sendbyte(uint8_t sendbyte) {

	uint8_t readbyte;

	HAL_SPI_TransmitReceive(&SpiHandle, &sendbyte, &readbyte, 1, 10);

	// Return the Byte read from the SPI bus 
	return readbyte;
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO unsigned long nCount) {
  while(nCount--) {
  }
}


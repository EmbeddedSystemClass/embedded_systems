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
#define CHANNEL	40
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t destination_addr[] = {0x12, 0x34, 0x56, 0x78, 0x90};
uint8_t source_addr[] = {0x42, 0x95, 0x2, 0x55, 0x6};
char packet_type = 0x20;
char payload[7];

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


		for(int i = 0; i < 7; i++){

			payload[i] = debug_getc();
			if(payload[i] = '\r') {
				debug_printf("You pressed \"Enter\"\n");
				break;
			}
		}

		debug_printf("Sending Packet\n");

		//hamming encoding and then send packet

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
	
	s4295255_radio_init();
	s4295255_radio_settxaddress(destination_addr);
	s4295255_radio_setchan(CHANNEL);
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


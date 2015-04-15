/**   
 ******************************************************************************   
 * @file    mylib/s4295255_manchester.c 
 * @author  Mani Batra – 42952556   
 * @date    10042015   
 * @brief  Manchester Encoding   
 *
 *			
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_manchester_byte_encode(uint8_t data) – manchester byte encode

 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 10/4/2015 - Created
	2 . 15/4/2015 - Added functionality to change the delay
 */


/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_manchester.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

void send_one(void); //function to output one as manchester code
void send_zero(void); //function to output zero as manchester code
void timeDelay(__IO unsigned long nCount);


/**
  * @brief  Manchester encode the 8 bits received 
  * @param  The char byte to encode
  * @retval none

  */
extern void s4295255_manchester_byte_encode(uint8_t data) {

	send_one(); //first start bit 
	send_one(); //second start bit

	uint16_t mask = 0x01; //mask to get bit form data


	int i = 0; 

	for(i = 0; i < 8; i++) {

		if((data & mask) == 0) {

			send_zero();

		} else {

			send_one();
		}

		mask <<= 1;

	}

		
	send_zero(); //stop bit
}
/**
  * @brief  Output zero on D1
  * @param  None
  * @retval none

  */
void send_zero(){
	Delay(delay1);  //0x04FF00 * is a delay of 10us
	HAL_GPIO_WritePin(BRD_D1_GPIO_PORT, BRD_D1_PIN, 0x01);

	Delay(delay1);  //0x04FF00 is a delay of 10us
	HAL_GPIO_WritePin(BRD_D1_GPIO_PORT, BRD_D1_PIN, 0x00);

	
	
}
/**
  * @brief  Output one on D1 
  * @param  none
  * @retval none

  */
void send_one(){
	Delay(delay1);  //0x04FF00 is a delay of 10us
	HAL_GPIO_WritePin(BRD_D1_GPIO_PORT, BRD_D1_PIN, 0x00);

	Delay(delay1);  //0x04FF00 is a delay of 10us
	HAL_GPIO_WritePin(BRD_D1_GPIO_PORT, BRD_D1_PIN, 0x01);

	
	
}


/**
  * @brief  Delay
  * @param  amount of delay
  * @retval none

  */
void timeDelay(__IO unsigned long nCount) {
  	while(nCount--) {
  	}
}

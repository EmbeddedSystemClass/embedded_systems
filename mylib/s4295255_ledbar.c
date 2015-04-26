/**   
 ******************************************************************************   
 * @file    mylib/s4295255_ledbar.c    
 * @author  Mani Batra – 42952556   
 * @date    06032015   
 * @brief   LED Light Bar peripheral driver   
 *	     REFERENCE: LEDLightBar_datasheet.pdf   
 *
 *			NOTE: REPLACE sxxxxxx with your student login.
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_ledbar_init() – intialise LED Light BAR
 * s4295255_ledbar_set() – set LED Light BAR value
 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 3/3/2015 - Created
 * 2. 10/3/2015 – Added functionality to set function.  
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_ledbar.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define STAGE2 1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Initialise LEDBar.
  * @param  None
  * @retval None
  */
extern void s4295255_ledbar_init(void) {

	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Enable the Clocks */
  	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();

	/* __BRD_D1_GPIO_CLK(); */

  	/* Configure the all LED pins as outputs */
	GPIO_InitStructure.Pin =  BRD_D2_PIN | BRD_D3_PIN | BRD_D7_PIN | BRD_D8_PIN | BRD_D9_PIN;				//Pin
  	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
  	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
 		//Initialise Pin

	GPIO_InitStructure.Pin =  BRD_D4_PIN | BRD_D5_PIN | BRD_D6_PIN | BRD_D10_PIN | BRD_D11_PIN;				//Pin
  	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
  	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
 		//Initialise Pin


	GPIO_InitStructure.Pin =  BRD_D0_PIN | BRD_D1_PIN;				//Pin
  	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
  	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
 		//Initialise Pin
	
}

/**
  * @brief  Set the LED Bar GPIO pins high or low, depending on the bit of ‘value’
  *         i.e. value bit 0 is 1 – LED Bar 0 on
  *          value bit 1 is 1 – LED BAR 1 on
  *
  * @param  value
  * @retval None
  */
extern void s4295255_ledbar_set(unsigned short value) {

	/* Use bit shifts (<< or >>) and bit masks (1 << bit_index) to determine if a bit is set

	   e.g. The following pseudo code checks if bit 0 of value is 1.
			if ((value & (1 << 0)) == (1 << 0))	{
				Turn on LED BAR Segment 0.
			}
		*/

#ifndef STAGE2

		if ((value & (1 << 0)) != 0)	{
				HAL_GPIO_WritePin(BRD_D0_GPIO_PORT, BRD_D0_PIN, 1 & 0x01);
			} else {	
				HAL_GPIO_WritePin(BRD_D0_GPIO_PORT, BRD_D0_PIN, 0 & 0x01);
			}

		if ((value & (1 << 1)) != 0)	{
				HAL_GPIO_WritePin(BRD_D1_GPIO_PORT, BRD_D1_PIN, 1 & 0x01);
			} else {	
				HAL_GPIO_WritePin(BRD_D1_GPIO_PORT, BRD_D1_PIN, 0 & 0x01);
			}

		if ((value & (1 << 2)) != 0)	{
				HAL_GPIO_WritePin(BRD_D2_GPIO_PORT, BRD_D2_PIN, 1 & 0x01);
			} else {	
				HAL_GPIO_WritePin(BRD_D2_GPIO_PORT, BRD_D2_PIN, 0 & 0x01);
			}

		if ((value & (1 << 3)) != 0)	{
				HAL_GPIO_WritePin(BRD_D3_GPIO_PORT, BRD_D3_PIN, 1 & 0x01);
			} else {	
				HAL_GPIO_WritePin(BRD_D3_GPIO_PORT, BRD_D3_PIN, 0 & 0x01);
			}

		if ((value & (1 << 4)) != 0)	{
				HAL_GPIO_WritePin(BRD_D4_GPIO_PORT, BRD_D4_PIN, 1 & 0x01);
			} else {	
				HAL_GPIO_WritePin(BRD_D4_GPIO_PORT, BRD_D4_PIN, 0 & 0x01);
			}

		if ((value & (1 << 5)) != 0)	{
				HAL_GPIO_WritePin(BRD_D5_GPIO_PORT, BRD_D5_PIN, 1 & 0x01);
			} else {	
				HAL_GPIO_WritePin(BRD_D5_GPIO_PORT, BRD_D5_PIN, 0 & 0x01);
			}

		if ((value & (1 << 6)) != 0)	{
				HAL_GPIO_WritePin(BRD_D6_GPIO_PORT, BRD_D6_PIN, 1 & 0x01);
			} else {	
				HAL_GPIO_WritePin(BRD_D6_GPIO_PORT, BRD_D6_PIN, 0 & 0x01);
			}

		if ((value & (1 << 7)) != 0)	{
				HAL_GPIO_WritePin(BRD_D7_GPIO_PORT, BRD_D7_PIN, 1 & 0x01);
			} else {	
				HAL_GPIO_WritePin(BRD_D7_GPIO_PORT, BRD_D7_PIN, 0 & 0x01);
			}

		if ((value & (1 << 8)) != 0)	{
				HAL_GPIO_WritePin(BRD_D8_GPIO_PORT, BRD_D8_PIN, 1 & 0x01);
			} else {	
				HAL_GPIO_WritePin(BRD_D8_GPIO_PORT, BRD_D8_PIN, 0 & 0x01);
			}

		if ((value & (1 << 9)) != 0)	{
				HAL_GPIO_WritePin(BRD_D9_GPIO_PORT, BRD_D9_PIN, 1 & 0x01);
			} else {	
				HAL_GPIO_WritePin(BRD_D9_GPIO_PORT, BRD_D9_PIN, 0 & 0x01);
			}	

#else
		if ((value & (1 << 0)) != 0)	{
				HAL_GPIO_WritePin(BRD_D2_GPIO_PORT, BRD_D2_PIN, 1 & 0x01);
			}

		if ((value & (1 << 1)) != 0)	{
				HAL_GPIO_WritePin(BRD_D3_GPIO_PORT, BRD_D3_PIN, 1 & 0x01);
			}

		if ((value & (1 << 2)) != 0)	{
				HAL_GPIO_WritePin(BRD_D4_GPIO_PORT, BRD_D4_PIN, 1 & 0x01);
			}

		if ((value & (1 << 3)) != 0)	{
				HAL_GPIO_WritePin(BRD_D5_GPIO_PORT, BRD_D5_PIN, 1 & 0x01);
			}

		if ((value & (1 << 4)) != 0)	{
				HAL_GPIO_WritePin(BRD_D6_GPIO_PORT, BRD_D6_PIN, 1 & 0x01);
			}

		if ((value & (1 << 5)) != 0)	{
				HAL_GPIO_WritePin(BRD_D7_GPIO_PORT, BRD_D7_PIN, 1 & 0x01);
			}

		if ((value & (1 << 6)) != 0)	{
				HAL_GPIO_WritePin(BRD_D8_GPIO_PORT, BRD_D8_PIN, 1 & 0x01);
			}

		if ((value & (1 << 7)) != 0)	{
				HAL_GPIO_WritePin(BRD_D9_GPIO_PORT, BRD_D9_PIN, 1 & 0x01);
			}

		if ((value & (1 << 8)) != 0)	{
				HAL_GPIO_WritePin(BRD_D10_GPIO_PORT, BRD_D10_PIN, 1 & 0x01);
			}

		if ((value & (1 << 9)) != 0)	{
				HAL_GPIO_WritePin(BRD_D11_GPIO_PORT, BRD_D11_PIN, 1 & 0x01);
			}	
			
#endif

}


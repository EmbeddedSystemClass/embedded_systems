/**   
 ******************************************************************************   
 * @file    mylib/s4295255_laser.c    
 * @author  Mani Batra – 42952556   
 * @date    10042015   
 * @brief   Laser peripheral driver   

 *
 *			NOTE: REPLACE sxxxxxx with your student login.
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_laser_init() – intialise D1 pin as the output pin 

 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 10/4/2015 - Created
 * 2. 10/4/2015 – Added functionality to initialise the laser input  
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_laser.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Initialise the output pin as D1 which will act as input for laser.
  * @param  None
  * @retval None
  */
extern void s4295255_laser_init(void) {

	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Enable the Clocks */

	__GPIOC_CLK_ENABLE();


	GPIO_InitStructure.Pin =  BRD_D1_PIN;				//Pin
  	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
  	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
 		//Initialise Pin
	
}




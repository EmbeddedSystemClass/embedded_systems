/**   
 ******************************************************************************   
 * @file    mylib/s4295255_sysmon.c    
 * @author  Mani Batra – 42952556   
 * @date    23042015   
 * @brief   Driver for System monitor for FreeRTOS  
 *	      
 *
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_sysmon_init() – intialise the system monitor on pin A3 to A5
 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 23/4/2015 - Created
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_sysmon.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Initialise the system monitor on pins A3 to A5.
  * @param  None
  * @retval None
  */
extern void s4295255_sysmon_init(void) {

	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Enable the Clocks */
	__GPIOC_CLK_ENABLE();


  	/* Configure the A3 to A5 pins as outputs */
	GPIO_InitStructure.Pin =  BRD_A3_PIN | BRD_A4_PIN | BRD_A5_PIN;		
  	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	GPIO_InitStructure.Pull = GPIO_PULLUP;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
  	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
 		//Initialise Pin

	
}



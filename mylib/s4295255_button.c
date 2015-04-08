/**   
 ******************************************************************************   
 * @file    mylib/s4295255_button.c    
 * @author  Mani Batra – 42952556   
 * @date    12032015   
 * @brief   Push Button Driver   
 *	       
 *
 *			
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_button_init() – intialise push button interrupt

 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 8/4/2015 - Created
 * 2. 10/3/2015 – Added functionality to set interrupt.
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define NOT_PROJECT   //uncomment when not using in project
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void exti_pb_interrupt_handler(void);
/**
  * @brief  Initialise the Push button interrupt.
  * @param  None
  * @retval None

  */


extern void s4295255_pushbutton_init(void) { 

	GPIO_InitTypeDef GPIO_InitStructure;	
	
	BRD_LEDInit();		//Initialise Blue LED
	BRD_LEDOff();		//Turn off Blue LED

	/* Enable A0 GPIO Clock */
	__BRD_PB_GPIO_CLK();



	/* Configure PB interrupt for Project 1, Task 3 */
	
	
	
	/* Set priority of external GPIO Interrupt [0 (HIGH priority) to 15(LOW priority)] */
	/* 	DO NOT SET INTERRUPT PRIORITY HIGHER THAN 3 */
	HAL_NVIC_SetPriority(BRD_PB_EXTI_IRQ, 10, 0);	//Set Main priority ot 10 and sub-priority ot 0

	//Enable external GPIO interrupt and interrupt vector for pin DO
	NVIC_SetVector(BRD_PB_EXTI_IRQ, (uint32_t)&exti_pb_interrupt_handler);  
	NVIC_EnableIRQ(BRD_PB_EXTI_IRQ);

  	/* Configure D0 pin as pull down input */
	GPIO_InitStructure.Pin = BRD_PB_PIN;				//Pin
  	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;		//interrupt Mode
  	GPIO_InitStructure.Pull = GPIO_PULLUP;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
  	HAL_GPIO_Init(BRD_PB_GPIO_PORT, &GPIO_InitStructure);	//Initialise Pin
	
}


#ifdef NOT_PROJECT
void exti_a2_interrupt_handler(void){

}
#endif



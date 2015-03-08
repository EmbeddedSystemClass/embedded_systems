/**
  ******************************************************************************
  * @file    stage1/main.c 
  * @author  Mani Batra
  * @date    10-January-2015
  * @brief   Prac 1 Template C main file - BCD timer and press counter.
  *			 NOTE: THIS CODE IS PSEUDOCODE AND DOES NOT COMPILE. 
  *				   GUIDELINES ARE GIVEN AS COMMENTS.
  *			 REFERENCES: ex1_led, ex2_gpio, ex3_gpio, ex11_character
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_ledbar.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t counter_value = 0;
uint16_t press_counter_val = 0;

/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);
void Delay(__IO unsigned long nCount);
void exti_a2_interrupt_handler(void);

/**
  * @brief  Main program - timer and press counter.
  * @param  None
  * @retval None
  */
void main(void) {

	BRD_init();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules
	int write_value = 0;
  	
	/* Main processing loop */
  	while (1) {

		counter_value++;	//Increment counter

		/****************** Display counter. ***************/
		/* First, turn off each LED light bar segment
			write 0 to D0
			Write 0 to D1
			....
			Write 0 to D9

			Call sxxxxxx_ledbar_set(0)

			then call
	*/
			//sxxxxxx_ledbar_set(counter_value);
		//*/ 

		
		HAL_GPIO_WritePin(BRD_D0_GPIO_PORT, BRD_D0_PIN, write_value & 0x01);
		HAL_GPIO_WritePin(BRD_D1_GPIO_PORT, BRD_D1_PIN, write_value & 0x01);	//Write Digital 0 bit value
		HAL_GPIO_WritePin(BRD_D2_GPIO_PORT, BRD_D2_PIN, write_value & 0x01);
		HAL_GPIO_WritePin(BRD_D3_GPIO_PORT, BRD_D3_PIN, write_value & 0x01);
		HAL_GPIO_WritePin(BRD_D4_GPIO_PORT, BRD_D4_PIN, write_value & 0x01);
		HAL_GPIO_WritePin(BRD_D5_GPIO_PORT, BRD_D5_PIN, write_value & 0x01);
		HAL_GPIO_WritePin(BRD_D6_GPIO_PORT, BRD_D6_PIN, write_value & 0x01);
		HAL_GPIO_WritePin(BRD_D7_GPIO_PORT, BRD_D7_PIN, write_value & 0x01);
		HAL_GPIO_WritePin(BRD_D8_GPIO_PORT, BRD_D8_PIN, write_value & 0x01);
		HAL_GPIO_WritePin(BRD_D9_GPIO_PORT, BRD_D9_PIN, write_value & 0x01);

		s4295255_ledbar_set(0);

		s4295255_ledbar_set(counter_value);

		/* Toggle 'Keep Alive Indicator' BLUE LED */
		BRD_LEDToggle();

    	Delay(0x7FFF00);	//Will delay for aprox 1s. Increasing the value will increase delay.

	}
}

/**
  * @brief  Initialise Hardware 
  * @param  None
  * @retval None
  */
void Hardware_init(void) {

		

	BRD_LEDInit();		//Initialise Blue LED
	BRD_LEDOff();		//Turn off Blue LED

	/* Initialise LEDBar
       Call
	   sxxxxxx_ledbar_init();

	*/

	s4295255_ledbar_init();

	/* Configure the GPIO_D1 pin
	
	 	.... 

		Configure the GPIO_D9 pin */

	/* Configure A2 interrupt for Prac 1, Task 2 or 3 only */

}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO unsigned long nCount) {
  
	/* Delay a specific amount before returning */
	while(nCount--)	{
  	}
}

/**
  * @brief  exti_a2 GPIO Interrupt handler
  * @param  None.
  * @retval None
  */
void exti_a2_interrupt_handler(void) {


	HAL_GPIO_EXTI_IRQHandler(BRD_A2_PIN);				//Clear A2 pin external interrupt flag

	/* Speed up the counter by reducing the delay value */
}

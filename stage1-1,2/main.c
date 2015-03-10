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
__IO unsigned long delay_val = 0x7FFF00;

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

		
		HAL_GPIO_WritePin(BRD_D0_GPIO_PORT, BRD_D0_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D1_GPIO_PORT, BRD_D1_PIN, 0 & 0x01);	//Write Digital 0 bit value
		HAL_GPIO_WritePin(BRD_D2_GPIO_PORT, BRD_D2_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D3_GPIO_PORT, BRD_D3_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D4_GPIO_PORT, BRD_D4_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D5_GPIO_PORT, BRD_D5_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D6_GPIO_PORT, BRD_D6_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D7_GPIO_PORT, BRD_D7_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D8_GPIO_PORT, BRD_D8_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D9_GPIO_PORT, BRD_D9_PIN, 0 & 0x01);

		s4295255_ledbar_set(0);

		s4295255_ledbar_set(counter_value);

		/* Toggle 'Keep Alive Indicator' BLUE LED */
		BRD_LEDToggle();

    	Delay(delay_val);	//Will delay for aprox 1s. Increasing the value will increase delay.

	}
}

/**
  * @brief  Initialise Hardware 
  * @param  None
  * @retval None
  */
void Hardware_init(void) {

	GPIO_InitTypeDef  GPIO_InitStructure;	

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

	__GPIOC_CLK_ENABLE();
	
	/* Set priority of external GPIO Interrupt [0 (HIGH priority) to 15(LOW priority)] */
	/* 	DO NOT SET INTERRUPT PRIORITY HIGHER THAN 3 */
	HAL_NVIC_SetPriority(BRD_A2_EXTI_IRQ, 10, 0);	//Set Main priority ot 10 and sub-priority ot 0

	//Enable external GPIO interrupt and interrupt vector for pin DO
	NVIC_SetVector(BRD_A2_EXTI_IRQ, (uint32_t)&exti_a2_interrupt_handler);  
	NVIC_EnableIRQ(BRD_A2_EXTI_IRQ);

  	/* Configure D0 pin as pull down input */
	GPIO_InitStructure.Pin = BRD_A2_PIN;				//Pin
  	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;		//interrupt Mode
  	GPIO_InitStructure.Pull = GPIO_PULLUP;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
  	HAL_GPIO_Init(BRD_A1_GPIO_PORT, &GPIO_InitStructure);	//Initialise Pin

}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO unsigned long nCount) {
  
	debug_printf("Counter Value: %08x\n", counter_value);
	//int tick = HAL_GetTick();

	//debug_printf("Tick Value: %d\n", tick);
	/*uint16_t x = counter_value;
	int n;
	for(n=0; n<8; n++)
   {
      if((x & 0x80) !=0)
      {
         debug_putc('1');
      }
      else
      {
         debug_putc('0');
      }


      
      x = x<<1;
   }

	debug_flush();*/
	


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
	
	HAL_Delay(50); //Switch Debouncing 

	HAL_GPIO_EXTI_IRQHandler(BRD_A2_PIN);				//Clear A2 pin external interrupt flag

	/* Speed up the counter by reducing the delay value */

	delay_val = ( delay_val/2 );
}

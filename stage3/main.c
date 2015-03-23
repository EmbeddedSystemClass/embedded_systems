/**
  ******************************************************************************
  * @file    ex6_pwm/main.c 
  * @author  MDS
  * @date    02022015
  * @brief   Enable the PWM output on pin DO.
  *			 See Section 18 (TIM3), P576 of the STM32F4xx Reference Manual.
  ******************************************************************************
  *  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "s4295255_servo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO unsigned long nCount);
void Hardware_init();
//void exti_a2_interrupt_handler(void);



int angle = 0;
int direction = 1;

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void main(void) {


	char RxChar;

	BRD_init();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules

  	while (1) {


	/* Receive characters using getc */
		RxChar = debug_getc();

		if (RxChar != '\0') {

			if(RxChar == 'p') {

				direction = 1;

			} else if(RxChar == 'n') {

				direction = -1;

			}

		}

		BRD_LEDToggle();	//Toggle 'Alive' LED on/off
    	Delay(0x7FFF00);	//Delay function
	}
}

/**
  * @brief  Configure the hardware, 
  * @param  None
  * @retval None
  */
void Hardware_init(void) {



	s4295255_joystick_init();
	s4295255_servo_init();
	s4295255_servo_setangle(angle);
	
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


void exti_a2_interrupt_handler(void) {
	

	
	angle = angle + (direction*10);

	if(angle < -90) {

		angle = -90;

	}

	if(angle > 90) {

		angle = 90;

	}

	s4295255_servo_setangle(angle);


	debug_printf("%d\n", angle);

	Delay(0x4C4B40); //Switch Debouncing 	

	HAL_GPIO_EXTI_IRQHandler(BRD_A2_PIN);				//Clear A2 pin external interrupt flag

	

}



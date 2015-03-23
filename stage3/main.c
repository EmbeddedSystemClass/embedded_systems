/**
  ******************************************************************************
  * @file    stage3/main.c 
  * @author  Mani Batra
  * @date    019032015
  * @brief   Control the pan of the servo motor by generating the PWM on D2
  ******************************************************************************
  *  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "s4295255_servo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define CONSOLE //Uncomment to use the console as direction provider, stage 3, design task 2
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO unsigned long nCount);
void Hardware_init();




int angle = 0; //angle of the servo
int direction = 1;  //direction the servo will move in if controlled from console

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

#ifdef CONSOLE
	/* Receive characters using getc, and set direction */
		RxChar = debug_getc();

		if (RxChar != '\0') {

			if(RxChar == 'p') {

				direction = 1;

			} else if(RxChar == 'n') {

				direction = -1;

			}

		}
#endif
			//recieve joystick values
		uint16_t adc_value = s4295255_joystick_get(0);
		/* Print ADC conversion values */
		//debug_printf("ADC Value: %d\n", angle);

		if(adc_value < 2001) {

			angle = angle - 1;
			if(angle < -90) {

				angle = -90;

			}
			s4295255_servo_setangle(angle);
			
		} else if(adc_value > 2020) {

			angle = angle + 1;
			if(angle > 90) {

				angle = 90;

			}
			
			s4295255_servo_setangle(angle);

		}

		BRD_LEDToggle();	//Toggle 'Alive' LED on/off
    	Delay(0x7FFF00/10);	//Delay function
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

/**
  * @brief  exti interrupt Function for A2.

  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void exti_a2_interrupt_handler(void) {
	

	//change the angle by 10 deg
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



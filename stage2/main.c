/**
  ******************************************************************************
  * @file    stage2-main.c 
  * @author  Mani Batra
  * @date    017032015
  * @brief  Generate the required duty cycle wave accroding to the joystich value. 
  ******************************************************************************
  *  
  */ 


/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_ledbar.h"
#include "s4295255_joystick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef TIM_Init;
int count_interrupt;	//increment each time a timer interrupt occurs
int write_value = 0;
int value = 50; //value to show the period of the wave function
int high = 0;  //denotes to the fraction for which the wave is high

/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);
void tim2_irqhandler (void);
void Delay(__IO unsigned long nCount);


/**
  * @brief  Main program - Timer 2 update compare interrupt.
  * @param  None
  * @retval None
  */
void main(void) {

	BRD_init();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules

  	/* Main processing loop waiting for interrupt */
  	while (1) {
		
		//get the value from the joystick
		uint16_t adc_value = s4295255_joystick_get(0);
		/* Print ADC conversion values */
		//debug_printf("ADC Value: %d\n\r", led_value);

		if(adc_value > 2001 && adc_value < 2015) {

			high = 50;
		} else {

			high = (adc_value/40.95);

			

		}

		//generate the pattern to display on the ledbar
		unsigned short led_value = 0;

		int shift = high/10;

		if(high % 10 == 9) {

			shift++;
		}


		while(shift > 0) {

			led_value = led_value | 1;
			led_value = led_value << 1;
			shift--;
		}

		HAL_GPIO_WritePin(BRD_D2_GPIO_PORT, BRD_D2_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D3_GPIO_PORT, BRD_D3_PIN, 0 & 0x01);	//Write Digital 0 bit value
		HAL_GPIO_WritePin(BRD_D4_GPIO_PORT, BRD_D4_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D5_GPIO_PORT, BRD_D5_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D6_GPIO_PORT, BRD_D6_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D7_GPIO_PORT, BRD_D7_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D8_GPIO_PORT, BRD_D8_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D9_GPIO_PORT, BRD_D9_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D10_GPIO_PORT, BRD_D10_PIN, 0 & 0x01);
		HAL_GPIO_WritePin(BRD_D11_GPIO_PORT, BRD_D11_PIN, 0 & 0x01);

		s4295255_ledbar_set(led_value);

		BRD_LEDToggle();	//Toggle LED on/off
		Delay(0x7FFF00);	//Delay function
			
	}
}

/**
  * @brief  Configure the hardware, 
  * @param  None
  * @retval None
  */
void Hardware_init(void) {

	unsigned short PrescalerValue;

	BRD_LEDInit();		//Initialise onboard LED
	BRD_LEDOff();		//Turn off onboard LED
  
  	/* Timer 2 clock enable */
	__TIM2_CLK_ENABLE();

	s4295255_ledbar_init();

	s4295255_joystick_init();


	/* Compute the prescaler value */
  	PrescalerValue = (uint16_t) ((SystemCoreClock /2)/500000) - 1;		//Set clock prescaler to 50kHz - SystemCoreClock is the system clock frequency.

  	/* Time base configuration */
	TIM_Init.Instance = TIM2;				//Enable Timer 2
  	TIM_Init.Init.Period = 50000/1000 * 2;			//Set period count to be 1ms, so timer interrupt occurs every 1ms.
  	TIM_Init.Init.Prescaler = PrescalerValue;	//Set presale value
  	TIM_Init.Init.ClockDivision = 0;			//Set clock division
	TIM_Init.Init.RepetitionCounter = 0;	// Set Reload Value
  	TIM_Init.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.

	/* Initialise Timer */
	HAL_TIM_Base_Init(&TIM_Init);

	/* Set priority of Timer 2 update Interrupt [0 (HIGH priority) to 15(LOW priority)] */
	/* 	DO NOT SET INTERRUPT PRIORITY HIGHER THAN 3 */
	HAL_NVIC_SetPriority(TIM2_IRQn, 10, 0);		//Set Main priority ot 10 and sub-priority ot 0.

	/* Enable timer update interrupt and interrupt vector for Timer  */
	NVIC_SetVector(TIM2_IRQn, (uint32_t)&tim2_irqhandler);  
	NVIC_EnableIRQ(TIM2_IRQn);

	/* Start Timer */
	HAL_TIM_Base_Start_IT(&TIM_Init);

	

}

/**
  * @brief  Timer 2 Interrupt handler
  * @param  None.
  * @retval None
  */
void tim2_irqhandler (void) {

	//Clear Update Flag
	__HAL_TIM_CLEAR_IT(&TIM_Init, TIM_IT_UPDATE);

		/*BRD_LEDToggle();
		write_value = ~write_value;
		HAL_GPIO_WritePin(BRD_D0_GPIO_PORT, BRD_D0_PIN, write_value & 0x01);*/

	
	//Toggle the state of the wave from high to low depending on the value required
	if (count_interrupt > value) {

		write_value = ~write_value;
		if((write_value & 0x01) == 1) { value = high - 1;}
		if((write_value & 0x01) == 0) { value = (99 - high); }
		HAL_GPIO_WritePin(BRD_D1_GPIO_PORT, BRD_D1_PIN, write_value & 0x01);
		count_interrupt = 0;
	} 


		 
		
	count_interrupt++;		//increment counter, when the interrupt occurs
	

}

void Delay(__IO unsigned long nCount) {
  	while(nCount--) {
  	}
}



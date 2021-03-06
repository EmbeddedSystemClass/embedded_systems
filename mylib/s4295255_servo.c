/**   
 ******************************************************************************   
 * @file    mylib/s4295255_servo.c    
 * @author  Mani Batra – 42952556   
 * @date    12032015   
 * @brief   Servo peripheral driver   
 *	       
 *
 *			
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_servo_init() – intialise servo
 * s4295255_servo_setangle(int angle) – set the servo to an angle
 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 19/3/2015 - Created
 * 2. 19/3/2015 – Added functionality to init function. 
 * 3. 23/3/2015 - Added functionality to the set angle function
 * 4. 8/4/2015 - Added functionality for tilt angle
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

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Initialise the servo
  * @param  None
  * @retval None

  */

ADC_HandleTypeDef AdcHandle;
ADC_ChannelConfTypeDef AdcChanConfig;
GPIO_InitTypeDef GPIO_InitStructure;
TIM_OC_InitTypeDef PWMConfig;
TIM_HandleTypeDef TIM_Init;

extern void s4295255_servo_init(void) { 


	uint16_t PrescalerValue = 0;

	BRD_LEDInit();		//Initialise Blue LED
	BRD_LEDOff();		//Turn off Blue LED

  	/* Timer 2 clock enable */
  	__TIM2_CLK_ENABLE();

  	/* Enable the D2 Clock */
  	__BRD_D2_GPIO_CLK();
	__BRD_D3_GPIO_CLK();

  	/* Configure the D2 pin with TIM2 output*/
	GPIO_InitStructure.Pin = BRD_D2_PIN | BRD_D3_PIN ;				//Pin
  	GPIO_InitStructure.Mode =GPIO_MODE_AF_PP; 		//Set mode to be output alternate
  	GPIO_InitStructure.Pull = GPIO_NOPULL;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;			//Pin latency
	GPIO_InitStructure.Alternate = GPIO_AF1_TIM2;	//Set alternate function to be timer 2
  	HAL_GPIO_Init(BRD_D2_GPIO_PORT, &GPIO_InitStructure);
	HAL_GPIO_Init(BRD_D3_GPIO_PORT, &GPIO_InitStructure);	//Initialise Pin

	/* Compute the prescaler value. SystemCoreClock = 168000000 - set for 500Khz clock */
  	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 500000) - 1;

	/* Configure Timer settings */
	TIM_Init.Instance = TIM2;					//Enable Timer 2
  	TIM_Init.Init.Period = 2*500000/100;			//Set for 20ms (50Hz) period
  	TIM_Init.Init.Prescaler = PrescalerValue;	//Set presale value
  	TIM_Init.Init.ClockDivision = 0;			//Set clock division
	TIM_Init.Init.RepetitionCounter = 0; 		// Set Reload Value
  	TIM_Init.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.
	
	/* PWM Mode configuration for Channel 4 - set pulse width*/
	PWMConfig.OCMode       = TIM_OCMODE_PWM1;	//Set PWM MODE (1 or 2 - NOT CHANNEL)

    PWMConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    PWMConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    PWMConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    PWMConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
    PWMConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	HAL_TIM_PWM_Init(&TIM_Init);	

	

}

/**
  * @brief Set the servo pan to an angle
  *         
  *          
  *
  * @param  angle
  * @retval X, Y or Z value
  */
extern void s4295255_servo_setangle(int angle) {

	//converting the angle range -90 to +90 TO 0 to 180

	int p_angle;

	p_angle = 90 + angle;

	if(p_angle < 15) {

		p_angle = 15;

	}

	if(p_angle > 175) {

		p_angle = 175;

	}

	//get the required pulse width
	PWMConfig.Pulse = ((1/90.0*p_angle) + 0.45)*500000/1000;	
	/* Enable PWM for Timer 2, channel 4 */
	//HAL_TIM_PWM_Init(&TIM_Init);	
	HAL_TIM_PWM_ConfigChannel(&TIM_Init, &PWMConfig, TIM_CHANNEL_4);	


		/* Start PWM */
	HAL_TIM_PWM_Start(&TIM_Init, TIM_CHANNEL_4);

}


/**
  * @brief Set the servo tilt to an angle

  *         
  *          
  *
  * @param  angle

  * @retval X, Y or Z value
  */
extern void s4295255_servo_settiltangle(int angle) {

	//converting the angle range -90 to +90 TO 0 to 180

	int t_angle;

	t_angle = 90 + angle;

	if(t_angle < 15) {

		t_angle = 15;

	}

	if(t_angle > 175) {

		t_angle = 175;

	}

	//get the required pulse width
	PWMConfig.Pulse = ((1/90.0*t_angle) + 0.45)*500000/1000;	
	/* Enable PWM for Timer 2, channel 4 */

	HAL_TIM_PWM_ConfigChannel(&TIM_Init, &PWMConfig, TIM_CHANNEL_3);	


		/* Start PWM */
	HAL_TIM_PWM_Start(&TIM_Init, TIM_CHANNEL_3);

}




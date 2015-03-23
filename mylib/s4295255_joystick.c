/**   
 ******************************************************************************   
 * @file    mylib/s4295255_joystick.c    
 * @author  Mani Batra – 42952556   
 * @date    12032015   
 * @brief   Joystick peripheral driver   
 *	       
 *
 *			
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_joystick_init() – intialise joystick
 * s4295255_joystick_get() – get X, Y or Z value
 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 3/3/2015 - Created
 * 2. 10/3/2015 – Added functionality to set function.
 * 2. 20/3/2015 - Added functionality to work with Stage3.   
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_ledbar.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define STAGE2   //uncomment when using Stage 2 or a program that doesnt take input from Z axis
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void exti_a2_interrupt_handler(void);
/**
  * @brief  Initialise the joystick.
  * @param  None
  * @retval None

  */

ADC_HandleTypeDef AdcHandle;
ADC_ChannelConfTypeDef AdcChanConfig;

extern void s4295255_joystick_init(void) { 
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	BRD_LEDInit();		//Initialise Blue LED
	BRD_LEDOff();		//Turn off Blue LED

	/* Enable A0 GPIO Clock */
	__BRD_A0_GPIO_CLK();


	/* Configure A0 as analog input */
  	GPIO_InitStructure.Pin = BRD_A0_PIN;			//Set A0 pin
  	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;		//Set to Analog input
  	GPIO_InitStructure.Pull = GPIO_NOPULL ;			//No Pull up resister

  	HAL_GPIO_Init(BRD_A0_GPIO_PORT, &GPIO_InitStructure);	//Initialise AO

	/* Enable ADC1 clock */
	__ADC1_CLK_ENABLE();

    /* Configure ADC1 */
    AdcHandle.Instance = (ADC_TypeDef *)(ADC1_BASE);						//Use ADC1
    AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
    AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
    AdcHandle.Init.ScanConvMode          = DISABLE;
    AdcHandle.Init.ContinuousConvMode    = DISABLE;
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;
    AdcHandle.Init.NbrOfDiscConversion   = 0;
    AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
    AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
    AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
    AdcHandle.Init.NbrOfConversion       = 1;
    AdcHandle.Init.DMAContinuousRequests = DISABLE;
    AdcHandle.Init.EOCSelection          = DISABLE;

    HAL_ADC_Init(&AdcHandle);		//Initialise ADC

	AdcChanConfig.Channel = BRD_A0_ADC_CHAN;							//Use AO pin
	AdcChanConfig.Rank         = 1;
    AdcChanConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    AdcChanConfig.Offset       = 0;    

	HAL_ADC_ConfigChannel(&AdcHandle, &AdcChanConfig);		//Initialise ADC channel


	/* Configure A2 interrupt for Prac 1, Task 2 or 3 only */
	
	
	
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
  * @brief  Get X, Y or Z value
  *         
  *          
  *
  * @param  axis
  * @retval X, Y or Z value
  */
extern uint16_t s4295255_joystick_get(int axis) {

	//Get X, Y or Z value
	/* Configure ADC Channel */
	

	HAL_ADC_Start(&AdcHandle); // Start ADC conversion

		//Wait for ADC Conversion to complete
	while (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK);
    uint16_t adc_value = (uint16_t)(HAL_ADC_GetValue(&AdcHandle));
	
	return adc_value;

}

#ifdef STAGE2 
void exti_a2_interrupt_handler(void){

}
#endif



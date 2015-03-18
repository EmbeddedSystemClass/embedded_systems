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
 * 1. 3/3/2015 - Created
 * 2. 10/3/2015 – Added functionality to set function.  
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

extern void s4295255_servo_init(void) { 
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
	
}

/**
  * @brief Set the servo to an angle
  *         
  *          
  *
  * @param  angle
  * @retval X, Y or Z value
  */
extern void servo_setangle(int angle) {

	//Get X, Y or Z value
	/* Configure ADC Channel */
	

	HAL_ADC_Start(&AdcHandle); // Start ADC conversion

		//Wait for ADC Conversion to complete
	while (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK);
    uint16_t adc_value = (uint16_t)(HAL_ADC_GetValue(&AdcHandle));
	
	return adc_value;

}


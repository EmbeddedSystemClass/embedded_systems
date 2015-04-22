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

#ifndef S4295255_SYSMON_H
#define S4295255_SYSMON_H

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

#define S4295255_LA_CHAN0_CLR()	HAL_GPIO_WritePin(GPIOC, BRD_A3_PIN, 0)
#define S4295255_LA_CHAN0_SET()	HAL_GPIO_WritePin(GPIOC, BRD_A3_PIN, 1)
#define S4295255_LA_CHAN1_CLR()	HAL_GPIO_WritePin(GPIOC, BRD_A4_PIN, 0)
#define S4295255_LA_CHAN1_SET()	HAL_GPIO_WritePin(GPIOC, BRD_A4_PIN, 1)
#define S4295255_LA_CHAN2_CLR()	HAL_GPIO_WritePin(GPIOC, BRD_A5_PIN, 0)
#define S4295255_LA_CHAN2_SET()	HAL_GPIO_WritePin(GPIOC, BRD_A5_PIN, 1)
	

extern void s4295255_sysmon_init(void);
#endif


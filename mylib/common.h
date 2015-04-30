/**   
 ******************************************************************************   
 * @file    mylib/s4295255_laser.c    
 * @author  Mani Batra – 42952556   
 * @date    10042015   
 * @brief   common shared variables   

 *
 *			NOTE: REPLACE sxxxxxx with your student login.
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************


 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 10/4/2015 - Created
 * 
 */
 
 

#ifndef S4295255_COMMON_H
#define S4295255_COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "semphr.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern SemaphoreHandle_t LaserOnSemaphore; /* Semaphore for switching laser on  */
extern SemaphoreHandle_t LaserOffSemaphore; /* Semaphore for switching laser off */

/* Private function prototypes -----------------------------------------------*/

#endif


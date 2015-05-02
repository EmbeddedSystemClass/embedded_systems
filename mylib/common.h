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
 * 2. 3/5/2015 - Added semaphore for challenge task
 */
 
 

#ifndef S4295255_COMMON_H
#define S4295255_COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
/* Private typedef -----------------------------------------------------------*/
struct Message {	/* Message consists of sequence number and payload string */
	int Sequence_Number;
	int angle;
};
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
QueueHandle_t PanMessageQueue;	/* Queue used */
QueueHandle_t TiltMessageQueue;	/* Queue used */
extern SemaphoreHandle_t LaserOnSemaphore; /* Semaphore for switching laser on  */
extern SemaphoreHandle_t LaserOffSemaphore; /* Semaphore for switching laser off */
extern SemaphoreHandle_t ChallengeSemaphore;



/* Private function prototypes -----------------------------------------------*/

#endif


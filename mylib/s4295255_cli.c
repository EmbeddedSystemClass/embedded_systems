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
#include "common.h"
#include <string.h>
#include <stdio.h>
#include "s4295255_cli.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/



/**
  * @brief  Laser Command.
  * @param  writebuffer, writebuffer length and command strength
  * @retval None
  */
extern BaseType_t prvLaserCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {

	long lParam_len; 
	const char *cCmd_string;

	/* Get parameters from command string */
	cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);

	if(!strcmp(cCmd_string, "on")) {
	//give the semaphore here


		/* Write command laser output string to write buffer. */
		xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", "Laser Turned On");

		if (LaserOnSemaphore != NULL) {	/* Check if semaphore exists */

			/* Give Laser Semaphore */
			xSemaphoreGive(LaserOnSemaphore);
			//debug_printf("Giving On Semaphore\n\r");
		}

	} else if(!strcmp(cCmd_string, "off")) {


		/* Write command laser output string to write buffer. */
		xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", "Laser Turned Off");

		//give the semaphore here
		if (LaserOffSemaphore != NULL) {	/* Check if semaphore exists */

			/* Give Laser Semaphore */
			xSemaphoreGive(LaserOffSemaphore);
			//debug_printf("Giving Semaphore1 so that task 2 can increment\n\r");
		}

	} else {


		/* Write command laser output string to write buffer. */
		xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", "Invalid laser parameter");


	}
	/* Return pdFALSE, as there are no more strings to return */
	/* Only return pdTRUE, if more strings need to be printed */
	return pdFALSE;
}




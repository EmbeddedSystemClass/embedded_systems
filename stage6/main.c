/**
  ******************************************************************************
  * @file    ex18_fr_cli/main.c 
  * @author  MDS
  * @date    9-May-2014
  * @brief   FreeRTOS CLI program.Creates a task to implement the CLI and flash
  *			 the onboard Blue LED.
  *
  *			 Implements the echo command  
  *			 See the FreeRTOSPlus CLI API for more information
  *			 http://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_CLI
  ******************************************************************************
  *  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include <string.h>
#include <stdio.h>
#include "s4295255_laser.h"
#include "s4295255_cli.h"
#include "common.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "FreeRTOS_CLI.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Hardware_init();
void ApplicationIdleHook( void ); /* The idle hook is used to blink the Blue 'Alive LED' every second */
void Laser_Task( void );
void CLI_Task(void);

/* Private variables ---------------------------------------------------------*/

SemaphoreHandle_t LaserOnSemaphore;
SemaphoreHandle_t LaserOffSemaphore;

CLI_Command_Definition_t xLaser = {	/* Structure that defines the "laser" command line command. */
	"laser",
	"laser <on/off> : Turn the laser on/off.\r\n",
	prvLaserCommand,
	1
};

/* Task Priorities ------------------------------------------------------------*/
#define mainLASER_PRIORITY					( tskIDLE_PRIORITY + 1 )
#define mainCLI_PRIORITY					( tskIDLE_PRIORITY + 2 )

/* Task Stack Allocations -----------------------------------------------------*/
#define mainLASER_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define mainCLI_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 3 )

/**
  * @brief  Starts all the other tasks, then starts the scheduler.
  * @param  None
  * @retval None
  */
int main( void ) {

	BRD_init();
	Hardware_init();
	
	/* Start the tasks to flash the LED and start the CLI. */
    xTaskCreate( (void *) &Laser_Task, (const signed char *) "LASER", mainLASER_TASK_STACK_SIZE, NULL, mainLASER_PRIORITY, NULL );
	xTaskCreate( (void *) &CLI_Task, (const signed char *) "CLI", mainCLI_TASK_STACK_SIZE, NULL, mainCLI_PRIORITY, NULL );

	//Create Semaphore
	LaserOnSemaphore = xSemaphoreCreateBinary();
	LaserOffSemaphore = xSemaphoreCreateBinary();
	
	
	/* Register CLI commands */
	FreeRTOS_CLIRegisterCommand(&xLaser);

	/* Start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */

	vTaskStartScheduler();
	
	/* We should never get here as control is now taken by the scheduler. */
  	return 0;
}

/**
  * @brief  LED Flashing Task.
  * @param  None
  * @retval None
  */
void Laser_Task( void ) {

	BRD_LEDOff();


	for (;;) {

		/* Toggle LED */
		BRD_LEDToggle();

		if (LaserOnSemaphore != NULL) {	/* Check if semaphore exists */
	
			/* See if we can obtain the Laser semaphore. If the semaphore is not available
          	 wait 10 ticks to see if it becomes free. */
			if ( xSemaphoreTake( LaserOnSemaphore, 10 ) == pdTRUE ) {

				//turn on the laser
				//debug_printf("got the semaphore");
				s4295255_laser_on();

			}

		}


		if (LaserOffSemaphore != NULL) {	/* Check if semaphore exists */
	
			/* See if we can obtain the Laser semaphore. If the semaphore is not available
          	 wait 10 ticks to see if it becomes free. */
			if ( xSemaphoreTake( LaserOffSemaphore, 10 ) == pdTRUE ) {

				//turn off the laser
				s4295255_laser_off();

			}

		}
		/* Delay the task for 50ms */
		vTaskDelay(50);

	}
}


/**
  * @brief  CLI Receive Task.
  * @param  None
  * @retval None
  */
void CLI_Task(void) {

	char cRxedChar;
	char cInputString[100];
	int InputIndex = 0;
	char *pcOutputString;
	BaseType_t xReturned;

	/* Initialise pointer to CLI output buffer. */
	memset(cInputString, 0, sizeof(cInputString));
	pcOutputString = FreeRTOS_CLIGetOutputBuffer();

	for (;;) {

		/* Receive character */
		cRxedChar = debug_getc();

		/* Process if chacater if not Null */
		if (cRxedChar != '\0') {

			/* Put byte into USB buffer */
			debug_putc(cRxedChar);
		
			/* Process only if return is received. */
			if (cRxedChar == '\r') {

				//Put new line and transmit buffer
				debug_putc('\n');
				debug_flush();

				/* Put null character in command input string. */
				cInputString[InputIndex] = '\0';
				
				xReturned = pdTRUE;
				/* Process command input string. */
				while (xReturned != pdFALSE) {

					/* Returns pdFALSE, when all strings have been returned */
					xReturned = FreeRTOS_CLIProcessCommand( cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE );

					/* Display CLI output string */
					debug_printf("%s\n\r",pcOutputString);
				    vTaskDelay(5);	//Must delay between debug_printfs.
				}

				memset(cInputString, 0, sizeof(cInputString));
				InputIndex = 0;

			} else {

				debug_flush();		//Transmit USB buffer

				if( cRxedChar == '\r' ) {

					/* Ignore the character. */
				} else if( cRxedChar == '\b' ) {

					/* Backspace was pressed.  Erase the last character in the
					 string - if any.*/
					if( InputIndex > 0 ) {
						InputIndex--;
						cInputString[ InputIndex ] = '\0';
					}

				} else {

					/* A character was entered.  Add it to the string
					   entered so far.  When a \n is entered the complete
					   string will be passed to the command interpreter. */
					if( InputIndex < 20 ) {
						cInputString[ InputIndex ] = cRxedChar;
						InputIndex++;
					}
				}
			}		
		}

		vTaskDelay(50);
	}
}



/**
  * @brief  Hardware Initialisation.
  * @param  None
  * @retval None
  */
void Hardware_init( void ) {
	
	portDISABLE_INTERRUPTS();	//Disable interrupts

	BRD_LEDInit();				//Initialise Blue LED
	BRD_LEDOff();				//Turn off Blue LED

	s4295255_laser_init(); //initialise the laser

	portENABLE_INTERRUPTS();	//Enable interrupts
}

/**
  * @brief  Application Tick Task.
  * @param  None
  * @retval None
  */
void vApplicationTickHook( void ) {

	BRD_LEDOff();
}

/**
  * @brief  Idle Application Task
  * @param  None
  * @retval None
  */
void vApplicationIdleHook( void ) {
	static portTickType xLastTx = 0;

	BRD_LEDOff();

	for (;;) {

		/* The idle hook simply prints the idle tick count, every second */
		if ((xTaskGetTickCount() - xLastTx ) > (1000 / portTICK_RATE_MS)) {

			xLastTx = xTaskGetTickCount();

			//debug_printf("IDLE Tick %d\n", xLastTx);

			/* Blink Alive LED */
			BRD_LEDToggle();		
		}
	}
}

/**
  * @brief  vApplicationStackOverflowHook
  * @param  Task Handler and Task Name
  * @retval None
  */
void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName ) {
	/* This function will get called if a task overflows its stack.   If the
	parameters are corrupt then inspect pxCurrentTCB to find which was the
	offending task. */

	BRD_LEDOff();
	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}


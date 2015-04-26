/**
  ******************************************************************************
  * @file    stage5/main.c 
  * @author  Mani Batra – 42952556
  * @date    23042015
  * @brief   Implement a signal monitor for FreeRTOS
  *			 Implement a 3 digit timer using tasks and semaphore
			 Use interrupt handling to control the timer 
  ******************************************************************************
  *  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_sysmon.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Hardware_init();
void ApplicationIdleHook( void ); /* The idle hook is used to blink the Blue 'Alive LED' every second */
void Task1_Task( void );
void Task2_Task( void );
void Task3_Task( void );

/* Task Priorities ------------------------------------------------------------*/
#define mainTASK1_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define mainTASK2_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define mainTASK3_PRIORITY					( tskIDLE_PRIORITY + 1 )

/* Task Stack Allocations -----------------------------------------------------*/
#define mainTASK1_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define mainTASK2_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define mainTASK3_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


/**
  * @brief  Starts all the other tasks, then starts the scheduler.
  * @param  None
  * @retval None
  */
int main( void ) {

	BRD_init();
	Hardware_init();
	
	/* Start the three tasks. */
    xTaskCreate( (void *) &Task1_Task, (const signed char *) "TASK1", mainTASK1_TASK_STACK_SIZE, NULL, mainTASK1_PRIORITY, NULL );
    xTaskCreate( (void *) &Task2_Task, (const signed char *) "TASK2", mainTASK2_TASK_STACK_SIZE, NULL, mainTASK2_PRIORITY, NULL );
    xTaskCreate( (void *) &Task3_Task, (const signed char *) "TASK3", mainTASK3_TASK_STACK_SIZE, NULL, mainTASK3_PRIORITY, NULL );

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
  * @brief  Task 1.
  * @param  None
  * @retval None
  */
void Task1_Task( void ) { 

	S4295255_LA_CHAN0_CLR();  			//Clear LA Channel 0


	for (;;) { 

		S4295255_LA_CHAN0_SET();       	//Set LA Channel 0
 		BRD_LEDToggle();				//Random instruction
		vTaskDelay(3);					//Extra Delay for 3ms
		S4295255_LA_CHAN0_CLR();     	//Clear LA Channel 0 
     	vTaskDelay(1);					//Mandatory delay 



	} 
}


/**
  * @brief  Task 2.
  * @param  None
  * @retval None
  */
void Task2_Task( void ) { 

	S4295255_LA_CHAN1_CLR();  			//Clear LA Channel 1

	for (;;) { 

		S4295255_LA_CHAN1_SET();       	//Set LA Channel 1
 		BRD_LEDToggle();				//Random instruction
		vTaskDelay(3);					//Extra Delay for 3ms
		S4295255_LA_CHAN1_CLR();     	//Clear LA Channel 1 
     	vTaskDelay(1);					//Mandatory delay 

	} 
}


/**
  * @brief  Task 3.
  * @param  None
  * @retval None
  */
void Task3_Task( void ) { 

	S4295255_LA_CHAN2_CLR();  			//Clear LA Channel 2

	for (;;) { 

		S4295255_LA_CHAN2_SET();       	//Set LA Channel 2
 		BRD_LEDToggle();				//Random instruction
		//vTaskDelay(3);					//Extra Delay for 3ms
		S4295255_LA_CHAN2_CLR();     	//Clear LA Channel 2 
		taskYIELD();
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

	portENABLE_INTERRUPTS();	//Enable interrupts
	s4295255_sysmon_init();  	//Initialise the system monitor
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


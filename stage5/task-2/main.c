/**
  ******************************************************************************
  * @file    stage5/task2/main.c 
  * @author  Mani Batra – 42952556
  * @date    024032015
  * @brief   A ripple based counter using two semaphores
  *
  *			 Press the NP2 pushbutton to start and stop the counter.
  *	
  *			 NOTE: THE IDLE TASK MUST BE DISABLED. 
  ******************************************************************************
  *  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_ledbar.h"
#include "s4295255_button.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SemaphoreHandle_t Semaphore1;	/* Semaphore for first overflow */
SemaphoreHandle_t Semaphore2;	/* Semaphore for second overflow */
SemaphoreHandle_t PBSemaphore; /* Semaphore for pushbutton interrupt */

uint8_t counter1 = 0;
uint8_t counter2 = 0;
uint8_t counter3 = 0;

/* Private function prototypes -----------------------------------------------*/
static void Hardware_init();
void ApplicationIdleHook( void ); /* The idle hook is just used to stream data to the USB port */
void Task1( void );
void Task2( void );
void Task3( void );
uint16_t display(void);

/* Task Priorities ------------------------------------------------------------*/
#define mainTASK1_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define mainTASK2_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define mainTASK3_PRIORITY					( tskIDLE_PRIORITY + 2 )

/* Task Stack Allocations -----------------------------------------------------*/
#define mainTASK1_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define mainTASK2_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define mainTASK3_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


/**
  * @brief  Starts all the other tasks, then starts the scheduler.
  * @param  None
  * @retval None
  */
int main( void ) {

	BRD_init();
	Hardware_init();
	
	/* Start task one, two and three. */
    xTaskCreate( (void *) &Task1, (const signed char *) "TASK1", mainTASK1_STACK_SIZE, NULL, mainTASK1_PRIORITY, NULL );
	xTaskCreate( (void *) &Task2, (const signed char *) "TASK2", mainTASK2_STACK_SIZE, NULL, mainTASK2_PRIORITY, NULL );
	xTaskCreate( (void *) &Task3, (const signed char *) "TASK3", mainTASK3_STACK_SIZE, NULL, mainTASK3_PRIORITY, NULL );

	/* Create Semaphores */
	Semaphore1 = xSemaphoreCreateBinary();
	Semaphore2 = xSemaphoreCreateBinary();
	PBSemaphore = xSemaphoreCreateBinary();

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
  * @brief  Give Task.Gives LED Semaphore every second.
  * @param  None
  * @retval None
  */
void Task1( void ) {
	
	uint8_t on = 0; // to toggle the timer on and off
	
	for (;;) {


		if( PBSemaphore != NULL) {
	
			if( xSemaphoreTake( PBSemaphore, 10 ) == pdTRUE) {

				on = ~on & 0x01;
	
			}
		}

		if(on) {
			s4295255_ledbar_set(0);
			s4295255_ledbar_set(display());
			counter1++;
			counter1 = (counter1 % 10);

			//display on the LED BAR 
		
		
			//debug_printf("displayed on LED : %x\n\r", display() << 2);

			if(counter1 == 0) {

				if (Semaphore1 != NULL) {	/* Check if semaphore exists */

					/* Give LED Semaphore */
					xSemaphoreGive(Semaphore1);
					//debug_printf("Giving Semaphore1 so that task 2 can increment\n\r");
				}

			}
		} else {

			counter1 = 0;
			counter2 = 0;
			counter3 = 0;
			s4295255_ledbar_set(0);

		}
		
		/* Wait for 100ms = 0.1second */
		vTaskDelay(100);

	}
}


/**
  * @brief  Take Task. Used to take semaphore events. Toggles LED.
  * @param  None
  * @retval None
  */
void Task2( void ) {

	
	BRD_LEDToggle();
	//s4295255_ledbar_set(0);	

	for (;;) {

		if (Semaphore1 != NULL) {	/* Check if semaphore exists */
	
			/* See if we can obtain the LED semaphore. If the semaphore is not available
          	 wait 10 ticks to see if it becomes free. */
			if ( xSemaphoreTake( Semaphore1, 10 ) == pdTRUE ) {

				counter1 = 0;
				counter2++;
				counter2 = (counter2 % 10);

				if(counter2 == 0){

					if (Semaphore2 != NULL) {	/* Check if semaphore exists */

						/* Give LED Semaphore */
						xSemaphoreGive(Semaphore2);
						//debug_printf("Giving Semaphore1 so that task 2 can increment\n\r");
					}
				}
        	}
		}

		taskYIELD();	


	}
}

/**
  * @brief  Take Task. Used to take semaphore events. Toggles LED.
  * @param  None
  * @retval None

  */
void Task3( void ) {

	
	BRD_LEDToggle();
	s4295255_ledbar_set(0);	

	for (;;) {

		if (Semaphore2 != NULL) {	/* Check if semaphore exists */
	
			/* See if we can obtain the LED semaphore. If the semaphore is not available

          	 wait 10 ticks to see if it becomes free. */
			if ( xSemaphoreTake( Semaphore2, 10 ) == pdTRUE ) {
				
				counter2 = 0;
				counter3++;
				counter3 = (counter3 % 4);
				if(counter3 == 0) {
					counter1 = 0;
					counter2 = 0;
				}

        	}
		}

		taskYIELD();	


	}
}


uint16_t display() {

		uint8_t D9 = !!(counter1 & 0x01);
		uint8_t D8 = !!(counter1 & 0x02);
		uint8_t D7 = !!(counter1 & 0x04);
		uint8_t D6 = !!(counter1 & 0x08);
		uint8_t D5 = !!(counter2 & 0x01);
		uint8_t D4 = !!(counter2 & 0x02);
		uint8_t D3 = !!(counter2 & 0x04);
		uint8_t D2 = !!(counter2 & 0x08);
		uint8_t D1 = !!(counter3 & 0x01);
		uint8_t D0 = !!(counter3 & 0x02);
		

		uint16_t display = (D9 << 9) | (D8 << 8) | (D7 << 7) | (D6 << 6) | (D5 << 5) | (D4 << 4) | (D3 << 3) | (D2 << 2) | (D1 << 1) | D0 ;
		
		//debug_printf("display : %x , counter 1 : %d, counter 2 : %d, counter 3 : %d\n ", display, counter1, counter2, counter3);
		return display;

}

/**
  * @brief  Hardware Initialisation.
  * @param  None
  * @retval None
  */
static void Hardware_init( void ) {

	GPIO_InitTypeDef GPIO_InitStructure;

	s4295255_ledbar_init();
	
	portDISABLE_INTERRUPTS();	//Disable interrupts

	BRD_LEDInit();				//Initialise Blue LED
	BRD_LEDOff();				//Turn off Blue LED

	s4295255_pushbutton_init();


	portENABLE_INTERRUPTS();	//Enable interrupts

}

/**
  * @brief  Pushbutton Interrupt handler. Gives PB Semaphore
  * @param  None.
  * @retval None
  */
void exti_pb_interrupt_handler(void) {
	
	BaseType_t xHigherPriorityTaskWoken;
	 
    /* Is it time for another Task() to run? */
    xHigherPriorityTaskWoken = pdFALSE;

	/* Check if Pushbutton external interrupt has occured */
  	HAL_GPIO_EXTI_IRQHandler(BRD_PB_PIN);				//Clear D0 pin external interrupt flag
    	
	if (PBSemaphore != NULL) {	/* Check if semaphore exists */
		xSemaphoreGiveFromISR( PBSemaphore, &xHigherPriorityTaskWoken );		/* Give PB Semaphore from ISR*/
		debug_printf("Triggered \n\r");    //Print press count value
	}
    
	/* Perform context switching, if required. */
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
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
  * @brief  Idle Application Task (Disabled)
  * @param  None
  * @retval None
  */
void vApplicationIdleHook( void ) {
	static portTickType xLastTx = 0;

	BRD_LEDOff();

	for (;;) {
		/* The idle hook simply prints the idle tick count */
		if ((xTaskGetTickCount() - xLastTx ) > (1000 / portTICK_RATE_MS)) {
			xLastTx = xTaskGetTickCount();
			//debug_printf("IDLE Tick %d\n", xLastTx);
			//BRD_LEDToggle();		
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


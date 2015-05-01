/**   
 ******************************************************************************   
 * @file    mylib/s4295255_servo.h  
 * @author  Mani Batra – 42952556   
 * @date    12032015   
 * @brief  Servo peripheral driver   
 *
 *			
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_servo_init() – intialise servo
 * s4295255_servo_setangle(intangle) – set the servo to an angle
 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 19/3/2015 - Created
 * 2. 19/3/2015 – Added functionality to init function. 
 * 3. 23/3/2015 - Added functionality to the set angle function
 */

#ifndef S4295255_CLI_H
#define S4295255_CLI_H

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern CLI_Command_Definition_t xLaser;
extern CLI_Command_Definition_t xPan;
extern CLI_Command_Definition_t xTilt;
extern CLI_Command_Definition_t xLeft;
extern CLI_Command_Definition_t xRight;
extern CLI_Command_Definition_t xUp;
extern CLI_Command_Definition_t xDown;

/* Private function prototypes -----------------------------------------------*/

extern BaseType_t prvLaserCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvPanCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvTiltCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvLeftCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvRightCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvUpCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvDownCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
#endif


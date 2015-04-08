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

#ifndef S4295255_SERVO_H
#define S4295255_SERVO_H

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

extern void s4295255_servo_init(void);
extern void s4295255_servo_setangle(int angle);
extern void s4295255_servo_settiltangle(int angle);
#endif


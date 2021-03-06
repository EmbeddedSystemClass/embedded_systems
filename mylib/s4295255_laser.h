/**   
 ******************************************************************************   
 * @file    mylib/s4295255_laser.c    
 * @author  Mani Batra – 42952556   
 * @date    10042015   
 * @brief   Laser peripheral driver   

 *
 *			NOTE: REPLACE sxxxxxx with your student login.
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_laser_init() – intialise D1 pin as the output pin 

 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 10/4/2015 - Created
 * 2. 10/4/2015 – Added functionality to initialise the laser input  
*  3. 30/4/2015 - Added functionality to turn the laser on/off
 */
 
 

#ifndef S4295255_LASER_H
#define S4295255_LASER_H

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

extern void s4295255_laser_init(void);
extern void s4295255_laser_on(void);
extern void s4295255_laser_off(void);
#endif


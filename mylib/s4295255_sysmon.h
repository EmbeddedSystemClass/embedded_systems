/**   
 ******************************************************************************   
 * @file    mylib/s4295255_ledbar.h  
 * @author  Mani Batra – 42952556   
 * @date    06032015   
 * @brief   LED Light Bar peripheral driver   
 *	     REFERENCE: LEDLightBar_datasheet.pdf   
 *
 *			NOTE: REPLACE sxxxxxx with your student login.
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_ledbar_init() – intialise LED Light BAR
 * s4295255_ledbar_set() – set LED Light BAR value
 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 3/3/2015 - Created
 * 2. 10/3/2015 – Added functionality to set function.  
 */

#ifndef S4295255_LEDBAR_H
#define S4295255_LEDBAR_H

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

extern void s4295255_ledbar_init(void);
extern void s4295255_ledbar_set(unsigned short value);
#endif


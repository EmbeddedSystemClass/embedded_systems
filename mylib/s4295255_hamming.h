/**   
 ******************************************************************************   
 * @file    mylib/s4295255_hamming.h  
 * @author  Mani Batra – 42952556   
 * @date    25032015   
 * @brief  Hamming Encoding   
 *
 *			
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_hamming_encode(unsigned char hbyte) – hamming byte encode
 * s4295255_hamming_decode(unsigned short hword) – hamming byte decode
 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 25/3/2015 - Created
 * 2. 19/3/2015 – 
 * 3. 23/3/2015 - 
 */

#ifndef S4295255_HAMMING_H
#define S4295255_HAMMING_H

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint16_t err_mask;
/* Private function prototypes -----------------------------------------------*/

extern uint16_t s4295255_hamming_encode(unsigned char hbyte);
extern unsigned char s4295255_hamming_decode(unsigned short hword);
#endif


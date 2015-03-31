/**   
 ******************************************************************************   
 * @file    mylib/s4295255_hamming.c 
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


/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_ledbar.h"
#include "s4295255_radio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Initialise the servo
  * @param  None
  * @retval None

  */

extern void s4295255_hamming_encode(unsigned char hbyte){

	uint8_t d0, d1, d2, d3;
	uint8_t p0 = 0, h0, h1, h2;
	uint8_t z;
	uint8_t out;
	
	/* extract bits */
	d0 = !!(hbyte & 0x1);
	d1 = !!(hbyte & 0x2);
	d2 = !!(hbyte & 0x4);
	d3 = !!(hbyte & 0x8);
	
	/* calculate hamming parity bits */
	h0 = d1 ^ d2 ^ d3;
	h1 = d0 ^ d2 ^ d3;
	h2 = d0 ^ d1 ^ d3;
	
	/* generate out byte without parity bit P0 */
	out = (h0 << 4) | (h1 << 5) | (h2 << 6) |
		(d0 << 0) | (d1 << 1) | (d2 << 2) | (d3 << 3);

	/* calculate even parity bit */
	for (z = 0; z<7; z++)		
		p0 = p0 ^ !!(out & (1 << z));
	
	out |= (p0 << 7);	//Put P0 into most significatn bit.

	s4295255_radio_sendpacket(out);//send the out packet

}
extern void s4295255_hamming_decode(unsigned short hword);

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

int error_count = 0;
int error_corrected = 0;
/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_radio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
int check_parity(unsigned char hbyte);
unsigned char hamming_byte_decode(unsigned char hbyte);

/**
  * @brief  Initialise the servo
  * @param  None
  * @retval None

  */

extern uint16_t s4295255_hamming_encode(unsigned char hbyte){

	uint8_t d0, d1, d2, d3;
	uint8_t p0 = 0, h0, h1, h2;
	uint8_t z;
	uint8_t out;
	uint16_t packet_to_send = 0; //2 bytes

	int i;

	uint8_t in = hbyte & 0xF;
	
	for(i = 0; i < 2; i++){

		if(i == 1) { 
			in = hbyte >> 4; //encode 4 MSBs
		}
	
		/* extract bits */
		d0 = !!(in & 0x1);
		d1 = !!(in & 0x2);
		d2 = !!(in & 0x4);
		d3 = !!(in & 0x8);
	
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

		if(i == 0) {

			packet_to_send = out;

		} else {

			packet_to_send |= (out << 8);

		}

		

	}

	return packet_to_send;

}
extern unsigned char s4295255_hamming_decode(unsigned short hword){


	

	unsigned char lsb = hamming_byte_decode(hword & 0xFF);
	unsigned char msb = hamming_byte_decode(hword >> 8);

		
	if(lsb == -1 || msb == -1) 
		return -1;

	else 

		return (lsb | (msb << 4));

	


}

unsigned char hamming_byte_decode(unsigned char hbyte) {

	uint8_t d0, d1, d2, d3;
	uint8_t h0, h1, h2;
	uint8_t p0;
	uint8_t s;

	d0 = !!(hbyte & 0x1);
	d1 = !!(hbyte & 0x2);
	d2 = !!(hbyte & 0x4);
	d3 = !!(hbyte & 0x8);
	h0 = !!(hbyte & 0x10);
	h1 = !!(hbyte & 0x20);
	h2 = !!(hbyte & 0x40);
	p0 = !!(hbyte & 0x80);


	uint8_t s0 = d1 ^ d2 ^ d3 ^ h0;
	uint8_t s1 = d0 ^ d2 ^ d3 ^ h1;
	uint8_t s2 = d0 ^ d1 ^ d3 ^ h2;

	s = s0 | (s1 << 1) | (s2 << 2);


	switch(s){
		case 0: 
			return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );
		
		case 1: 
			hbyte ^= 1 << 6;
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;

	    case 2:
			hbyte ^= 1 << 5;
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;

		case 3:
			 d0 = !d0;
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;


		case 4:
			hbyte ^= 1 << 4;
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;


		case 5:
			d1 = !d1;
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;


		case 6:
			d2 = !d2;
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;

		case 7:
			d3 = !d3;
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;

		default:
			break;

	}


	return -1;


}

int check_parity(unsigned char hbyte) {

	int p =0;
	uint8_t p0 = !!(hbyte & 0x80);
	int z;

	for (z = 0; z<7; z++)		
		p = p ^ !!(hbyte & (1 << z));


	if(p == p0)
		return 1;
	else 
		return 0;
	


}

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
#include "s4295255_radio.h"
#include "s4295255_hamming.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int error_count = 0;
int error_corrected = 0;
int lower = 1; //used to indicate

/* Private function prototypes -----------------------------------------------*/
int check_parity(unsigned char hbyte); // uses parity bit to check for errors
unsigned char hamming_byte_decode(unsigned char hbyte); //used to decode one byte of the hamming code out of the short received  

/**
  * @brief  Hamming encode the 8 bits received 
  * @param  The char byte to encode
  * @retval An encoded 16 bit word

  */

extern uint16_t s4295255_hamming_encode(unsigned char hbyte){

	uint8_t d0, d1, d2, d3;
	uint8_t p0 = 0, h0, h1, h2;
	uint8_t z;
	uint8_t out;
	uint16_t packet_to_send = 0; //2 bytes

	int i;

	uint8_t in = hbyte & 0xF; //encode 4 LSB bits
	
	for(i = 0; i < 2; i++){
		p0 = 0;
		if(i == 1) { 
			in = hbyte >> 4; //encode 4 MSB bits
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


/**
  * @brief  Hamming decode  the 16 bits received 
  * @param  The short to decode
  * @retval An decoded 8 bit word

  */

extern unsigned char s4295255_hamming_decode(unsigned short hword){


	
	//dividing the hword received into lsb and msb to decode one at a time
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

	//extracting the data bits and the hamming, parity bits
	d0 = !!(hbyte & 0x1);
	d1 = !!(hbyte & 0x2);
	d2 = !!(hbyte & 0x4);
	d3 = !!(hbyte & 0x8);
	h0 = !!(hbyte & 0x10);
	h1 = !!(hbyte & 0x20);
	h2 = !!(hbyte & 0x40);
	p0 = !!(hbyte & 0x80);

	//calculating the syndrome
	uint8_t s0 = d1 ^ d2 ^ d3 ^ h0;
	uint8_t s1 = d0 ^ d2 ^ d3 ^ h1;
	uint8_t s2 = d0 ^ d1 ^ d3 ^ h2;

	s = s0 | (s1 << 1) | (s2 << 2);

	//check the syndrome - if it is 
	switch(s){
		case 0: 
			lower = !(lower);
			return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );
		
		case 1: 
			hbyte ^= 1 << 6;
			if(lower){ //sets the error mask accordingly if the byte is LSB or MSB
				err_mask ^= 1 << 6;

			} else {
				err_mask ^= 1 << 14;

			}
			lower = !(lower);
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;

	    case 2:
			hbyte ^= 1 << 5;
			if(lower){
				err_mask ^= 1 << 5;

			} else {
				err_mask ^= 1 << 13;

			}
			lower = !(lower);
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;

		case 3:
			 d0 = !d0;
			 hbyte ^= 1 << 0;
			if(lower){
				err_mask ^= 1 << 0;

			} else {
				err_mask ^= 1 << 8;

			}
			lower = !(lower);
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;


		case 4:
			hbyte ^= 1 << 4;
			if(lower){
				err_mask ^= 1 << 4;

			} else {
				err_mask ^= 1 << 12;

			}

			lower = !(lower);
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;


		case 5:
			d1 = !d1;
			hbyte ^= 1 << 1;
			if(lower){
				err_mask ^= 1 << 1;

			} else {
				err_mask ^= 1 << 9;

			}
			lower = !(lower);
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;


		case 6:
			d2 = !d2;
			hbyte ^= 1 << 2;
			if(lower){
				err_mask ^= 1 << 2;

			} else {
				err_mask ^= 1 << 10;

			}
			lower = !(lower);
			error_count ++;
			if(check_parity(hbyte)){
				error_corrected++;	
				return ( d0 | (d1 << 1) | (d2 <<2) | (d3 << 3) );

			} else 
				return -1;

		case 7:
			d3 = !d3;
			hbyte ^= 1 << 3;
			if(lower){
				err_mask ^= 1 << 3;

			} else {
				err_mask ^= 1 << 11;

			}
			lower = !(lower);
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

/**
  * @brief  Check for the parity bit, used to detect two bit errors
  * @param  The hamming byte whose parity is checked
  * @retval 1 if the parity is right else 0

  */
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

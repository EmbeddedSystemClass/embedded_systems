/**   
 ******************************************************************************   
 * @file    mylib/s4295255_radio.c 
 * @author  Mani Batra – 42952556   
 * @date    25032015   
 * @brief  Radio peripheral driver   
 *
 *			
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4295255_radio_init() – initialise radio
 * s4295255_radio_setchan(unsigned char chan) – Set the channel of the radio
 * s4295255_radio_settxaddress(unsigned char *addr) – Set the transmit address of the radio
 * s4295255_radio_sendpacket(unsigned char *txpacket) - Send a packet
 * s4295255_radio_getpacket(unsigned char *txpacket) - Recieve a packet
 ******************************************************************************   
 *     REVISION HISTORY
 ******************************************************************************
 * 1. 25/3/2015 - Created
 * 2. 15/4/2015 –  Setting the channel via the SPI commands
 *  
 */

#ifndef S4295255_RADIO_H
#define S4295255_RADIO_H

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

extern void s4295255_radio_init();
extern void s4295255_radio_setchan(unsigned char chan); 
extern void s4295255_radio_settxaddress(unsigned char *addr); 
extern void s4295255_radio_sendpacket(unsigned char *txpacket);
extern int s4295255_radio_getpacket(unsigned char *txpacket);
#endif


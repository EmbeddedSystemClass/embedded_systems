/**
  ******************************************************************************
  * @file    ex10_spi/main.c 
  * @author  MDS
  * @date    03022015
  * @brief   SPI Read 32-bit Register nrf24l01plus status (0x07) register
  *			 NOTE: This example does not send or transmit with the nrf24l01plus.
  *			 REFERENCE: nrf24l01p_datasheet.pdf
  ******************************************************************************
  *  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "s4295255_radio.h"
#include "s4295255_hamming.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CHANNEL	40
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t destination_addr[] = {0x12, 0x34, 0x56, 0x78};
uint8_t source_addr[] = {0x42, 0x95, 0x25, 0x56};
char packet_type = 0x20;
char payload[7];
uint8_t packetbuffer[32];	/* Packet buffer initialised to 32 bytes (max length) */
unsigned char rec_packet_type;
unsigned char rec_destination_address[4];
unsigned char rec_source_address[4];
unsigned char rec_payload[4];


/* Private function prototypes -----------------------------------------------*/
void Delay(__IO unsigned long nCount);
void HardwareInit();
void print_packet();

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {

	uint8_t status;
	
	BRD_init();
	HardwareInit();

	/* Main Processing Loop */
    while (1) {

		int i = 0;

		for(i = 0; i < 7; i++){

			payload[i] = debug_getc();
			if(payload[i] = '\r') {
				debug_printf("You pressed \"Enter\"\n");
				break;
			}
		}

		debug_printf("Sending Packet\n");

		//hamming encoding and then send packet
	
		s4295255_hamming_encode(packet_type); //encode packet type

		for(i = 3; i >= 0; i--) {
			s4295255_hamming_encode(destination_addr[i]); //encode destination address, starting from LSB
		}

		for(i = 3; i >= 0; i--) {
			s4295255_hamming_encode(source_addr[i]); //encode source address, starting from LSB
		}

		for(i = 6; i >= 0; i--) {
			s4295255_hamming_encode(payload[i]); //encode payload, starting from LSB
		}


		s4295255_radio_getpacket(packetbuffer);

		int ptr = 0;

		rec_packet_type = s4295255_hamming_decode((packetbuffer[ptr] << 8 | packetbuffer[ptr++]));



		for(i = 0; i < 4; i++) {

			rec_destination_address[i] = s4295255_hamming_decode((packetbuffer[ptr] << 8 | packetbuffer[ptr++]));
			if(rec_destination_address[i] != source_addr[3-i]){

				ptr = 0;
				break;

			}

		}
		
		if(ptr == 0) {

			continue;
		}

		
		for(i = 0; i < 4; i++) {

			rec_source_address[i] = s4295255_hamming_decode((packetbuffer[ptr] << 8 | packetbuffer[ptr++]));
		

		}


		for(i = 0; i < 4; i++) {

			rec_payload[i] = s4295255_hamming_decode((packetbuffer[ptr] << 8 | packetbuffer[ptr++]));
		

		}


		print_packet();		
		

    	BRD_LEDToggle();	//Toggle LED on/off
    	Delay(0x7FFF00);	//Delay function
  	}

}

/**
  * @brief  Initialise hardware modules
  * @param  None
  * @retval None
  */
void HardwareInit() {
	
	s4295255_radio_init();
	s4295255_radio_settxaddress(destination_addr);
	s4295255_radio_setchan(CHANNEL);
}



/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO unsigned long nCount) {
  while(nCount--) {
  }
}


void print_packet() {

	debug_printf("Packet Type : %X\nDestination Address : ", rec_packet_type);
	debug_flush();

	int i;	

	for(i = 3 ; i >= 0; i--){ 
		debug_printf(" %X", rec_destination_address[i]);
		Delay(100);


	}


	debug_flush();

	debug_printf("\nSource Address : ");

	for(i = 3 ; i >= 0; i--){ 
		debug_printf(" %X", rec_source_address[i]);
		Delay(100);

	}

	debug_printf("\nPayload : ");

	for(i = 6 ; i >= 0; i--){ 
		debug_printf(" %X", rec_payload[i]);
		Delay(100);

	}

	debug_printf("\n");
	

}


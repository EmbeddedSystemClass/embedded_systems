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
//#include "nrf24l01plus.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CHANNEL	50
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t destination_addr[] = {0x7B, 0x56, 0x34, 0x12, 0x00};
uint8_t source_addr[] = {0x00,0x42, 0x95, 0x25, 0x56};
char packet_type = 0x20;
char payload[7];
uint8_t packet[32];	/* Packet buffer initialised to 32 bytes (max length) */
uint8_t r_packet[32];
uint8_t decoded_packet[16];
unsigned char rec_packet_type;
unsigned char rec_destination_address[4];
unsigned char rec_source_address[4];
unsigned char rec_payload[4];


/* Private function prototypes -----------------------------------------------*/
void Delay(__IO unsigned long nCount);
void HardwareInit();
void print_packet();
void get_payload();

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {

	uint8_t status;
	
	BRD_init();
	HardwareInit();

	/* Initialise NRF24l01plus */ 
	//nrf24l01plus_init();
	
	/* Put NRF24l01plus in RX mode */
	//nrf24l01plus_mode_rx();

	int ptr = 0;

	int rec = 0;

	/* Main Processing Loop */
    while (1) {


		//get_payload();
		
		
		
		char RxChar;

		RxChar = debug_getc();
		
		if(RxChar != '\0') {
			payload[ptr] = RxChar;

			debug_printf("%c\n", payload[ptr]);
						ptr++;
			Delay(0x7FFF00/20);

			
		}


		if(ptr == 7) {
			debug_printf("Payload was received , now send the packet\n");
			ptr = 0;


		//debug_printf("Sending Packet\n");

		//hamming encoding and then send packet
		int p_ptr = 0;
		int i;
		uint16_t encoded_data;
		encoded_data = s4295255_hamming_encode(packet_type); //encode packet type
		packet[p_ptr++] = encoded_data & 0xFF;
		packet[p_ptr++] = encoded_data >> 8;
		

		for(i = 0; i ,i<= 3; i++) {
			encoded_data = s4295255_hamming_encode(destination_addr[i]); //encode destination address, starting from LSB
			packet[p_ptr++] = encoded_data & 0xFF;
			packet[p_ptr++] = encoded_data >> 8;
		}

		for(i = 4; i >= 1; i--) {
			encoded_data = s4295255_hamming_encode(source_addr[i]); //encode source address, starting from LSB
			packet[p_ptr++] = encoded_data & 0xFF;
			packet[p_ptr++] = encoded_data >> 8;
		}

		for(i = 0; i <= 6; i++) {
			encoded_data = s4295255_hamming_encode(payload[i]); //encode payload, starting from LSB
			packet[p_ptr++] = encoded_data & 0xFF;
			packet[p_ptr++] = encoded_data >> 8;
		}

		//Print the packet to be sent

		for(i = 0; i < 32; i++) { 

			debug_printf("%x ", packet[i]);
			Delay(0x7FFF00/20);
		}

		debug_printf("\n");
			

		s4295255_radio_sendpacket(packet);
		r_packet[0] = 0x1;
		rec = 0;
		while(rec == 0) {
		
			if(s4295255_radio_getpacket(r_packet) == 1) {

				debug_printf("Still Working	;\n");
				rec =1;
				//Print the packet recieved
				debug_printf("Recieved : ");
				Delay(0x7FFF00/20);

				for(i = 0; i < 32; i++) { 

					debug_printf("%x ", r_packet[i]);
					Delay(0x7FFF00/20);
				}

				debug_printf("\n");

				int d_ptr = 0; //pointer for decoded data

				

				decoded_packet[d_ptr++] = s4295255_hamming_decode(r_packet[0] | (r_packet[1] << 8)); //payload

				for(i = 2; i < 10; i +=2) {

					decoded_packet[d_ptr++] = s4295255_hamming_decode(r_packet[i] | (r_packet[i+1] << 8)); //Destination address

				}

				for(i = 10; i < 18; i +=2) {

					decoded_packet[d_ptr++] = s4295255_hamming_decode(r_packet[i] | (r_packet[i+1] << 8)); //Source address

				}

				for(i = 18; i < 32; i +=2) {

					decoded_packet[d_ptr++] = s4295255_hamming_decode(r_packet[i] | (r_packet[i+1] << 8)); //Payload

				}

				//Print the packet recieved
				debug_printf("Decoded Packet : ");
				Delay(0x7FFF00/20);

				for(i = 0; i < 16; i++) { 

					debug_printf("%x ", decoded_packet[i]);
					Delay(0x7FFF00/20);
				}

				debug_printf("\n");
				
		
		}
		
		

		int ptr = 0;

		//rec_packet_type = s4295255_hamming_decode((packetbuffer[ptr] << 8 | packetbuffer[ptr++]));


		/*
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
		
		*/

		} 
		
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

	BRD_LEDInit();	//Initialise Blue LED
	BRD_LEDOff();	//Turn off Blue LED
	
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

void get_payload() {


	int ptr = 0;

	char RxChar;

	while(ptr != 7) {

		
		RxChar = debug_getc();
		
		if(RxChar != '\0') {
			payload[ptr] = RxChar;

			debug_printf("%c\n", payload[ptr]);
						ptr++;
			Delay(0x7FFF00/20);

			
		}

	}


}


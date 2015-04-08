/**
  ******************************************************************************
  * @file    stage3/main.c 
  * @author  Mani Batra
  * @date    019032015
  * @brief   Control the pan of the servo motor by generating the PWM on D2
  ******************************************************************************
  *  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "s4295255_servo.h"
#include "s4295255_button.h"
#include "s4295255_radio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define CONSOLE //Uncomment to use the console as direction provider, stage 3, design task 2
#define DEBUG  //Uncomment to print debug statements
#define CHANNEL	50 //channel of the radio
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t destination_addr[] = {0x7B, 0x56, 0x34, 0x12, 0x00};
uint8_t source_addr[] = {0x00,0x42, 0x95, 0x25, 0x56};
char packet_type = 0xA1;
char payload[19];
uint8_t packet[32];

int pan_angle = 0; //angle of the servo
int tilt_angle = 0;
int console = 0; // to activate(1) or deactivate(0) console control of the servos

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO unsigned long nCount);
void Hardware_init();
void set_new_panangle(uint16_t adc_x_value);
void set_new_tiltangle(uint16_t adc_y_value);
int print_counter  = 0; // Counter to tell when the pan and tilt values will be printed




 
//int direction = 1;  //direction the servo will move in if controlled from console

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void main(void) {


	char RxChar;
	int payload_ptr = 0; //ptr for the payload recieved to be sent for task4

	BRD_init();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules

  	while (1) {


		if(console) {	//servo control is transferred to the console

			payload_ptr = 0;
			RxChar = debug_getc();

			if (RxChar != '\0') {

				if(RxChar == 'w') {

					set_new_tiltangle(2061);

				} else if(RxChar == 's') {

					set_new_tiltangle(2019);

				} else if(RxChar == 'a') {

					set_new_panangle(2026);

				} else if(RxChar == 'd') {

					set_new_panangle(1994);

				}

			}
		} else {
			//recieve joystick values
			uint16_t adc_x_value = s4295255_joystick_get(0);
			uint16_t adc_y_value = s4295255_joystick_get(1);

#ifdef DEBUG		
			/* Print ADC conversion values */
			if(adc_x_value > 2025 || adc_x_value < 1995) {
			
				debug_printf("ADC X Value: %d\n", adc_x_value);

			}
			if(adc_y_value > 2060 || adc_y_value < 2020) {
			
				debug_printf("ADC Y Value: %d\n", adc_y_value);

			}
#endif		
			set_new_panangle(adc_x_value);
			set_new_tiltangle(adc_y_value);

			
			//get chars from the terminal for sending as uncoded RF
			RxChar = debug_getc();
		
			if(RxChar != '\0') {
				
				if(RxChar != '\r') {
					payload[payload_ptr] = RxChar;

#ifdef DEBUG
					debug_printf("%c\n", payload[payload_ptr]);
#endif
					payload_ptr++;
				} else {
					
					for(;payload_ptr < 19;payload_ptr++) {

						payload[payload_ptr] = '\0';
					}
					


				}

			}

			if(payload_ptr > 18) {

				int packet_ptr = 0; //ptr for the packet that is to be sent
				int i = 0; //for loop variable

				packet[packet_ptr++] = packet_type;
				
				for(i=0; i <= 3; i++){
					packet[packet_ptr++] = destination_addr[i];
				}

				for(i=4; i > 0; i--) {
					packet[packet_ptr++] = source_addr[i];
				} 

				for(i=0; i < 19; i++) {
					packet[packet_ptr++] = payload[i]; //may have to reverse the direction of sending of payload
				}

				for(;packet_ptr < 32; packet_ptr++) {
					packet[packet_ptr++] = 0;
				}

				payload_ptr = 0;

#ifdef DEBUG
				debug_printf("Sending : ");
				for(i = 0; i < 32; i++) { 

					debug_printf("%x ", packet[i]);
					Delay(0x7FFF00/20);
				}
#endif

				s4295255_radio_sendpacket(packet);
				

			}
		}

		if(print_counter > 20) { 
			print_counter = 0;
			debug_printf("PAN : %d  TILT : %d\n", pan_angle, tilt_angle); //printing out the angles to the console
		}

		print_counter++;
		BRD_LEDToggle();	//Toggle 'Alive' LED on/off
    	Delay(0x7FFF00/10);	//Delay function
	}
}

/**
  * @brief  Configure the hardware, 
  * @param  None
  * @retval None
  */
void Hardware_init(void) {



	s4295255_joystick_init();
	s4295255_pushbutton_init();
	s4295255_servo_init();
	s4295255_servo_setangle(pan_angle);
	s4295255_servo_settiltangle(tilt_angle);
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

/**
  * @brief  exti interrupt Function for A2.

  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
/*
void exti_a2_interrupt_handler(void) {
	

	//change the angle by 10 deg
	angle = angle + (direction*10);

	if(angle < -90) {

		angle = -90;

	}

	if(angle > 90) {

		angle = 90;

	}

	s4295255_servo_setangle(angle);


	debug_printf("%d\n", angle);

	Delay(0x4C4B40); //Switch Debouncing 	

	HAL_GPIO_EXTI_IRQHandler(BRD_A2_PIN);				//Clear A2 pin external interrupt flag

	

}

*/

/**
  * @brief  exti interrupt Function for PB.


  * @param  None
  * @retval None

  */

void exti_pb_interrupt_handler(void) {
	



	console = !(console);

#ifdef DEBUG	
	debug_printf("Push Button Interrupt Recieved Console = %d\n", console);
#endif

	Delay(0x4C4B40); //Switch Debouncing 	


	HAL_GPIO_EXTI_IRQHandler(BRD_PB_PIN);				//Clear PB pin external interrupt flag

	


}


//get the new value of the angle that needs to be changed
void set_new_panangle(uint16_t adc_x_value) {

	//setting the value of the pan angle according to the value from x - axis
		if(adc_x_value < 1995) {

			pan_angle = pan_angle - 1;
			if(pan_angle < -75) {

				pan_angle = -75;

			}
			s4295255_servo_setangle(pan_angle);
			
		} else if(adc_x_value > 2025) {

			pan_angle = pan_angle + 1;
			if(pan_angle > 75) {

				pan_angle = 75;

			}
			
			s4295255_servo_setangle(pan_angle);

		}

}

void set_new_tiltangle(uint16_t adc_y_value) {

	//setting the value of the tilt angle according to the value from y - axis
		if(adc_y_value < 2020) {

			tilt_angle = tilt_angle - 1;
			if(tilt_angle < -85) {

				tilt_angle = -85;

			}
			s4295255_servo_settiltangle(tilt_angle);
			
		} else if(adc_y_value > 2060) {

			tilt_angle = tilt_angle + 1;
			if(tilt_angle > 75) {

				tilt_angle = 75;

			}
			
			s4295255_servo_settiltangle(tilt_angle);

		}

}



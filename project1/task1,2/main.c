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
#include "s4295255_laser.h"
#include "s4295255_manchester.h"

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
uint8_t packet[32]; //packet to be sent
uint8_t r_packet[32]; //packet to be recieved via radio
uint8_t l_packet[2] = {0x00, 0x00}; //packet recieved via laser
int bit_count = 0; //count of the bits recieved via laser
int byte_count = 0; //count of the bytes recieved via laser
int counter = 0;
int delay1 =20000;


unsigned int prev_count = 0;
unsigned int count = 0; 
int time_period = 0;
int syn = 0;
int current_period;
int speed_mode = 0; //switch on to show challenge 8

uint8_t current_bit;
int capture = 0;
uint8_t test;
int edges = 0;
int laser_received = 0; //to indicate if received from laser
char decoded_laser_byte;
uint16_t test_p;
int byte_counts[128];
int byte_count_ptr = 0;
int prev_ptr = -1;
int flag = 0;
int retransmission = 0; //used for duplex communication
char temp; //char for retransmission


int pan_angle = -13; //angle of the servo
int tilt_angle = 75;
int console = 0; // to activate(1) or deactivate(0) console control of the servos
int laser = 0; //to activate(1) or deactivate(0) transmission through laser, when laser is on 
			   //RF wont work. Toggled by *. 
int laser_receiver = 0; //0 when not receiving 
int data_length = 0; //length of the data recieved from the terminal
uint16_t err_mask = 0x0000;

TIM_HandleTypeDef TIM_Initi;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO unsigned long nCount);
void Hardware_init();
void set_new_panangle(uint16_t adc_x_value);
void set_new_tiltangle(uint16_t adc_y_value);
void tim3_irqhandler (void);
void go_sync(void);
void manchester_decode(void);
int print_counter  = 0; // Counter to tell when the pan and tilt values will be printed





 


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void main(void) {


	char RxChar;
	int i = 0; //for loop variable
	int payload_ptr = 0; //ptr for the payload recieved to be sent for task4

	BRD_init();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules

  	while (1) {


		if(prev_ptr == byte_count_ptr && counter == 50 && flag == 0) {

			
			//debug_printf("All counts received %d\n", byte_count_ptr); //can decode once this happens
			counter = 0;

			
			manchester_decode();
			prev_ptr = -1;
	

			

		} else {

			prev_ptr = byte_count_ptr;
			counter++;
			if(counter > 50)
				counter = 0;
			

		
		}

		
		//r_packet[0] = 0x1;
		if(console) {	//servo control is transferred to the console

			payload_ptr = 0;
			RxChar = debug_getc();

			if (RxChar != '\0') {

				if(RxChar == 'w') {

					set_new_tiltangle(2081);

				} else if(RxChar == 's') {

					set_new_tiltangle(1999);

				} else if(RxChar == 'a') {

					set_new_panangle(2051);

				} else if(RxChar == 'd') {

					set_new_panangle(1959);

				}

			}
		} else {
			//recieve joystick values
			uint16_t adc_x_value = s4295255_joystick_get(0);
			uint16_t adc_y_value = s4295255_joystick_get(1);

#ifdef DEBUG		
			/* Print ADC conversion values */
			if(adc_x_value > 2050 || adc_x_value < 1960) {
			
				debug_printf("ADC X Value: %d\n", adc_x_value);

			}
			if(adc_y_value > 2080 || adc_y_value < 2000) {
			
				debug_printf("ADC Y Value: %d\n", adc_y_value);

			}
#endif		
			set_new_panangle(adc_x_value);
			set_new_tiltangle(adc_y_value);

			
			//get chars from the terminal for sending as uncoded RF
			RxChar = debug_getc();
		
			if(RxChar != '\0') {
				
				if(RxChar != '\r') {
					
					if(RxChar == 42) { //toggles the use of laser
						laser = !(laser);
						retransmission = 0;
#ifdef DEBUG
						debug_printf("Toggled the laser\n");
#endif

					} else if(RxChar == 33) {
						
						delay1 = 20000;
						debug_printf("Data rate set at 1K bits/sec\n");
							
					} else if(RxChar == 35) {
						
						debug_printf("Speed mode toggled\n");
						speed_mode = !(speed_mode);

					}else if(RxChar == 64) {

						delay1 = 9000;
						debug_printf("Data rate set at 2K bits/sec\n");

					}else if(RxChar == 36) { //stimulates an error packet input to reciever i.e. D0

						l_packet[0] = 0xe3;
						l_packet[1] = 0x66;
						laser_received = 1;

					} else {
								
						payload[payload_ptr] = RxChar;

					
#ifdef DEBUG
						debug_printf("%c\n", payload[payload_ptr]);
#endif
						payload_ptr++;
						data_length++;

					}
				} else {
					
					for(;payload_ptr < 19;payload_ptr++) {

						payload[payload_ptr] = '\0';
					}
					


				}

			}

			if(payload_ptr > 18) {

				int packet_ptr = 0; //ptr for the packet that is to be sent


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



				if(!laser) {
					if(retransmission)
						laser = 1;
#ifdef DEBUG
					debug_printf("Sending : ");
					for(i = 0; i < 32; i++) { 

						debug_printf("%x ", packet[i]);
						Delay(0x7FFF00/20);
					}
#endif
					s4295255_radio_sendpacket(packet);
				} else { //encode , modulate and send the data through the laser

#ifdef DEBUG
						debug_printf("Sending via laser :");
#endif					
						temp = payload[i];
#ifdef DEBUG

					for(i = 0; i < data_length; i++) { 

						debug_printf("%x ", payload[i]);
						Delay(0x7FFF00/20);
					}
#endif
					uint16_t encoded_value; //value received after encoding 1 char
					//DO STUFF with laser
					uint8_t encoded_data[2*data_length]; //the data to be sent
					int encoded_data_ptr = 0; //ptr to fill up the encoded data
					//hamming encode the data : done
					for(i = 0; i < data_length; i++) {

						encoded_value = s4295255_hamming_encode(payload[i]);
#ifdef DEBUG
						debug_printf("Encoded Value : %x , %x \n", encoded_value & 0xFF, (encoded_value >> 8));
#endif	
						encoded_data[encoded_data_ptr++] = encoded_value & 0xFF;
						encoded_data[encoded_data_ptr++] = encoded_value >> 8;
						

					}

					//add start and stop bits : to be done when the modulating starts : done
					//manchester modulate the data, waveform will be output to pin1 and send via laser: done
						 //Start Input Capture 

									HAL_TIM_IC_Start_IT(&TIM_Initi, TIM_CHANNEL_2); 
					for(i = 0; i < encoded_data_ptr; i++) {

						s4295255_manchester_byte_encode(encoded_data[i]);

					}



					data_length = 0;

					 
					

				}
				
				

			}
		}
		
		if(s4295255_radio_getpacket(r_packet) == 1) {
			
			debug_printf("RECEIVED FROM RADIO: ");
			Delay(0x7FFF00/20);

			for(i = 0; i < 32; i++) { 

				debug_printf("%x ", r_packet[i]);
				Delay(0x7FFF00/20);
			}

			char error[6];
			error[0] = r_packet[9];
			error[1] = r_packet[10];
			error[2] = r_packet[11];
			error[3] = r_packet[12];
			error[4] = r_packet[13];
			error[5] = '\0';

			if(!strcmp(error, "ERROR")){
				payload[i] = temp;
				for(i=1; i < 19; i++)
					payload[i] = '\0';
					payload_ptr = 19;


			}

			
		
				debug_printf("\n");
		}


		if(print_counter > 20) { 
			print_counter = 0;
			debug_printf("PAN : %d  TILT : %d  %d  %x   %x\n", pan_angle, tilt_angle, edges, l_packet[1], l_packet[0]); //printing out the angles to the console
		}

		if(laser_received){
			decoded_laser_byte = s4295255_hamming_decode(l_packet[1] << 8 | l_packet[0]);
			Delay(0x7FFF00/20);
			debug_printf("RECEIVED FROM LASER: %c - Raw :%x%x  (ErrMask %04x)\n", decoded_laser_byte, l_packet[1], l_packet[0], err_mask);
			if(speed_mode == 1 && err_mask == 0x0000) {

				if(delay1 < 5000) {
					debug_printf("Maximum speed acheived  %d\n", delay1);

				} else {
					delay1-=2000;
					debug_printf("Increase the speed of the laser  %d\n", delay1);

				}
			} else if(speed_mode == 1) {

				delay1+=2000;
				debug_printf("Decrease the speed of the laser %d\n", delay1);

			} else {
			if(err_mask != 0x0000) {
				
				payload[0] = 'E';
				payload[1] = 'R';
				payload[2] = 'R';
				payload[3] = 'O';
				payload[4] = 'R';
				for(i=5; i < 19; i++)
					payload[i] = '\0';

			} else {
				payload[0] = decoded_laser_byte;
				for(i=1; i < 19; i++)
					payload[i] = '\0';
			}
			payload_ptr = 19;
			retransmission = 1;
			laser = 0;
			}
			l_packet[0] = 0x00;
			l_packet[1] = 0x00;
			laser_received = 0;
			byte_count_ptr = 0;
			for( i = 0; i < 128; i++) {
				
				byte_counts[i] = 0;

			}

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

	/* Timer 3 clock enable */
  	__TIM3_CLK_ENABLE();

	s4295255_joystick_init();
	s4295255_pushbutton_init();
	s4295255_servo_init();
	s4295255_servo_setangle(pan_angle);
	s4295255_servo_settiltangle(tilt_angle);
	s4295255_radio_init();
	s4295255_radio_settxaddress(destination_addr);
	s4295255_radio_setchan(CHANNEL);
	s4295255_laser_init();

	TIM_IC_InitTypeDef  TIM_ICInitStructure;
	uint16_t PrescalerValue = 0;


	


		// Compute the prescaler value. SystemCoreClock = 168000000 - set for 50Khz clock 
  	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 50000) - 1;

	// Configure Timer 3 settings 
	TIM_Initi.Instance = TIM3;					//Enable Timer 3
  	TIM_Initi.Init.Period = 2*50000/10;			//Set for 100ms (10Hz) period
  	TIM_Initi.Init.Prescaler = PrescalerValue;	//Set presale value
  	TIM_Initi.Init.ClockDivision = 0;			//Set clock division
	TIM_Initi.Init.RepetitionCounter = 0; 		// Set Reload Value
  	TIM_Initi.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.
	
	//Configure TIM3 Input capture 
  	TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;			//Set to trigger on rising edge
  	TIM_ICInitStructure.ICSelection = TIM_ICSELECTION_DIRECTTI;
  	TIM_ICInitStructure.ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStructure.ICFilter = 0;

	// Set priority of Timer 3 Interrupt [0 (HIGH priority) to 15(LOW priority)] 
	HAL_NVIC_SetPriority(TIM3_IRQn, 10, 0);	//Set Main priority ot 10 and sub-priority ot 0.

	//Enable Timer 3 interrupt and interrupt vector
	NVIC_SetVector(TIM3_IRQn, (uint32_t)&tim3_irqhandler);  
	NVIC_EnableIRQ(TIM3_IRQn);

	// Enable input capture for Timer 3, channel 2 
	HAL_TIM_IC_Init(&TIM_Initi);
	HAL_TIM_IC_ConfigChannel(&TIM_Initi, &TIM_ICInitStructure, TIM_CHANNEL_2);




	
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
	//Delay(20000);
	
	//setting the value of the pan angle according to the value from x - axis
		if(adc_x_value < 1960) {

			pan_angle = pan_angle - 1;
			if(pan_angle < -75) {

				pan_angle = -75;

			}
			s4295255_servo_setangle(pan_angle);
			
		} else if(adc_x_value > 2050) {

			pan_angle = pan_angle + 1;
			if(pan_angle > 75) {

				pan_angle = 75;

			}
			
			s4295255_servo_setangle(pan_angle);

		}

}

void set_new_tiltangle(uint16_t adc_y_value) {
	//Delay(20000);
	//setting the value of the tilt angle according to the value from y - axis
		if(adc_y_value < 2000) {

			tilt_angle = tilt_angle - 1;
			if(tilt_angle < -85) {

				tilt_angle = -85;

			}
			s4295255_servo_settiltangle(tilt_angle);
			
		} else if(adc_y_value > 2080) {

			tilt_angle = tilt_angle + 1;
			if(tilt_angle > 75) {

				tilt_angle = 75;

			}
			
			s4295255_servo_settiltangle(tilt_angle);
		}

}



void tim3_irqhandler (void) {





	byte_counts[byte_count_ptr++] = HAL_TIM_ReadCapturedValue(&TIM_Initi, TIM_CHANNEL_2);

	
		//Clear Input Capture Flag
	__HAL_TIM_CLEAR_IT(&TIM_Initi, TIM_IT_TRIGGER);







}


void manchester_decode(){

 int i = 0;
 int syn = 0;
err_mask = 0x0000;
for(; i < byte_count_ptr; i++) {
	count = byte_counts[i];
	if(syn == 0) {

		debug_printf("IC : %d  %d\n", time_period, count );
	/*	if(edges > 3) {
			
			prev_count = 0;
			count = 0;
			syn = 0;
			bit_count = 0;
			byte_count = !(byte_count);
			capture = 0;
			time_period = 0;
			edges = 0;
			err_mask = 0xFF00;
			break;

		}*/


	/*	if(prev_count == 0) {

			prev_count = count;


		} else if(time_period == 0) {
				
			time_period = count - prev_count;
			prev_count = count;


		} else {

			current_period = (count - prev_count) % 1000;
			

			if((current_period) > (time_period - time_period/3) && (current_period) < (time_period + time_period/3)){
				
					syn = 1;	
				current_bit = 0x01;
				prev_count = count;	

			} else {
				prev_count = count;

			}


		} */

		if(edges ==0)
			prev_count = count;
		else if(edges == 1) {
			
			prev_count = count;

		} else if(edges== 2) {
			time_period = count - prev_count;
			prev_count = count;
			syn = 1;
			current_bit = 0x01;

		}
				edges++;

	} else {

		current_period = (count - prev_count) % 1000;

#ifdef DEBUG
		debug_printf("Current_period : %d\n", current_period);
		Delay(0x7FFF00/20);
#endif
		if((current_period > (time_period - time_period/3)) && (current_period < (time_period + time_period/3))){


			
			if(capture){

				if(bit_count < 8)

					l_packet[byte_count] |= (current_bit << bit_count);

#ifdef DEBUG
				debug_printf("current bit : %d %d\n", current_bit, bit_count);
				Delay(0x7FFF00/20);
#endif
				capture = !(capture);
				bit_count++;

			} else {


				capture = 1;

			}


			prev_count = count;

				

		} else {



			current_bit =  !(current_bit);
#ifdef DEBUG
				debug_printf("current bit : %d %d\n", current_bit, bit_count);
				Delay(0x7FFF00/20);
#endif
			if(bit_count < 8)

				l_packet[byte_count] |=  (current_bit << bit_count);
			bit_count++;
			prev_count =count;

			

			
		}

		if(bit_count == 9) { //reinitialise everything for the second word


			if(byte_count == 1){
				laser_received = 1;


			}

			prev_count = 0;

			count = 0;
			syn = 0;
			bit_count = 0;
			byte_count = !(byte_count);
			edges = 0;
			capture = 0;
			time_period = 0;


		}


	
		



	}

}


HAL_TIM_Base_Stop_IT(&TIM_Initi);
byte_count_ptr = 0;
flag = 0;
prev_ptr = -1;
prev_count = 0;

}

		



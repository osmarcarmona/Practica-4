/****************************************************************************
 * Project developed as a simple Blink the LED's to 
 * demonstrate basic CodeWarrior functionality and capability.
 * Borrowed from Freedom Example Projects: Blinky
 * 
 * Date: 11 Sept 2012
 * Author: M. Ruthenbeck
 * Revision Level 1.0
 */

#include "derivative.h" /* include peripheral declarations */

typedef unsigned char T_UBYTE;
typedef unsigned int T_UWORD;


#define SLOW_BLINK      (10000000)
#define FAST_BLINK      (1000000)
#define BLINK_DELAY     FAST_BLINK

#define RED				(18) 
#define RED_SHIFT		(1 << 18)
#define GREEN			(19)
#define GREEN_SHIFT		(1 << 19)
#define BLUE			(1)
#define BLUE_SHIFT		(1 << 1)

#define RED_OFF			(GPIOB_PSOR = RED_SHIFT)
#define RED_ON			(GPIOB_PCOR = RED_SHIFT)
#define RED_TOGGLE		(GPIOB_PTOR = RED_SHIFT)

#define GREEN_OFF		(GPIOB_PSOR = GREEN_SHIFT)
#define GREEN_ON		(GPIOB_PCOR = GREEN_SHIFT)
#define GREEN_TOGGLE	(GPIOB_PTOR = GREEN_SHIFT)

#define BLUE_OFF		(GPIOD_PSOR = BLUE_SHIFT)
#define BLUE_ON			(GPIOD_PCOR = BLUE_SHIFT)
#define BLUE_TOGGLE		(GPIOD_PTOR = BLUE_SHIFT)

T_UWORD ruw_state = 0;
T_UWORD ruw_cnt_debounce_fwd = 0, ruw_cnt_debounce_rwd = 0;
T_UBYTE rub_pista = 1, rub_reproduciendo = 0, rub_flag = 0, rub_stop_flag = 0, rub_button_flag = 0;
T_UWORD ruw_i=0, ruw_o=0;

void delay_time(int);
void init_leds();
void inputsInit();
void red_on();
void red_off();
void green_on();
void green_off();
void blue_on();
void blue_off();
void uart_test(void);





void state_machine(void)
{
	switch(ruw_state)
	{
		case 0: 
			break;
		case 1: 
			//VOLUMEN
			rub_stop_flag = 0;
			break;
		case 2: //FWD
			if(rub_pista == 5)
				rub_pista = 1;
			else
				rub_pista++;
			ruw_i=0;
			ruw_o=0;
			if(rub_reproduciendo == 1)
				ruw_state = 16;
			else
				ruw_state = 17;
			rub_stop_flag = 0;
			break;
		case 8: 
			//STOP
			red_off();
			green_off();
			blue_off();
			rub_flag = 0;
			rub_reproduciendo = 0;
			if(rub_stop_flag == 1)
				rub_pista = 1;
			rub_stop_flag = 1;
			ruw_state = 0;
			ruw_i=0;
			ruw_o=0;
			break;
		case 16:
			//PLAY
			rub_reproduciendo = 1;
			ruw_state = 16;
			rub_stop_flag = 0;
			break;
		case 17:
			//PAUSE
			rub_reproduciendo = 0;
			rub_stop_flag = 0;
			break;
		case 128: 
			//VOLUMEN
			rub_stop_flag = 0;
			break;
			//RWD
		case 129:
			if(ruw_i>10 || ruw_o>0){
				ruw_i=0;
				ruw_o=0;
			}
			else if((ruw_i<10) && (ruw_o==0))
			{
				if(rub_pista == 1)
					rub_pista = 5;
				else
					rub_pista--;
				ruw_i = 0;
				ruw_o = 0;
			}
			if(rub_reproduciendo == 1)
				ruw_state = 16;
			else
				ruw_state = 17;
			rub_stop_flag = 0;
			break;
		default:
			rub_stop_flag = 0;
			break;
	}
}

void red_on(){
	RED_ON;
}

void red_off(){
	RED_OFF;
}

void green_on(){
	GREEN_ON;
}

void green_off(){
	GREEN_OFF;
}

void blue_on(){
	BLUE_ON;
}

void blue_off(){
	BLUE_OFF;
}

/********************************************************************/

void delay_time(int number){
  int cnt;
  for(cnt=0;cnt<number;cnt++);
}
/********************************************************************/

/********************************************************************/
/*	init_leds()
 * initialize the ports for LEDs
 * ******************************************************************/
 

 void init_leds(void)
 {
	 
    /* 
	 * Initialize the Red LED (PTB18)
	 */

		/* Turn on clock to PortB module */
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

		/* Set the PTB18 pin multiplexer to GPIO mode */
		PORTB_PCR18 = PORT_PCR_MUX(1);

		/* Set the initial output state to high */
		GPIOB_PSOR |= RED_SHIFT;

		/* Set the pins direction to output */
		GPIOB_PDDR |= RED_SHIFT;


	/*
	 * Initialize the Green LED (PTB19)
	 */

		/* Turn on clock to PortB module */
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

		/* Set the PTB19 pin multiplexer to GPIO mode */
		PORTB_PCR19 = PORT_PCR_MUX(1);

		/* Set the initial output state to high */
		GPIOB_PSOR |= GREEN_SHIFT;

		/* Set the pins direction to output */
		GPIOB_PDDR |= GREEN_SHIFT;



	/*
	 * Initialize the Blue LED (PTD1)
	 */

		/* Turn on clock to PortB module */
		SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;

		/* Set the PTD1 pin multiplexer to GPIO mode */
		PORTD_PCR1 = PORT_PCR_MUX(1);

		/* Set the initial output state to high */
		GPIOD_PSOR = BLUE_SHIFT;

		/* Set the pins direction to output */
		GPIOD_PDDR |= BLUE_SHIFT;
	}

 void inputsInit(void)
 {
 	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //CLK PORTC
 	PORTC_PCR0 = PORT_PCR_MUX(1);//SELECT AS GPIO
 	PORTC_PCR3 = PORT_PCR_MUX(1);//SELECT AS GPIO
 	PORTC_PCR4 = PORT_PCR_MUX(1);//SELECT AS GPIO
 	PORTC_PCR7 = PORT_PCR_MUX(1);//SELECT AS GPIO
 	
 	GPIOC_PDDR = 0<<0;//PIN 0 PORT C CONFIGURED AS INPUT
 	GPIOC_PDDR = 0<<3;//PIN 2 PORT C CONFIGURED AS INPUT
 	GPIOC_PDDR = 0<<4;//PIN 3 PORT C CONFIGURED AS INPUT
 	GPIOC_PDDR = 0<<7;//PIN 4 PORT C CONFIGURED AS INPUT
 }
 void inputsRead(void)
 {
 	//static unsigned char debounce = 0;
 	static unsigned int pValue = 0;
 	static char rValue = 0;
 	
 	pValue = GPIOC_PDIR;
	if(pValue == 0 || pValue == 1 	|| pValue == 16 ||pValue == 8 ||pValue == 128)
	{
		
		if(rub_button_flag == 0)
		{
			if(pValue != 0 && (pValue == 1 	|| pValue == 16 ||pValue == 8 ||pValue == 128))
				rValue = pValue;
			switch(pValue)
			{
			case 0:
				if(ruw_cnt_debounce_rwd>200 && rValue == 128)
				{
					ruw_state = rValue+1;
					rValue = 0;
					//ruw_cnt_debounce_rwd = 0;
				}
				else if(ruw_cnt_debounce_fwd>200 && rValue == 1)
				{
					ruw_state = rValue + 1;
					rValue = 0;
				//ruw_cnt_debounce_fwd = 0;
				}
				else if(rValue == 16)
				{
					if(rub_flag == 0)
					{
						rub_flag = 1;
						ruw_state = rValue;
						rValue = 0;
					}
					else
					{
						rub_flag = 0;
						ruw_state = rValue+1;
						rValue = 0;
					}
				}
				else if(rValue == 8 || (rValue == 1 && ruw_cnt_debounce_fwd<200) || (ruw_cnt_debounce_rwd<200 && rValue == 128))
					ruw_state = rValue;
				rValue = 0;
				ruw_cnt_debounce_fwd = 0;
				ruw_cnt_debounce_rwd = 0;
				break;
			case 1:
				ruw_cnt_debounce_fwd++;
				break;
			case 128:
				ruw_cnt_debounce_rwd++;
				break;
			default:
				break;
			}

		}
		else if(rub_button_flag == 1 && pValue == 0)
			rub_button_flag = 0;
	}
	else
	{
		rValue = 0;
		rub_button_flag = 1;
	}


 /*	if(debounce >= 1)
 	{
 		if(pValue/2 == (GPIOC_PDIR))
 		{
 			ruw_state = pValue/2;
 			debounce = 0;
 			pValue = 0;
 			return pValue/2;
 		}
 		else
 		{
 			debounce = 0;
 			pValue = 0;
 			return 0;			
 		}
 	}
 	debounce++;*/
 }

 void player(void)
 {
	 switch(rub_pista)
	 {
	 case 0:
		red_off();
		green_off();
		blue_off();
		ruw_i=0;
		ruw_o=0;
		break;
	 case 1:
		if(rub_reproduciendo == 1)
		{
			if(ruw_o<4)
			{
				if(ruw_i>0)
				{
					red_off();
					green_on();
					blue_off();
				}
				if(ruw_i>6)
				{
					red_on();
					green_on();
					blue_on();
				}
				if(ruw_i>10)	
				{
					blue_off();
					red_on();
					green_off();
				}
				if(ruw_i>12)
				{
					blue_on();
					red_off();
					green_off();
				}					
				if(ruw_i>16)
				{
					ruw_i = 0;
					ruw_o++;
				}
				ruw_i++;
			}
			else
			{
				ruw_i=0;
				ruw_o=0;
				rub_pista++;
			}
		}
		break;
	 case 2:
			if(rub_reproduciendo == 1)
			{
				if(ruw_o<4)
				{
					if(ruw_i>0)
					{
						red_off();
						green_on();
						blue_on();
					}
					if(ruw_i>4)
					{
						red_on();
						green_off();
						blue_on();
					}
					if(ruw_i>8)	
					{
						blue_off();
						red_off();
						green_on();
					}
					if(ruw_i>14)
					{
						blue_off();
						red_on();
						green_off();
					}					
					if(ruw_i>16)
					{
						ruw_i = 0;
						ruw_o++;
					}
					ruw_i++;
				}
				else
				{
					ruw_i=0;
					ruw_o=0;
					rub_pista++;
				}
			}
			break;
	 case 3:
			if(rub_reproduciendo == 1)
			{
				if(ruw_o<4)
				{
					if(ruw_i>0)
					{
						red_on();
						green_on();
						blue_on();
					}
					if(ruw_i>6)
					{
						red_on();
						green_on();
						blue_off();
					}
					if(ruw_i>12)	
					{
						blue_on();
						red_off();
						green_off();
					}
					if(ruw_i>16)
					{
						blue_off();
						red_on();
						green_off();
					}					
					if(ruw_i>18)
					{
						ruw_i = 0;
						ruw_o++;
					}
					ruw_i++;
				}
				else
				{
					ruw_i=0;
					ruw_o=0;
					rub_pista++;
				}
			}
			break;
	 case 4:
			if(rub_reproduciendo == 1)
			{
				if(ruw_o<3)
				{
					if(ruw_i>0)
					{
						red_on();
						green_on();
						blue_off();
					}
					if(ruw_i>4)
					{
						red_on();
						green_off();
						blue_on();
					}
					if(ruw_i>10)	
					{
						blue_off();
						red_off();
						green_on();
					}
					if(ruw_i>14)
					{
						blue_on();
						red_off();
						green_off();
					}					
					if(ruw_i>20)
					{
						ruw_i = 0;
						ruw_o++;
					}
					ruw_i++;
				}
				else
				{
					ruw_i=0;
					ruw_o=0;
					rub_pista++;
				}
			}
			break;
	 case 5:
			if(rub_reproduciendo == 1)
			{
				if(ruw_o<3)
				{
					if(ruw_i>0)
					{
						red_on();
						green_off();
						blue_off();
					}
					if(ruw_i>5)
					{
						red_on();
						green_on();
						blue_on();
					}
					if(ruw_i>9)	
					{
						blue_on();
						red_off();
						green_off();
					}
					if(ruw_i>15)
					{
						blue_off();
						red_on();
						green_on();
					}					
					if(ruw_i>21)
					{
						ruw_i = 0;
						ruw_o++;
					}
					ruw_i++;
				}
				else
				{
					ruw_i=0;
					ruw_o=0;
					rub_pista = 1;
				}
			}
			break;
		default:
			red_off();
			green_off();
			blue_off();
			ruw_i=0;
			ruw_o=0;
		 break;
	 }
	 
 }
 

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
#include "mcl\CrystalClock.h"
#include "mcl\ARM_SysTick.h"
#include "gsc_scheduler\gsc_sch_core\gsc_sch_core_intf.h"
#include "main.h"
#include "mcg.h"
#include "gpio.h"
#include "uart.h"
#include "io.h"
#include "nvic.h"
#include "libs.h"
#include <stdio.h>

volatile unsigned int sys_tick_counter = 0;

void WatchDog_cfg(void);
void uart_test(void);
/********************************************************************/
int main (void)
{
	//int i=0;
	
	/* Disable Watch Dog */
	WatchDog_cfg();
	/* initialize clock system for 48 MHz */
	InitClock(); 
	/* Configure the timer and the interrupt to be used to generate the tick of the scheduler */
	InitSysTick();
	/* Scheduler Initialization and tasks initialization  */
	gsc_sch_core_Init();
	/* Execute Scheduler */
	gsc_sch_core_exec();
	

	return 0;
}


/*UART test*/
void uart_test(void)
{
/*#if UART_MODE == INTERRUPT

	int counter = 0;
	
	put("\r\nHello World! UART Lab using Interrupt mode\r\n");
	
	EnableInterrupts;
	enable_irq(INT_UART0 -16);
	set_irq_priority((INT_UART0 -16), 2);
	
	while(TRUE)
	{
		counter++;
		if(counter >= DLY)
		{
			GREEN_TOGGLE;
			counter = 0;
		}
	}
#else*/	
	//Polling mode
	char a,b,c,ch, mask_cmd=0x70, val;
	static int counter = 0; 
	
	//put("\r\nHello World! UART Lab using Polling mode\r\n");

		a = in_char();
		ch = in_char();
		b = in_char();
		ch = in_char();
		c = in_char();
		ch = in_char();
		
		out_char(0xF ^ 0xF);
		out_char(b);
		out_char(c);

		//printf("el bebe");

		
/*		if(ch != 10)
		{
			if(counter==0)
			{
				switch(ch)
				{
					case 0x26:	//&
					out_char('C');
					out_char('M');
					out_char('D');
					counter++;
					break;
					case 0x66:	//f
						out_char('E');
					out_char('Q');
					out_char('U');
					counter++;
					break;
				case 0x44:	//D
					out_char('A');
					out_char('L');
					out_char('B');
					counter++;
					break;
				default:
					out_char('I');
					out_char('N');
					out_char('V');
					out_char(ch);
					break;
				}			
			}
			else if(counter==1)
			{
				val = ch&mask_cmd;
				switch(val)
				{
				case 0:
					out_char('P');
					out_char('L');
					out_char('A');
					out_char('Y');
					//counter++;
					break;
				}
			}
			//out_char(ch);
			BLUE_TOGGLE;	
		}*/
		
//#endif
}

 
 /*
  * Init watch dog
  */
 void WatchDog_cfg(void)
 {
	 /* Disable watch dog */
	 SIM_COPC |= SIM_COPC_COPT(0);
 }
 
 void SysTick_Handler(void)
 {
 	sys_tick_counter++;
 	gsc_sch_core_tick_isr();
 }


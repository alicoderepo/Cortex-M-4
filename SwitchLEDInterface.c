/********************************************************************
 This program is an implementation of Lab 8 of the course UTAustinX:
 UT.6.03x.

 It makes use of the microcontroller LM4F120/TM4C123 and interfaces
 an external LED and push-button to it.
 
 When the button is pressed, the LED starts blinking at a rate of
 5Hz. Releasing the button keeps the LED turned on. 

 FUNCTIONS

    DisableInterrupts
    EnableInterrupts
    Port_Init
    Delay1ms

 DEPENDENCIES

    TExaS.H
    tm4c123gh6pm.h

********************************************************************/

/***** 1. Pre-processor Directives Section *****/
#include "TExaS.h"
#include "tm4c123gh6pm.h"

/***** 2. Global Declarations Section *****/

/*FUNCTION PROTOTYPES */
void DisableInterrupts(void); /* Disable interrupts */
void EnableInterrupts(void);  /* Enable interrupts */

void Port_Init();
void Delay1ms(unsigned long msec);

/***** 3. Subroutines Section *****/


/*                  HARDWARE PINOUTS
PE0 is connected to positive logic momentary switch using 10k ohm
pull down resistor. PE1 is connected to positive logic LED through
470 ohm current limiting resistor To avoid damaging your hardware,
ensure that your circuits match the schematic shown in
Lab8_artist.sch (PCB Artist schematic file) or Lab8_artist.pdf
(compatible with many various readers like Adobe Acrobat).
*/

/********************************************************************
*
*   FUNCTION
*
*       main
*
*   DESCRIPTION
*
*       This function waits for a button-press event and triggers a
*       square wave of frequency 5Hz.
*
*   INPUTS
*       
*       None
*
*   OUTPUTS
*
*       None
*
********************************************************************/
int main(void){

  /* activate grader and set system clock to 80 MHz */
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);
  

  /* Make PE1 an output and make PEO an input */
  
  Port_Init(); /* Initialize the fucking ports */
  
  EnableInterrupts();     /* Enable interrupts for the grader  */

  
  
  GPIO_PORTE_DATA_R |= 0x02; /* System starts with LED on */

  while(1){
    Delay1ms(100); /* Wait for about 100 ms */

    if (GPIO_PORTE_DATA_R & 0X01) /* If switch PE0 is pressed */
    {
      GPIO_PORTE_DATA_R ^= 0x02; /* Toggle LED */
    }
    else /* turn LED on */
    {
      GPIO_PORTE_DATA_R |= 0x02;
    }
  }

}


/********************************************************************
*
*   FUNCTION
*
*       Port_Init()
*
*   DESCRIPTION
*
*       This function waits for a button-press event and triggers a
*       square wave of frequency 5Hz.
*
*   INPUTS
*       
*       None
*
*   OUTPUTS
*
*       None
*
********************************************************************/
void Port_Init(void)
{
  volatile unsigned long delay;
  /* Setup E clock hard */
  SYSCTL_RCGC2_R |= 0x10;

  /* Wait 3-5 busy cycles */
  delay = SYSCTL_RCGC2_R;

    /* Configure PE1 as output */
  GPIO_PORTE_DIR_R |= 0x02;

  /* Configure PE0 input */
  GPIO_PORTE_DIR_R &= ~0x01;

  /* Disable alternative on the port */
  GPIO_PORTE_AFSEL_R &= ~0x17;

  /* No analog functionality being used */
  GPIO_PORTE_AMSEL_R &= ~0x17;

  /* Clear god knows why/what */
  GPIO_PORTE_PCTL_R &= ~0x03;

  /* Enable Fucking port E */
  GPIO_PORTE_DEN_R |= 0x17;
}


/********************************************************************
*
*   FUNCTION
*
*       Delay1ms()
*
*   DESCRIPTION
*
*       This function waits for a button-press event and triggers a
*       square wave of frequency 5Hz.
*
*   INPUTS
*       
*       msec      The amount of time to be delayed in milli-seconds 
*
*   OUTPUTS
*
*       None
*
********************************************************************/
void Delay1ms(unsigned long msec){

  unsigned long itr;

  while(msec > 0){
    itr = 16000;  /* Via logic analyzer this number has been 
                   measured to give an approx. 1ms delay */
    while(itr > 0){
      itr = itr - 1;
    }
    msec = msec - 1; /* decrements every 1 ms */
  }

}
//------------------------------------------------------------------------------
// Description: This file contains the Main Routine - "While" Operating System
//
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "functions.h"
#include "msp430.h"
#include "macros.h"


#define TWO_CIRCLE_TIME 1010
#define RX 2
#define TX 3

// Global Variables
//timers
  extern unsigned int Second_Count;


//others
char state = WAIT;
unsigned int state_count;

void main(void){
//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;
  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_Serial();
  Init_LCD();                          // Initialize LCD
  Init_ADC();                          // Initialize ADC
  Init_REF();                          // Initialize internal voltage refrence to 2.5V
  Init_DAC();                          // Initialize DAC

  // Display
  clear_lcd();
  lcd_line1("L,R,Thumb1");
  


//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run? Yes, yes it can

    menu();
    wheel_polarity_error();             //slap some error checking function in this bad boi

  } // End of While Always
//------------------------------------------------------------------------------
} // End of main();

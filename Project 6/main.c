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
#include <string.h>

// Function Prototypes
  void main(void);

  
// Global Variables

//LCD Display
extern volatile unsigned int Time_Sequence;

//others


char change_display;
char state;
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
  Init_LCD();                          // Initialize LCD
  Init_ADC();                          // Initialize ADC


  // Display
  clear_lcd();
  lcd_line1("L,R,Thumb");
  


//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run? Yes, yes it can

    wheel_polarity_error();             //slap some error checking function in this bad boi
    
    switch(state){
    case WAIT:
      state_count =0;
      break;
    case DRIVE:
      if(state_count < ONE_SECOND)
        break;
      if((return_vleft_average() > BLACK_LINE) && (return_vleft_average() > BLACK_LINE)){
        R_stop();L_stop();
        state = REVERSE_STATE;
        state_count =0;
      }else{
        R_forward(10000);L_forward(10000);
      }
      break;
    case REVERSE_STATE:
       if(state_count > 30){
        R_stop();L_stop();
        state = BLACK_LINE_DETECTED;
      }else{
        R_reverse(10000);L_reverse(10000);
      }
      break;
      
    case BLACK_LINE_DETECTED:
      if(state_count > 280){
        R_stop();L_stop();
        state = WAIT;
      }else{
        R_forward(10000);L_reverse(10000);
      }
      break;
    default:break;
    }


  } // End of While Always
//------------------------------------------------------------------------------
} // End of main();

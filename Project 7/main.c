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

// Function Prototypes
  void main(void);

  
// Global Variables

//LCD Display
extern unsigned int Time_Sequence;

//others


char change_display;
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
    
    switch(state){
    case WAIT:
      state_count =0;
      break;
//    case DRIVE:
//      if(state_count < ONE_SECOND)
//        break;
//      if((return_vleft_average() > BLACK_LINE) && (return_vright_average() > BLACK_LINE)){
//        R_stop();L_stop();
//        state = REVERSE_STATE;
//        state_count =0;
//      }else{
//        R_forward(2000);L_forward(2000);
//      }
//      break;
//    case REVERSE_STATE:
//       if(state_count > 80){
//        R_stop();L_stop();
//        state = BLACK_LINE_DETECTED;
//      }else{
//        R_reverse(2000);L_reverse(2000);
//      }
//      break;
//      
//    case BLACK_LINE_DETECTED:
//      if(state_count > 220){
//        R_stop();L_stop();
//        state = NAVIGATION;
//      }else{
//        R_forward(2000);L_reverse(2000);
//      }
//      break;
    case NAVIGATION:
      if(state_count < ONE_SECOND)
        break;

      line_nav(1850);
  
      break;
    default:break;
    }


  } // End of While Always
//------------------------------------------------------------------------------
} // End of main();

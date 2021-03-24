//------------------------------------------------------------------------------
// Description: This file contains the Main Routine - "While" Operating System
//
//
//File Name:    main.c
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
  char state = WAIT;            //sets the default state to WAIT
  unsigned int state_count;     //the state counter that will get updated every 5ms 
                                //a timer interrupt

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


  // Static Display text
  clear_lcd();                          //clears the LCD screen
  lcd_line1("L,R,Thumb1");              //Puts "L,R,Thumb1" to the screen
  


//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Operating system

    menu();
    wheel_polarity_error();             //Error checking for wheel direction to prevent FET damage.
    
    switch(state){
    case WAIT:                          //Waiting for switch to trigger the state machine.
      state_count = RESET_COUNT;
      break;
    case DRIVE:                         //Drive until black line is intercepted.
      if(state_count < ONE_SECOND)
        break;
                                        //Detection of the black line.
      if((return_vleft_average() > BLACK_LINE) && (return_vright_average() > BLACK_LINE)){
        R_stop();L_stop();              //Stop the wheels and advance the state and reset the count.
        state = REVERSE_STATE;          
        state_count = RESET_COUNT;
      }else{            
        R_forward(SLOW);L_forward(SLOW);//Not detected so continue forward.
      }
      break;
    case REVERSE_STATE:                 //Once detected pulse reverse to stop forward progress.
       if(state_count > REVERSE_TIME){
        R_stop();L_stop();
        state = BLACK_LINE_DETECTED;    //Once reverse time is up advnace state and reset count.
        state_count = RESET_COUNT;
      }else{
        R_reverse(SLOW);L_reverse(SLOW);
      }
      break;
      
    case BLACK_LINE_DETECTED:           //Car is now on the black line and must orient itself
      if(state_count > SPIN_TIME){      //If the car is done spinning stop and enter wait state
        R_stop();L_stop();
        state = WAIT;
      }else{
        R_forward(SLOW);L_reverse(SLOW);//Otherwise spin to orient.
      }
      break;
    default:break;
    }


  } // End of While Always
//------------------------------------------------------------------------------
} // End of main();

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
#define ONE_SECOND 200
#define TWO_SECONDS 400
#define THREE_SECONDS 600
#define FOUR_SECONDS 800
#define FIVE_SECONDS 1000

char change_display;
char state;
unsigned int state_count;
char set_lcd_wait = TRUE;

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
  char wait[11] = "WAIT";
  char reverse[11] = "REVERSE";
  char forward[11] = "FORWARD";
  char cw[11] = "ROTATE CW";
  char ccw[11] = "ROTATE CCW";
  clear_lcd();
  lcd_line1("L,R,Thumb");
  


//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run? Yes, yes it can

    wheel_polarity_error();             //slap some error checking function in this bad boi
    
    switch(state){
    case WAIT_1: 
      state_count = RESET_STATE; 
      change_display = TRUE; 
      state = FORWARD_1;
      break;
    case FORWARD_1:
      if(change_display){
        change_display = FALSE;
        lcd_line2(forward);
      }
      
      if(state_count > ONE_SECOND){
        L_stop();R_stop();
        if(set_lcd_wait){
          set_lcd_wait = FALSE;
          clear_lcd();
          lcd_line2(wait);
        }
        if(state_count > TWO_SECONDS){
          state_count = RESET_STATE;
          state = REVERSE_1;
          change_display = TRUE;
          set_lcd_wait = TRUE;
        }
      }else{
        L_forward(25000);R_forward(25000);
      }
      break;
    case REVERSE_1:
      if(change_display){
        change_display = FALSE;
        lcd_line2(reverse);
      }
      
      
      if(state_count > TWO_SECONDS){
        L_stop();R_stop();
        if(set_lcd_wait){
          set_lcd_wait = FALSE;
          clear_lcd();
          lcd_line2(wait);
        }
        if(state_count > THREE_SECONDS){
          state_count = RESET_STATE;
          state = FORWARD_2;
          change_display = TRUE;
          set_lcd_wait = TRUE;
        }
      }else{
        L_reverse(25000);R_reverse(25000);
      }
      break;
    case FORWARD_2:
      if(change_display){
        change_display = FALSE;
        lcd_line2(forward);
      }
      
      
      if(state_count > ONE_SECOND){
        L_stop();R_stop();
        if(set_lcd_wait){
          set_lcd_wait = FALSE;
          clear_lcd();
          lcd_line2(wait);
        }
        if(state_count > TWO_SECONDS){
          state_count = RESET_STATE;
          state = CLOCKWISE;
          change_display = TRUE;
          set_lcd_wait = TRUE;
        }
      }else{
        L_forward(25000);R_forward(25000);
      }
      break;
    case CLOCKWISE:
      if(change_display){
        change_display = FALSE;
        lcd_line2(cw);
      }
      
      
      if(state_count > THREE_SECONDS){
        L_stop();R_stop();
        if(set_lcd_wait){
          set_lcd_wait = FALSE;
          clear_lcd();
          lcd_line2(wait);
        }
        if(state_count > FIVE_SECONDS){
          state_count = RESET_STATE;
          state = COUNTER_CLOCKWISE;
          change_display = TRUE;
          set_lcd_wait = TRUE;
        }
      }else{
        L_forward(25000);R_reverse(25000);
      }
      break;
    case COUNTER_CLOCKWISE:
      if(change_display){
        change_display = FALSE;
        lcd_line2(ccw);
      }
      
      
      if(state_count > THREE_SECONDS){
        L_stop();R_stop();
        if(set_lcd_wait){
          set_lcd_wait = FALSE;
          clear_lcd();
          lcd_line2(wait);
        }
        if(state_count > FIVE_SECONDS){
          state_count = RESET_STATE;
          state = RESET_STATE;
          change_display = TRUE;
          set_lcd_wait = TRUE;
        }
      }else{
        L_reverse(25000);R_forward(25000);
      }
      break;
    default:break;
    }


  } // End of While Always
//------------------------------------------------------------------------------
} // End of main();

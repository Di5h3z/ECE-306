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
#include <string.h>
#include "macros.h"


// Function Prototypes
  void main(void);
  void Init_Conditions(void);
  void Init_LEDs(void);
  void Switches_Process(void);
  void Timers_Process(void);
  
  //Display Functions
  void clear_lcd(void);
  void lcd_line1(char* line);
  void lcd_line2(char* line);
  void lcd_line3(char* line);
  void lcd_line4(char* line);
  
  //drive functions
  void L_stop(void);
  void R_stop(void);
  void L_forward(unsigned int speed);
  void R_forward(unsigned int speed);
  void L_reverse(unsigned int speed);
  void R_reverse(unsigned int speed);
  
// Global Variables

//LCD Display
extern volatile unsigned int Time_Sequence;

//others

#define WAIT_1 1
#define FORWARD_1 2
#define REVERSE_1 3
#define FORWARD_2 4
#define CLOCKWISE 5
#define COUNTER_CLOCKWISE 6


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
  P6OUT |= LCD_BACKLITE;               // Turn on backlight
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD


  // diplays
  char wait[11] = "WAIT";
  char reverse[11] = "REVERSE";
  char forward[11] = "FORWARD";
  char cw[11] = "ROTATE CW";
  char ccw[11] = "ROTATE CCW";
  clear_lcd();
  lcd_line2(wait);


//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run

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
      
      L_forward(250);R_forward(250);
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
      }  
      break;
    case REVERSE_1:
      if(change_display){
        change_display = FALSE;
        lcd_line2(reverse);
      }
      
      L_reverse(250);R_reverse(250);
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
      }
      break;
    case FORWARD_2:
      if(change_display){
        change_display = FALSE;
        lcd_line2(forward);
      }
      
      L_forward(250);R_forward(250);
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
      }
      break;
    case CLOCKWISE:
      if(change_display){
        change_display = FALSE;
        lcd_line2(cw);
      }
      
      L_reverse(250);R_forward(250);
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
      }
      break;
    case COUNTER_CLOCKWISE:
      if(change_display){
        change_display = FALSE;
        lcd_line2(ccw);
      }
      
      L_forward(250);R_reverse(250);
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
      }
      break;
    default:state = RESET_STATE;break;
    }

    Switches_Process();                // Check for switch state change
    Timers_Process();


  } // End of While Always
//------------------------------------------------------------------------------
} // End of main();

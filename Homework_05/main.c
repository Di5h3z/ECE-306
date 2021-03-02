//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//
//  Jim Carlson
//  Jan 2018
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Init_LEDs(void);
void Timers_Process(void);
  // Global Variables
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;

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
// Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
//

  strcpy(display_line[0], "          ");
  update_string(display_line[0], 0);
  strcpy(display_line[1], "Homework 5");
  update_string(display_line[1], 1);
  strcpy(display_line[2], "          ");
  update_string(display_line[3], 3);
  enable_display_update();
  display_changed = 1;
  update_display = 1;
//  Display_Update(3,1,0,0);

  
  
//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                    // Can the Operating system run
    
    switch(Time_Sequence){           //this makes the led's blink... wheeee
      case 200:
        P6OUT |= GRN_LED;            // Change State of LED 5
        break;
        
      case 150:
        P1OUT |= RED_LED;            // Change State of LED 4
        P6OUT &= ~GRN_LED;           // Change State of LED 5
        break;
        
      case 100:
        P6OUT |= GRN_LED;            // Change State of LED 5
        break;
        
      case  50:
        P1OUT &= ~RED_LED;           // Change State of LED 4
        P6OUT &= ~GRN_LED;           // Change State of LED 5
        break;
        
      default: break;
    }
    
    Timers_Process();
    Switches_Process();                // Check for switch state change
    Display_Process();

  }
//------------------------------------------------------------------------------
}

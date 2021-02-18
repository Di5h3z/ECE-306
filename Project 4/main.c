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
void forward(void);
void stop(void);
void Switches_Process(void);
void Timers_Process(void);


void circle(void);
void figure_eight(void);
void triangle(void);
// Global Variables
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;

extern unsigned int cycle_time;

char next_state;
char state;


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

  strcpy(display_line[0], "  CIRCLE  ");
  update_string(display_line[0], 0);
  strcpy(display_line[1], " FIGURE_8 ");
  update_string(display_line[1], 1);
  strcpy(display_line[2], " TRIANGLE ");
  update_string(display_line[3], 3);
  enable_display_update();
//  Display_Update(3,1,0,0);


  display_changed=1;
  update_display=1;
  
  cycle_time=0;
  next_state = CIRCLE;

//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run

    switch(state){
    case WAIT:break;
    case CIRCLE: circle();break;
    case FIGURE_8: figure_eight();break;
    case TRIANGLE: triangle();break;
    default:break;
    }

    Switches_Process();                // Check for switch state change
    Display_Process();
    Timers_Process();





  } // End of While Always
//------------------------------------------------------------------------------
} // End of main();

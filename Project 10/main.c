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
extern char state;

extern int command_timer;
int command_time = 0;
int next_command_time = 0;
void (*command)(int) = &right_turn;
void (*next_command)(int) = &right_turn;
char curr_screen4_line2[10];
char next_screen4_line2[10];
int command_speed = 2000;

char station_counter;

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
    

    char* IOT_command = verify_pin(&IOT_rx()[1]);
    switch(IOT_command[0]){
    case 'F': 
        command_timer = 0;
        next_command = &forward;
        next_command_time = str_to_int(&IOT_command[1]);
        IOT_command[4] = '\0';
        str_cpy(next_screen4_line2, IOT_command);
        break;
    case 'B': 
        command_timer = 0;
        next_command = &reverse;
        next_command_time = str_to_int(&IOT_command[1]);
        IOT_command[4] = '\0';
        str_cpy(next_screen4_line2, IOT_command);
        break;
        
    case 'L': 
        command_timer = 0;
        next_command = &left_turn;
        next_command_time = str_to_int(&IOT_command[1]);
        IOT_command[4] = '\0';
        str_cpy(next_screen4_line2, IOT_command);
        break;
        
    case 'R': 
        command_timer = 0;
        next_command = &right_turn;
        next_command_time = str_to_int(&IOT_command[1]);
        IOT_command[4] = '\0';
        str_cpy(next_screen4_line2, IOT_command);
        break;
    case 'S':
        if(str_to_int(&IOT_command[1]) >= 50)
          command_speed = MAX_SPEED;
        else
          command_speed = str_to_int(&IOT_command[1])*200;
        break;
    case 'N':
        P2OUT |= IR_LED; 
        next_command = &navigate;
        IOT_command[4] = '\0';
        str_cpy(next_screen4_line2, IOT_command);
        break;
        
    case 'E':
      state = BL_EXIT;
      command = &exit;
      break;
    case 'I':
      station_counter++;
      break;
    case 'C':
      set_clock(0);
      enable_clock();
      break;
    default:break;
    
    }
    screen4_line2 = curr_screen4_line2;
    (*command)(command_time);
    



  } // End of While Always
//------------------------------------------------------------------------------
} // End of main();

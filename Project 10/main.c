//------------------------------------------------------------------------------
// Description: This file contains the Main Routine - "While" Operating System
//
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include "functions.h"
#include "msp430.h"
#include "macros.h"

//Globals
  //Timers
  extern unsigned int Second_Count;

  //IOT Command Globals
  extern int command_timer;
  int command_time = ZERO;
  int next_command_time = ZERO;
  void (*command)(int) = &right_turn;
  void (*next_command)(int) = &right_turn;
  char curr_screen4_line2[MAX_LCD_LENGTH];
  char next_screen4_line2[MAX_LCD_LENGTH];
  int command_speed = SLOW_SPEED;
  char station_counter;
  extern char state;


//------------------------------------------------------------------------------
// the Background part of the foraground/background OS and init                 main
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void main(void){
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
// Begining of the Background Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                       // Can the Operating system run? Yes, yes it can

    menu();
    wheel_polarity_error();                     //slap some error checking function in this bad boi
    

    char* IOT_command = verify_pin(&IOT_rx()[SECOND_CHAR]);
    switch(IOT_command[ZERO]){
    case FOWARD_COMMAND_CHAR:                   //Forward Command
        command_timer = ZERO;
        next_command = &forward;
        next_command_time = str_to_int(&IOT_command[SECOND_CHAR]);
        IOT_command[FITH_CHAR] = NULL_CHAR;
        str_cpy(next_screen4_line2, IOT_command);
        break;
    case REVERSE_COMMAND_CHAR:                  //Reverse Command
        command_timer = ZERO;
        next_command = &reverse;
        next_command_time = str_to_int(&IOT_command[SECOND_CHAR]);
        IOT_command[FITH_CHAR] = NULL_CHAR;
        str_cpy(next_screen4_line2, IOT_command);
        break;
        
    case LEFT_COMMAND_CHAR:                     //Left Turn Command
        command_timer = ZERO;
        next_command = &left_turn;
        next_command_time = str_to_int(&IOT_command[SECOND_CHAR]);
        IOT_command[FITH_CHAR] = NULL_CHAR;
        str_cpy(next_screen4_line2, IOT_command);
        break;
        
    case RIGHT_COMMAND_CHAR:                    //Right Turn Command
        command_timer = ZERO;
        next_command = &right_turn;
        next_command_time = str_to_int(&IOT_command[SECOND_CHAR]);
        IOT_command[FITH_CHAR] = NULL_CHAR;
        str_cpy(next_screen4_line2, IOT_command);
        break;
        
    case SPEED_COMMAND_CHAR:                    //Speed Change Command
        if(str_to_int(&IOT_command[SECOND_CHAR]) >= MAX_SPEED_PRIOR_MULT)
          command_speed = MAX_SPEED;
        else
          command_speed = str_to_int(&IOT_command[SECOND_CHAR])*SPEED_MUTIPLIER;
        break;
        
    case INTNAV_COMMAND_CHAR:                   //Intercept + Naviagte Command
        P2OUT |= IR_LED; 
        next_command = &navigate;
        IOT_command[FITH_CHAR] = NULL_CHAR;
        str_cpy(next_screen4_line2, IOT_command);
        break;
        
    case EXIT_COMMAND_CHAR:                     //Exit Command
      state = BL_EXIT;
      command = &exit;
      break;
      
    case INC_COMMAND_CHAR:                      //Increment Station Command
      station_counter++;
      break;
      
    case CLOCK_COMMAND_CHAR:                    //Start Clock Command
      set_clock(ZERO);
      enable_clock();
      break;
      
    default:break;
    
    }
                                                //puts current command to the screen and calls it
    screen4_line2 = curr_screen4_line2;
    (*command)(command_time);
    



  } // End of While Always
//------------------------------------------------------------------------------
} // End of main();

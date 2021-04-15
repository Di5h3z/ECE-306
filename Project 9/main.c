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
char command_state;
unsigned int state_count;
extern int command_timer;
int command_time = 0;
int next_command_time = 0;
void (*command)(int) = &right_turn;
void (*next_command)(int) = &right_turn;
char curr_screen4_line2[10];
char next_screen4_line2[10];

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
    
    switch(USB_rx()[1]){ //this will run 1x per command recieved
    case 'F': //FAST IOT baud
      
      UCA0BRW = 4; // 115,200 baud
      UCA0MCTLW = 0x5551;
      
      USB_tx("115200 IOT Baud");
      break;
    case 'S': // SLOW IOT baud
      
      UCA0BRW = 52; // 9,600 baud
      UCA0MCTLW = 0x4911;
      
      USB_tx("9600 IOT Baud");
      break;
    case '$': //Test command
      USB_tx("ECHO TEST");
      break;
    default: break;
    }
    
    
    
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
        
      default:break;
    }
    screen4_line2 = curr_screen4_line2;
    (*command)(command_time);
    
//    if(Second_Count >= 2){
//      USB_tx("testUSB");
//      OIT_tx("testIOT");
//      Second_Count = 0;
//    }
//    IOT_rx();
//    USB_rx();
    
//    switch(state){ //intercept and line follow not needed for HW 8 and Project 8
//    case WAIT:
//      state_count =0;
//      break;
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
//       if(state_count > 60){
//        R_stop();L_stop();
//        state = SPIN_BLACK_LINE;
//        state_count =0;
//      }else{
//        R_reverse(2000);L_reverse(2000);
//      }
//      break;
//      
//    case SPIN_BLACK_LINE:
//      if(state_count > 220){
//        enable_clock();
//        set_clock(0);
//        R_stop();L_stop();
//        state_count =0;
//        state = NAVIGATION;
//      }else{
//        R_reverse(2000);L_forward(2000);
//      }
//      break;
//    case NAVIGATION:
//      if(get_clock_time() < 1010){
//        line_nav(1750);
//      }else{
//        R_stop();L_stop();
//        state_count =0;
//        state = SPIN_CENTER;
//      }
//        
//      break;
//    case SPIN_CENTER:
//      if(state_count > 300){
//        R_stop();L_stop();
//        state_count =0;
//        state = DRIVE_CETNER;
//      }else{
//        R_reverse(2000);L_forward(2000);
//      }
//      break;
//    case DRIVE_CETNER:
//      if(state_count < 350){
//        R_forward(2000);L_forward(2000);
//      }else{
//        disable_clock();
//        R_stop();L_stop();
//        state = WAIT;
//      }
//      break;
//    default:break;
//    }


  } // End of While Always
//------------------------------------------------------------------------------
} // End of main();
